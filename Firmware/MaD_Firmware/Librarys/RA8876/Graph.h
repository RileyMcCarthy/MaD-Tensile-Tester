#ifndef GRAPH_H
#define GRAPH_H
#include "RA8876.h"
#include "Style.h"
#define MAX_GRAPH_MARKERS 10

typedef enum graphtype_e
{
    GRAPH_STATIC,
    GRAPH_DYNAMIC
} GraphType;

typedef struct graph_t
{
    Display *display;
    int startX;
    int startY;
    int width;
    int height;
    float maxY;
    float minY;
    int lastX;
    int lastY;
    float markerY[MAX_GRAPH_MARKERS];
    int markerCount;
    char *title;
    char *units;
    GraphType type;
} Graph;

void graph_draw(Graph *graph, Display *display, float newValue);
void graph_add_marker(Graph *graph, float value);
Graph *graph_create(Display *display, int startX, int startY, int width, int height, float maxY, float minY, const char *units, const char *title);
void graph_destroy(Graph *graph);
#endif
