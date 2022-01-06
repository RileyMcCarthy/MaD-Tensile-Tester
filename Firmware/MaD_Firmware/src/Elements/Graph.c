#include "Graph.h"

Graph *graph_create(int startX, int startY, int width, int height, float maxY, float minY, char *units, char *title)
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));
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