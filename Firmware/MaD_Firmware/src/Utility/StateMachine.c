#include "StateMachine.h"

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
            machineState->motionParameters.status = MOTIONSTATUS_FAULTED;
        }
        else
        {
            machineState->motionParameters.status = MOTIONSTATUS_DISABLED;
        }
        // printf("machine check failed\n");
        machineState->motionParameters.mode = MODE_MANUAL;
        return newState;
    }

    // Check internal parameters
    if (machineState->motionParameters.status != MOTIONSTATUS_DISABLED)
    {
        switch (machineState->motionParameters.condition) // Update status based on condition
        {
        case CONDITION_LENGTH:
            machineState->motionParameters.status = MOTIONSTATUS_SAMPLE_LIMIT;
            break;
        case CONDITION_FORCE:
            machineState->motionParameters.status = MOTIONSTATUS_SAMPLE_LIMIT;
            break;
        case CONDITION_TENSION:
            machineState->motionParameters.status = MOTIONSTATUS_MACHINE_LIMIT;
            break;
        case CONDITION_COMPRESSION:
            machineState->motionParameters.status = MOTIONSTATUS_MACHINE_LIMIT;
            break;
        case CONDITION_UPPER:
            machineState->motionParameters.status = MOTIONSTATUS_MACHINE_LIMIT;
            break;
        case CONDITION_LOWER:
            machineState->motionParameters.status = MOTIONSTATUS_MACHINE_LIMIT;
            break;
        case CONDITION_DOOR:
            machineState->motionParameters.status = MOTIONSTATUS_MACHINE_LIMIT;
            break;
        case CONDITION_STOPPED:
            if (machineState->motionParameters.status != MOTIONSTATUS_ENABLED && machineState->motionParameters.status != MOTIONSTATUS_DISABLED)
            {
                // Motion returning from condition, enter enabled state
                machineState->motionParameters.status = MOTIONSTATUS_ENABLED;
            }
            break;
        case CONDITION_MOVING:
            if (machineState->motionParameters.status != MOTIONSTATUS_ENABLED && machineState->motionParameters.status != MOTIONSTATUS_DISABLED)
            {
                // Motion returning from condition, enter enabled state
                machineState->motionParameters.status = MOTIONSTATUS_ENABLED;
            }
            break;
        }
    }
    else
    {
        machineState->motionParameters.mode = MODE_MANUAL;
    }

    return STATE_MOTION;
}

static void state_machine_update(MachineState *machineState)
{
    while (!_locktry(machineState->_lock))
        ;
    // printf("waiting for lock to release\n"); // waits for lock to be available then claims it
    machineState->state = state_machine_motion(machineState);
    _lockrel(machineState->_lock);
}

bool state_machine_self_check_equal(SelfCheckParameters *selfCheckParameters1, SelfCheckParameters *selfCheckParameters2)
{
    return selfCheckParameters1->chargePump == selfCheckParameters2->chargePump;
}

bool state_machine_check_equal(MachineCheckParameters *motionParameters1, MachineCheckParameters *motionParameters2)
{
    return motionParameters1->switchedPower == motionParameters2->switchedPower &&
           motionParameters1->esdTravelLimit == motionParameters2->esdTravelLimit &&
           motionParameters1->esdSwitch == motionParameters2->esdSwitch &&
           motionParameters1->servoOK == motionParameters2->servoOK &&
           motionParameters1->forceGaugeCom == motionParameters2->forceGaugeCom &&
           motionParameters1->servoCom == motionParameters2->servoCom &&
           motionParameters1->rtcCom == motionParameters2->rtcCom;
}

bool state_machine_motion_equal(MotionParameters *motionParameters1, MotionParameters *motionParameters2)
{
    return motionParameters1->status == motionParameters2->status &&
           motionParameters1->mode == motionParameters2->mode &&
           motionParameters1->condition == motionParameters2->condition;
}

bool state_machine_equal(MachineState *machineState1, MachineState *machineState2)
{
    return state_machine_self_check_equal(&(machineState1->selfCheckParameters), &(machineState2->selfCheckParameters)) &&
           state_machine_check_equal(&(machineState1->machineCheckParameters), &(machineState2->machineCheckParameters)) &&
           state_machine_motion_equal(&(machineState1->motionParameters), &(machineState2->motionParameters)) &&
           machineState1->state == machineState2->state &&
           machineState1->_function == machineState2->_function &&
           machineState1->_functionData == machineState2->_functionData;
}

//@TODO use locks to prevent concurrent access to machineState (only require lock if upddating information)
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
            if (machineState->motionParameters.mode != MODE_TEST && state == MODE_TEST_RUNNING)
            { // Must be in test mode to run test
                printf("Must be in test mode to run test\n");
                break;
            }

            machineState->motionParameters.mode = state;
            state_machine_update(machineState);
        }
    case PARAM_FUNCTION:
        if (machineState->_function != state)
        {
            machineState->_function = state;
            state_machine_update(machineState);
        }
        break;
    }
}

void machine_state_init(MachineState *machineState)
{
    machineState->state = STATE_SELFCHECK;

    machineState->selfCheckParameters.chargePump = false;

    machineState->machineCheckParameters.switchedPower = false;
    machineState->machineCheckParameters.esdTravelLimit = MOTION_LIMIT_OK;
    machineState->machineCheckParameters.esdSwitch = false;
    machineState->machineCheckParameters.servoOK = false;
    machineState->machineCheckParameters.forceGaugeCom = false;
    machineState->machineCheckParameters.servoCom = false;
    machineState->machineCheckParameters.rtcCom = true;

    machineState->motionParameters.condition = CONDITION_STOPPED;
    machineState->motionParameters.mode = MODE_MANUAL;
    machineState->motionParameters.status = MOTIONSTATUS_DISABLED;
    machineState->_function = FUNC_MANUAL_OFF;

    machineState->_lock = _locknew();
    if (machineState->_lock == -1)
    {
        // printf("Error allocating new lock\n");
    }
}
