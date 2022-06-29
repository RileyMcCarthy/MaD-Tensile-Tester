#include "Module.h"

#define ANIMATION_PAGE PAGE4_START_ADDR

static void module_animation_touch(Display *display, Module *module)
{
    int moveY = 5;
    int speedms = 50; // animation time
    int bufferPx = 2;

    int x = module->x - module->borderWidth - bufferPx;
    int y = module->y - module->borderWidth - bufferPx;
    int w = module->w + module->borderWidth * 2 + bufferPx * 2;
    int h = module->h + module->borderWidth * 2 + bufferPx * 2;

    // Save current state
    display_bte_memory_copy(display, PAGE1_START_ADDR, SCREEN_WIDTH, x, y, ANIMATION_PAGE, SCREEN_WIDTH, x, y, w, h + moveY);

    // Move Down
    int i = 0;
    for (i = 1; i < moveY; i++)
    {
        display_bte_memory_copy(display, ANIMATION_PAGE, SCREEN_WIDTH, x, y, PAGE1_START_ADDR, SCREEN_WIDTH, x, y + i, w, h);
        display_draw_line(display, x, y + i, x + w, y + i, module->parent->foregroundColor);
        _waitms(speedms / moveY);
    }

    // Move Down
    for (; i > 0; i--)
    {
        display_bte_memory_copy(display, ANIMATION_PAGE, SCREEN_WIDTH, x, y, PAGE1_START_ADDR, SCREEN_WIDTH, x, y + i, w, h);
        display_draw_line(display, x, y + i, x + w, y + i, module->parent->foregroundColor);
        _waitms(speedms / moveY);
    }

    // Paste current state
    display_bte_memory_copy(display, ANIMATION_PAGE, SCREEN_WIDTH, x, y, PAGE1_START_ADDR, SCREEN_WIDTH, x, y, w, h + moveY);
}

void module_animation_switch_page_right(Display *display, Module *module)
{
    int speedms = 100; // animation time
    int movePx = 100;
    for (int i = 0; i < SCREEN_WIDTH; i += movePx)
    {
        display_bte_memory_copy(display, ANIMATION_PAGE, SCREEN_WIDTH, i, 0, PAGE1_START_ADDR, SCREEN_WIDTH, SCREEN_WIDTH - movePx, 0, movePx, SCREEN_HEIGHT);   // Copy new page in
        display_bte_memory_copy(display, PAGE1_START_ADDR, SCREEN_WIDTH, movePx, 0, PAGE1_START_ADDR, SCREEN_WIDTH, 0, 0, SCREEN_WIDTH - movePx, SCREEN_HEIGHT); // Move current page right
    }
    display_bte_memory_copy(display, ANIMATION_PAGE, SCREEN_WIDTH, 0, 0, PAGE1_START_ADDR, SCREEN_WIDTH, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); // Copy new page in
}

void module_animation_switch_page_up(Display *display, Module *module)
{
    int speedms = 100; // animation time
    int movePx = 100;

    for (int i = movePx; i < SCREEN_HEIGHT - movePx; i += movePx)
    {
        display_bte_memory_copy(display, ANIMATION_PAGE, SCREEN_WIDTH, 0, i, PAGE1_START_ADDR, SCREEN_WIDTH, 0, SCREEN_HEIGHT - movePx, SCREEN_WIDTH, movePx);   // Copy new page in
        display_bte_memory_copy(display, PAGE1_START_ADDR, SCREEN_WIDTH, 0, movePx, PAGE1_START_ADDR, SCREEN_WIDTH, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT - movePx); // Move current page up
    }
    display_bte_memory_copy(display, ANIMATION_PAGE, SCREEN_WIDTH, 0, 0, PAGE1_START_ADDR, SCREEN_WIDTH, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); // Copy new page in
}

void module_animation_switch_page_down(Display *display, Module *module)
{
    int speedms = 100; // animation time
    int movePx = 40;
    for (int i = movePx; i < SCREEN_HEIGHT - movePx; i += movePx)
    {
        display_bte_memory_copy(display, ANIMATION_PAGE, SCREEN_WIDTH, 0, SCREEN_HEIGHT - i, PAGE1_START_ADDR, SCREEN_WIDTH, 0, 0, SCREEN_WIDTH, i); // Copy new page in
        // display_bte_memory_copy(display, PAGE1_START_ADDR, SCREEN_WIDTH, 0, 0, PAGE1_START_ADDR, SCREEN_WIDTH, 0, movePx, SCREEN_WIDTH, SCREEN_HEIGHT - movePx); // Move current page down
    }
    display_bte_memory_copy(display, ANIMATION_PAGE, SCREEN_WIDTH, 0, 0, PAGE1_START_ADDR, SCREEN_WIDTH, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); // Copy new page in
}

