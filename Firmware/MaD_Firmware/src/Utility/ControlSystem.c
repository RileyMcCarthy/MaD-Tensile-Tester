#include "Utility/ControlSystem.h"
#include "Utility/Monitor.h"
#include "Utility/Debug.h"
#include "MotionPlanning.h"
#include "ForceGauge.h"
#include <propeller.h>
#include "i2cNavKey.h"
#include "Memory/MachineProfile.h"
#define CONTROL_MEMORY_SIZE 20000

#define CONTROL_DEGUB 0

static long control_stack[CONTROL_MEMORY_SIZE];

#define SERVO_CHECK_COUNT_MAX 3

typedef enum MoveModes
{
    FUNC_MANUAL_OFF,               // Terminates current motion
    FUNC_MANUAL_INCREMENTAL_JOG,   // Jog by incremental amount
    FUNC_MANUAL_CONTINUOUS_JOG,    // Jog continuously
    FUNC_MANUAL_POSITIONAL_MOVE,   // Move to position
    FUNC_MANUAL_HOME,              // Move to home position
    FUNC_MANUAL_MOVE_GAUGE_LENGTH, // Move gauge to length
    FUNC_MANUAL_MOVE_FORCE,        // Move gauge to force
    FUNC_TEST_LOAD,                // Load test profile
    FUNC_TEST_RUN,                 // Run test profile
    FUNC_TEST_STOP,                // Stop test profile
    FUNC_TEST_TOGGLE_HOLD_RESUME   // Toggle hold/resume
} MoveModes;

extern bool monitorLogData;

static NavKey navkey;

typedef enum homingstate_e
{
    HOMING_NONE,
    HOMING_COMPLETE,
    HOMING_SEEKING,
    HOMING_BACKING_OFF,
    HOMING_SEEKING_SLOW
} HomingState;

typedef enum movetype_e
{
    MOVE_RELATIVE,
    MOVE_ABSOLUTE,
    MOVE_SPEED,
    MOVE_STOP
} MoveType;

