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
    NavKey *navkey = navkey_create(I2C_ADDR);
    navkey_begin(navkey, 29, 28, INT_DATA | WRAP_DISABLE | DIRE_RIGHT | IPUP_ENABLE);

    navkey_write_counter(navkey, 0);   /* Reset the counter value */
    navkey_write_max(navkey, 100000);  /* Set the maximum threshold*/
    navkey_write_min(navkey, -100000); /* Set the minimum threshold */
    navkey_write_step(navkey, 1);      /* Set the step to 1*/

    navkey_write_double_push_period(navkey, 300); /*Set a period for the double push of 300ms */

    navkey_write_counter(navkey, 0); // reset counter to position
    MachineState lastState;

    bool initial = true;
    int servoCheckCount = 0; // count number of times servo has not communicated properly
    MonitorData lastData = *control->monitorData;
    while (1)
    {
        MachineState currentMachineState = *(control->stateMachine);
        MonitorData data = *(control->monitorData); // Get latest monitor data
        MachinePerformance machinePerformance = *(control->machineProfile->performance);
        mcp_update_register(mcp);

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
        else if (mcp_get_pin(mcp, DISTANCE_LIMIT_MAX, DISTANCE_LIMIT_MAX_REGISTER) == 1) // UPPER
        {
            // Error machine out of bounds (Upper Limit)
            // Update state machine
            state_machine_set(control->stateMachine, PARAM_MOTION_CONDITION, MOTION_UPPER);
        }
        else if (mcp_get_pin(mcp, DISTANCE_LIMIT_MIN, DISTANCE_LIMIT_MIN_REGISTER) == 1) // LOWER
        {
            // Error machine out of bounds
            // Update state machine
            state_machine_set(control->stateMachine, PARAM_MOTION_CONDITION, MOTION_LOWER);
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
                // dyn4_send_command(control->dyn4, dyn4_)
            }
            if (currentMachineState.motionParameters.status == STATUS_DISABLED)
            {
                dyn4_send_command(control->dyn4, dyn4_go_rel_pos, 0);
            }
            else if (currentMachineState.motionParameters.status == STATUS_ENABLED)
            {
                if (lastState.motionParameters.status != STATUS_ENABLED) // Motion enabled initial
                {
                    // dyn4_send_command(control->dyn4, dyn4_go_rel_pos, -1000);
                    // _waitms(300);
                    // dyn4_send_command(control->dyn4, dyn4_sin_wave, 1000);
                }
                if (currentMachineState.motionParameters.mode == MODE_MANUAL)
                {
                    if (lastState.motionParameters.mode != MODE_MANUAL) //  sync navkey and encoder position
                    {
                        // navkey_reset(navkey);
                        // navkey_update_status(navkey);
                    }
                    /* Set functions based on navkey */
                    navkey_update_status(navkey); // Update navkey status registers
                    if (navkey->status.CTRR > 0)  // Center button released
                    {
                        printf("CTRR:%d\n", navkey->status.CTRR);

                        if (currentMachineState.motionParameters.condition == MOTION_STOPPED)
                        {
                            printf("stopped\n");
                            switch (currentMachineState.function)
                            {
                            case FUNC_MANUAL_OFF:
                                control->stateMachine->function = FUNC_MANUAL_INCREMENTAL_JOG;
                                break;
                            case FUNC_MANUAL_INCREMENTAL_JOG:
                                control->stateMachine->function = FUNC_MANUAL_CONTINUOUS_JOG;
                                break;
                            case FUNC_MANUAL_CONTINUOUS_JOG:
                                control->stateMachine->function = FUNC_MANUAL_POSITIONAL_MOVE;
                                break;
                            case FUNC_MANUAL_POSITIONAL_MOVE:
                                control->stateMachine->function = FUNC_MANUAL_HOME;
                                break;
                            case FUNC_MANUAL_HOME:
                                control->stateMachine->function = FUNC_MANUAL_OFF;
                                break;
                            case FUNC_MANUAL_MOVE_GAUGE_LENGTH:
                                break;
                            case FUNC_MANUAL_MOVE_FORCE:
                                break;
                            default:
                                control->stateMachine->function = FUNC_MANUAL_OFF;
                                break;
                            }
                            printf("Function: %d\n", control->stateMachine->function);
                        }
                        else if (control->stateMachine->motionParameters.condition == MOTION_MOVING)
                        {
                            printf("moving\n");
                            control->stateMachine->function = FUNC_MANUAL_OFF;
                        }
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
                            dyn4_send_command(control->dyn4, dyn4_go_rel_pos, 0);
                            control->stateMachine->functionData = 100; // Default step size
                        }
                        if (navkey->status.LTR > 0) // Left released
                        {
                            control->stateMachine->functionData += 100; // Increase step size by 10
                        }
                        if (navkey->status.RTR > 0) // Right released
                        {
                            if (control->stateMachine->functionData > 100)
                            {
                                control->stateMachine->functionData -= 100; // Decrease step size by 10
                            }
                        }
                        if (navkey->status.UPR > 0) // Up released
                        {
                            dyn4_send_command(control->dyn4, dyn4_go_rel_pos, control->stateMachine->functionData); // Increment by step
                        }
                        if (navkey->status.DNR > 0) // Down released
                        {
                            dyn4_send_command(control->dyn4, dyn4_go_rel_pos, -1 * control->stateMachine->functionData); // Increment by step
                        }
                        break;
                    case FUNC_MANUAL_CONTINUOUS_JOG: // Setup the navkey for continuous jog (turn on hold)
                        if (lastState.function != FUNC_MANUAL_CONTINUOUS_JOG)
                        {
                            dyn4_send_command(control->dyn4, dyn4_go_rel_pos, 0);
                            control->stateMachine->functionData = 100; // Default step size
                        }
                        if (navkey->status.LTR > 0) // Left released
                        {
                            control->stateMachine->functionData += 100; // Increase step size by 10
                        }
                        if (navkey->status.RTR > 0) // Right released
                        {
                            if (control->stateMachine->functionData > 100)
                            {
                                control->stateMachine->functionData -= 100; // Decrease step size by 10
                            }
                        }
                        if (navkey->status.UPP > 0) // Up pressed
                        {
                            dyn4_send_command(control->dyn4, dyn4_rotate_const_speed, control->stateMachine->functionData); // Turn CW
                        }
                        if (navkey->status.DNP > 0) // Down pressed
                        {
                            dyn4_send_command(control->dyn4, dyn4_rotate_const_speed, -1 * control->stateMachine->functionData); // Turn CCW
                        }
                        if (navkey->status.UPR > 0) // Up released
                        {
                            dyn4_send_command(control->dyn4, dyn4_go_rel_pos, 0); // Stop motion
                        }
                        if (navkey->status.DNR > 0) // Down released
                        {
                            dyn4_send_command(control->dyn4, dyn4_go_rel_pos, 0); // Stop motion
                        }
                        break;
                    case FUNC_MANUAL_POSITIONAL_MOVE:
                        if (lastState.function != FUNC_MANUAL_POSITIONAL_MOVE)
                        {
                            dyn4_send_command(control->dyn4, dyn4_go_rel_pos, 0);
                            control->stateMachine->functionData = data.position; // Default position
                        }
                        if (navkey->status.LTR > 0) // Left released
                        {
                            control->stateMachine->functionData += 100; // Increase position by 10
                        }
                        if (navkey->status.RTR > 0) // Right released
                        {
                            if (control->stateMachine->functionData > 100)
                            {
                                control->stateMachine->functionData -= 100; // Decrease position by 10
                            }
                        }
                        if (navkey->status.UPR > 0) // Up released
                        {
                            dyn4_send_command(control->dyn4, dyn4_go_rel_pos, control->stateMachine->functionData - data.position); // Stop motion
                        }
                        if (navkey->status.DNR > 0) // Down released
                        {
                            dyn4_send_command(control->dyn4, dyn4_go_rel_pos, control->stateMachine->functionData - data.position); // Stop motion
                        }
                        break;
                    case FUNC_MANUAL_HOME:
                        if (lastState.function != FUNC_MANUAL_HOME)
                        {
                            dyn4_send_command(control->dyn4, dyn4_go_rel_pos, 0);
                            control->stateMachine->functionData = (int)false; // Set to false, will be set to true when home is complete
                        }

                        if (mcp_get_pin(mcp, DISTANCE_LIMIT_MIN, DISTANCE_LIMIT_MIN_REGISTER) == 1 && control->stateMachine->functionData == (int)false) // Wait for limit switch trigger
                        {
                            dyn4_send_command(control->dyn4, dyn4_go_rel_pos, 0);
                            _waitms(100);
                            dyn4_send_command(control->dyn4, dyn4_rotate_const_speed, -10); // Turn CWW at homing speeds/10

                            control->stateMachine->functionData = 2; // Set to 2, will be set to 1 when home is complete
                                                                     // dyn4_send_command(control->dyn4, dyn4_set_origin, 0);
                        }
                        else if (mcp_get_pin(mcp, DISTANCE_LIMIT_MIN, DISTANCE_LIMIT_MIN_REGISTER) == 0 && control->stateMachine->functionData == 2)
                        {
                            dyn4_send_command(control->dyn4, dyn4_go_rel_pos, 0);
                            monitor_set_position(0);
                            control->stateMachine->functionData = (int)true;
                        }
                        if (navkey->status.UPR > 0) // Up released
                        {
                            control->stateMachine->functionData = (int)false;               // Set to false, will be set to true when home is complete
                            dyn4_send_command(control->dyn4, dyn4_rotate_const_speed, 100); // Turn CW at homing speeds
                        }
                        if (navkey->status.DNR > 0) // Down released
                        {
                            control->stateMachine->functionData = (int)false;               // Set to false, will be set to true when home is complete
                            dyn4_send_command(control->dyn4, dyn4_rotate_const_speed, 100); // Turn CW at homing speeds
                        }
                        break;
                    case FUNC_MANUAL_MOVE_GAUGE_LENGTH:
                        dyn4_send_command(control->dyn4, dyn4_go_rel_pos, 0);
                        break;
                    case FUNC_MANUAL_MOVE_FORCE:
                        if (lastState.function != FUNC_MANUAL_MOVE_FORCE)
                        {
                            control->stateMachine->functionData = 0; // Set force to zero
                        }
                        if (navkey->status.LTR > 0) // Left released
                        {
                            control->stateMachine->functionData += 100; // Increase position by 10
                        }
                        if (navkey->status.RTR > 0) // Right released
                        {
                            if (control->stateMachine->functionData > 100)
                            {
                                control->stateMachine->functionData -= 100; // Decrease position by 10
                            }
                        }
                        if (data.force * 1000 < control->stateMachine->functionData)
                        {
                            if (navkey->status.UPP > 0) // Up pressed
                            {
                                dyn4_send_command(control->dyn4, dyn4_rotate_const_speed, 100); // turn CW
                            }
                            if (navkey->status.DNP > 0) // Down pressed
                            {
                                dyn4_send_command(control->dyn4, dyn4_rotate_const_speed, 100); // turn ccw
                            }
                        }
                        else
                        {
                            dyn4_send_command(control->dyn4, dyn4_go_rel_pos, 0); // stop motion
                        }
                        if (navkey->status.UPR > 0) // Up released
                        {
                            dyn4_send_command(control->dyn4, dyn4_go_rel_pos, 0); // stop motion
                        }
                        if (navkey->status.DNR > 0) // Down released
                        {
                            dyn4_send_command(control->dyn4, dyn4_go_rel_pos, 0); // stop motion
                        }
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
            dyn4_send_command(control->dyn4, dyn4_go_rel_pos, 0);
        }
        lastState = currentMachineState;
        lastData = data;
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
