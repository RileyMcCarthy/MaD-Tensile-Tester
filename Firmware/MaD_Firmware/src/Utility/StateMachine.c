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
    if (params.esdChain == ESD_OK &&
        params.servoOK &&
        params.forceGaugeCom &&
        params.servoCom
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
        (machine_state.machineCheckParameters.esdChain != ESD_OK)) // Check for esd fault
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

//@TODO use locks to prevent concurrent access to machineState (only require lock if upddating information)
bool state_machine_set(Parameter param, int state)
{
    bool res = true;
    while (!_locktry(machine_state_lock))
        ;
    switch (param)
    {
    case PARAM_SELF_CHARGE_PUMP:
        {
            machine_state.selfCheckParameters.chargePump = state;
        }
        break;
    case PARAM_MACHINE_ESD_CHAIN:
        {
            machine_state.machineCheckParameters.esdChain = state;
            
        }
        break;
    case PARAM_MACHINE_SERVO_OK:
        {
            machine_state.machineCheckParameters.servoOK = state;
            
        }
        break;
    case PARAM_MACHINE_FORCE_GAUGE_COM:
        {
            machine_state.machineCheckParameters.forceGaugeCom = state;
            
        }
        break;
    case PARAM_MACHINE_SERVO_COM:
        {
            machine_state.machineCheckParameters.servoCom = state;
            
        }
        break;
    case PARAM_MOTION_STATUS:
        {
            machine_state.motionParameters.status = state;
            
        }
        break;
    case PARAM_MOTION_CONDITION:
        {
            machine_state.motionParameters.condition = state;
            
        }
        break;
    case PARAM_MOTION_MODE:
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

#define SELF_CHECK_STRING "SELF CHECK"
#define MACHINE_CHECK_STRING "MACHINE CHECK"
#define MOTION_STRING "MOTION"

char * machine_state_to_string(State state)
{
    switch (state)
    {
    case STATE_SELFCHECK:
        return SELF_CHECK_STRING;
    case STATE_MACHINECHECK:
        return MACHINE_CHECK_STRING;
    case STATE_MOTION:
        return MOTION_STRING;
    default:
        return "UNKNOWN";
    }
}

State string_to_machine_state(char *state)
{
    if (strcmp(state, SELF_CHECK_STRING) == 0)
    {
        return STATE_SELFCHECK;
    }
    else if (strcmp(state, MACHINE_CHECK_STRING) == 0)
    {
        return STATE_MACHINECHECK;
    }
    else if (strcmp(state, MOTION_STRING) == 0)
    {
        return STATE_MOTION;
    }
    else
    {
        return -1;
    }
}

#define MOTION_STATUS_DISABLED_STRING "DISABLED"
#define MOTION_STATUS_ENABLED_STRING "ENABLED"
#define MOTION_STATUS_FAULTED_STRING "FAULTED"
#define MOTION_STATUS_SAMPLE_LIMIT_STRING "SAMPLE_LIMIT"
#define MOTION_STATUS_MACHINE_LIMIT_STRING "MACHINE_LIMIT"

char * motion_status_to_string(MotionStatus motionStatus)
{
    switch (motionStatus)
    {
    case MOTIONSTATUS_DISABLED:
        return MOTION_STATUS_DISABLED_STRING;
    case MOTIONSTATUS_ENABLED:
        return MOTION_STATUS_ENABLED_STRING;
    case MOTIONSTATUS_FAULTED:
        return MOTION_STATUS_FAULTED_STRING;
    case MOTIONSTATUS_SAMPLE_LIMIT:
        return MOTION_STATUS_SAMPLE_LIMIT_STRING;
    case MOTIONSTATUS_MACHINE_LIMIT:
        return MOTION_STATUS_MACHINE_LIMIT_STRING;
    default:
        return "UNKNOWN";
    }
}

MotionStatus string_to_motion_status(char *motionStatus)
{
    if (strcmp(motionStatus, MOTION_STATUS_DISABLED_STRING) == 0)
    {
        return MOTIONSTATUS_DISABLED;
    }
    else if (strcmp(motionStatus, MOTION_STATUS_ENABLED_STRING) == 0)
    {
        return MOTIONSTATUS_ENABLED;
    }
    else if (strcmp(motionStatus, MOTION_STATUS_FAULTED_STRING) == 0)
    {
        return MOTIONSTATUS_FAULTED;
    }
    else if (strcmp(motionStatus, MOTION_STATUS_SAMPLE_LIMIT_STRING) == 0)
    {
        return MOTIONSTATUS_SAMPLE_LIMIT;
    }
    else if (strcmp(motionStatus, MOTION_STATUS_MACHINE_LIMIT_STRING) == 0)
    {
        return MOTIONSTATUS_MACHINE_LIMIT;
    }
    else
    {
        return -1;
    }
}

#define ESD_CHAIN_OK_STRING "OK"
#define ESD_CHAIN_POWER_STRING "POWER"
#define ESD_CHAIN_LOWER_STRING "LOWER"
#define ESD_CHAIN_UPPER_STRING "UPPER"
#define ESD_CHAIN_SWITCH_STRING "SWITCH"

char * esd_chain_to_string(ESDChain esdChain)
{
    switch (esdChain)
    {
    case ESD_OK:
        return ESD_CHAIN_OK_STRING;
    case ESD_POWER:
        return ESD_CHAIN_POWER_STRING;
    case ESD_LOWER:
        return ESD_CHAIN_LOWER_STRING;
    case ESD_UPPER:
        return ESD_CHAIN_UPPER_STRING;
    case ESD_SWITCH:
        return ESD_CHAIN_SWITCH_STRING;
    default:
        return "UNKNOWN";
    }
}

ESDChain string_to_esd_chain(char *esdChain)
{
    if (strcmp(esdChain, ESD_CHAIN_OK_STRING) == 0)
    {
        return ESD_OK;
    }
    else if (strmp(esdChain, ESD_CHAIN_POWER_STRING) == 0)
    {
        return ESD_POWER;
    }
    else if (strcmp(esdChain, ESD_CHAIN_LOWER_STRING) == 0)
    {
        return ESD_LOWER;
    }
    else if (strcmp(esdChain, ESD_CHAIN_UPPER_STRING) == 0)
    {
        return ESD_UPPER;
    }
    else if (strcmp(esdChain, ESD_CHAIN_SWITCH_STRING) == 0)
    {
        return ESD_SWITCH;
    }
    else
    {
        return -1;
    }
}

#define CONDITION_FORCE_STRING "FORCE"
#define CONDITION_TENSION_STRING "TENSION"
#define CONDITION_COMPRESSION_STRING "COMPRESSION"
#define CONDITION_UPPER_STRING "UPPER"
#define CONDITION_LOWER_STRING "LOWER"
#define CONDITION_DOOR_STRING "DOOR"
#define CONDITION_STOPPED_STRING "STOPPED"
#define CONDITION_MOVING_STRING "MOVING"

char * motion_condition_to_string(MotionCondition motionCondition)
{
    switch (motionCondition)
    {
    case CONDITION_FORCE:
        return CONDITION_FORCE_STRING;
    case CONDITION_TENSION:
        return CONDITION_TENSION_STRING;
    case CONDITION_COMPRESSION:
        return CONDITION_COMPRESSION_STRING;
    case CONDITION_UPPER:
        return CONDITION_UPPER_STRING;
    case CONDITION_LOWER:
        return CONDITION_LOWER_STRING;
    case CONDITION_DOOR:
        return CONDITION_DOOR_STRING;
    case CONDITION_STOPPED:
        return CONDITION_STOPPED_STRING;
    case CONDITION_MOVING:
        return CONDITION_MOVING_STRING;
    default:
        return "UNKNOWN";
    }
}

MotionCondition string_to_motion_condition(char *motionCondition)
{
    if (strcmp(motionCondition, CONDITION_FORCE_STRING) == 0)
    {
        return CONDITION_FORCE;
    }
    else if (strcmp(motionCondition, CONDITION_TENSION_STRING) == 0)
    {
        return CONDITION_TENSION;
    }
    else if (strcmp(motionCondition, CONDITION_COMPRESSION_STRING) == 0)
    {
        return CONDITION_COMPRESSION;
    }
    else if (strcmp(motionCondition, CONDITION_UPPER_STRING) == 0)
    {
        return CONDITION_UPPER;
    }
    else if (strcmp(motionCondition, CONDITION_LOWER_STRING) == 0)
    {
        return CONDITION_LOWER;
    }
    else if (strcmp(motionCondition, CONDITION_DOOR_STRING) == 0)
    {
        return CONDITION_DOOR;
    }
    else if (strcmp(motionCondition, CONDITION_STOPPED_STRING) == 0)
    {
        return CONDITION_STOPPED;
    }
    else if (strcmp(motionCondition, CONDITION_MOVING_STRING) == 0)
    {
        return CONDITION_MOVING;
    }
    else
    {
        return -1;
    }
}

#define MODE_MANUAL_STRING "MANUAL"
#define MODE_TEST_STRING "TEST"
#define MODE_TEST_RUNNING_STRING "TEST_RUNNING"

char * motion_mode_to_string(MotionMode motionMode)
{
    switch (motionMode)
    {
    case MODE_MANUAL:
        return MODE_MANUAL_STRING;
    case MODE_TEST:
        return MODE_TEST_STRING;
    case MODE_TEST_RUNNING:
        return MODE_TEST_RUNNING_STRING;
    default:
        return "UNKNOWN";
    }
}

MotionMode string_to_motion_mode(char *motionMode)
{
    if (strcmp(motionMode, MODE_MANUAL_STRING) == 0)
    {
        return MODE_MANUAL;
    }
    else if (strcmp(motionMode, MODE_TEST_STRING) == 0)
    {
        return MODE_TEST;
    }
    else if (strcmp(motionMode, MODE_TEST_RUNNING_STRING) == 0)
    {
        return MODE_TEST_RUNNING;
    }
    else
    {
        return -1;
    }
}

void machine_state_init()
{
    machine_state.state = STATE_SELFCHECK;

    machine_state.selfCheckParameters.chargePump = false;

    machine_state.machineCheckParameters.esdChain = ESD_OK;
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
