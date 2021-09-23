#include "StateMachine.h"

static long stack[64];

/**
* @brief Initial state on power up. Automatic transition into if any Self Check condition not satisfied. No motion shall occur in Self Check state. 
* 
*/
static State state_machine_self_check(MachineState *machineState)
{
    State newState;

    //Decide the next state
    if (machineState->selfCheckParameters.chargePumpOK)
    {
        newState = State_MachineCheck;
    }
    else
    {
        newState = State_SelfCheck;
    }

    return newState;
}

/**
 * @brief Automatic transition into when all Self Check conditions satisfied or if any Machine Check condition not satisfied and Self Check conditions are satisfied. 
 * No motion shall occur in Machine Check.  
 * Motion Status is Disabled in Machine Check State. 
 * 
 */
static State state_machine_check(MachineState *machineState)
{
    State newState;

    //Check internal parameters

    //Decide the next state
    if (state_machine_self_check() != State_MachineCheck) //Ensure self check state is satisfied
    {
        return State_SelfCheck;
    }
    if (machineState->machineCheckParameters.forceGaugeResponding && machineState->machineCheckParameters.dyn4Responding)
    {
        newState = State_Motion;

        //Set state based internal parameters
        machineState->machineCheckParameters.machineReady = true;
    }
    else
    {
        newState = State_MachineCheck;

        //Set state based internal parameters
        machineState->machineCheckParameters.machineReady = false;
    }

    return newState;
}

static State state_machine_motion(MachineState *machineState)
{
    State newState;

    //Check internal parameters

    //Decide the next state
    if (MachineCheckState() != State_Motion) //Check if previous state conditions are still valid back
    {
        //Set state based internal parameters
        machineState->motionParameters.status = STATUS_DISABLED;
        machineState->motionParameters.condition = MOTION_STOPPED;
        machineState->motionParameters.mode = MODE_MANUAL;
        return State_MachineCheck;
    }

    //Dump temporary parameters to machine state
    machineState->motionParameters.status = tempStatus;
    machineState->motionParameters.condition = tempCondition;
    machineState->motionParameters.mode = tempMode;
    return State_Motion;
}

static void state_machine_cog(MachineState *machineState)
{
    //Get MachineState as parameter from hub memory
    State previousState = machineState->currentState;
    while (1)
    {
        State newState;
        switch (previousState)
        {
        case State_SelfCheck:
            newState = SelfCheckState(machineState);
            break;
        case State_MachineCheck:
            newState = MachineCheckState(machineState);
            break;
        case State_Motion:
            newState = MotionState(machineState);
            break;
        default:
            break;
        }
        machineState->currentState = newState;
    }
}

MachineState *state_machine_run()
{
    MachineState *machineState = (MachineState *)malloc(sizeof(MachineState *));
    machineState->cogid = __builtin_cogstart(state_machine_cog(machineState), &stack[0]);
    if (machineState->cogid != -1)
    {
        return machineState;
    }
    return NULL;
}

void state_machine_stop(MachineState *machineState)
{
    //stop cog and free MachineState
}

//External State Setter Functions

void state_machine_set_servo_ready(MachineState *machineState, bool ready)
{
    machineState->selfCheckParameters.servoReady = ready;
}

void state_machine_set_force_gauge_responding(MachineState *machineState, bool responding)
{
    machineState->machineCheckParameters.forceGaugeResponding = responding;
}

void state_machine_set_dyn4_responding(MachineState *machineState, bool responding)
{
    machineState->machineCheckParameters.dyn4Responding = responding;
}

void state_machine_set_status(MachineState *machineState, MotionStatus status)
{
    //Set rules for changing status, and update internal parameters
    machineState->motionParameters.status = status;
}

void state_machine_set_condition(MachineState *machineState, MotionCondition condition)
{
    machineState->motionParameters.condition = condition;
}

void state_machine_set_mode(MachineState *machineState, MotionMode mode)
{
    machineState->motionParameters.mode = mode;
}