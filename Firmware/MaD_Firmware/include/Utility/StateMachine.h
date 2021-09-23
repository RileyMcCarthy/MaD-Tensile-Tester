#ifndef StateMachine_H
#define StateMachine_H
#include "simpletools.h"
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
    bool chargePumpOK; //Internal State
} SelfCheckParameters;

typedef struct MachineCheck_t
{
    bool power;                //Internal State
    bool upperLimit;           //Internal State
    bool lowerLimit;           //Internal State
    bool esd;                  //Internal State
    bool servoReady;           //External State
    bool forceGaugeResponding; //External State
    bool dyn4Responding;       //External State
    bool machineReady;         //Internal State
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

//External State Setters
void state_machine_set_servo_ready(MachineState *machineState, bool ready);
void state_machine_set_force_gauge_responding(MachineState *machineState, bool responding);
void state_machine_set_dyn4_responding(MachineState *machineState, bool responding);
void state_machine_set_status(MachineState *machineState, MotionStatus status);
void state_machine_set_condition(MachineState *machineState, MotionCondition condition);
void state_machine_set_mode(MachineState *machineState, MotionMode mode);

#endif
