#include "StateMachine.h"

static long state_stack[64];

/**
 * @brief Initial state on power up. Automatic transition into if any Self Check condition not satisfied. No motion shall occur in Self Check state.
 *
 */
static State state_machine_self_check(MachineState *machineState)
{
    State newState;

    machineState->selfCheckParameters.chargePumpOK = true;

    // Decide the next state
    if (machineState->selfCheckParameters.chargePumpOK)
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

    // Check previous state
    State newState = state_machine_self_check(machineState);
    if (newState != STATE_MACHINECHECK) // Ensure self check state is satisfied
    {
        return newState;
    }

    MachineCheckParameters params = machineState->machineCheckParameters;
    // Check internal parameters
    if (params.switchedPowerOK &&
        params.overTravelLimit == MOTION_OVER_TRAVEL_OK &&
        params.esdOK &&
        params.servoOK &&
        params.forceGaugeOK &&
        params.dyn4OK &&
        params.rtcOK)
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

    // Decide the next state
    State newState = state_machine_check(machineState);
    if (newState != STATE_MOTION) // Check if previous state conditions are still valid back
    {
        // Set state based internal parameters
        machineState->motionParameters.status = STATUS_DISABLED;
        machineState->motionParameters.condition = MOTION_STOPPED;
        machineState->motionParameters.mode = MODE_MANUAL;
        return newState;
    }

    MotionParameters params = machineState->motionParameters;
    // Check internal parameters
    if (params.travelLimit != MOTION_OVER_TRAVEL_OK)
    {
        // Upper limit reached, override mode
        machineState->motionParameters.status = STATUS_RESTRICTED;
        machineState->motionParameters.mode = MODE_OVERRIDE;
        switch (params.travelLimit)
        {
        case MOTION_OVER_TRAVEL_UPPER:
            machineState->motionParameters.condition = MOTION_UPPER;
            break;
        case MOTION_OVER_TRAVEL_LOWER:
            machineState->motionParameters.condition = MOTION_LOWER;
            break;
        default:
            break;
        }
    }
    if (params.softLimit != MOTION_OVER_TRAVEL_OK)
    {
        machineState->motionParameters.status = STATUS_RESTRICTED;
        machineState->motionParameters.mode = MODE_OVERRIDE;
        switch (params.softLimit)
        {
        case MOTION_OVER_TRAVEL_UPPER:
            machineState->motionParameters.condition = MOTION_UPPER;
            break;
        case MOTION_OVER_TRAVEL_LOWER:
            machineState->motionParameters.condition = MOTION_LOWER;
            break;
        default:
            break;
        }
    }
    if (params.forceOverload)
    {
        machineState->motionParameters.status = STATUS_RESTRICTED;
        machineState->motionParameters.mode = MODE_OVERRIDE;
        // Need to tell state machine weather its in tension or compression
    }
    return STATE_MOTION;
}

static void state_machine_update(MachineState *machineState)
{
    machineState->currentState = state_machine_motion(machineState);
}

void state_machine_set(MachineState *machineState, Parameter param, int state)
{
    switch (param)
    {
    case PARAM_CHARGEPUMPOK:
        machineState->selfCheckParameters.chargePumpOK = state;
        break;
    case PARAM_SWITCHEDPOWEROK:
        machineState->machineCheckParameters.switchedPowerOK = state;
        break;
    case PARAM_OVERTRAVELLIMIT:
        machineState->machineCheckParameters.overTravelLimit = state;
        break;
    case PARAM_ESDOK:
        machineState->machineCheckParameters.esdOK = state;
        break;
    case PARAM_SERVOOK:
        machineState->machineCheckParameters.servoOK = state;
        break;
    case PARAM_FORCEGAUGEOK:
        machineState->machineCheckParameters.forceGaugeOK = state;
        break;
    case PARAM_DYN4OK:
        machineState->machineCheckParameters.dyn4OK = state;
        break;
    case PARAM_RTCOK:
        machineState->machineCheckParameters.rtcOK = state;
        break;
    case PARAM_STATUS:
        machineState->motionParameters.status = state;
        break;
    case PARAM_CONDITION:
        machineState->motionParameters.condition = state;
        break;
    case PARAM_MODE:
        machineState->motionParameters.mode = state;
        break;
    case PARAM_TRAVELLIMIT:
        machineState->motionParameters.travelLimit = state;
        break;
    case PARAM_SOFTLIMIT:
        machineState->motionParameters.softLimit = state;
        break;
    case PARAM_FORCEOVERLOAD:
        machineState->motionParameters.forceOverload = state;
        break;
    }
    state_machine_update(machineState);
}

MachineState *machine_state_create()
{
    MachineState *machineState = (MachineState *)malloc(sizeof(MachineState));
    machineState->currentState = STATE_SELFCHECK;

    machineState->selfCheckParameters.chargePumpOK = false;

    machineState->machineCheckParameters.switchedPowerOK = false;
    machineState->machineCheckParameters.overTravelLimit = MOTION_OVER_TRAVEL_OK;
    machineState->machineCheckParameters.esdOK = false;
    machineState->machineCheckParameters.servoOK = false;
    machineState->machineCheckParameters.forceGaugeOK = false;
    machineState->machineCheckParameters.dyn4OK = false;
    machineState->machineCheckParameters.rtcOK = false;

    machineState->motionParameters.condition = MOTION_STOPPED;
    machineState->motionParameters.forceOverload = false;
    machineState->motionParameters.travelLimit = MOTION_OVER_TRAVEL_OK;
    machineState->motionParameters.mode = MODE_MANUAL;
    machineState->motionParameters.softLimit = MOTION_OVER_TRAVEL_OK;
    machineState->motionParameters.status = STATUS_DISABLED;
    return machineState;
}