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
    bool switchedPowerOK;             // Switched power is activated to IO board. RED/GREEN buttons. ( GPI_1). Control.c. implemented, changed to GPI12
    MotionOverTravel overTravelLimit; // Over travel limit status of upper/lower ( GPI_2/3 ). Control.c. implemented, needs switched power on
    bool esdOK;                       // User ESD switch is activated ( GPI_4 ). Control.c. implemented (big red button)
    bool servoOK;                     // Servo ready signal recieved ( GPI_11 ). Control.c, implemented
    bool forceGaugeOK;                // Force gauge communicating. Control.c, implemented
    bool dyn4OK;                      // DYN4 is communicating. Control.c
    bool rtcOK;                       // RTC is communicating. Control.c , not implmeented

} MachineCheckParameters;

typedef struct Motion_t
{
    MotionStatus status;       // internal and external
    MotionCondition condition; // internal and external
    MotionMode mode;           // internal and external

    MotionOverTravel travelLimit; // Updated by control.c, travellimit and softlimit replaced by condition
    MotionOverTravel softLimit;   // Updated by control.c
    bool forceOverload;           // Updated by control.c
} MotionParameters;

typedef struct MachineState_t
{
    State currentState;
    SelfCheckParameters selfCheckParameters;
    MachineCheckParameters machineCheckParameters;
    MotionParameters motionParameters;
    int cogid;
} MachineState;

typedef enum parameters_e
{
    PARAM_CHARGEPUMPOK,
    PARAM_SWITCHEDPOWEROK,
    PARAM_OVERTRAVELLIMIT,
    PARAM_ESDOK,
    PARAM_SERVOOK,
    PARAM_FORCEGAUGEOK,
    PARAM_DYN4OK,
    PARAM_RTCOK,
    PARAM_STATUS,
    PARAM_CONDITION,
    PARAM_MODE,
    PARAM_TRAVELLIMIT,
    PARAM_SOFTLIMIT,
    PARAM_FORCEOVERLOAD,
} Parameter;

MachineState *machine_state_create();

// External State Setters
void state_machine_set(MachineState *machineState, Parameter param, int state);
#endif
