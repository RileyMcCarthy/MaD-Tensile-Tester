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
        if (machineState->state == STATE_MOTION && // Ensure previous state is MOTION
            (machineState->machineCheckParameters.esdTravelLimit != MOTION_LIMIT_OK ||
             !machineState->machineCheckParameters.esdSwitch)) // Check for esd fault
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
            if (machineState->motionParameters.status != STATUS_ENABLED && machineState->motionParameters.status != STATUS_DISABLED)
            {
                // Motion returning from condition, enter enabled state
                machineState->motionParameters.status = STATUS_ENABLED;
            }
            break;
        case MOTION_MOVING:
            if (machineState->motionParameters.status != STATUS_ENABLED && machineState->motionParameters.status != STATUS_DISABLED)
            {
                // Motion returning from condition, enter enabled state
                machineState->motionParameters.status = STATUS_ENABLED;
            }
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
        if (machineState->selfCheckParameters.chargePump != state)
        {
            machineState->selfCheckParameters.chargePump = state;
            state_machine_update(machineState);
        }
        break;
    case PARAM_MACHINE_SWITCHED_POWER:
        if (machineState->machineCheckParameters.switchedPower != state)
        {
            machineState->machineCheckParameters.switchedPower = state;
            state_machine_update(machineState);
        }
        break;
    case PARAM_MACHINE_ESD_TRAVEL_LIMIT:
        if (machineState->machineCheckParameters.esdTravelLimit != state)
        {
            machineState->machineCheckParameters.esdTravelLimit = state;
            state_machine_update(machineState);
        }
        break;
    case PARAM_MACHINE_ESD_SWITCH:
        if (machineState->machineCheckParameters.esdSwitch != state)
        {
            machineState->machineCheckParameters.esdSwitch = state;
            state_machine_update(machineState);
        }
        break;
    case PARAM_MACHINE_SERVO_OK:
        if (machineState->machineCheckParameters.servoOK != state)
        {
            machineState->machineCheckParameters.servoOK = state;
            state_machine_update(machineState);
        }
        break;
    case PARAM_MACHINE_FORCE_GAUGE_COM:
        if (machineState->machineCheckParameters.forceGaugeCom != state)
        {
            machineState->machineCheckParameters.forceGaugeCom = state;
            state_machine_update(machineState);
        }
        break;
    case PARAM_MACHINE_SERVO_COM:
        if (machineState->machineCheckParameters.servoCom != state)
        {
            machineState->machineCheckParameters.servoCom = state;
            state_machine_update(machineState);
        }
        break;
    case PARAM_MACHINE_RTC_COM:
        if (machineState->machineCheckParameters.rtcCom != state)
        {
            machineState->machineCheckParameters.rtcCom = state;
            state_machine_update(machineState);
        }
        break;
    case PARAM_MOTION_STATUS:
        if (machineState->motionParameters.status != state)
        {
            machineState->motionParameters.status = state;
            state_machine_update(machineState);
        }
        break;
    case PARAM_MOTION_CONDITION:
        if (machineState->motionParameters.condition != state)
        {
            machineState->motionParameters.condition = state;
            state_machine_update(machineState);
        }
        break;
    case PARAM_MOTION_MODE:
        if (machineState->motionParameters.mode != state)
        {
            machineState->motionParameters.mode = state;
            state_machine_update(machineState);
        }
    case PARAM_FUNCTION:
        if (machineState->function != state)
        {
            machineState->function = state;
            state_machine_update(machineState);
        }
        break;
    }
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
    machineState->function = FUNC_MANUAL_OFF;
    return machineState;
}
