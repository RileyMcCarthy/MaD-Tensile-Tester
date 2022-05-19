#include "Module.h"

Module *module_create(Module *parent)
{
    Module *module = (Module *)malloc(sizeof(Module));
    module->parent = parent;
    module->child = NULL;
    module->type = MODULE_ROOT;
    module->data = NULL;
    module->numChildren = 0;
    module->visible = true;

    module->lastPressed = 0;
    module->debouncems = 100;
    module->onTouch = NULL;

    if (parent != NULL)
    {
        parent->numChildren++;
        parent->child = (Module **)realloc(parent->child, parent->numChildren * sizeof(Module *));
        parent->child[parent->numChildren - 1] = module;
    }
    return module;
}

void module_update_callback(Module *module, void (*onUpdate)(Display *display, Module *module, void *arg))
{
    module->onUpdate = onUpdate;
}

void module_update_check(Display *display, Module *module, void *arg)
{
    if (module->type == MODULE_WINDOW)
    {
        arg = module->data;
    }
    for (int i = 0; i < module->numChildren; i++)
    {
        module_update_check(display, module->child[i], arg);
    }
    if (module->onUpdate != NULL)
    {
        module->onUpdate(display, module, arg);
    }
}

void module_touch_callback(Module *module, void (*onTouch)(int id, void *page), int id)
{
    module->onTouch = onTouch;
    module->touchId = id;
}

int module_touch_check(Module *root, Display *display, void *arg)
{
    if (display->locationCount == 0)
    {
        return 0;
    }
    if (root->type == MODULE_WINDOW)
    {
        arg = root->data;
    }
    int touchCount = 0;
    for (int i = 0; i < root->numChildren; i++)
    {
        touchCount += module_touch_check(root->child[i], display, arg);
    }
    if (root->onTouch == NULL)
    {
        // return 0;
    }
    if (((SCREEN_WIDTH - display->location[0].x) > root->x) && ((SCREEN_WIDTH - display->location[0].x) < (root->x + root->w)))
    {
        if (((SCREEN_HEIGHT - display->location[0].y) > root->y) && ((SCREEN_HEIGHT - display->location[0].y) < (root->y + root->h)))
        {
            if (root->onTouch != NULL)
            {
                root->lastPressed = _getms();
                root->onTouch(root->touchId, arg);
                touchCount++;
            }
        }
    }
    return touchCount;
}

void module_set_visable(Module *module, bool visible)
{
    module->visible = visible;
}

void module_copy(Module *to, Module *from)
{
    to->type = from->type;
    to->x = from->x;
    to->y = from->y;
    to->w = from->w;
    to->h = from->h;
    to->px = from->px;
    to->py = from->py;
    to->backgroundColor = from->backgroundColor;
    to->foregroundColor = from->foregroundColor;
}

void module_save(Module *module, Display *display)
{
    display_bte_memory_copy(display, PAGE1_START_ADDR, SCREEN_WIDTH, module->x, module->y, PAGE3_START_ADDR, SCREEN_WIDTH, module->x, module->y, module->w, module->h);
}

void module_paste(Module *module, Display *display)
{
    display_bte_memory_copy(display, PAGE3_START_ADDR, SCREEN_WIDTH, module->x, module->y, PAGE1_START_ADDR, SCREEN_WIDTH, module->x, module->y, module->w, module->h);
}

void module_graph_add_marker(Module *module, float value)
{
    Graph *graph = (Graph *)module->data;
    if (graph->markerCount < MAX_GRAPH_MARKERS)
    {
        graph->markerY[graph->markerCount] = value;
        graph->markerCount++;
    }
}

void module_set_window(Module *module, void *window)
{
    module->type = MODULE_WINDOW;
    module->data = window;
}

void module_set_line(Module *module, int w)
{
    module_set_line_one(module, w, 0);
}

void module_set_line_one(Module *module, int x, int y)
{
    module->type = MODULE_LINE;
    module->data = NULL;
    module->w = x;
    module->h = y;
}

void module_set_rectangle(Module *module, int w, int h)
{
    module->type = MODULE_RECTANGLE;
    module->data = NULL;
    module->w = w;
    module->h = h;
}

void module_set_rectangle_circle(Module *module, int w, int h)
{
    module->type = MODULE_RECTANGLE_CIRCLE;
    module->data = NULL;
    module->w = w;
    module->h = h;
}

