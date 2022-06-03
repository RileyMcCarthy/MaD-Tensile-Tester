#ifndef GRAPH_H
#define GRAPH_H
#include "RA8876.h"
#include "Style.h"
#include "Module.h"
#define MAX_GRAPH_MARKERS 10

typedef struct graph_t
{
    double data[2];
    double maxY;
    double minY;
    Module *graphArea;
    float markerY[MAX_GRAPH_MARKERS];
    int markerCount;
    char *title;
    char *units;
} Graph;

void module_set_graph(Module *window, const char *title, const char *units);
void module_graph_add_marker(Module *module, float value);
void module_graph_insert(Module *module, double value);
void module_graph_set_range(Module *module, double maxY, double minY);
int module_graph_get_max_data(Module *module);

#endif
