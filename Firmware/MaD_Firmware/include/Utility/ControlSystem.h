#ifndef ControlSystem_H
#define ControlSystem_H
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
    MachineState *stateMachine;
    MotionProfile motionProfile;
    int cogid;
} ControlSystem;

bool control_begin(ControlSystem *control, MachineProfile *machineProfile, MachineState *stateMachine,  MonitorData *monitorData);

#endif
