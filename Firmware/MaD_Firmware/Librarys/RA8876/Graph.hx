#ifndef GRAPH_H
#define GRAPH_H
#include "RA8876.h"
#include "Style.h"
#include "Module.h"
#define MAX_GRAPH_MARKERS 10

/*typedef struct graph_t
{
    Display *display;
    int dataCount;
    double *data;
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
} Graph;*/

typedef struct graph_t
{
    int dataCount;
    double *data;
    Module *graphArea;
    float markerY[MAX_GRAPH_MARKERS];
    int markerCount;
    char *title;
    char *units;
} Graph;

bool graph_draw_dynamic(Graph *graph, Display *display, int x, int y, int w, int h);
bool graph_draw_static(Graph *graph, Display *display, int x, int y, int w, int h);
void graph_add_marker(Graph *graph, float value);
Graph *graph_create(Display *display, int startX, int startY, int width, int height, float maxY, float minY, const char *units, const char *title);
void graph_destroy(Graph *graph);
#endif
