#include "ControlSystem.h"
#include "MotionPlanning.h"
#include "ForceGauge.h"
#include "DYN.h"
#include <propeller2.h>

#define CONTROL_MEMORY_SIZE 20000

#define CONTROL_DEGUB 0

static long control_stack[CONTROL_MEMORY_SIZE];

#define SERVO_CHECK_COUNT_MAX 3

extern bool monitorWriteData;

static NavKey navkey;
static MCP23017 mcp;

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

static bool move_servo(ControlSystem *control, MoveType type, int value)
{
    if (control == NULL)
    {
        serial_debug("move_servo: control is null\n");
        return false;
    }
    // move servo and check if move is allowed (conditions or machine limits)
    bool moveAllowed = true;
    switch (type)
    {
    case MOVE_STOP:
        move_rel32(DYN1_ADDR, 0); // stop servo
        break;
    case MOVE_RELATIVE:
    {
        // serial_debug("moving relitive\n");

        int positionSteps = mm_to_steps(value / 1000.0, &(control->machineProfile->configuration)) + control->monitorData->encoderRaw;
        // serial_debug("positionSteps:%d,%d\n", positionSteps, control->monitorData->encoderRaw);
        move_abs32(DYN1_ADDR, positionSteps);
        break;
    }
    case MOVE_ABSOLUTE:
    {
        int deltaSteps = mm_to_steps((value) / 1000.0, &(control->machineProfile->configuration));
        move_abs32(DYN1_ADDR, deltaSteps);
        break;
    }
    case MOVE_SPEED:
    {
        int rpm = ((double)value / 1000.0) * (60.0 / 80.0); // um/s to rpm (mm/s)*((sec/min)/(mm/rev))
        Turn_const_speed(DYN1_ADDR, rpm);                   // Stop motion (mm/r)*(r/min)/(sec/min)        break;
    }
    }

    return true;
}

static RunMotionProfile run;

