#include "Control.h"
static long control_stack[64];

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
    while (1)
    {
        MachineState currentMachineState = *(control->stateMachine);
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
            mcp_set_direction(mcp, CHARGE_PUMP_PIN, CHARGE_PUMP_REGISTER, 0); // set to output
            mcp_set_pin(mcp, CHARGE_PUMP_PIN, CHARGE_PUMP_REGISTER, 1);       // set to low (inverted)
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
            control->stateMachine->motionParameters.softUpperLimit = true;
        }
        else if (data.position < machinePerformance.maxPosition)
        {
            // Error machine out of bounds
            // Update state machine
            control->stateMachine->motionParameters.softLowerLimit = true;
        }
        else
        {
            // Machine operating withen bounds, update state machine
            control->stateMachine->motionParameters.softUpperLimit = false;
            control->stateMachine->motionParameters.softLowerLimit = false;
        }

        /*Check force overload*/
        if (data.force >= 0)
        {
            // Tensile forces
            if (data.force > machinePerformance.maxForceTensile)
            {
                control->stateMachine->motionParameters.forceOverload = true;
            }
            else
            {
                control->stateMachine->motionParameters.forceOverload = false;
            }
        }
        else
        {
            // Compression forces
            if (data.force < -machinePerformance.maxForceCompression)
            {
                control->stateMachine->motionParameters.forceOverload = true;
            }
            else
            {
                control->stateMachine->motionParameters.forceOverload = false;
            }
        }

        /*Update Machine Check State parameters*/

        // Switched power
        if (mcp_get_pin(mcp, SWITCHED_POWER_PIN, SWITCHED_POWER_REGISTER) == 0)
        {
            control->stateMachine->machineCheckParameters.switchedPowerOK = true;
        }
        else
        {
            control->stateMachine->machineCheckParameters.switchedPowerOK = false;
        }

        // ESD
        if (mcp_get_pin(mcp, ESD_ACTIVE_PIN, ESD_ACTIVE_REGISTER) == 0)
        {
            control->stateMachine->machineCheckParameters.esdOK = true;
        }
        else
        {
            control->stateMachine->machineCheckParameters.esdOK = false;
        }

        // Servo Ready
        if (mcp_get_pin(mcp, PIN_SRVORDY_PIN, PIN_SRVORDY_REGISTER) == 1)
        {
            control->stateMachine->machineCheckParameters.servoOK = true;
        }
        else
        {
            control->stateMachine->machineCheckParameters.servoOK = false;
        }

        // Force Gauge
        if (data.forceRaw == -1)
        { /*need to change to proper error value, not -1*/
            control->stateMachine->machineCheckParameters.forceGaugeOK = false;
        }
        else
        {
            control->stateMachine->machineCheckParameters.forceGaugeOK = true;
        }

        // RTC

        /*Check IO Expansion for hard upper/lower limits*/
        if (mcp_get_pin(mcp, DISTANCE_LIMIT_MAX, DISTANCE_LIMIT_MAX_REGISTER) == 1)
        {
            control->stateMachine->machineCheckParameters.overTravelLimit = MOTION_OVER_TRAVEL_UPPER;
        }
        else if (mcp_get_pin(mcp, DISTANCE_LIMIT_MIN, DISTANCE_LIMIT_MIN_REGISTER) == 1)
        {
            control->stateMachine->machineCheckParameters.overTravelLimit = MOTION_OVER_TRAVEL_LOWER;
        }
        else
        {
            control->stateMachine->machineCheckParameters.overTravelLimit = MOTION_OVER_TRAVEL_OK;
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
    control->cogid = _cogstart_C(control_cog, control, &control_stack[0], sizeof(long) * 64);
    if (control->cogid != -1)
    {
        return true;
    }
    return false;
}
