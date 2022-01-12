#include "Control.h"

static long control_stack[64 * 4];

#define SERVO_CHECK_COUNT_MAX 3

static bool move_servo(DYN4 servo, int type, int position)
{
    // move servo and check if move is allowed (conditions or machine limits)
}

/*responsible for moving machine, updating state machine, checking for faults*/
static void control_cog(Control *control)
{
    /*Initialize mcp23017*/
    MCP23017 *mcp = mcp23017_create();
    mcp23017_begin(mcp, GPIO_ADDR, GPIO_SDA, GPIO_SCL);

    /*Initialize NavKey*/
    NavKey *navkey = navkey_create(I2C_SCL, I2C_SDA, I2C_ADDR);
    navkey_reset(navkey);
    navkey_begin(navkey, INT_DATA | WRAP_ENABLE | DIRE_RIGHT | IPUP_ENABLE);

    navkey_write_counter(navkey, (int32_t)0);  /* Reset the counter value */
    navkey_write_max(navkey, (int32_t)10000);  /* Set the maximum threshold*/
    navkey_write_min(navkey, (int32_t)-10000); /* Set the minimum threshold */
    navkey_write_step(navkey, (int32_t)100);   /* Set the step to 1*/

    navkey_write_double_push_period(navkey, 30); /*Set a period for the double push of 300ms */

    int position = control->dyn4->encoder.value();
    navkey_write_counter(navkey, position); // reset counter to position
    MachineState lastState;

    unsigned long timems = _getms();
    unsigned long intervalDYN4Check = 100;
    unsigned long intervalDYN4CheckLast = 0;
    bool initial = true;
    int servoCheckCount = 0; // count number of times servo has not communicated properly

    while (1)
    {
        MachineState currentMachineState = *(control->stateMachine);
        MonitorData data = *(control->monitorData); // Get latest monitor data
        MachinePerformance machinePerformance = *(control->machineProfile->performance);

        /*Check self check state*/
        // Charge Pump
        if (currentMachineState.selfCheckParameters.chargePump)
        {
            mcp_set_pin(mcp, CHARGE_PUMP_PIN, CHARGE_PUMP_REGISTER, 0);
        }
        else
        {
            mcp_set_pin(mcp, CHARGE_PUMP_PIN, CHARGE_PUMP_REGISTER, 1);
        }

        /*Update Machine Check State parameters*/
        // Switched power
        if (mcp_get_pin(mcp, SWITCHED_POWER_PIN, SWITCHED_POWER_REGISTER) == 0)
        {
            state_machine_set(control->stateMachine, PARAM_MACHINE_SWITCHED_POWER, (int)true);
        }
        else
        {
            state_machine_set(control->stateMachine, PARAM_MACHINE_SWITCHED_POWER, (int)false);
        }

        // ESD Distance limits
        if (mcp_get_pin(mcp, ESD_LIMIT_MIN_PIN_NUMBER, ESD_LIMIT_MIN_PIN_REGISTER) == 0)
        {
            state_machine_set(control->stateMachine, PARAM_MACHINE_ESD_TRAVEL_LIMIT, MOTION_LIMIT_LOWER);
        }
        else if (mcp_get_pin(mcp, ESD_LIMIT_MAX_PIN_NUMBER, ESD_LIMIT_MAX_PIN_REGISTER) == 0)
        {
            state_machine_set(control->stateMachine, PARAM_MACHINE_ESD_TRAVEL_LIMIT, MOTION_LIMIT_UPPER);
        }
        else
        {
            state_machine_set(control->stateMachine, PARAM_MACHINE_ESD_TRAVEL_LIMIT, MOTION_LIMIT_OK);
        }

        // ESD
        if (mcp_get_pin(mcp, ESD_ACTIVE_PIN, ESD_ACTIVE_REGISTER) == 1)
        {
            state_machine_set(control->stateMachine, PARAM_MACHINE_ESD_SWITCH, (int)true);
        }
        else
        {
            state_machine_set(control->stateMachine, PARAM_MACHINE_ESD_SWITCH, (int)false);
        }

        // Servo Ok
        if (mcp_get_pin(mcp, SRVORDY_PIN, SRVORDY_REGISTER) == 1)
        {
            state_machine_set(control->stateMachine, PARAM_MACHINE_SERVO_OK, (int)true);
        }
        else
        {
            state_machine_set(control->stateMachine, PARAM_MACHINE_SERVO_OK, (int)false);
        }

        // Force Gauge
        if (data.forceRaw == -1)
        { /*need to change to proper error value, not -1, this is possible force value*/
            state_machine_set(control->stateMachine, PARAM_MACHINE_FORCE_GAUGE_COM, (int)false);
        }
        else
        {
            state_machine_set(control->stateMachine, PARAM_MACHINE_FORCE_GAUGE_COM, (int)true);
        }

        // DYN4
        DYN4_Status status;
        if (dyn4_get_status(control->dyn4, &status) != SUCCESS)
        {
            servoCheckCount++;
            if (servoCheckCount > SERVO_CHECK_COUNT_MAX)
            {
                state_machine_set(control->stateMachine, PARAM_MACHINE_SERVO_COM, (int)false);
            }
        }
        else
        {
            servoCheckCount = 0;
            state_machine_set(control->stateMachine, PARAM_MACHINE_SERVO_COM, (int)true);
        }
        // RTC

        /*Update Motion State parameters*/
        // Check conditions for motion
        if (false) // LEGNTH
        {
        }
        else if (false) // FORCE
        {
        }
        else if (data.force >= 0 && data.force > machinePerformance.maxForceTensile) // Tension
        {
            state_machine_set(control->stateMachine, PARAM_MOTION_CONDITION, MOTION_TENSION);
        }
        else if (data.force < 0 && data.force < -machinePerformance.maxForceCompression) // Compression
        {
            state_machine_set(control->stateMachine, PARAM_MOTION_CONDITION, MOTION_COMPRESSION);
        }
        else if (mcp_get_pin(control->stateMachine, DISTANCE_LIMIT_MAX, DISTANCE_LIMIT_MAX_REGISTER) == 1) // UPPER
        {
            // Error machine out of bounds (Upper Limit)
            // Update state machine
            state_machine_set(control->stateMachine, PARAM_MOTION_CONDITION, MOTION_UPPER);
        }
        else if (mcp_get_pin(control->stateMachine, DISTANCE_LIMIT_MIN, DISTANCE_LIMIT_MIN_REGISTER) == 1) // LOWER
        {
            // Error machine out of bounds
            // Update state machine
        }
        else if (false) // Door
        {
        }
        else if (!status.motorBusy) // STOPPED
        {
            state_machine_set(control->stateMachine, PARAM_MOTION_CONDITION, MOTION_STOPPED);
        }
        else // MOVING
        {
            state_machine_set(control->stateMachine, PARAM_MOTION_CONDITION, MOTION_MOVING);
        }

        if (currentMachineState.state == STATE_MOTION) // Motion Enabled
        {
            if (lastState.state != STATE_MOTION)
            {
                // mcp_set_direction(mcp, CHARGE_PUMP_PIN, CHARGE_PUMP_REGISTER, 0); // set to output
                // mcp_set_pin(mcp, CHARGE_PUMP_PIN, CHARGE_PUMP_REGISTER, 1);       // set to low (inverted)
            }
            if (currentMachineState.motionParameters.status == STATUS_DISABLED)
            {
                dyn4_send_command(control->dyn4, dyn4_go_rel_pos, 0);
            }
            else if (currentMachineState.motionParameters.status == STATUS_ENABLED)
            {
                if (lastState.motionParameters.status != STATUS_ENABLED) // Motion enabled initial
                {
                    dyn4_send_command(control->dyn4, dyn4_go_rel_pos, -1000);
                    _waitms(300);
                    dyn4_send_command(control->dyn4, dyn4_sin_wave, 1000);
                }
                if (currentMachineState.motionParameters.mode == MODE_MANUAL)
                {
                    if (lastState.motionParameters.mode != MODE_MANUAL) //  sync navkey and encoder position
                    {
                        int position = control->dyn4->encoder.value();
                        navkey_write_counter(navkey, position);  // reset counter to position
                        navkey_write_step(navkey, (int32_t)100); // Set increment for manual mode
                    }
                    // Execute manual mode functions
                    switch (currentMachineState.function)
                    {
                    case FUNC_MANUAL_OFF: // Stop current motion
                        dyn4_send_command(control->dyn4, dyn4_go_rel_pos, 0);
                        break;
                    case FUNC_MANUAL_INCREMENTAL_JOG: // Setup the navkey for incremental jog (turn off hold)
                        if (lastState.function != FUNC_MANUAL_INCREMENTAL_JOG)
                        {
                            navkey_write_step(navkey, (int32_t)100); // Set increment
                        }
                        dyn4_send_command(control->dyn4, dyn4_go_rel_pos, 0); // Stop current motion
                        break;
                    case FUNC_MANUAL_CONTINUOUS_JOG: // Setup the navkey for continuous jog (turn on hold)
                        if (lastState.function != FUNC_MANUAL_INCREMENTAL_JOG)
                        {
                            navkey_write_step(navkey, (int32_t)100); // Set increment for manual mode
                        }
                        dyn4_send_command(control->dyn4, dyn4_go_rel_pos, 0); // Stop current motion
                        break;
                    case FUNC_MANUAL_POSITIONAL_MOVE:
                        //
                        dyn4_send_command(control->dyn4, dyn4_go_abs_pos, 0);
                        break;
                    case FUNC_MANUAL_HOME:
                        dyn4_send_command(control->dyn4, dyn4_go_abs_pos, 0);
                        break;
                    case FUNC_MANUAL_MOVE_GAUGE_LENGTH:
                        dyn4_send_command(control->dyn4, dyn4_go_rel_pos, 0);
                        break;
                    case FUNC_MANUAL_MOVE_FORCE:
                        dyn4_send_command(control->dyn4, dyn4_go_rel_pos, 0);
                        break;
                    }
                }
                else if (currentMachineState.motionParameters.mode == MODE_TEST)
                {
                    // Run the loaded test profile
                    if (control->testProfile != NULL)
                    {
                    }
                }
            }
            else if (currentMachineState.motionParameters.status == STATUS_SAMPLE_LIMIT)
            {
            }
            else if (currentMachineState.motionParameters.status == STATUS_MACHINE_LIMIT)
            {
            }
            else if (currentMachineState.motionParameters.status == STATUS_FAULTED)
            {
            }
        }
        else
        {
            // turn off all motion
            // turn off IO that powers dyn4
            // mcp_set_direction(mcp, CHARGE_PUMP_PIN, CHARGE_PUMP_REGISTER, 0); // set to output
            // mcp_set_pin(mcp, SERVO_ENABLE_PIN, SERVO_ENABLE_REGISTER, 0); // low will enable servo
            dyn4_send_command(control->dyn4, dyn4_go_rel_pos, 0);
        }
        lastState = currentMachineState;
    }
}

Control *control_create(MachineProfile *machineProfile, MachineState *stateMachine, DYN4 *dyn4, MonitorData *monitorData)
{
    Control *control = (Control *)malloc(sizeof(Control));
    control->machineProfile = machineProfile;
    control->monitorData = monitorData;
    control->stateMachine = stateMachine;
    control->testProfile = NULL;
    control->dyn4 = dyn4;
    control->cogid = -1;
    return control;
}
bool control_begin(Control *control)
{
    control->cogid = _cogstart_C(control_cog, control, &control_stack[0], sizeof(control_stack));
    if (control->cogid != -1)
    {
        return true;
    }
    return false;
}
