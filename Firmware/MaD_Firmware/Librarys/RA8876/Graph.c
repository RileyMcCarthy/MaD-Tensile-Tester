#include "Graph.h"
#include "Module.h"
Graph *graph_create(Display *display, int startX, int startY, int width, int height, float maxY, float minY, const char *units, const char *title)
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->display = display;
    graph->startX = startX;
    graph->startY = startY;
    graph->width = width;
    graph->height = height;
    graph->maxY = maxY;
    graph->minY = minY;
    graph->lastX = NULL;
    graph->lastY = NULL;
    graph->markerCount = 0;
    graph->units = (char *)malloc(strlen(units) + 1);
    strcpy(graph->units, units);
    graph->title = (char *)malloc(sizeof(char) * (strlen(title) + 1));
    strcpy(graph->title, title);
    return graph;
}

void graph_destroy(Graph *graph)
{
    free(graph->title);
    free(graph->units);
    free(graph);
}

void graph_add_marker(Graph *graph, float value)
{
    if (graph->markerCount < MAX_GRAPH_MARKERS)
    {
        graph->markerY[graph->markerCount] = value;
        graph->markerCount++;
    }
}

bool graph_draw_static(Graph *graph, double *data, int numdata)
{
    // Create Background
    Module *root = module_create(NULL);

    // Create Window
    Module *window = module_create(root);
    module_set_rectangle_circle(window, graph->width, graph->height);
    module_set_position(window, explorer->x, explorer->y);
    module_set_padding(window, 10, 10);
    module_set_color(window, COLOR65K_LIGHTBLUE, COLOR65K_BLUE);

    // Create Title
    Module *title = module_create(window);
    module_set_padding(title, 10, 10);
    module_set_text(title, graph->title);
    module_set_font(title, RA8876_CHAR_HEIGHT_32);
    module_set_color(title, COLOR65K_BLACK, title->parent->foregroundColor);
    module_align_inner_top(title);
    module_align_center(title);
    module_add_underline(title);

    // Create Graph Area
    Module *graphArea = module_create(window);
    module_align_below(graphArea, title);
    module_fit_below(graphArea, title);
    module_fit_width(graphArea);
    module_align_inner_left(graphArea);
    module_set_color(graphArea, graphArea->parent->foregroundColor, graphArea->parent->backgroundColor);
    module_set_padding(graphArea, 0, 0);

    // Create Graph Horizontal Line
    Module *centerLine = module_create(graphArea);
    module_set_line(centerLine, 0);
    module_fit_width(centerLine);
    module_align_center(centerLine);
    module_align_middle(centerLine);
    module_set_color(centerLine, COLOR65K_BLACK, COLOR65K_BLACK);

    // Create Graph Vertical Line
    Module *verticleLine = module_create(graphArea);
    module_set_line(verticleLine, 0);
    module_fit_height(verticleLine);
    module_align_inner_left(verticleLine);
    module_set_color(verticleLine, COLOR65K_BLACK, COLOR65K_BLACK);

    module_draw(graph->display, root);
    module_destroy(root);
}

// make graph an object so it has memory!
bool graph_draw(Graph *graph, Display *display, float newValue)
{
    display_bte_memory_copy(display, PAGE1_START_ADDR, SCREEN_WIDTH, graph->startX + 2, graph->startY, PAGE1_START_ADDR, SCREEN_WIDTH, graph->startX, graph->startY, graph->width, graph->height);
    display_draw_line(display, graph->startX, graph->startY, graph->startX, graph->startY + graph->height, COLOR65K_BLACK);
    display_draw_line(display, graph->startX, graph->startY + graph->height, graph->startX + graph->width, graph->startY + graph->height, COLOR65K_BLACK);

    // Draw markerts

    int relativeHeight = ((newValue - graph->minY) / (graph->maxY - graph->minY)) * (graph->height); // Scale new value to graphs height
    if (relativeHeight < 0)
    {
        relativeHeight = 0;
    }
    else if (relativeHeight > graph->height)
    {
        relativeHeight = graph->height;
    }
    int x = graph->startX + graph->width;
    int y = (int)(graph->startY + graph->height - relativeHeight);

    if (graph->lastX == NULL)
    {
        printf("init\n");
        graph->lastX = x;
        graph->lastY = y;
    }

    // Erase old information
    display_draw_line(display, graph->lastX - 2, graph->startY, graph->lastX - 2, graph->startY + graph->height, MAINCOLOR);
    display_draw_line(display, x, graph->startY, x, graph->startY + graph->height, MAINCOLOR);
    // Draw new value
    display_draw_line(display, graph->lastX - 2, graph->lastY, x, y, COLOR65K_GREEN);

    // draw markers
    for (int i = 0; i < graph->markerCount; i++)
    {
        int markerY = (int)(graph->startY + graph->height - ((graph->markerY[i] - graph->minY) / (graph->maxY - graph->minY)) * (graph->height));
        display_draw_line(display, graph->startX, markerY, graph->startX + graph->width, markerY, COLOR65K_RED);
    }

    display_text_color(display, MAINTEXTCOLOR, MAINCOLOR);

    display_draw_string(display, graph->startX + graph->width / 2 - strlen(graph->title) * 6, graph->startY - 20, graph->title);
    display_text_color(display, COLOR65K_BLACK, MAINCOLOR);

    char buf[20];
    sprintf(buf, "%0.1f%s", graph->maxY, graph->units);
    display_draw_string(display, graph->startX, graph->startY - 20, buf);
    sprintf(buf, "%0.1f%s", graph->minY, graph->units);
    display_draw_string(display, graph->startX, graph->startY + 4 + graph->height, buf);
    graph->lastX = x;
    graph->lastY = y;
    return true;
}
