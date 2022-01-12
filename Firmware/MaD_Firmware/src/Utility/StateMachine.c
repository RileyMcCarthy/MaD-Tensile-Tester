#include "StateMachine.h"

static long state_stack[64];

/**
 * @brief Initial state on power up. Automatic transition into if any Self Check condition not satisfied. No motion shall occur in Self Check state.
 *
 */
static State state_machine_self_check(MachineState *machineState)
{
    State newState;

    machineState->selfCheckParameters.chargePump = true;

    // Decide the next state
    if (machineState->selfCheckParameters.chargePump)
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
    if (params.switchedPower &&
        params.esdTravelLimit == MOTION_LIMIT_OK &&
        params.esdSwitch &&
        params.servoOK &&
        params.forceGaugeCom &&
        params.servoCom &&
        params.rtcCom)
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
    if (newState != STATE_MOTION) // Set default exit state parameters
    {
        // Set state based internal parameters
        if (machineState->state == STATE_MOTION &&                                    // Ensure previous state is MOTION
            machineState->machineCheckParameters.esdTravelLimit != MOTION_LIMIT_OK && // Check for esd fault
            machineState->machineCheckParameters.esdSwitch)
        {
            // Motion exited due to esd fault
            // printf("motion faulted\n");
            machineState->motionParameters.status = STATUS_FAULTED;
        }
        else
        {
            machineState->motionParameters.status = STATUS_DISABLED;
        }
        // printf("machine check failed\n");
        machineState->motionParameters.condition = MOTION_STOPPED;
        machineState->motionParameters.mode = MODE_MANUAL;
        return newState;
    }

    // Check internal parameters
    if (machineState->motionParameters.status != STATUS_DISABLED)
        switch (machineState->motionParameters.condition) // Update status based on condition
        {
        case MOTION_LENGTH:
            machineState->motionParameters.status = STATUS_SAMPLE_LIMIT;
            break;
        case MOTION_FORCE:
            machineState->motionParameters.status = STATUS_SAMPLE_LIMIT;
            break;
        case MOTION_TENSION:
            machineState->motionParameters.status = STATUS_MACHINE_LIMIT;
            break;
        case MOTION_COMPRESSION:
            machineState->motionParameters.status = STATUS_MACHINE_LIMIT;
            break;
        case MOTION_UPPER:
            machineState->motionParameters.status = STATUS_MACHINE_LIMIT;
            break;
        case MOTION_LOWER:
            machineState->motionParameters.status = STATUS_MACHINE_LIMIT;
            break;
        case MOTION_DOOR:
            machineState->motionParameters.status = STATUS_MACHINE_LIMIT;
            break;
        case MOTION_STOPPED:
            break;
        case MOTION_MOVING:
            break;
        }

    return STATE_MOTION;
}

static void state_machine_update(MachineState *machineState)
{
    machineState->state = state_machine_motion(machineState);
}

void state_machine_set(MachineState *machineState, Parameter param, int state)
{
    switch (param)
    {
    case PARAM_SELF_CHARGE_PUMP:
        machineState->selfCheckParameters.chargePump = state;
        break;
    case PARAM_MACHINE_SWITCHED_POWER:
        machineState->machineCheckParameters.switchedPower = state;
        break;
    case PARAM_MACHINE_ESD_TRAVEL_LIMIT:
        machineState->machineCheckParameters.esdTravelLimit = state;
        break;
    case PARAM_MACHINE_ESD_SWITCH:
        machineState->machineCheckParameters.esdSwitch = state;
        break;
    case PARAM_MACHINE_SERVO_OK:
        machineState->machineCheckParameters.servoOK = state;
        break;
    case PARAM_MACHINE_FORCE_GAUGE_COM:
        machineState->machineCheckParameters.forceGaugeCom = state;
        break;
    case PARAM_MACHINE_SERVO_COM:
        machineState->machineCheckParameters.servoCom = state;
        break;
    case PARAM_MACHINE_RTC_COM:
        machineState->machineCheckParameters.rtcCom = state;
        break;
    case PARAM_MOTION_STATUS:
        machineState->motionParameters.status = state;
        break;
    case PARAM_MOTION_CONDITION:
        machineState->motionParameters.condition = state;
        break;
    case PARAM_MOTION_MODE:
        machineState->motionParameters.mode = state;
        break;
    }
    state_machine_update(machineState);
}

MachineState *machine_state_create()
{
    MachineState *machineState = (MachineState *)malloc(sizeof(MachineState));
    machineState->state = STATE_SELFCHECK;

    machineState->selfCheckParameters.chargePump = false;

    machineState->machineCheckParameters.switchedPower = false;
    machineState->machineCheckParameters.esdTravelLimit = MOTION_LIMIT_OK;
    machineState->machineCheckParameters.esdSwitch = false;
    machineState->machineCheckParameters.servoOK = false;
    machineState->machineCheckParameters.forceGaugeCom = false;
    machineState->machineCheckParameters.servoCom = false;
    machineState->machineCheckParameters.rtcCom = false;

    machineState->motionParameters.condition = MOTION_STOPPED;
    machineState->motionParameters.mode = MODE_MANUAL;
    machineState->motionParameters.status = STATUS_DISABLED;
    return machineState;
}