Module *module_create(Module *parent)
{
    Module *module = (Module *)malloc(sizeof(Module));
    module->parent = parent;
    module->type = MODULE_ROOT;
    module->numChildren = 0;
    module->visible = true;

    module->lastPressed = 0;
    module->debouncems = 100;
    module->onTouch = NULL;
    module->onUpdate = NULL;
    module->draw = NULL;
    module->r = -1;
    module->borderWidth = 0;
    module->touchAnimate = module_animation_touch;
    module->drawAnimate = NULL;

    if (parent != NULL)
    {
        parent->numChildren++;
        parent->child[parent->numChildren - 1] = module;
    }
    return module;
}

void module_init(Module *module, Module *parent)
{
    module->parent = parent;
    module->type = MODULE_ROOT;
    module->numChildren = 0;
    module->visible = true;

    module->lastPressed = 0;
    module->debouncems = 100;
    module->onTouch = NULL;
    module->onUpdate = NULL;
    module->draw = NULL;

    module->x = 0;
    module->y = 0;
    module->w = 0;
    module->h = 0;
    module->px = 0;
    module->py = 0;
    module->mx = 0;
    module->my = 0;
    module->r = -1;
    module->borderWidth = 0;
    module->touchAnimate = module_animation_touch;
    module->drawAnimate = NULL;

    if (parent != NULL)
    {
        if (parent->numChildren < MODULE_MAX_CHILDREN)
        {
            parent->numChildren++;
            parent->child[parent->numChildren - 1] = module;
        }
        else
        {
            printf("Error: Too many children\n");
        }
    }
}

void module_update_callback(Module *module, void (*onUpdate)(Display *display, Module *module, void *arg))
{
    module->onUpdate = onUpdate;
}

