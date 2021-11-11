#ifndef Control_H
#define Control_H
#include "JSON.h"
#include "Monitor.h"
#include "i2cNavKey.h"
#include "StateMachine.h"

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

Control *control_create();
void control_destry(Control *control);
bool control_begin(Control *control, MachineProfile *machineProfile, MonitorData *monitorData, MachineState *stateMachine, MCP23017 *mcp, DYN4 *dyn4, NavKey *navkey);

#endif