#include "Control.h"
static long control_stack[64 * 4];

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
    while (1)
    {
        MachineState currentMachineState = *(control->stateMachine);

        // DYN4
        DYN4_Status status;

        if (dyn4_get_status(control->dyn4, &status) != SUCCESS)
        {
            state_machine_set(control->stateMachine, PARAM_DYN4OK, (int)false);
        }
        else
        {
            state_machine_set(control->stateMachine, PARAM_DYN4OK, (int)true);
        }

        if (currentMachineState.currentState == STATE_MOTION)
        {
            if (lastState.currentState != STATE_MOTION)
            {
                mcp_set_direction(mcp, CHARGE_PUMP_PIN, CHARGE_PUMP_REGISTER, 0); // set to output
                mcp_set_pin(mcp, CHARGE_PUMP_PIN, CHARGE_PUMP_REGISTER, 1);       // set to low (inverted)
            }

            if (currentMachineState.motionParameters.mode == MODE_MANUAL)
            {
                if (lastState.motionParameters.mode != MODE_MANUAL)
                {
                    int position = control->dyn4->encoder.value();
                    navkey_write_counter(navkey, position);  // reset counter to position
                    navkey_write_step(navkey, (int32_t)100); // Set increment for manual mode
                }
                // Use Navkey to move motor
                int position = navkey_read_counter_int(navkey);
                dyn4_send_command(control->dyn4, dyn4_go_abs_pos, position);
            }
            else if (currentMachineState.motionParameters.mode == MODE_AUTOMATIC)
            {
                // Run the loaded test profile
                if (control->testProfile != NULL)
                {
                }
            }
            else if (currentMachineState.motionParameters.mode == MODE_OVERRIDE)
            {
                if (lastState.motionParameters.mode != MODE_OVERRIDE)
                {
                    // stop motor from moving, keep dyn4 powered
                    dyn4_send_command(control->dyn4, dyn4_go_rel_pos, 0);
                    int position = control->dyn4->encoder.value();
                    navkey_write_counter(navkey, position); // reset counter to position
                    navkey_write_step(navkey, (int32_t)10); // Set increment for manual mode
                }
                // Use Navkey to move motor very slowly
                int position = navkey_read_counter_int(navkey);
                dyn4_send_command(control->dyn4, dyn4_go_abs_pos, position);
            }
        }
        else
        {
            // turn off all motion
            // turn off IO that powers dyn4
            // mcp_set_direction(mcp, CHARGE_PUMP_PIN, CHARGE_PUMP_REGISTER, 0); // set to output
            // mcp_set_pin(mcp, SERVO_ENABLE_PIN, SERVO_ENABLE_REGISTER, 0); // low will enable servo
        }

        if (currentMachineState.selfCheckParameters.chargePumpOK)
        {
            mcp_set_pin(mcp, CHARGE_PUMP_PIN, CHARGE_PUMP_REGISTER, 0);
        }
        else
        {
            mcp_set_pin(mcp, CHARGE_PUMP_PIN, CHARGE_PUMP_REGISTER, 1);
        }

        // Check for system exeeding performance conditions
        MonitorData data = *(control->monitorData); // Get latest monitor data
        MachinePerformance machinePerformance = *(control->machineProfile->performance);

        /*Update Motion State parameters*/

        /*Check soft positional limits*/
        if (data.position > machinePerformance.maxPosition)
        {
            // Error machine out of bounds (Upper Limit)
            // Update state machine
            state_machine_set(control->stateMachine, PARAM_CONDITION, MOTION_UPPER);
        }
        else if (data.position < machinePerformance.maxPosition)
        {
            // Error machine out of bounds
            // Update state machine
            state_machine_set(control->stateMachine, PARAM_CONDITION, MOTION_LOWER);
        }
        else if (data.force >= 0 && data.force > machinePerformance.maxForceTensile) /*Check force overload tension*/
        {
            state_machine_set(control->stateMachine, PARAM_CONDITION, MOTION_TENSION);
        }
        else if (data.force < 0 && data.force < -machinePerformance.maxForceCompression) /*Check force overload compression*/
        {
            state_machine_set(control->stateMachine, PARAM_CONDITION, MOTION_COMPRESSION);
        }
        else
        {
            if (status.motorBusy)
            {
                state_machine_set(control->stateMachine, PARAM_CONDITION, MOTION_MOVING);
            }
            else
            {
                state_machine_set(control->stateMachine, PARAM_CONDITION, MOTION_STOPPED);
            }
        }

        /*Update Machine Check State parameters*/

        // Switched power
        if (mcp_get_pin(mcp, SWITCHED_POWER_PIN, SWITCHED_POWER_REGISTER) == 0)
        {
            state_machine_set(control->stateMachine, PARAM_SWITCHEDPOWEROK, (int)true);
        }
        else
        {
            state_machine_set(control->stateMachine, PARAM_SWITCHEDPOWEROK, (int)false);
        }

        // ESD Distance limits
        if (mcp_get_pin(mcp, ESD_LIMIT_MIN_PIN_NUMBER, ESD_LIMIT_MIN_PIN_REGISTER) == 0)
        {
            state_machine_set(control->stateMachine, PARAM_OVERTRAVELLIMIT, MOTION_OVER_TRAVEL_LOWER);
        }
        else if (mcp_get_pin(mcp, ESD_LIMIT_MAX_PIN_NUMBER, ESD_LIMIT_MAX_PIN_REGISTER) == 0)
        {
            state_machine_set(control->stateMachine, PARAM_OVERTRAVELLIMIT, MOTION_OVER_TRAVEL_UPPER);
        }
        else
        {
            state_machine_set(control->stateMachine, PARAM_OVERTRAVELLIMIT, MOTION_OVER_TRAVEL_OK);
        }

        // ESD
        if (mcp_get_pin(mcp, ESD_ACTIVE_PIN, ESD_ACTIVE_REGISTER) == 1)
        {
            state_machine_set(control->stateMachine, PARAM_ESDOK, (int)true);
        }
        else
        {
            state_machine_set(control->stateMachine, PARAM_ESDOK, (int)false);
        }

        // Servo ready
        if (mcp_get_pin(mcp, SRVORDY_PIN, SRVORDY_REGISTER) == 1)
        {
            state_machine_set(control->stateMachine, PARAM_SERVOOK, (int)true);
        }
        else
        {
            state_machine_set(control->stateMachine, PARAM_SERVOOK, (int)false);
        }

        // Force Gauge
        if (data.forceRaw == -1)
        { /*need to change to proper error value, not -1, this is possible force value*/
            state_machine_set(control->stateMachine, PARAM_FORCEGAUGEOK, (int)false);
        }
        else
        {
            state_machine_set(control->stateMachine, PARAM_FORCEGAUGEOK, (int)true);
        }
        // RTC

        // Travel Limits
        if (mcp_get_pin(mcp, DISTANCE_LIMIT_MIN, DISTANCE_LIMIT_MIN_REGISTER) == 1)
        {
            state_machine_set(control->stateMachine, PARAM_TRAVELLIMIT, MOTION_OVER_TRAVEL_LOWER);
        }
        else if (mcp_get_pin(mcp, DISTANCE_LIMIT_MAX, DISTANCE_LIMIT_MAX_REGISTER) == 1)
        {
            state_machine_set(control->stateMachine, PARAM_TRAVELLIMIT, MOTION_OVER_TRAVEL_UPPER);
        }
        else
        {
            state_machine_set(control->stateMachine, PARAM_TRAVELLIMIT, MOTION_OVER_TRAVEL_OK);
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