// Makes a empty window a graph, have position and size already defined
// Creates an empty graph that has data of size of the centerLine in pixels
void module_set_graph(Module *window, const char *titleName, const char *units)
{
    window->type = MODULE_GRAPH;

    Graph *graph = (Graph *)malloc(sizeof(Graph));

    graph->title = titleName;
    graph->units = units;

    // Create Title
    Module *title = module_create(window);
    module_set_padding(title, 10, 10);
    module_set_text(title, graph->title);
    module_set_font(title, RA8876_CHAR_HEIGHT_32);
    module_set_color(title, COLOR65K_BLACK, title->parent->foregroundColor);
    module_align_inner_top(title);
    module_align_center(title);
    module_add_underline(title);

    // Create min
    char buf[15];
    Module *minText = module_create(window);
    module_set_padding(minText, 3, 3);
    sprintf(buf, "%.2f", graph->minY);
    module_set_text(minText, buf);
    module_set_font(minText, RA8876_CHAR_HEIGHT_16);
    module_set_color(minText, COLOR65K_BLACK, minText->parent->foregroundColor);
    module_align_inner_bottom(minText);
    module_align_inner_left(minText);

    // Create Graph Area
    Module *graphArea = module_create(window);
    module_align_below(graphArea, title);
    module_fit_below(graphArea, title);
    module_fit_right(graphArea, minText);
    module_align_right(graphArea, minText);
    module_set_color(graphArea, graphArea->parent->foregroundColor, graphArea->parent->backgroundColor);
    module_set_padding(graphArea, 0, 0);

    // Create Units
    Module *units_module = module_create(window);
    module_set_padding(units_module, 10, 10);
    module_set_text(units_module, graph->units);
    module_set_font(units_module, RA8876_CHAR_HEIGHT_32);
    module_set_color(units_module, COLOR65K_BLACK, units_module->parent->foregroundColor);
    module_align_above(units_module, graphArea);
    module_align_inner_left(units_module);

    // Create zero
    char buf[15];
    Module *zeroText = module_create(graphArea);
    module_set_padding(zeroText, 3, 3);
    module_set_text(zeroText, "0");
    module_set_font(zeroText, RA8876_CHAR_HEIGHT_16);
    module_set_color(zeroText, COLOR65K_BLACK, zeroText->parent->foregroundColor);
    module_align_left(zeroText, graphArea);
    module_align_middle(zeroText);

    // Create max
    char buf[15];
    Module *maxText = module_create(graphArea);
    module_set_padding(maxText, 3, 3);
    sprintf(buf, "%.2f", graph->maxY);
    module_set_text(maxText, buf);
    module_set_font(maxText, RA8876_CHAR_HEIGHT_16);
    module_set_color(maxText, COLOR65K_BLACK, maxText->parent->foregroundColor);
    module_align_inner_top(maxText);
    module_align_left(maxText, graphArea);

    // Create Graph Horizontal Line
    Module *centerLine = module_create(graphArea);
    module_set_rectangle(centerLine, 0, 0);
    module_fit_width(centerLine);
    module_align_center(centerLine);
    module_align_middle(centerLine);
    module_set_color(centerLine, COLOR65K_BLACK, COLOR65K_BLACK);

    // Create Graph Vertical Line
    Module *verticleLine = module_create(graphArea);
    module_set_rectangle(verticleLine, 0, 0);
    module_fit_height(verticleLine);
    module_align_inner_left(verticleLine);
    module_align_inner_top(verticleLine);
    module_set_color(verticleLine, COLOR65K_BLACK, COLOR65K_BLACK);

    graph->graphArea = graphArea;
    graph->dataCount = 0; // Start Graph with no data

    window->data = graph;
}

void module_graph_insert(Module *module, double value)
{
    Graph *graph = (Graph *)module->data;
    if (value > graph->maxY || value < graph->minY)
    {
        return;
    }
    double temp = graph->data[0];
    graph->data[1] = temp;
    graph->data[0] = value;
    /*if (graph->dataCount < graph->graphArea->w) // If there is space in the graph, insert at end
    {
        graph->data = realloc(graph->data, (graph->dataCount + 1) * sizeof(double));
        graph->data[graph->dataCount] = value;
        graph->dataCount++;
    }
    else // If there is no space in the graph, remove the first value and insert at end
    {
        for (int i = 0; i < graph->dataCount - 1; i++)
        {
            graph->data[i] = graph->data[i + 1];
        }
        graph->data[graph->dataCount - 1] = value;
    }*/
}

