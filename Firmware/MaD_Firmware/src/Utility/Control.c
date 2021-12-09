#include "Control.h"
static long control_stack[64];

/*responsible for moving machine, updating state machine, checking for faults*/
static void control_cog(Control *control)
{
    int position = control->dyn4->encoder.value();
    navkey_write_counter(control->navkey, position); // reset counter to position
    State lastState = NULL;
    while (1)
    {
        MachineState currentMachineState = *(control->stateMachine);
        if (currentMachineState.currentState == STATE_MOTION)
        {
            if (currentMachineState.motionParameters.mode == MODE_MANUAL)
            {
                if (lastState != MODE_MANUAL)
                {
                    int position = control->dyn4->encoder.value();
                    navkey_write_counter(control->navkey, position);  // reset counter to position
                    navkey_write_step(control->navkey, (int32_t)100); // Set increment for manual mode
                }
                // Use Navkey to move motor
                int position = navkey_read_counter_int(control->navkey);
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
                if (lastState != MODE_OVERRIDE)
                {
                    // stop motor from moving, keep dyn4 powered
                    dyn4_send_command(control->dyn4, dyn4_go_rel_pos, 0);
                    int position = control->dyn4->encoder.value();
                    navkey_write_counter(control->navkey, position); // reset counter to position
                    navkey_write_step(control->navkey, (int32_t)10); // Set increment for manual mode
                }
                // Use Navkey to move motor very slowly
                int position = navkey_read_counter_int(control->navkey);
                dyn4_send_command(control->dyn4, dyn4_go_abs_pos, position);
            }
        }
        else
        {
            // turn off all motion
            // turn off IO that powers dyn4
            mcp_set_pin(mcp, PIN_ENABLEDYN4, 0);
        }
        // Check for system exeeding performance conditions
        MonitorData data = control->monitorData[0]; // Get latest monitor data
        MachinePerformance machinePerformance = *(control->machineProfile->performance);

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
        if (data.force == -1)
        { /*need to change to proper error value, not -1*/
            control->stateMachine->machineCheckParameters.forceGaugeResponding = false;
        }
        else
        {
            control->stateMachine->machineCheckParameters.forceGaugeResponding = true;
        }

        /*Check IO Expansion for hard upper/lower limits*/
        if (mcp_get_pin(mcp, PIN_UPPER_LIMIT) == 1)
        {
            control->stateMachine->motionParameters.hardUpperLimit = true;
        }
        if (mcp_get_pin(mcp, PIN_LOWER_LIMIT) == 1)
        {
            control->stateMachine->motionParameters.hardLowerLimit = true;
        }

        /*Check ESD pin*/
        if (mcp_get_pin(mcp, PIN_ESD) == 1)
        {
            control->stateMachine->machineCheckParameters.esd = true;
        }
        else
        {
            control->stateMachine->machineCheckParameters.esd = false;
        }

        /*Check IO Expansion for servoReady*/
        if (mcp_get_pin(mcp, PIN_SRVORDY) == 1)
        {
            control->stateMachine->machineCheckParameters.servoReady = true;
        }
        else
        {
            control->stateMachine->machineCheckParameters.servoReady = false;
        }
        lastState = currentMachineState.currentState;
    }
}

bool control_begin(Control *control, MachineProfile *machineProfile, MonitorData *monitorData, MachineState *stateMachine, MCP23017 *mcp, DYN4 *dyn4, NavKey *navkey)
{
    control->machineProfile = machineProfile;
    control->monitorData = monitorData;
    control->stateMachine = stateMachine;
    control->mcp = mcp;
    control->testProfile = NULL;
    control->navkey = navkey;
    control->dyn4 = dyn4;
    control->cogid = _cogstart_C(control_cog, control, &control_stack[0], sizeof(long) * 64);
    if (control->cogid != -1)
    {
        return true;
    }
    return false;
}
