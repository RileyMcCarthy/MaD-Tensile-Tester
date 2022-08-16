#include "Graph.h"
#include "Module.h"
#include <string.h>
static void module_graph_draw(Display *display, Module *module)
{
    ModuleGraph *graph = (ModuleGraph *)module->data.ptr;

    double range = graph->maxY - graph->minY;
    display_bte_memory_copy(display, PAGE1_START_ADDR, SCREEN_WIDTH, graph->graphArea.x + 4, graph->graphArea.y, PAGE1_START_ADDR, SCREEN_WIDTH, graph->graphArea.x + 2, graph->graphArea.y, graph->graphArea.w - 2, graph->graphArea.h);
    display_draw_square_fill(display, graph->graphArea.x + graph->graphArea.w, graph->graphArea.y, graph->graphArea.x + graph->graphArea.w + 2, graph->graphArea.y + graph->graphArea.h, module->parent->foregroundColor);
    int y = graph->graphArea.y + graph->graphArea.h / 2 - (int)(graph->data[0] * (double)((graph->graphArea.h / 2) / (range / 2)));
    int lastY = graph->graphArea.y + graph->graphArea.h / 2 - (int)(graph->data[1] * (double)((graph->graphArea.h / 2) / (range / 2)));
    display_draw_line(display, graph->graphArea.x + graph->graphArea.w, lastY, graph->graphArea.x + graph->graphArea.w + 2, y, COLOR65K_RED);
}

static void module_graph_redraw(Display *display, Module *module, void *arg)
{
    ModuleGraph *graph = (ModuleGraph *)module->data.ptr;
    double newData = *(graph->dataPtr);
    // serial_debug("Redrawing graph with newData: %f\n", newData);
    if (newData > graph->maxY || newData < graph->minY)
    {
        return;
    }
    graph->data[1] = graph->data[0];
    graph->data[0] = newData;
    module_graph_draw(display, module);
}

void module_set_graph(Module *window, ModuleGraph *graph, const char *title, const char *units, double *data)
{
    window->type = MODULE_GRAPH;

    window->draw = module_graph_draw;
    window->reDraw = module_graph_redraw;

    window->data.ptr = graph;
    graph->initialDraw = true;

    graph->dataPtr = data;

    strncpy(graph->title, title, MAX_GRAPH_TITLE_LENGTH);

    strncpy(graph->units, units, MAX_GRAPH_UNITS_LENGTH);

    // Give parent reference to graph modules
    // Initialize Title
    module_init(&(graph->graphTitle), window);
    module_set_margin(&(graph->graphTitle), 10, 10);
    module_set_text(&(graph->graphTitle), graph->title);
    module_text_font(&(graph->graphTitle), RA8876_CHAR_HEIGHT_32);
    module_text_fit(&(graph->graphMinText));
    module_set_color(&(graph->graphTitle), MAINTEXTCOLOR, graph->graphTitle.parent->foregroundColor);
    module_align_inner_top(&(graph->graphTitle));
    module_align_center(&(graph->graphTitle));
    module_text_underline(&(graph->graphTitle));

    // Initialize min text
    module_init(&(graph->graphMinText), window);
    sprintf(graph->minyBuffer, "%.2f", graph->minY);
    module_set_margin(&(graph->graphMinText), 3, 3);
    module_set_text(&(graph->graphMinText), graph->minyBuffer);
    module_text_font(&(graph->graphMinText), RA8876_CHAR_HEIGHT_16);
    module_text_fit(&(graph->graphMinText));
    module_set_color(&(graph->graphMinText), MAINTEXTCOLOR, graph->graphMinText.parent->foregroundColor);
    module_align_inner_bottom(&(graph->graphMinText));
    module_align_inner_left(&(graph->graphMinText));
    module_text_align(&(graph->graphMinText), MODULE_TEXT_ALIGN_INNER_LEFT);

    // Initialize max text
    sprintf(graph->maxyBuffer, "%.2f", graph->maxY);
    module_init(&(graph->graphMaxText), window);
    module_set_margin(&(graph->graphMaxText), 3, 3);
    module_set_text(&(graph->graphMaxText), graph->maxyBuffer);
    module_text_font(&(graph->graphMaxText), RA8876_CHAR_HEIGHT_16);
    module_text_fit(&(graph->graphMaxText));
    module_align_inner_left(&(graph->graphMaxText));
    module_text_align(&(graph->graphMaxText), MODULE_TEXT_ALIGN_INNER_LEFT);
    module_set_color(&(graph->graphMaxText), MAINTEXTCOLOR, graph->graphMaxText.parent->foregroundColor);
    module_align_below(&(graph->graphMaxText), &(graph->graphTitle));

    // Initialize ModuleGraph Area
    module_init(&(graph->graphArea), window);
    module_align_below(&(graph->graphArea), &(graph->graphTitle));
    module_fit_below(&(graph->graphArea), &(graph->graphTitle));
    module_fit_right(&(graph->graphArea), &(graph->graphMinText));
    module_align_right(&(graph->graphArea), &(graph->graphMinText));
    module_set_color(&(graph->graphArea), graph->graphArea.parent->foregroundColor, graph->graphArea.parent->backgroundColor);

    // Initialize Units
    module_init(&(graph->grapUnits), window);
    module_set_margin(&(graph->grapUnits), 10, 10);
    module_set_text(&(graph->grapUnits), graph->units);
    module_text_font(&(graph->grapUnits), RA8876_CHAR_HEIGHT_32);
    module_set_color(&(graph->grapUnits), MAINTEXTCOLOR, graph->grapUnits.parent->foregroundColor);
    module_align_inner_top(&(graph->grapUnits));
    module_align_inner_left(&(graph->grapUnits));

    // Initialize ModuleGraph Horizontal Line
    module_init(&(graph->graphCenterLine), &(graph->graphArea));
    module_set_rectangle(&(graph->graphCenterLine), 0, 0);
    module_fit_width(&(graph->graphCenterLine));
    module_align_center(&(graph->graphCenterLine));
    module_align_middle(&(graph->graphCenterLine));
    module_set_color(&(graph->graphCenterLine), MAINTEXTCOLOR, MAINTEXTCOLOR);

    // Initialize ModuleGraph Vertical Line
    module_init(&(graph->graphVerticleLine), &(graph->graphArea));
    module_set_rectangle(&(graph->graphVerticleLine), 0, 0);
    module_fit_height(&(graph->graphVerticleLine));
    module_align_inner_left(&(graph->graphVerticleLine));
    module_align_inner_top(&(graph->graphVerticleLine));
    module_set_color(&(graph->graphVerticleLine), MAINTEXTCOLOR, MAINTEXTCOLOR);
}

void module_graph_insert(Module *module, double value)
{
    ModuleGraph *graph = (ModuleGraph *)module->data.ptr;
    if (value > graph->maxY || value < graph->minY)
    {
        return;
    }
    graph->data[1] = graph->data[0];
    graph->data[0] = value;
}

void module_graph_set_range(Module *module, double maxY, double minY)
{
    ModuleGraph *graph = (ModuleGraph *)module->data.ptr;
    graph->maxY = maxY;
    graph->minY = minY;
}

void module_graph_add_marker(Module *module, double value)
{
    ModuleGraph *graph = (ModuleGraph *)module->data.ptr;
    if (graph->markerCount < MAX_GRAPH_MARKERS)
    {
        graph->markerY[graph->markerCount] = value;
        graph->markerCount++;
    }
}

int module_graph_get_max_data(Module *module)
{
    ModuleGraph *graph = (ModuleGraph *)module->data.ptr;
    return graph->graphArea.w / 2;
}