void module_graph_set_range(Module *module, float maxY, float minY)
{
    Graph *graph = (Graph *)module->data;
    graph->maxY = maxY;
    graph->minY = minY;
}

void module_set_image(Module *module, Image *image)
{
    module->type = MODULE_IMAGE;
    module->data = image;
    module->w = image->width;
    module->h = image->height;
}

void module_add_underline(Module *module)
{
    Module *underline = module_create(module);
    underline->type = MODULE_LINE;
    underline->w = module->w;
    underline->h = 1;
    module_set_color(underline, module->foregroundColor, module->backgroundColor);
    module_set_position(underline, module->x, module->y + module->h);
}

void module_set_text(Module *module, const char *text)
{
    module->type = MODULE_TEXT;

    if (module->data != NULL) // Update text only
    {
        ModuleText *textModule = (ModuleText *)module->data;
        free(textModule->text);
        textModule->text = (char *)malloc(strlen(text) + 1);
        strcpy(textModule->text, text);

        module->data = textModule;
        module_set_font(module, textModule->font); // Recalculate size
    }
    else
    {
        ModuleText *textModule = (ModuleText *)malloc(sizeof(ModuleText));
        textModule->text = (char *)malloc(strlen(text) + 1);
        strcpy(textModule->text, text);
        textModule->maxChar = 0;
        textModule->font = RA8876_CHAR_HEIGHT_24; // Default font
        textModule->alignment = MODULE_TEXT_ALIGN_NONE;

        module->data = textModule;
        module_set_font(module, textModule->font); // Recalculate size
    }
}

void module_text_max_char(Module *module, int maxChar)
{
    ((ModuleText *)module->data)->maxChar = maxChar;
}

void module_text_align(Module *module, ModuleTextAignType alignment)
{
    if (module->type != MODULE_TEXT)
        return;
    ModuleText *textModule = (ModuleText *)module->data;
    textModule->alignment = alignment;
}

// Sets modules w/h from font size. Font size: 32,24,16 or -1 for auto
void module_set_font(Module *module, int font)
{
    ModuleText *textModule = (ModuleText *)module->data;
    switch (font)
    {
    case RA8876_CHAR_HEIGHT_32:
    {
        module->h = 32;
        module->w = 16 * strlen(textModule->text);
        textModule->font = RA8876_CHAR_HEIGHT_32;
        break;
    }
    case RA8876_CHAR_HEIGHT_24:
    {
        module->h = 24;
        module->w = 12 * strlen(textModule->text);
        textModule->font = RA8876_CHAR_HEIGHT_24;
        break;
    }
    case RA8876_CHAR_HEIGHT_16:
    {
        module->h = 16;
        module->w = 8 * strlen(textModule->text);
        textModule->font = RA8876_CHAR_HEIGHT_16;
        break;
    }
    default:
    {
        if (module->parent->h > 32)
        {
            int width;
            if (module->parent->w > (width = strlen(textModule->text) * 16))
            {
                module->h = 32;
                module->w = width;
                textModule->font = RA8876_CHAR_HEIGHT_32;
            }
            else if (module->parent->w > (width = strlen(textModule->text) * 12))
            {
                module->h = 24;
                module->w = width;
                textModule->font = RA8876_CHAR_HEIGHT_24;
            }
            else
            {
                module->h = 16;
                module->w = strlen(textModule->text) * 8;
                textModule->font = RA8876_CHAR_HEIGHT_16;
            }
        }
        else if (module->parent->h > 24)
        {
            int width;
            if (module->parent->w > (width = strlen(textModule->text) * 12))
            {
                module->h = 24;
                module->w = width;
                textModule->font = RA8876_CHAR_HEIGHT_24;
            }
            else
            {
                module->h = 16;
                module->w = strlen(textModule->text) * 8;
                textModule->font = RA8876_CHAR_HEIGHT_16;
            }
        }
        else
        {
            module->h = 16;
            module->w = strlen(textModule->text) * 8;
            textModule->font = RA8876_CHAR_HEIGHT_16;
        }
        break;
    }
    }
}