/*responsible for moving machine, updating state machine, checking for faults*/
static void control_cog(ControlSystem *control)
{
    /*Initialize mcp23017*/
    while (!mcp23017_begin(&mcp, GPIO_ADDR, GPIO_SDA, GPIO_SCL))
    {
        serial_debug("MCP23017 not communicating, trying again\n");
        _waitms(100);
    }

    // Start dyn communication
    dyn_init();

    /*Initialize NavKey*/
    navkey_begin(&navkey, 29, 28, I2C_ADDR, INT_DATA | WRAP_DISABLE | DIRE_RIGHT | IPUP_ENABLE);
    navkey_write_counter(&navkey, 0);              /* Reset the counter value */
    navkey_write_max(&navkey, 100000);             /* Set the maximum threshold*/
    navkey_write_min(&navkey, -100000);            /* Set the minimum threshold */
    navkey_write_step(&navkey, 1);                 /* Set the step to 1*/
    navkey_write_double_push_period(&navkey, 300); /*Set a period for the double push of 300ms */
    navkey_write_counter(&navkey, 0);              // reset counter to position

    MachineState lastState = *(control->stateMachine);
    _waitms(1000);

    serial_debug("Control cog started\n");

    // For running test profiles;
    long startTime = 0;
    int startPosition = 0;
    int lastPosition = 0;

    int lastEncoderRead = 0;

    bool initial = true;
    int servoCheckCount = 0; // count number of times servo has not communicated properly
    MonitorData lastData = *control->monitorData;
#if CONTROL_DEGUB
    int testms = 0;
#endif
    while (1)
    {
#if CONTROL_DEGUB
        testms = _getms();
#endif
        MachineState currentMachineState = *(control->stateMachine);
        MonitorData data = *(control->monitorData); // Get latest monitor data
        int forcemN = raw_to_force(control->monitorData->forceRaw, &(control->machineProfile->configuration));
        MachinePerformance machinePerformance = control->machineProfile->performance;
        mcp_update_register(&mcp);
        mcp_set_pin(&mcp, SERVO_ENABLE_PIN, SERVO_ENABLE_REGISTER, 0);

#if CONTROL_DEGUB
        serial_debug("TIME1: %d\n", _getms() - testms);
        testms = _getms();
#endif

        /*Check self check state*/
        // Charge Pump
        if (currentMachineState.selfCheckParameters.chargePump)
        {
            mcp_set_pin(&mcp, CHARGE_PUMP_PIN, CHARGE_PUMP_REGISTER, 0);
        }
        else
        {
            mcp_set_pin(&mcp, CHARGE_PUMP_PIN, CHARGE_PUMP_REGISTER, 0);
        }

        /*Update Machine Check State parameters*/
        // Switched power
        if (mcp_get_pin(&mcp, SWITCHED_POWER_PIN, SWITCHED_POWER_REGISTER) == 0)
        {
            state_machine_set(control->stateMachine, PARAM_MACHINE_SWITCHED_POWER, (int)true);
        }
        else
        {
            state_machine_set(control->stateMachine, PARAM_MACHINE_SWITCHED_POWER, (int)false);
        }

        // ESD Distance limits
        if (mcp_get_pin(&mcp, ESD_LIMIT_MIN_PIN_NUMBER, ESD_LIMIT_MIN_PIN_REGISTER) == 0)
        {
            state_machine_set(control->stateMachine, PARAM_MACHINE_ESD_TRAVEL_LIMIT, MOTION_LIMIT_LOWER);
        }
        else if (mcp_get_pin(&mcp, ESD_LIMIT_MAX_PIN_NUMBER, ESD_LIMIT_MAX_PIN_REGISTER) == 0)
        {
            state_machine_set(control->stateMachine, PARAM_MACHINE_ESD_TRAVEL_LIMIT, MOTION_LIMIT_UPPER);
        }
        else
        {
            state_machine_set(control->stateMachine, PARAM_MACHINE_ESD_TRAVEL_LIMIT, MOTION_LIMIT_OK);
        }

        // ESD
        if (mcp_get_pin(&mcp, ESD_ACTIVE_PIN, ESD_ACTIVE_REGISTER) == 1)
        {
            state_machine_set(control->stateMachine, PARAM_MACHINE_ESD_SWITCH, (int)true);
        }
        else
        {
            state_machine_set(control->stateMachine, PARAM_MACHINE_ESD_SWITCH, (int)false);
        }

        // Servo Ok
        if (mcp_get_pin(&mcp, SRVORDY_PIN, SRVORDY_REGISTER) == 1)
        {
            state_machine_set(control->stateMachine, PARAM_MACHINE_SERVO_OK, (int)true);
        }
        else
        {
            state_machine_set(control->stateMachine, PARAM_MACHINE_SERVO_OK, (int)false);
        }

#if CONTROL_DEGUB
        serial_debug("TIME2: %d\n", _getms() - testms);
        testms = _getms();
#endif
        // DYN$
        if (servoCheckCount == 0)
        {
            // ReadMotorPosition32(DYN1_ADDR);
            servoCheckCount++;
        }
        else
        {
            if (1 && !GetMotorPosition32())
            {
                servoCheckCount++;
            }
            else
            {
                state_machine_set(control->stateMachine, PARAM_MACHINE_SERVO_COM, (int)true);
                servoCheckCount = 0;
            }
            if (servoCheckCount > 1000)
            {
                servoCheckCount = 0;
                state_machine_set(control->stateMachine, PARAM_MACHINE_SERVO_COM, (int)false);
            }
        }
        // RTC

#if CONTROL_DEGUB
        serial_debug("TIME3: %d\n", _getms() - testms);
        testms = _getms();
#endif

        /*Update Motion State parameters*/
        // Check conditions for motion
        if (false) // LEGNTH
        {
        }
        else if (false) // FORCE
        {
        }
        else if (forcemN >= 0 && forcemN > machinePerformance.maxForceTensile * 1000) // Tension
        {
            state_machine_set(control->stateMachine, PARAM_MOTION_CONDITION, MOTION_TENSION);
        }
        else if (forcemN < 0 && forcemN < -machinePerformance.maxForceCompression * 1000) // Compression
        {
            state_machine_set(control->stateMachine, PARAM_MOTION_CONDITION, MOTION_COMPRESSION);
        }
        else if (mcp_get_pin(&mcp, DISTANCE_LIMIT_MAX, DISTANCE_LIMIT_MAX_REGISTER) == 1) // UPPER
        {
            // Error machine out of bounds (Upper Limit)
            // Update state machine
            state_machine_set(control->stateMachine, PARAM_MOTION_CONDITION, MOTION_UPPER);
        }
        else if (mcp_get_pin(&mcp, DISTANCE_LIMIT_MIN, DISTANCE_LIMIT_MIN_REGISTER) == 1) // LOWER
        {
            // Error machine out of bounds
            // Update state machine
            state_machine_set(control->stateMachine, PARAM_MOTION_CONDITION, MOTION_LOWER);
        }
        else if (mcp_get_pin(&mcp, DOOR_SWITCH, DOOR_SWITCH_REGISTER) == 1) // Door
        {
            // Error machine door open
            // Update state machine
            state_machine_set(control->stateMachine, PARAM_MOTION_CONDITION, MOTION_DOOR);
        }
        else if (abs(lastEncoderRead - control->monitorData->encoderRaw) < 2) // STOPPED
        {
            state_machine_set(control->stateMachine, PARAM_MOTION_CONDITION, MOTION_STOPPED);
        }
        else // MOVING
        {
            state_machine_set(control->stateMachine, PARAM_MOTION_CONDITION, MOTION_MOVING);
        }

#if CONTROL_DEGUB
        serial_debug("TIME4: %d\n", _getms() - testms);
        testms = _getms();
#endif

        if (currentMachineState.state == STATE_MOTION) // Motion Enabled
        {
            if (lastState.state != STATE_MOTION || initial)
            {
                // dyn4_send_command(&dyn4, dyn4_)
            }
            if (currentMachineState.motionParameters.status == STATUS_DISABLED)
            {
                move_servo(control, MOVE_STOP, 0);
            }
            else
            {
                if (lastState.motionParameters.status != STATUS_ENABLED || initial) // Motion enabled initial
                {
                    // dyn4_send_command(&dyn4, dyn4_go_rel_pos, -1000);
                    // _waitms(300);
                    // dyn4_send_command(&dyn4, dyn4_sin_wave, 1000);
                }
                if (currentMachineState.motionParameters.mode == MODE_MANUAL)
                {
                    if (lastState.motionParameters.mode != MODE_MANUAL || initial) //  sync navkey and encoder position
                    {
                        // navkey_reset(navkey);
                        // navkey_update_status(navkey);
                    }
                    /* Set functions based on navkey */
                    navkey_update_status(&navkey); // Update navkey status registers
                    if (navkey.status.CTRR > 0)    // Center button released
                    {
                        // serial_debug("CTRR:%d\n", navkey.status.CTRR);

                        if (currentMachineState.motionParameters.condition == MOTION_STOPPED)
                        {
                            // serial_debug("stopped\n");
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
                            // serial_debug("Function: %d\n", control->stateMachine->function);
                        }
                        else if (control->stateMachine->motionParameters.condition == MOTION_MOVING)
                        {
                            control->stateMachine->function = FUNC_MANUAL_OFF;
                        }
                    }

                    // Execute manual mode functions
                    switch (currentMachineState.function)
                    {
                    case FUNC_MANUAL_OFF: // Stop current motion
                        move_servo(control, MOVE_STOP, 0);
                        break;
                    case FUNC_MANUAL_INCREMENTAL_JOG: // Setup the navkey for incremental jog (turn off hold)
                        if (lastState.function != FUNC_MANUAL_INCREMENTAL_JOG || initial)
                        {
                            move_servo(control, MOVE_STOP, 0);
                            control->stateMachine->functionData = 1000; // Default step size in um = 1mm
                        }
                        if (navkey.status.LTR > 0) // Left released
                        {
                            if (control->stateMachine->functionData < 10000) // No step size above 10mm
                                control->stateMachine->functionData /= 10;   // Increase step size by multiple of 10
                        }
                        if (navkey.status.RTR > 0) // Right released
                        {
                            if (control->stateMachine->functionData > 10) // No step size below 0.01mm
                            {
                                control->stateMachine->functionData *= 10; // Decrease step size by multiple of 10
                            }
                        }
                        if (navkey.status.UPR > 0) // Up released
                        {
                            serial_debug("up released\n");
                            move_servo(control, MOVE_RELATIVE, control->stateMachine->functionData);
                        }
                        if (navkey.status.DNR > 0) // Down released
                        {
                            serial_debug("down released\n");
                            move_servo(control, MOVE_RELATIVE, -1 * control->stateMachine->functionData);
                        }
                        break;
                    case FUNC_MANUAL_CONTINUOUS_JOG: // Setup the navkey for continuous jog (turn on hold)
                        if (lastState.function != FUNC_MANUAL_CONTINUOUS_JOG || initial)
                        {
                            move_servo(control, MOVE_STOP, 0);
                            control->stateMachine->functionData = 10000; // 10000um/s = 10mm/s
                        }
                        if (navkey.status.LTR > 0) // Left released
                        {
                            control->stateMachine->functionData -= 1000; // Increase step size by 1mm
                        }
                        if (navkey.status.RTR > 0) // Right released
                        {
                            control->stateMachine->functionData += 1000; // Decrease step size by 1mm
                        }
                        if (navkey.status.UPP > 0) // Up pressed
                        {
                            move_servo(control, MOVE_SPEED, control->stateMachine->functionData);
                        }
                        if (navkey.status.DNP > 0) // Down pressed
                        {
                            move_servo(control, MOVE_SPEED, -1 * control->stateMachine->functionData);
                        }
                        if (navkey.status.UPR > 0) // Up released
                        {
                            move_servo(control, MOVE_STOP, 0);
                        }
                        if (navkey.status.DNR > 0) // Down released
                        {
                            move_servo(control, MOVE_STOP, 0);
                        }
                        break;
                    case FUNC_MANUAL_POSITIONAL_MOVE:
                        if (lastState.function != FUNC_MANUAL_POSITIONAL_MOVE || initial)
                        {
                            move_servo(control, MOVE_STOP, 0);
                            control->stateMachine->functionData = 1000 * round(steps_to_mm(control->monitorData->encoderRaw, &(control->machineProfile->configuration)) / 1000.0); // Default position in um rounded to nearest mm
                        }
                        if (navkey.status.LTR > 0) // Left released
                        {
                            control->stateMachine->functionData -= 1000; // Increase position by 1mm
                        }
                        if (navkey.status.RTR > 0) // Right released
                        {
                            control->stateMachine->functionData += 1000; // Decrease position by 1mm
                        }
                        if (navkey.status.UPR > 0) // Up released
                        {
                            move_servo(control, MOVE_ABSOLUTE, control->stateMachine->functionData);
                        }
                        if (navkey.status.DNR > 0) // Down released
                        {
                            move_servo(control, MOVE_ABSOLUTE, control->stateMachine->functionData);
                        }
                        break;
                    case FUNC_MANUAL_HOME:
                        if (lastState.function != FUNC_MANUAL_HOME || initial)
                        {
                            move_servo(control, MOVE_STOP, 0);
                            control->stateMachine->functionData = HOMING_NONE; // Set to false, will be set to true when home is complete
                        }

                        if (mcp_get_pin(&mcp, DISTANCE_LIMIT_MIN, DISTANCE_LIMIT_MIN_REGISTER) == 1 && control->stateMachine->functionData == HOMING_SEEKING) // Wait for limit switch trigger
                        {
                            move_servo(control, MOVE_STOP, 0);
                            _waitms(100);
                            move_servo(control, MOVE_SPEED, -1500); // Turn CWW at homing speeds/10

                            control->stateMachine->functionData = HOMING_BACKING_OFF; // Set to 2, will be set to 1 when home is complete
                                                                                      // dyn4_send_command(&dyn4, dyn4_set_origin, 0);
                        }
                        else if (mcp_get_pin(&mcp, DISTANCE_LIMIT_MIN, DISTANCE_LIMIT_MIN_REGISTER) == 0 && control->stateMachine->functionData == HOMING_BACKING_OFF)
                        {
                            move_servo(control, MOVE_STOP, 0);
                            _waitms(100);
                            move_servo(control, MOVE_SPEED, 1500); // Turn CCW at homing speeds/10
                            control->stateMachine->functionData = HOMING_SEEKING_SLOW;
                        }
                        else if (mcp_get_pin(&mcp, DISTANCE_LIMIT_MIN, DISTANCE_LIMIT_MIN_REGISTER) == 1 && control->stateMachine->functionData == HOMING_SEEKING_SLOW)
                        {
                            move_servo(control, MOVE_STOP, 0);
                            _waitms(1000);
                            // dyn4_send_command(&dyn4, dyn4_set_origin, 0x00); // Set dyn4 origin
                            //  move_servo(control, MOVE_RELATIVE, 5000); // Move 5mm to clear the limit switch
                            control->stateMachine->functionData = HOMING_COMPLETE;
                        }
                        if (navkey.status.UPR > 0) // Up released
                        {
                            control->stateMachine->functionData = HOMING_SEEKING; // Set to false, will be set to true when home is complete
                            move_servo(control, MOVE_SPEED, 10000);               // Turn CW at homing speeds
                        }
                        if (navkey.status.DNR > 0) // Down released
                        {
                            control->stateMachine->functionData = HOMING_SEEKING; // Set to false, will be set to true when home is complete
                            move_servo(control, MOVE_SPEED, 10000);               // Turn CW at homing speeds
                        }
                        break;
                    case FUNC_MANUAL_MOVE_GAUGE_LENGTH:
                        move_servo(control, MOVE_STOP, 0);
                        break;
                    case FUNC_MANUAL_MOVE_FORCE:
                        if (lastState.function != FUNC_MANUAL_MOVE_FORCE || initial)
                        {
                            control->stateMachine->functionData = 0; // Set force to zero
                        }
                        if (navkey.status.LTR > 0) // Left released
                        {
                            control->stateMachine->functionData += 100; // Increase position by 10
                        }
                        if (navkey.status.RTR > 0) // Right released
                        {
                            if (control->stateMachine->functionData > 100)
                            {
                                control->stateMachine->functionData -= 100; // Decrease position by 10
                            }
                        }
                        if (forcemN * 1000 < control->stateMachine->functionData)
                        {
                            if (navkey.status.UPP > 0) // Up pressed
                            {
                                move_servo(control, MOVE_SPEED, 100); // turn CW
                            }
                            if (navkey.status.DNP > 0) // Down pressed
                            {
                                move_servo(control, MOVE_SPEED, 100); // turn ccw
                            }
                        }
                        else
                        {
                            move_servo(control, MOVE_STOP, 0); // stop motion
                        }
                        if (navkey.status.UPR > 0) // Up released
                        {
                            move_servo(control, MOVE_STOP, 0); // stop motion
                        }
                        if (navkey.status.DNR > 0) // Down released
                        {
                            move_servo(control, MOVE_STOP, 0); // stop motion
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
                        serial_debug("running test\n");
                        run_motion_profile_init(&run); // Create new RunMotionProfile structure
                        startTime = _getus();
                        startPosition = control->monitorData->position;
                        // serial_debug("start time: %d\n", startTime);
                        //  json_print_motion_profile(&(control->motionProfile));
                        json_print_motion_profile(&(control->motionProfile));
                        monitorWriteData = true;
                    }
                    // Run the loaded test profile

                    if (!run.profileComplete)
                    {
                        /*if (_getus() - startTime > 15000000)
                        {
                            run.profileComplete = true;
                        }*/
                        double t = (_getus() - startTime) / 1000000.0;
                        double position = position_profile(t, &run, &(control->motionProfile));
                        // serial_debug("%f,%f,%f\n", t, position, control->monitorData->position);
                        move_servo(control, MOVE_ABSOLUTE, startPosition + (int)(position * 1000));
                    }
                    else
                    {
                        serial_debug("test complete\n");
                        monitorWriteData = false;
                        state_machine_set(control->stateMachine, PARAM_MOTION_MODE, MODE_TEST);
                    }
                }
            }
        }
        else
        {
            move_servo(control, MOVE_STOP, 0);
        }
        lastEncoderRead = control->monitorData->encoderRaw;
        lastState = currentMachineState;
        lastData = data;
        initial = false;
    }
}

bool control_begin(ControlSystem *control, MachineProfile *machineProfile, MachineState *stateMachine, MonitorData *monitorData)
{
    control->machineProfile = machineProfile;
    control->monitorData = monitorData;
    control->stateMachine = stateMachine;
    control->cogid = _cogstart_C(control_cog, control, &control_stack[0], sizeof(long) * CONTROL_MEMORY_SIZE);
    if (control->cogid != -1)
    {
        return true;
    }
    return false;
}
