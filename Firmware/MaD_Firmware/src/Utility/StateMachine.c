#include "StateMachine.h"

static long stack[64];

/**
 * @brief Initial state on power up. Automatic transition into if any Self Check condition not satisfied. No motion shall occur in Self Check state.
 *
 */
static State state_machine_self_check(MachineState *machineState)
{
    State newState;
    SelfCheckParameters params = machineState->selfCheckParameters;

    // Decide the next state
    if (params.chargePumpOK && params.rtcReady)
    {
        newState = STATE_MACHINECHECK;
    }
    else
    {
        newState = STATE_SELFCHECK;
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

    // Check previous state
    if (state_machine_self_check(machineState) != STATE_MACHINECHECK) // Ensure self check state is satisfied
    {
        return STATE_SELFCHECK;
    }

    MachineCheckParameters params = machineState->machineCheckParameters;
    // Check internal parameters
    if (params.power &&
        params.esd &&
        params.servoReady &&
        params.forceGaugeResponding &&
        params.dyn4Responding &&
        params.machineReady)
    {
        return STATE_MOTION; // Internal parameters passed, change to MOTION
    }
    else
    {
        // Internal parameters failed, stay in Machine Check
        return STATE_MACHINECHECK;
    }
}

static State state_machine_motion(MachineState *machineState)
{
    State newState;

    // Decide the next state
    if (state_machine_check(machineState) != STATE_MOTION) // Check if previous state conditions are still valid back
    {
        // Set state based internal parameters
        machineState->motionParameters.status = STATUS_DISABLED;
        machineState->motionParameters.condition = MOTION_STOPPED;
        machineState->motionParameters.mode = MODE_MANUAL;
        return STATE_MACHINECHECK;
    }

    MotionParameters params = machineState->motionParameters;
    // Check internal parameters
    if (params.softLowerLimit ||
        params.hardLowerLimit)
    {
        // Lower limit reached, override mode
        machineState->motionParameters.status = STATUS_RESTRICTED;
        machineState->motionParameters.condition = MOTION_LOWER;
        machineState->motionParameters.mode = MODE_OVERRIDE;
    }
    if (!params.softUpperLimit &&
        !params.hardUpperLimit)
    {
        // Upper limit reached, override mode
        machineState->motionParameters.status = STATUS_RESTRICTED;
        machineState->motionParameters.condition = MOTION_UPPER;
        machineState->motionParameters.mode = MODE_OVERRIDE;
    }
    if (params.forceOverload)
    {
        machineState->motionParameters.status = STATUS_RESTRICTED;
        machineState->motionParameters.mode = MODE_OVERRIDE;
        // Need to tell state machine weather its in tension or compression
    }
    return STATE_MOTION;
}

static void state_machine_cog(MachineState *machineState)
{

    printf("Running state machine\n");
    // Get MachineState as parameter from hub memory
    State currentState = machineState->currentState;
    while (1)
    {
        // make snippet of current state
        State newState;
        switch (machineState->currentState)
        {
        case STATE_SELFCHECK:
            newState = state_machine_self_check(machineState);
            break;
        case STATE_MACHINECHECK:
            newState = state_machine_check(machineState);
            break;
        case STATE_MOTION:
            newState = state_machine_motion(machineState);
            break;
        default:
            break;
        }
        machineState->currentState = newState;
    }
}

static MachineState *get_machine_state()
{
    MachineState *machineState = (MachineState *)malloc(sizeof(MachineState));
    machineState->currentState = STATE_SELFCHECK;

    machineState->selfCheckParameters.chargePumpOK = false;
    machineState->selfCheckParameters.rtcReady = false;

    machineState->machineCheckParameters.dyn4Responding = false;
    machineState->machineCheckParameters.esd = false;
    machineState->machineCheckParameters.forceGaugeResponding = false;
    machineState->machineCheckParameters.machineReady = false;
    machineState->machineCheckParameters.power = false;
    machineState->machineCheckParameters.servoReady = false;

    machineState->motionParameters.condition = MOTION_STOPPED;
    machineState->motionParameters.forceOverload = false;
    machineState->motionParameters.hardLowerLimit = false;
    machineState->motionParameters.hardUpperLimit = false;
    machineState->motionParameters.mode = MODE_MANUAL;
    machineState->motionParameters.softLowerLimit = false;
    machineState->motionParameters.softUpperLimit = false;
    machineState->motionParameters.status = STATUS_DISABLED;
    return machineState;
}

MachineState *state_machine_run()
{
    MachineState *machineState = get_machine_state();
    machineState->cogid = _cogstart_C(state_machine_cog, machineState, &stack[0], sizeof(long) * 64);
    if (machineState->cogid != -1)
    {
        return machineState;
    }
    return NULL;
}

void state_machine_stop(MachineState *machineState)
{
    // stop cog and free MachineState
}

// External State Setter Functions

void state_machine_set_servo_ready(MachineState *machineState, bool ready)
{
    machineState->machineCheckParameters.servoReady = ready;
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
    // Set rules for changing status, and update internal parameters
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
