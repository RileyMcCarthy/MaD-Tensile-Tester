#ifndef Control_H
#define Control_H
#include "JSON.h"
#include "Monitor.h"
#include "i2cNavKey.h"
#include "StateMachine.h"
#include "IOBoard.h"

typedef struct control_t
{
    MachineProfile *machineProfile;
    MonitorData *monitorData;
    NavKey *navkey;
    MCP23017 *mcp;
    MachineState *stateMachine;
    TestProfile *testProfile;
    DYN4 *dyn4;
    int cogid;
} Control;

Control *control_create(MachineProfile *machineProfile, MachineState *stateMachine, DYN4 *dyn4, MonitorData *monitorData);
void control_destry(Control *control);
bool control_begin(Control *control);

#endif
