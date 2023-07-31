#include "Utility/ControlSystem.h"
#include "Utility/Monitor.h"
#include "Utility/Debug.h"
#include "MotionPlanning.h"
#include "ForceGauge.h"
#include <propeller.h>
#include "i2cNavKey.h"
#include "Memory/MachineProfile.h"
#include "Main/MaD.h"
#include "Utility/Motion.h"
#define CONTROL_MEMORY_SIZE 5000

#define CONTROL_DEGUB 0

static long control_stack[CONTROL_MEMORY_SIZE];

#define SERVO_CHECK_COUNT_MAX 3

typedef enum MoveModes
{
    FUNC_MANUAL_OFF,               // Terminates current motion
    FUNC_MANUAL_INCREMENTAL_JOG,   // Jog by incremental amount
    FUNC_MANUAL_CONTINUOUS_JOG,    // Jog continuously
    FUNC_MANUAL_HOME,              // Move to home position
    FUNC_MANUAL_MOVE_GAUGE_LENGTH, // Move gauge to length
    FUNC_SET_GAUGE_LENGTH,         // Set gauge length
    FUNC_MAX,
} MoveModes;

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

    /* Initialize IO */
    _pinr(ESD_UPPER_PIN);
    _pinr(ESD_LOWER_PIN);
    _pinr(ESD_SWITCH_PIN);
    _pinr(ENDSTOP_UPPER_PIN);
    _pinr(ENDSTOP_LOWER_PIN);
    _pinr(ENDSTOP_DOOR_PIN);


    /*Initialize NavKey*/
    navkey_begin(&navkey, NAVKEY_SCL, NAVKEY_SDA, NAVKEY_I2C_ADDR, INT_DATA | WRAP_DISABLE | DIRE_RIGHT | IPUP_ENABLE);
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
    Move manual_move;

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
            _pinw(CHARGE_PUMP_PIN, CHARGE_PUMP_PIN_ENABLE);
        }
        else
        {
            _pinw(CHARGE_PUMP_PIN, !CHARGE_PUMP_PIN_ENABLE);
        }

        /*Update Machine Check State parameters*/

        // ESD Distance limits
        if (_pinr(ESD_POWER_PIN) == ESD_POWER_PIN_TRIGGERED && FALSE)
        {
            state_machine_set(PARAM_MACHINE_ESD_CHAIN, ESD_POWER);
        }
        else if (_pinr(ESD_SWITCH_PIN) == ESD_SWITCH_PIN_TRIGGERED)
        {
            state_machine_set(PARAM_MACHINE_ESD_CHAIN, ESD_SWITCH);
        }
        else if (_pinr(ESD_UPPER_PIN) == ESD_UPPER_PIN_TRIGGERED)
        {
            state_machine_set(PARAM_MACHINE_ESD_CHAIN, ESD_UPPER);
        }
        else if (_pinr(ESD_LOWER_PIN) == ESD_LOWER_PIN_TRIGGERED)
        {
            state_machine_set(PARAM_MACHINE_ESD_CHAIN, ESD_LOWER);
        }
        else
        {
            state_machine_set(PARAM_MACHINE_ESD_CHAIN, ESD_OK);
        }

        // Servo Ok
        state_machine_set(PARAM_MACHINE_SERVO_OK, (int)true);

        // Servo Communicating
        if (!currentMachineState.machineCheckParameters.servoCom)
        {
            // Do some check to see if servo is communicating (send step and read encoder)
            state_machine_set(PARAM_MACHINE_SERVO_COM, (int)true);
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
        else if (_pinr(ENDSTOP_UPPER_PIN) == ENDSTOP_UPPER_PIN_TRIGGERED) // UPPER
        {
            // Error machine out of bounds (Upper Limit)
            // Update state machine
            state_machine_set(PARAM_MOTION_CONDITION, CONDITION_UPPER);
        }
        else if (_pinr(ENDSTOP_LOWER_PIN) == ENDSTOP_LOWER_PIN_TRIGGERED) // LOWER
        {
            // Error machine out of bounds
            // Update state machine
            state_machine_set(PARAM_MOTION_CONDITION, CONDITION_LOWER);
        }
        else if (_pinr(ENDSTOP_DOOR_PIN) == ENDSTOP_DOOR_PIN_TRIGGERED) // Door
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
            else if (false && currentMachineState.motionParameters.status == MOTIONSTATUS_MACHINE_LIMIT)
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
                        //navkey_reset(&navkey);
                        // navkey_update_status(navkey);
                    }
                    /* Set functions based on navkey */
                    navkey_update_status(&navkey); // Update navkey status registers
                    NavKeyStatus *status = &(navkey.status);
                    if (status->raw != 0)
                    {
                        DEBUG_INFO("NAVKEY=UPR:%d,UPP:%d,DNR:%d,DNP:%d,RTR:%d,RTP:%d,LTR:%d,LTP:%d,CTRR:%d,CTRP:%d,CTRDP:%d,RINC:%d,RDEC:%d,RMAX:%d,RMIN:%d\n",
                                   status->UPR, status->UPP, status->DNR, status->DNP, status->RTR, status->RTP, status->LTR, status->LTP, status->CTRR, status->CTRP, status->CTRDP, status->RINC, status->RDEC, status->RMAX, status->RMIN);
                    }
                    // WARNING PRESSED AND RELEASE CAN OCCUR ON SAME STATUS CYCLE, DO NOT USE ELSE IF
                    if (navkey.status.CTRR)    // Center button released
                    {
                        currentMoveMode++;
                        if (currentMoveMode == FUNC_MAX)
                        {
                            currentMoveMode = FUNC_MANUAL_OFF;
                        }
                        DEBUG_INFO("FUNC=%d\n", currentMoveMode);
                    }

                    // Execute manual mode functions
                    switch (currentMoveMode)
                    {
                    case FUNC_MANUAL_OFF: // Stop current motion
                        break;
                    case FUNC_MANUAL_INCREMENTAL_JOG: // Setup the navkey for incremental jog (turn off hold)
                        if (lastMoveMode != FUNC_MANUAL_INCREMENTAL_JOG)
                        {
                            manual_move.f = 10; // 10mm/min
                            manual_move.g = 1; // G1 Command
                            manual_move.x = 1; // Move in 1mm in x direction
                        }
                        if (navkey.status.LTR) // Left released
                        {
                            if (manual_move.x > 1) // No step size below 1mm
                            {
                                manual_move.x /= 10;   // Decrease step size by multiple of 10
                            }
                            DEBUG_INFO("DECREASING STEP SIZE, STEP=%f\n", manual_move.x);
                        }
                        if (navkey.status.RTR) // Right released
                        {
                            if (manual_move.x < 100) // No step size above 100mm
                            {
                                manual_move.x *= 10; // Increase step size by multiple of 10
                            }
                            DEBUG_INFO("INCREASING STEP SIZE, STEP=%f\n", manual_move.x);
                        }
                        Move absolute;
                        Move manual_move_copy;
                        memcpy(&absolute, &manual_move, sizeof(Move));
                        absolute.g = 90;
                        if (navkey.status.UPR) // Up released
                        {
                            manual_move_copy.x = abs(manual_move.x);
                            motion_add_move(&absolute);
                            motion_add_move(&manual_move_copy);
                            DEBUG_INFO("FUNCTION=INCREMENTAL_JOG, SPEED=%f, STEP=%f\n", manual_move_copy.f, manual_move_copy.x);
                        }
                        if (navkey.status.DNR > 0) // Down released
                        {
                            manual_move_copy.x = -1*abs(manual_move.x);
                            motion_add_move(&absolute);
                            motion_add_move(&manual_move_copy);
                            DEBUG_INFO("FUNCTION=INCREMENTAL_JOG, SPEED=%f, STEP=%f\n", manual_move_copy.f, manual_move_copy.x);
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
                    }
                }
                else if (currentMachineState.motionParameters.mode == MODE_TEST)
                {
                }
                else if (currentMachineState.motionParameters.mode == MODE_TEST_RUNNING)
                {
                
                    
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
