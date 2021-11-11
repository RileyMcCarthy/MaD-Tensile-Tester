#ifndef StateMachine_H
#define StateMachine_H
#ifdef __MEMORY_CHECK__
#include "leak_detector_c.h"
#endif
#include <simpletools.h>
#include <stdbool.h>
#include "DYN4.h"
#include "MCP23017.h"

typedef enum States_e
{
    STATE_SELFCHECK,
    STATE_MACHINECHECK,
    STATE_MOTION
} State;

typedef enum MotionStatus_e
{
    STATUS_DISABLED,
    STATUS_ENABLED,
    STATUS_RESTRICTED
} MotionStatus;

typedef enum MotionCondition_e
{
    MOTION_STOPPED,
    MOTION_MOVING,
    MOTION_TENSION,
    MOTION_COMPRESSION,
    MOTION_UPPER,
    MOTION_LOWER,
    MOTION_DOOR
} MotionCondition;

typedef enum MotionMode_e
{
    MODE_MANUAL,
    MODE_AUTOMATIC,
    MODE_OVERRIDE
} MotionMode;

typedef struct SelfCheck_t
{
    bool chargePumpOK; // Updated by MaD initalization
    bool rtcReady;     // Updated by MaD initalization
} SelfCheckParameters;

typedef struct MachineCheck_t
{
    bool power;                // Updated by control.c
    bool esd;                  // Updated by control.c
    bool servoReady;           // Updated by control.c
    bool forceGaugeResponding; // Updated by control.c
    bool dyn4Responding;       // Updated by control.c
    bool machineReady;         // External State, Set by user button input
} MachineCheckParameters;

typedef struct Motion_t
{
    MotionStatus status;       // internal and external
    MotionCondition condition; // internal and external
    MotionMode mode;           // internal and external

    bool hardUpperLimit; // Updated by control.c
    bool hardLowerLimit; // Updated by control.c
    bool softUpperLimit; // Updated by control.c
    bool softLowerLimit; // Updated by control.c
    bool forceOverload;  // Updated by control.c
} MotionParameters;

typedef struct MachineState_t
{
    State currentState;
    SelfCheckParameters selfCheckParameters;
    MachineCheckParameters machineCheckParameters;
    MotionParameters motionParameters;
    int cogid;
} MachineState;

MachineState *state_machine_run();
void state_machine_stop(MachineState *machineState);

// External State Setters
void state_machine_set_servo_ready(MachineState *machineState, bool ready);
void state_machine_set_force_gauge_responding(MachineState *machineState, bool responding);
void state_machine_set_dyn4_responding(MachineState *machineState, bool responding);
void state_machine_set_status(MachineState *machineState, MotionStatus status);
void state_machine_set_condition(MachineState *machineState, MotionCondition condition);
void state_machine_set_mode(MachineState *machineState, MotionMode mode);

#endif