void module_set_padding(Module *module, int px, int py)
{
    module->px = px;
    module->py = py;
}

void module_set_position(Module *module, int x, int y)
{
    module->x = x;
    module->y = y;
}

void module_set_size(Module *module, int w, int h)
{
    module->w = w;
    module->h = h;
}

void module_set_color(Module *module, int foreground, int background)
{
    module->foregroundColor = foreground;
    module->backgroundColor = background;
}

void module_align_inner_top(Module *module)
{
    module->y = module->parent->y + module->parent->py;
}

void module_align_inner_left(Module *module)
{
    module->x = module->parent->x + module->parent->px;
}

void module_align_inner_right(Module *module)
{
    module->x = module->parent->x + module->parent->w - module->w - module->parent->px;
}

void module_align_inner_bottom(Module *module)
{
    module->y = module->parent->y + module->parent->h - module->h - module->parent->py;
}

void module_align_middle(Module *module)
{
    module->y = module->parent->y + module->parent->h / 2 - module->h / 2;
}

void module_align_center(Module *module)
{
    module_align_center_sector(module, 1, 2);
}

void module_align_center_sector(Module *module, int section, int sections)
{
    module->x = module->parent->x + module->parent->px + section * (module->parent->w - module->parent->px * 2) / sections - module->w / 2;
}

void module_align_space_even(Module *module, int section, int sections)
{
    module->x = module->parent->x + module->parent->px + section * (module->parent->w - 2 * module->parent->px - sections * module->w) / (sections + 1) + (section - 1) * module->w;
}

void module_align_space_even_verticle(Module *module, int section, int sections)
{
    module->y = module->parent->y + module->parent->py + section * (module->parent->h - 2 * module->parent->py - sections * module->h) / (sections + 1) + (section - 1) * module->h;
}

void module_align_above(Module *module, Module *ref)
{
    module->y = ref->y - module->h - ref->py;
}

void module_align_left(Module *module, Module *ref)
{
    module->x = ref->x - module->w - module->px;
}

void module_align_below(Module *module, Module *ref)
{
    module->y = ref->y + ref->h + ref->py;
}

void module_align_right(Module *module, Module *ref)
{
    module->x = ref->x + ref->w + ref->px;
}

void module_fit_space_even(Module *module, int sections)
{
    module->w = (module->parent->w - 2 * module->parent->px - (sections - 0) * module->px) / sections;
}

void module_fit_space_even_verticle(Module *module, int sections)
{
    module->h = (module->parent->h - 2 * module->parent->py - (sections - 0) * module->py) / sections;
}

void module_fit_height(Module *module)
{
    module->h = module->parent->h - module->parent->py - module->parent->py;
}

void module_fit_below(Module *module, Module *ref)
{
    module_align_below(module, ref);
    module->h = module->parent->h - 2 * module->parent->py - ref->h - 2 * ref->py;
}

void module_fit_width(Module *module)
{
    module->w = module->parent->w - module->parent->px - module->parent->px;
}

void module_fit_right(Module *module, Module *ref)
{
    module->w = module->parent->w - 2 * module->parent->px - ref->w - 2 * ref->px;
}

