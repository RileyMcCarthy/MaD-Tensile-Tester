#ifndef GRAPH_H
#define GRAPH_H
#include "RA8876.h"
#include "Style.h"
#include "Module.h"
typedef struct graph_t
{
    double *dataPtr;
    double data[2];
    double maxY;
    double minY;
    int x, y, w, h; // Position and size of graph area to copy
    double markerY[MAX_GRAPH_MARKERS];
    int markerCount;
    char title[MAX_GRAPH_TITLE_LENGTH];
    char units[MAX_GRAPH_UNITS_LENGTH];

    bool initialDraw;

    Module graphTitle;
    Module graphMinText;
    Module graphMaxText;
    Module graphArea;
    Module grapUnits;
    Module graphCenterLine;
    Module graphVerticleLine;

    char minyBuffer[10];
    char maxyBuffer[10];
} ModuleGraph;

void module_set_graph(Module *window, ModuleGraph *graph, const char *title, const char *units, double *data);
void module_graph_add_marker(Module *module, double value);
void module_graph_insert(Module *module, double value);
void module_graph_set_range(Module *module, double maxY, double minY);
int module_graph_get_max_data(Module *module);
#endif
