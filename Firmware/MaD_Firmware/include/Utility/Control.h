#ifndef Control_H
#define Control_H
#include "JSON.h"
#include "Monitor.h"
#include "i2cNavKey.h"
#include "StateMachine.h"
#include "IOBoard.h"

typedef enum homingstate_e
{
    HOMING_NONE,
    HOMING_COMPLETE,
    HOMING_SEEKING,
    HOMING_BACKING_OFF,
    HOMING_SEEKING_SLOW
} HomingState;

typedef struct control_t
{
    MachineProfile *machineProfile;
    MonitorData *monitorData;
    NavKey *navkey;
    MCP23017 *mcp;
    MachineState *stateMachine;
    MotionProfile *motionProfile;
    DYN4 *dyn4;
    int cogid;
    long setpointum;
} Control;

void control_init(Control *control, MachineProfile *machineProfile, MachineState *stateMachine, DYN4 *dyn4, MonitorData *monitorData);
void control_destry(Control *control);
bool control_begin(Control *control);

#endif
