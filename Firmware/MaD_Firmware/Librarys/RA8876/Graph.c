#include "Graph.h"
#include "Module.h"

void module_graph_draw(Display *display, Module *module)
{
    Graph *graph = (Graph *)module->data;

    double range = graph->maxY - graph->minY;
    display_bte_memory_copy(display, PAGE1_START_ADDR, SCREEN_WIDTH, graph->graphArea->x + 4, graph->graphArea->y, PAGE1_START_ADDR, SCREEN_WIDTH, graph->graphArea->x + 2, graph->graphArea->y, graph->graphArea->w - 2, graph->graphArea->h);
    display_draw_square_fill(display, graph->graphArea->x + graph->graphArea->w, graph->graphArea->y, graph->graphArea->x + graph->graphArea->w + 2, graph->graphArea->y + graph->graphArea->h, module->parent->foregroundColor);
    int y = graph->graphArea->y + graph->graphArea->h / 2 - (int)(graph->data[0] * (double)((graph->graphArea->h / 2) / (range / 2)));
    int lastY = graph->graphArea->y + graph->graphArea->h / 2 - (int)(graph->data[1] * (double)((graph->graphArea->h / 2) / (range / 2)));
    display_draw_line(display, graph->graphArea->x + graph->graphArea->w, lastY, graph->graphArea->x + graph->graphArea->w + 2, y, COLOR65K_RED);
}

void module_set_graph(Module *window, const char *titleName, const char *units)
{
    window->type = MODULE_GRAPH;

    Graph *graph = (Graph *)malloc(sizeof(Graph));
    window->draw = module_graph_draw;

    graph->title = malloc(strlen(titleName) + 1);
    strcpy(graph->title, titleName);

    graph->units = malloc(strlen(units) + 1);
    strcpy(graph->units, units);

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
    module_set_text(minText, "00.00");
    module_text_max_char(minText, 5);
    module_set_font(minText, RA8876_CHAR_HEIGHT_16);
    module_set_color(minText, COLOR65K_BLACK, minText->parent->foregroundColor);
    module_align_inner_bottom(minText);
    module_align_inner_left(minText);
    module_text_align(minText, MODULE_TEXT_ALIGN_INNER_LEFT);

    // Create max
    Module *maxText = module_create(window);
    module_set_padding(maxText, 3, 3);
    module_set_text(maxText, "00.00");
    module_text_max_char(maxText, 5);
    module_set_font(maxText, RA8876_CHAR_HEIGHT_16);
    module_align_inner_left(maxText);
    module_text_align(maxText, MODULE_TEXT_ALIGN_INNER_LEFT);
    module_set_color(maxText, COLOR65K_BLACK, maxText->parent->foregroundColor);
    module_align_below(maxText, title);

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
    /*Module *zeroText = module_create(graphArea);
    module_set_padding(zeroText, 3, 3);
    module_set_text(zeroText, "0");
    module_set_font(zeroText, RA8876_CHAR_HEIGHT_16);
    module_set_color(zeroText, COLOR65K_BLACK, zeroText->parent->foregroundColor);
    module_align_left(zeroText, graphArea);
    module_align_middle(zeroText);*/

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

    window->data = graph;
}

void module_graph_insert(Module *module, double value)
{
    Graph *graph = (Graph *)module->data;
    if (value > graph->maxY || value < graph->minY)
    {
        return;
    }
    graph->data[1] = graph->data[0];
    graph->data[0] = value;
}

void module_graph_set_range(Module *module, double maxY, double minY)
{
    Graph *graph = (Graph *)module->data;
    graph->maxY = maxY;
    graph->minY = minY;
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

int module_graph_get_max_data(Module *module)
{
    Graph *graph = (Graph *)module->data;
    return graph->graphArea->w / 2;
}
