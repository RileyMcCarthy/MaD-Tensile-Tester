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

typedef enum MotionOverTravel_e
{
    MOTION_OVER_TRAVEL_OK,
    MOTION_OVER_TRAVEL_UPPER,
    MOTION_OVER_TRAVEL_LOWER
} MotionOverTravel;

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
    bool chargePumpOK; // Charge pump signal is activated, self check is satisfied. StateMachine, working
} SelfCheckParameters;

typedef struct MachineCheck_t
{
    bool switchedPowerOK;             // Switched power is activated to IO board. RED/GREEN buttons. ( GPI_1). Control.c. changed to GPI12
    MotionOverTravel overTravelLimit; // Over travel limit status of upper/lower ( GPI_2/3 ). Control.c. working, needs switched power on
    bool esdOK;                       // User ESD switch is activated ( GPI_4 ). Control.c. working (big red button)
    bool servoOK;                     // Servo ready signal recieved ( Pin 7 ). Control.c
    bool forceGaugeOK;                // Force gauge communicating. Control.c
    bool dyn4OK;                      // DYN4 is communicating. Control.c
    bool rtcOK;                       // RTC is communicating. Control.c
    bool machineOK;                   // User input to enable machine. Status.c

} MachineCheckParameters;

typedef struct Motion_t
{
    MotionStatus status;       // internal and external
    MotionCondition condition; // internal and external
    MotionMode mode;           // internal and external

    bool hardUpperLimit; // Updated by control.c
    bool hardLowerLimit; // Updated by control.c
    bool softUpperLimit; // Updated by control.c  (GPI_7)
    bool softLowerLimit; // Updated by control.c. (GPI_6)
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