void module_update_check(Display *display, Module *module, void *arg)
{
    if (module->type == MODULE_WINDOW)
    {
        arg = module->data.ptr;
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
    if (display->locationCount == 0 || !root->visible)
    {
        return 0;
    }
    if (root->type == MODULE_WINDOW)
    {
        arg = root->data.ptr;
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
                if (root->touchAnimate != NULL)
                {
                    root->touchAnimate(display, root);
                }
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
    to->mx = from->mx;
    to->my = from->my;
    to->r = from->r;
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

void module_animate_draw(Module *module, void (*drawAnimate)(Display *display, struct Module_s *module))
{
    module->drawAnimate = drawAnimate;
}

void module_set_window(Module *module, void *window)
{
    module->type = MODULE_WINDOW;
    module->data.ptr = window;
}

void module_set_line(Module *module, int w)
{
    module_set_line_one(module, w, 0);
}

void module_set_line_one(Module *module, int x, int y)
{
    module->type = MODULE_LINE;
    module->w = x;
    module->h = y;
}

void module_set_rectangle(Module *module, int w, int h)
{
    module->type = MODULE_RECTANGLE;
    module->w = w;
    module->h = h;
}

void module_set_rectangle_circle(Module *module, int w, int h)
{
    module->type = MODULE_RECTANGLE_CIRCLE;
    module->w = w;
    module->h = h;
}

void module_set_radius(Module *module, int r)
{
    module->r = r;
}

void module_set_image(Module *module, Image *image)
{
    module->type = MODULE_IMAGE;
    module->data.image = image;
    module->w = image->width;
    module->h = image->height;
}

void module_text_underline(Module *module)
{
    if (module->type != MODULE_TEXT)
        return;
    module->data.text.underline = true;
}

// Text is only a reference, not a copy
void module_set_text(Module *module, char *text)
{
    module->type = MODULE_TEXT;
    module->data.text.value = text;
    module->data.text.font = RA8876_CHAR_HEIGHT_24;
    module->data.text.alignmentH = MODULE_TEXT_ALIGN_INNER_LEFT;
    module->data.text.alignmentV = MODULE_TEXT_ALIGN_VTOP;
    module->data.text.underline = false;
    module_text_fit(module);
}

void module_set_text2(Module *module, char *text)
{
    module->type = MODULE_TEXT;
    module->data.text.value = text;
    module->data.text.font = RA8876_CHAR_HEIGHT_24;
    module->data.text.alignmentH = MODULE_TEXT_ALIGN_INNER_LEFT;
    module->data.text.alignmentV = MODULE_TEXT_ALIGN_VTOP;
    module->data.text.underline = false;
}

void module_text_update(Module *module, char *text)
{
    module->data.text.value = text;
}

char *module_text_get(Module *module)
{
    if (module->type == MODULE_TEXT)
    {
        return module->data.text.value;
    }
    return NULL;
}

void module_text_set(Module *module, char *text)
{
    if (module->type == MODULE_TEXT)
    {
        module->data.text.value = text;
    }
}

void module_text_fit(Module *module)
{
    switch (module->data.text.font)
    {
    case RA8876_CHAR_HEIGHT_32:
        module->w = strlen(module->data.text.value) * 16;
        module->h = 32;
        break;
    case RA8876_CHAR_HEIGHT_24:
        module->w = strlen(module->data.text.value) * 12;
        module->h = 24;
        break;
    case RA8876_CHAR_HEIGHT_16:
        module->w = strlen(module->data.text.value) * 8;
        module->h = 16;
        break;
    }
    if (module->data.text.underline)
    {
        module->h += 2;
    }
}

void module_text_align(Module *module, ModuleTextAignType alignment)
{
    if (module->type != MODULE_TEXT)
        return;
    module->data.text.alignmentH = alignment;
}
void module_text_align_verticle(Module *module, ModuleTextAignType alignment)
{
    if (module->type != MODULE_TEXT)
        return;
    module->data.text.alignmentV = alignment;
}

int module_text_font_width(Module *module)
{
    if (module->type != MODULE_TEXT)
        return 0;
    switch (module->data.text.font)
    {
    case RA8876_CHAR_HEIGHT_32:
        return strlen(module->data.text.value) * 16;
    case RA8876_CHAR_HEIGHT_24:
        return strlen(module->data.text.value) * 12;
    case RA8876_CHAR_HEIGHT_16:
        return strlen(module->data.text.value) * 8;
    }
    return 0;
}

int module_text_font_height(Module *module)
{
    if (module->type != MODULE_TEXT)
        return 0;
    switch (module->data.text.font)
    {
    case RA8876_CHAR_HEIGHT_32:
        return 32;
    case RA8876_CHAR_HEIGHT_24:
        return 24;
    case RA8876_CHAR_HEIGHT_16:
        return 16;
    }
    return 0;
}

// Sets modules w/h from font size. Font size: 32,24,16 or -1 for auto
void module_text_font(Module *module, int font)
{
    if (module->type != MODULE_TEXT)
        return;

    if (font == -1)
    {
        if (module->h >= 32)
        {
            if (module->w > (int)strlen(module->data.text.value) * 16)
            {
                module->data.text.font = RA8876_CHAR_HEIGHT_32;
            }
            else if (module->w > (int)strlen(module->data.text.value) * 12)
            {
                module->data.text.font = RA8876_CHAR_HEIGHT_24;
            }
            else
            {
                module->data.text.font = RA8876_CHAR_HEIGHT_16;
            }
        }
        else if (module->parent->h >= 24)
        {
            if (module->parent->w > (int)strlen(module->data.text.value) * 12)
            {
                module->data.text.font = RA8876_CHAR_HEIGHT_24;
            }
            else
            {
                module->data.text.font = RA8876_CHAR_HEIGHT_16;
            }
        }
        else
        {
            module->data.text.font = RA8876_CHAR_HEIGHT_16;
        }
    }
    else
    {
        module->data.text.font = font;
    }
}

void module_set_padding(Module *module, int px, int py)
{
    module->px = px;
    module->py = py;
}

void module_set_margin(Module *module, int mx, int my)
{
    module->mx = mx;
    module->my = my;
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

void module_add_border(Module *module, int color, int width)
{
    module->borderColor = color;
    module->borderWidth = width;
    module->w += width * 2;
    module->h += width * 2;
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
    module->x = module->parent->x + section * (module->parent->w - sections * module->w) / (sections + 1) + (section - 1) * module->w;
}

void module_align_space_even_verticle(Module *module, int section, int sections)
{
    module->y = module->parent->y + section * (module->parent->h - sections * module->h) / (sections + 1) + (section - 1) * module->h;
}

void module_align_above(Module *module, Module *ref)
{
    module->y = ref->y - module->h - ref->my;
}

void module_align_left(Module *module, Module *ref)
{
    module->x = ref->x - module->w - ref->mx;
}

void module_align_below(Module *module, Module *ref)
{
    module->y = ref->y + ref->h + ref->my;
}

void module_align_right(Module *module, Module *ref)
{
    module->x = ref->x + ref->w + ref->mx;
}

void module_fit_space_even(Module *module, int sections)
{
    module->w = (module->parent->w - 2 * module->parent->mx - (sections - 0) * module->mx) / sections;
}

void module_fit_space_even_verticle(Module *module, int sections)
{
    module->h = (module->parent->h - 2 * module->parent->my - (sections - 0) * module->my) / sections;
}

void module_fit_height(Module *module)
{
    module->h = module->parent->h - module->parent->py - module->parent->py;
}

void module_fit_below(Module *module, Module *ref)
{
    module_align_below(module, ref);
    module->h = module->parent->h - (ref->y - module->parent->y) - ref->h - ref->my - module->parent->py;
}

void module_fit_width(Module *module)
{
    module->w = module->parent->w - module->parent->px - module->parent->px;
}

void module_fit_right(Module *module, Module *ref)
{
    module->w = module->parent->w - (ref->x - module->parent->x) - ref->w - ref->mx - module->parent->px;
}

static void draw_border_rectangle(Display *display, Module *module, int width)
{
    if (module->borderWidth == 0)
        return;
    display_draw_square_fill(display, module->x, module->y, module->x + module->w, module->y + module->h, module->borderColor);
    display_draw_square_fill(display, module->x + width, module->y + width, module->x + module->w - width, module->y + module->h - width, module->foregroundColor);
}

static int get_radius(Module *module)
{
    if (module->r == -1)
        return round(module->w * 0.07);
    else
        return module->r;
}

static bool draw_border_rectangle_rounded(Display *display, Module *module, int innerColor, int radius)
{
    if (module->borderWidth == 0)
        return false;
    int width = module->borderWidth;
    display_draw_circle_square_fill(display, module->x - width, module->y - width, module->x + module->w + width, module->y + module->h + width, radius, radius, module->borderColor);
    display_draw_circle_square_fill(display, module->x, module->y, module->x + module->w, module->y + module->h, radius, radius, innerColor);
    return true;
}

void module_draw(Display *display, Module *module)
{
    int charH;
    int radius;
    if (!module->visible)
    {
        return;
    }
    if (module->drawAnimate != NULL)
    {
        display_canvas_image_start_address(display, ANIMATION_PAGE);
    }
    switch (module->type)
    {
    case MODULE_TEXT:
    {
        int x = 0;
        int y = module->y;
        int width = module_text_font_width(module);
        int height = module_text_font_height(module);
        switch (module->data.text.alignmentH)
        {
        case MODULE_TEXT_ALIGN_INNER_LEFT:
            x = module->x + module->px;
            break;
        case MODULE_TEXT_ALIGN_INNER_CENTER:
            x = module->x + (module->w - width) / 2;
            break;
        case MODULE_TEXT_ALIGN_INNER_RIGHT:
            x = module->x + module->w - width - module->px;
            break;
        }

        switch (module->data.text.alignmentV)
        {
        case MODULE_TEXT_ALIGN_VTOP:
            y = module->y + module->py;
            break;
        case MODULE_TEXT_ALIGN_VCENTER:
            y = module->y + (module->h - height) / 2;
            break;
        case MODULE_TEXT_ALIGN_VBOTTOM:
            y = module->y + module->h - height - module->py;
            break;
        }

        display_set_text_parameter1(display, RA8876_SELECT_INTERNAL_CGROM, module->data.text.font, RA8876_SELECT_8859_1);
        display_set_text_parameter2(display, RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);

        // Clear the text area then draw text
        if (!draw_border_rectangle_rounded(display, module, module->backgroundColor, get_radius(module)))
        {
            display_draw_square_fill(display, module->x, module->y, module->x + module->w, module->y + module->h, module->backgroundColor);
        }
        display_text_color(display, module->foregroundColor, module->backgroundColor);
        display_draw_string(display, x, y, module->data.text.value);
        if (module->data.text.underline)
            display_draw_line(display, x, y + height + 1, x + width, y + height + 1, module->foregroundColor);
        break;
    }
    case MODULE_IMAGE:
    {
        display_bte_memory_copy_image(display, module->data.image, module->x, module->y);
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
        radius = get_radius(module);
        draw_border_rectangle_rounded(display, module, module->foregroundColor, radius);
        display_draw_circle_square_fill(display, module->x, module->y, module->x + module->w, module->y + module->h, radius, radius, module->foregroundColor);
        break;
    }
    case MODULE_GRAPH:
    {
        module->draw(display, module);
        break;
    }
    case MODULE_KEYBOARD:
    {
        module->draw(display, module);
        break;
    }
    }

    for (int i = 0; i < module->numChildren; i++)
    {
        module_draw(display, module->child[i]);
    }
    if (module->drawAnimate != NULL)
    {
        module->drawAnimate(display, module);
        display_canvas_image_start_address(display, PAGE1_START_ADDR);
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
    }
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
        break;
    }
    default:
        break;
    }

    free(root);
}
