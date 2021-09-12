#ifndef StateMachine_H
#define StateMachine_H
#include "propeller.h"
#include "stdbool.h"

typedef enum States_e
{
    State_SelfCheck,
    State_MachineCheck,
    State_Motion
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
    bool chargePumpOK; //external
} SelfCheckParameters;

typedef struct MachineCheck_t
{
    bool power;                //internal
    bool upperLimit;           //internal
    bool lowerLimit;           //internal
    bool esd;                  //internal
    bool servoReady;           //external
    bool forceGaugeResponding; //external
    bool dyn4Responding;       //external
    bool machineReady;         //internal
} MachineCheckParameters;

typedef struct Motion_t
{
    MotionStatus status;       //internal and external
    MotionCondition condition; //internal and external
    MotionMode mode;           //internal and external

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
#endif
