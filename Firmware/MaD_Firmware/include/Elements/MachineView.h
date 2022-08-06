#ifndef MACHINEVIEW_H
#define MACHINEVIEW_H

#include "Graph.h"
#include "JSON.h"
#include "Monitor.h"

typedef struct MachineView_s
{
    Module window;
    Module title;
    Module forceValue;
    Module positionValue;
    Module positionContainer;
    Module forceContainer;

    ModuleGraph positionGraph;
    ModuleGraph forceGraph;

    char forceValueBuffer[20];
    char positionValueBuffer[20];
    MonitorData *data;
} MachineView;

void machine_view_init(MachineView *view, Module *container, MonitorData *data);
#endif
