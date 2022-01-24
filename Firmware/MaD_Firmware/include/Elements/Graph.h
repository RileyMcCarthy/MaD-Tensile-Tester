#include "RA8876.h"
#include "Style.h"
#define MAX_GRAPH_MARKERS 10

typedef struct graph_t
{
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
} Graph;

void graph_draw(Graph *graph, Display *display, float newValue);
void graph_add_marker(Graph *graph, float value);
Graph *graph_create(int startX, int startY, int width, int height, float maxY, float minY, const char *units, const char *title);
void graph_destroy(Graph *graph);
