#include "Utility/StateMachine.h"
#include "Utility/Debug.h"
static MachineState machine_state;
static int machine_state_lock;

/**
 * @brief Initial state on power up. Automatic transition into if any Self Check condition not satisfied. No motion shall occur in Self Check state.
 *
 */
static State state_machine_self_check()
{
    State newState;

    machine_state.selfCheckParameters.chargePump = true;

    // Decide the next state
    if (machine_state.selfCheckParameters.chargePump)
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
static State state_machine_check()
{
    // Check previous state
    State newState = state_machine_self_check();
    if (newState != STATE_MACHINECHECK) // Ensure self check state is satisfied
    {
        return newState;
    }

    MachineCheckParameters params = machine_state.machineCheckParameters;
    // Check internal parameters
    if (params.switchedPower &&
        params.esdTravelLimit == MOTION_LIMIT_OK &&
        params.esdSwitch &&
        params.servoOK &&
        params.forceGaugeCom
        //params.servoCom &&
        )
    {
        return STATE_MOTION; // Internal parameters passed, change to MOTION
    }
    else
    {
        // Internal parameters failed, stay in Machine Check
        return STATE_MACHINECHECK;
    }
}

static State state_machine_motion()
{

    // Decide the next state
    State newState = state_machine_check();
    if (newState != STATE_MOTION) // Set default exit state parameters
    {
        // Set state based internal parameters
        if (machine_state.state == STATE_MOTION && // Ensure previous state is MOTION
            (machine_state.machineCheckParameters.esdTravelLimit != MOTION_LIMIT_OK ||
             !machine_state.machineCheckParameters.esdSwitch)) // Check for esd fault
        {
            // Motion exited due to esd fault
            // printf("motion faulted\n");
            machine_state.motionParameters.status = MOTIONSTATUS_FAULTED;
        }
        else
        {
            machine_state.motionParameters.status = MOTIONSTATUS_DISABLED;
        }
        // printf("machine check failed\n");
        machine_state.motionParameters.mode = MODE_MANUAL;
        return newState;
    }

    // Check internal parameters
    if (machine_state.motionParameters.status != MOTIONSTATUS_DISABLED)
    {
        switch (machine_state.motionParameters.condition) // Update status based on condition
        {
        case CONDITION_LENGTH:
            machine_state.motionParameters.status = MOTIONSTATUS_SAMPLE_LIMIT;
            break;
        case CONDITION_FORCE:
            machine_state.motionParameters.status = MOTIONSTATUS_SAMPLE_LIMIT;
            break;
        case CONDITION_TENSION:
            machine_state.motionParameters.status = MOTIONSTATUS_MACHINE_LIMIT;
            break;
        case CONDITION_COMPRESSION:
            machine_state.motionParameters.status = MOTIONSTATUS_MACHINE_LIMIT;
            break;
        case CONDITION_UPPER:
            machine_state.motionParameters.status = MOTIONSTATUS_MACHINE_LIMIT;
            break;
        case CONDITION_LOWER:
            machine_state.motionParameters.status = MOTIONSTATUS_MACHINE_LIMIT;
            break;
        case CONDITION_DOOR:
            machine_state.motionParameters.status = MOTIONSTATUS_MACHINE_LIMIT;
            break;
        case CONDITION_STOPPED:
            if (machine_state.motionParameters.status != MOTIONSTATUS_ENABLED && machine_state.motionParameters.status != MOTIONSTATUS_DISABLED)
            {
                // Motion returning from condition, enter enabled state
                machine_state.motionParameters.status = MOTIONSTATUS_ENABLED;
            }
            break;
        case CONDITION_MOVING:
            if (machine_state.motionParameters.status != MOTIONSTATUS_ENABLED && machine_state.motionParameters.status != MOTIONSTATUS_DISABLED)
            {
                // Motion returning from condition, enter enabled state
                machine_state.motionParameters.status = MOTIONSTATUS_ENABLED;
            }
            break;
        }
    }
    else
    {
        machine_state.motionParameters.mode = MODE_MANUAL;
    }

    return STATE_MOTION;
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
           motionParameters1->servoCom == motionParameters2->servoCom;
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
           machineState1->state == machineState2->state;
}

//@TODO use locks to prevent concurrent access to machineState (only require lock if upddating information)
bool state_machine_set(Parameter param, int state)
{
    bool res = true;
    while (!_locktry(machine_state_lock))
        ;
    switch (param)
    {
    case PARAM_SELF_CHARGE_PUMP:
        if (machine_state.selfCheckParameters.chargePump != state)
        {
            machine_state.selfCheckParameters.chargePump = state;
        }
        break;
    case PARAM_MACHINE_SWITCHED_POWER:
        if (machine_state.machineCheckParameters.switchedPower != state)
        {
            machine_state.machineCheckParameters.switchedPower = state;
            
        }
        break;
    case PARAM_MACHINE_ESD_TRAVEL_LIMIT:
        if (machine_state.machineCheckParameters.esdTravelLimit != state)
        {
            machine_state.machineCheckParameters.esdTravelLimit = state;
            
        }
        break;
    case PARAM_MACHINE_ESD_SWITCH:
        if (machine_state.machineCheckParameters.esdSwitch != state)
        {
            machine_state.machineCheckParameters.esdSwitch = state;
            
        }
        break;
    case PARAM_MACHINE_SERVO_OK:
        if (machine_state.machineCheckParameters.servoOK != state)
        {
            machine_state.machineCheckParameters.servoOK = state;
            
        }
        break;
    case PARAM_MACHINE_FORCE_GAUGE_COM:
        if (machine_state.machineCheckParameters.forceGaugeCom != state)
        {
            machine_state.machineCheckParameters.forceGaugeCom = state;
            
        }
        break;
    case PARAM_MACHINE_SERVO_COM:
        if (machine_state.machineCheckParameters.servoCom != state)
        {
            machine_state.machineCheckParameters.servoCom = state;
            
        }
        break;
    case PARAM_MOTION_STATUS:
        if (machine_state.motionParameters.status != state)
        {
            machine_state.motionParameters.status = state;
            
        }
        break;
    case PARAM_MOTION_CONDITION:
        if (machine_state.motionParameters.condition != state)
        {
            machine_state.motionParameters.condition = state;
            
        }
        break;
    case PARAM_MOTION_MODE:
        if (machine_state.motionParameters.mode != state)
        {
            if (machine_state.motionParameters.mode != MODE_TEST && state == MODE_TEST_RUNNING)
            { // Must be in test mode to run test
                DEBUG_ERROR("%s","Must be in test mode to run test\n");
                res = false;
                break;
            }
            else
            {
                machine_state.motionParameters.mode = state;
            }
        }
        break;
    }

    machine_state.state = state_machine_motion();
    _lockrel(machine_state_lock);
    return res;
}

bool get_machine_state(MachineState *machineState)
{
    while (!_locktry(machine_state_lock))
    {
        //DEBUG_ERROR("%s","Failed to get machine state lock\n");
    }
    memcpy(machineState, &machine_state, sizeof(MachineState));
    _lockrel(machine_state_lock);
    return true;
}

void machine_state_init()
{
    machine_state.state = STATE_SELFCHECK;

    machine_state.selfCheckParameters.chargePump = false;

    machine_state.machineCheckParameters.switchedPower = false;
    machine_state.machineCheckParameters.esdTravelLimit = MOTION_LIMIT_OK;
    machine_state.machineCheckParameters.esdSwitch = false;
    machine_state.machineCheckParameters.servoOK = false;
    machine_state.machineCheckParameters.forceGaugeCom = false;
    machine_state.machineCheckParameters.servoCom = false;

    machine_state.motionParameters.condition = CONDITION_STOPPED;
    machine_state.motionParameters.mode = MODE_MANUAL;
    machine_state.motionParameters.status = MOTIONSTATUS_DISABLED;

    machine_state_lock = _locknew();
    if (machine_state_lock== -1)
    {
         DEBUG_ERROR("%s", "Error allocating new lock\n");
    }
}