void module_draw(Display *display, Module *module)
{
    int charH;
    int radius;
    if (!module->visible)
    {
        return;
    }

    switch (module->type)
    {
    case MODULE_TEXT:
    {
        ModuleText *moduleText = (ModuleText *)module->data;

        // Blank module to avoid flickering
        Module *blank = module_create(module->parent);
        if (moduleText->maxChar != 0)
        {
            module_copy(blank, module);

            char *emptyString = (char *)malloc(moduleText->maxChar + 1);
            strcpy(emptyString, "");
            for (int i = 0; i < moduleText->maxChar; i++)
            {
                strcat(emptyString, " ");
            }
            printf(",%s,\n", emptyString);
            module_set_text(blank, emptyString);
            module_set_font(blank, moduleText->font);
            free(emptyString);
        }

        // Testing dynamic alignment during the drawing phase, only Text modules are supported
        module_set_font(module, moduleText->font); // Recalculate w,h
        switch (moduleText->alignment)
        {
        case MODULE_TEXT_ALIGN_LEFT:
            module_align_inner_left(module);
            module_align_inner_left(blank);
            break;
        case MODULE_TEXT_ALIGN_CENTER:
            module_align_center(module);
            module_align_center(blank);
            break;
        case MODULE_TEXT_ALIGN_RIGHT:
            module_align_inner_right(module);
            module_align_inner_right(blank);
            break;
        }

        display_set_text_parameter1(display, RA8876_SELECT_INTERNAL_CGROM, moduleText->font, RA8876_SELECT_8859_1);
        display_set_text_parameter2(display, RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
        display_text_color(display, module->foregroundColor, module->backgroundColor);

        // Draw blank string to remove previous text (dynamic alignment only)
        module_draw(display, blank);
        module_trim(blank);
        module_destroy(blank);

        display_draw_string(display, module->x, module->y, moduleText->text);
        break;
    }
    case MODULE_IMAGE:
    {
        display_bte_memory_copy_image(display, (Image *)module->data, module->x, module->y);
        break;
    }
    case MODULE_LINE:
    {
        display_draw_line(display, module->x, module->y, module->x + module->w, module->y, module->foregroundColor);
        break;
    }
    case MODULE_RECTANGLE:
    {
        display_draw_square_fill(display, module->x, module->y, module->x + module->w, module->y + module->h, module->foregroundColor);
        break;
    }
    case MODULE_RECTANGLE_CIRCLE:
    {
        radius = round(module->w * 0.07);
        display_draw_circle_square_fill(display, module->x, module->y, module->x + module->w, module->y + module->h, radius, radius, module->foregroundColor);
        break;
    }
    case MODULE_GRAPH:
    {
        Graph *graph = (Graph *)module->data;

        double factor = (double)graph->graphArea->w / (double)graph->dataCount;
        double range = graph->maxY - graph->minY;
        display_bte_memory_copy(display, PAGE1_START_ADDR, SCREEN_WIDTH, graph->graphArea->x + 4, graph->graphArea->y, PAGE1_START_ADDR, SCREEN_WIDTH, graph->graphArea->x + 2, graph->graphArea->y, graph->graphArea->w - 2, graph->graphArea->h);
        display_draw_square_fill(display, graph->graphArea->x + graph->graphArea->w, graph->graphArea->y, graph->graphArea->x + graph->graphArea->w + 2, graph->graphArea->y + graph->graphArea->h, module->parent->foregroundColor);
        int y = graph->graphArea->y + graph->graphArea->h / 2 + (int)(graph->data[0] * (double)((graph->graphArea->h / 2) / (range / 2)));
        int lastY = graph->graphArea->y + graph->graphArea->h / 2 + (int)(graph->data[1] * (double)((graph->graphArea->h / 2) / (range / 2)));
        display_draw_line(display, graph->graphArea->x + graph->graphArea->w, lastY, graph->graphArea->x + graph->graphArea->w + 2, y, COLOR65K_RED);
        break;
    }
    }

    for (int i = 0; i < module->numChildren; i++)
    {
        module_draw(display, module->child[i]);
    }
}

void module_destroy_children(Module *module)
{
    for (int i = 0; i < module->numChildren; i++)
    {
        Module *toDestroy = module->child[i];
        module_trim(toDestroy);    // Remove child from parent
        module_destroy(toDestroy); // Destroy child
    }
    module->numChildren = 0;
}

void module_trim(Module *module)
{
    // Remove module from parents children
    if (module->parent != NULL)
    {
        for (int i = 0; i < module->parent->numChildren; i++)
        {
            if (module->parent->child[i] == module)
            {
                for (int j = i; j < module->parent->numChildren - 1; j++)
                {
                    module->parent->child[j] = module->parent->child[j + 1];
                }
                break;
            }
        }
        module->parent->numChildren--;
        module->parent->child = realloc(module->parent->child, sizeof(Module *) * module->parent->numChildren);
    }
}

void module_text_destroy(ModuleText *text)
{
    free(text->text);
    free(text);
}

void module_destroy(Module *root)
{

    // Free module children
    int children = root->numChildren;
    for (int i = 0; i < children; i++)
    {
        module_destroy(root->child[i]);
    }

    // Free module
    switch (root->type)
    {
    case MODULE_TEXT:
    {
        module_text_destroy(root->data);
        break;
    }
    default:
        free(root->data);
        break;
    }

    free(root->child);
    free(root);
}