/*responsible for sending moves from using navkey, updating state machine, checking for faults*/
static void control_cog(void *arg)
{
    arg;

    /*Initialize NavKey*/
    navkey_begin(&navkey, 29, 28, I2C_ADDR, INT_DATA | WRAP_DISABLE | DIRE_RIGHT | IPUP_ENABLE);
    navkey_write_counter(&navkey, 0);              /* Reset the counter value */
    navkey_write_max(&navkey, 100000);             /* Set the maximum threshold*/
    navkey_write_min(&navkey, -100000);            /* Set the minimum threshold */
    navkey_write_step(&navkey, 1);                 /* Set the step to 1*/
    navkey_write_double_push_period(&navkey, 300); /*Set a period for the double push of 300ms */
    navkey_write_counter(&navkey, 0);              // reset counter to position

    MachineState lastState;
    get_machine_state(&lastState);

    _waitms(1000);

    int lastEncoderRead = 0; // Used to check if the machine is stopped
    
    MonitorData lastData;
    get_monitor_data(&lastData, 10);

    MoveModes currentMoveMode = FUNC_MANUAL_OFF;
    MoveModes lastMoveMode = -1;
    int currentMoveModeParameter = 0;

    // Need to get initial copy of machine profile
    MachineProfile *profile_ptr;
    MachineProfile profile;
    while (!lock_machine_profile_ms(&profile_ptr,1000))
    {
        DEBUG_ERROR("%s","Failed to lock machine profile\n");
    }
    memcpy(&profile, profile_ptr, sizeof(MachineProfile));
    unlock_machine_profile();

    while (1)
    {
        MachineState currentMachineState;
        get_machine_state(&currentMachineState);

        MonitorData monitor_data;
        get_monitor_data(&monitor_data,10);

        // Attempt to update machine profile information
        if (lock_machine_profile(&profile_ptr))
        {
            memcpy(&profile, profile_ptr, sizeof(MachineProfile));
            unlock_machine_profile();
        }

        int forcemN = raw_to_force(monitor_data.forceRaw, profile.configuration.forceGaugeOffset, profile.configuration.forceGaugeGain);

        //mcp_set_pin(&mcp, SERVO_ENABLE_PIN, SERVO_ENABLE_REGISTER, 0);

        /*Check self check state*/
        // Charge Pump
        if (currentMachineState.selfCheckParameters.chargePump)
        {
        //   mcp_set_pin(&mcp, CHARGE_PUMP_PIN, CHARGE_PUMP_REGISTER, 0);
        }
        else
        {
         //   mcp_set_pin(&mcp, CHARGE_PUMP_PIN, CHARGE_PUMP_REGISTER, 0);
        }

        /*Update Machine Check State parameters*/
        // Switched power
        if (true)
        {
            state_machine_set(PARAM_MACHINE_SWITCHED_POWER, (int)true);
        }
        else
        {
            state_machine_set(PARAM_MACHINE_SWITCHED_POWER, (int)false);
        }

        // ESD Distance limits
        if (false)
        {
            state_machine_set(PARAM_MACHINE_ESD_TRAVEL_LIMIT, MOTION_LIMIT_LOWER);
        }
        else if (false)
        {
            state_machine_set(PARAM_MACHINE_ESD_TRAVEL_LIMIT, MOTION_LIMIT_UPPER);
        }
        else
        {
            state_machine_set(PARAM_MACHINE_ESD_TRAVEL_LIMIT, MOTION_LIMIT_OK);
        }

        // ESD
        if (true)
        {
            state_machine_set(PARAM_MACHINE_ESD_SWITCH, (int)true);
        }
        else
        {
            state_machine_set(PARAM_MACHINE_ESD_SWITCH, (int)false);
        }

        // Servo Ok
        if (true)
        {
            state_machine_set(PARAM_MACHINE_SERVO_OK, (int)true);
        }
        else
        {
            state_machine_set(PARAM_MACHINE_SERVO_OK, (int)false);
        }

        /*Update Motion State parameters*/
        // Check conditions for motion
        if (false) // LEGNTH
        {
        }
        else if (false) // FORCE
        {
        }
        else if (forcemN >= 0 && forcemN > profile.performance.maxForceTensile) // Tension
        {
            state_machine_set(PARAM_MOTION_CONDITION, CONDITION_TENSION);
        }
        else if (forcemN < 0 && forcemN < -1*profile.performance.maxForceCompression) // Compression
        {
            state_machine_set(PARAM_MOTION_CONDITION, CONDITION_COMPRESSION);
        }
        else if (false) // UPPER
        {
            // Error machine out of bounds (Upper Limit)
            // Update state machine
            state_machine_set(PARAM_MOTION_CONDITION, CONDITION_UPPER);
        }
        else if (false) // LOWER
        {
            // Error machine out of bounds
            // Update state machine
            state_machine_set(PARAM_MOTION_CONDITION, CONDITION_LOWER);
        }
        else if (false) // Door
        {
            // Error machine door open
            // Update state machine
            state_machine_set(PARAM_MOTION_CONDITION, CONDITION_DOOR);
        }
        else if (abs(lastEncoderRead - monitor_data.encoderRaw) < 2) // STOPPED
        {
            state_machine_set(PARAM_MOTION_CONDITION, CONDITION_STOPPED);
        }
        else // MOVING
        {
            state_machine_set(PARAM_MOTION_CONDITION, CONDITION_MOVING);
        }

        if (currentMachineState.state == STATE_MOTION) // Motion Enabled
        {
            if (lastState.state != STATE_MOTION)
            {
            }
            if (currentMachineState.motionParameters.status == MOTIONSTATUS_DISABLED)
            {
                motion_disable();
            }
            else if (currentMachineState.motionParameters.status == MOTIONSTATUS_MACHINE_LIMIT)
            {
                motion_disable();
            }
            else
            {
                motion_enable();
                if (lastState.motionParameters.status != MOTIONSTATUS_ENABLED) // Motion enabled initial
                {
                    // dyn4_send_command(&dyn4, dyn4_go_rel_pos, -1000);
                    // _waitms(300);
                    // dyn4_send_command(&dyn4, dyn4_sin_wave, 1000);
                }
                if (currentMachineState.motionParameters.mode == MODE_MANUAL)
                {
                    if (lastState.motionParameters.mode != MODE_MANUAL) //  sync navkey and encoder position
                    {
                        // navkey_reset(navkey);
                        // navkey_update_status(navkey);
                    }
                    /* Set functions based on navkey */
                    navkey_update_status(&navkey); // Update navkey status registers
                    if (navkey.status.CTRR > 0)    // Center button released
                    {
                        // printf("CTRR:%d\n", navkey.status.CTRR);

                        if (currentMachineState.motionParameters.condition == CONDITION_STOPPED)
                        {
                            // printf("stopped\n");
                            switch (currentMoveMode)
                            {
                            case FUNC_MANUAL_OFF:
                                currentMoveMode = FUNC_MANUAL_INCREMENTAL_JOG;
                                break;
                            case FUNC_MANUAL_INCREMENTAL_JOG:
                                currentMoveMode = FUNC_MANUAL_CONTINUOUS_JOG;
                                break;
                            case FUNC_MANUAL_CONTINUOUS_JOG:
                                currentMoveMode = FUNC_MANUAL_POSITIONAL_MOVE;
                                break;
                            case FUNC_MANUAL_POSITIONAL_MOVE:
                                currentMoveMode = FUNC_MANUAL_HOME;
                                break;
                            case FUNC_MANUAL_HOME:
                                currentMoveMode = FUNC_MANUAL_OFF;
                                break;
                            case FUNC_MANUAL_MOVE_GAUGE_LENGTH:
                                break;
                            case FUNC_MANUAL_MOVE_FORCE:
                                break;
                            default:
                                currentMoveMode = FUNC_MANUAL_OFF;
                                break;
                            }
                            // printf("Function: %d\n", control->stateMachine->function);
                        }
                        else if (currentMachineState.motionParameters.condition == CONDITION_MOVING)
                        {
                            currentMoveMode = FUNC_MANUAL_OFF;
                        }
                    }

                    // Execute manual mode functions
                    switch (currentMoveMode)
                    {
                    case FUNC_MANUAL_OFF: // Stop current motion
                        //move_servo(control, MOVE_STOP, 0);
                        break;
                    case FUNC_MANUAL_INCREMENTAL_JOG: // Setup the navkey for incremental jog (turn off hold)
                        if (lastMoveMode != FUNC_MANUAL_INCREMENTAL_JOG)
                        {
                            //move_servo(control, MOVE_STOP, 0);
                            currentMoveModeParameter = 1000; // Default step size in um = 1mm
                        }
                        if (navkey.status.LTR > 0) // Left released
                        {
                            if (currentMoveModeParameter < 10000) // No step size above 10mm
                                currentMoveModeParameter /= 10;   // Increase step size by multiple of 10
                        }
                        if (navkey.status.RTR > 0) // Right released
                        {
                            if (currentMoveModeParameter > 10) // No step size below 0.01mm
                            {
                                currentMoveModeParameter *= 10; // Decrease step size by multiple of 10
                            }
                        }
                        if (navkey.status.UPR > 0) // Up released
                        {
                            //move_servo(control, MOVE_RELATIVE, currentMoveModeParameter);
                        }
                        if (navkey.status.DNR > 0) // Down released
                        {
                            //move_servo(control, MOVE_RELATIVE, -1 * currentMoveModeParameter);
                        }
                        break;
                    case FUNC_MANUAL_CONTINUOUS_JOG: // Setup the navkey for continuous jog (turn on hold)
                        if (lastMoveMode != FUNC_MANUAL_CONTINUOUS_JOG)
                        {
                            //move_servo(control, MOVE_STOP, 0);
                            currentMoveModeParameter = 10000; // 10000um/s = 10mm/s
                        }
                        if (navkey.status.LTR > 0) // Left released
                        {
                            currentMoveModeParameter -= 1000; // Increase step size by 1mm
                        }
                        if (navkey.status.RTR > 0) // Right released
                        {
                            currentMoveModeParameter += 1000; // Decrease step size by 1mm
                        }
                        if (navkey.status.UPP > 0) // Up pressed
                        {
                            //move_servo(control, MOVE_SPEED, currentMoveModeParameter);
                        }
                        if (navkey.status.DNP > 0) // Down pressed
                        {
                            //move_servo(control, MOVE_SPEED, -1 * currentMoveModeParameter);
                        }
                        if (navkey.status.UPR > 0) // Up released
                        {
                            //move_servo(control, MOVE_STOP, 0);
                        }
                        if (navkey.status.DNR > 0) // Down released
                        {
                            //move_servo(control, MOVE_STOP, 0);
                        }
                        break;
                    case FUNC_MANUAL_POSITIONAL_MOVE:
                        if (lastMoveMode != FUNC_MANUAL_POSITIONAL_MOVE)
                        {
                            //move_servo(control, MOVE_STOP, 0);
                            currentMoveModeParameter = monitor_data.encoderum; 
                        }
                        if (navkey.status.LTR > 0) // Left released
                        {
                            currentMoveModeParameter -= 1000; // Increase position by 1mm
                        }
                        if (navkey.status.RTR > 0) // Right released
                        {
                            currentMoveModeParameter += 1000; // Decrease position by 1mm
                        }
                        if (navkey.status.UPR > 0) // Up released
                        {
                            //move_servo(control, MOVE_ABSOLUTE, currentMoveModeParameter);
                        }
                        if (navkey.status.DNR > 0) // Down released
                        {
                            //move_servo(control, MOVE_ABSOLUTE, currentMoveModeParameter);
                        }
                        break;
                    case FUNC_MANUAL_HOME:
                        if (lastMoveMode != FUNC_MANUAL_HOME)
                        {
                            //move_servo(control, MOVE_STOP, 0);
                            currentMoveModeParameter = HOMING_NONE; // Set to false, will be set to true when home is complete
                        }

                        /*if (mcp_get_pin(&mcp, DISTANCE_LIMIT_MIN, DISTANCE_LIMIT_MIN_REGISTER) == 1 && currentMoveModeParameter == HOMING_SEEKING) // Wait for limit switch trigger
                        {
                            move_servo(control, MOVE_STOP, 0);
                            _waitms(100);
                            move_servo(control, MOVE_SPEED, -1500); // Turn CWW at homing speeds/10

                            currentMoveModeParameter = HOMING_BACKING_OFF; // Set to 2, will be set to 1 when home is complete
                                                                                       // dyn4_send_command(&dyn4, dyn4_set_origin, 0);
                        }
                        else if (mcp_get_pin(&mcp, DISTANCE_LIMIT_MIN, DISTANCE_LIMIT_MIN_REGISTER) == 0 && currentMoveModeParameter == HOMING_BACKING_OFF)
                        {
                            move_servo(control, MOVE_STOP, 0);
                            _waitms(100);
                            move_servo(control, MOVE_SPEED, 1500); // Turn CCW at homing speeds/10
                            currentMoveModeParameter = HOMING_SEEKING_SLOW;
                        }
                        else if (mcp_get_pin(&mcp, DISTANCE_LIMIT_MIN, DISTANCE_LIMIT_MIN_REGISTER) == 1 && currentMoveModeParameter == HOMING_SEEKING_SLOW)
                        {
                            move_servo(control, MOVE_STOP, 0);
                            _waitms(1000);
                            // dyn4_send_command(&dyn4, dyn4_set_origin, 0x00); // Set dyn4 origin
                            //  move_servo(control, MOVE_RELATIVE, 5000); // Move 5mm to clear the limit switch
                            currentMoveModeParameter = HOMING_COMPLETE;
                        }*/
                        if (navkey.status.UPR > 0) // Up released
                        {
                            currentMoveModeParameter = HOMING_SEEKING; // Set to false, will be set to true when home is complete
                            //move_servo(control, MOVE_SPEED, 10000);                // Turn CW at homing speeds
                        }
                        if (navkey.status.DNR > 0) // Down released
                        {
                            currentMoveModeParameter = HOMING_SEEKING; // Set to false, will be set to true when home is complete
                            //move_servo(control, MOVE_SPEED, 10000);                // Turn CW at homing speeds
                        }
                        break;
                    case FUNC_MANUAL_MOVE_GAUGE_LENGTH:
                        //move_servo(control, MOVE_STOP, 0);
                        break;
                    case FUNC_MANUAL_MOVE_FORCE:
                        if (lastMoveMode != FUNC_MANUAL_MOVE_FORCE)
                        {
                            currentMoveModeParameter = 0; // Set force to zero
                        }
                        if (navkey.status.LTR > 0) // Left released
                        {
                            currentMoveModeParameter += 100; // Increase position by 10
                        }
                        if (navkey.status.RTR > 0) // Right released
                        {
                            if (currentMoveModeParameter > 100)
                            {
                                currentMoveModeParameter -= 100; // Decrease position by 10
                            }
                        }
                        if (forcemN * 1000 < currentMoveModeParameter)
                        {
                            if (navkey.status.UPP > 0) // Up pressed
                            {
                                //move_servo(control, MOVE_SPEED, 100); // turn CW
                            }
                            if (navkey.status.DNP > 0) // Down pressed
                            {
                                //move_servo(control, MOVE_SPEED, 100); // turn ccw
                            }
                        }
                        else
                        {
                            //move_servo(control, MOVE_STOP, 0); // stop motion
                        }
                        if (navkey.status.UPR > 0) // Up released
                        {
                            //move_servo(control, MOVE_STOP, 0); // stop motion
                        }
                        if (navkey.status.DNR > 0) // Down released
                        {
                            //move_servo(control, MOVE_STOP, 0); // stop motion
                        }
                        break;
                    }
                }
                else if (currentMachineState.motionParameters.mode == MODE_TEST)
                {
                }
                else if (currentMachineState.motionParameters.mode == MODE_TEST_RUNNING)
                {
                    if (lastState.motionParameters.mode != MODE_TEST_RUNNING)
                    {
                        motion_test_start();
                        monitorLogData = true;
                    }
                    // Run the loaded test profile

                    if (motion_test_is_empty()) // not a great method of checking if the test is complete, but it works for now
                    {
                        monitorLogData = false;
                        motion_test_end();
                        state_machine_set(PARAM_MOTION_MODE, MODE_TEST);
                        DEBUG_ERROR("%s","Test complete\n");
                    }
                    
                }
            }
        }
        else
        {
            motion_disable();
        }

        lastMoveMode = currentMoveMode;
        lastEncoderRead = monitor_data.encoderRaw;
        memcpy(&lastState, &currentMachineState, sizeof(MachineState));
        memcpy(&lastData, &monitor_data, sizeof(MonitorData));
    }
}

bool control_begin()
{
    int id = _cogstart_C(control_cog, NULL, &control_stack[0], sizeof(long) * CONTROL_MEMORY_SIZE);
    if (id != -1)
    {
        return true;
    }
    return false;
}
