#include "Utility/Motion.h"
#include "Main/MaD.h"
#include <stdlib.h>
#include "StaticQueue.h"
#include <propeller.h>
#include "Utility/Debug.h"
#include "Memory/MachineProfile.h"
#include <smartpins.h>
#include <math.h>
#include "Utility/StateMachine.h"
#include "Memory/CogStatus.h"

#define G0_RAPID_MOVE 0
#define G1_LINEAR_MOVE 1
#define G2_ClOCKWISE_ARC 2
#define G3_COUNTERCLOCKWISE_ARC 3
#define G4_DWELL 4
#define G28_HOME 28
#define G90_ABSOLUTE 90
#define G91_RELATIVE 91
#define G122 122 // End of profile

#define MOTION_MEMORY_SIZE 10000
static long motion_stack[MOTION_MEMORY_SIZE];

static bool motion_enabled = false;
static int motion_setpoint_steps = 0;
static int motion_position_steps = 0;
static long motion_feedrate_steps_per_second = 0;

#define MAX_SIZE_MANUAL 100
#define MAX_SIZE_TEST 100

static Move manual_buffer[MAX_SIZE_MANUAL];
static Move test_buffer[MAX_SIZE_TEST];

static StaticQueue manual_queue;
static StaticQueue test_queue;

static bool test_mode = false;
static bool relative_mode = false;

void motion_enable()
{
  motion_enabled = true;
}

// End test before queue is cleared
void motion_disable()
{
  motion_enabled = false;
}

void motion_clear()
{
    queue_empty(&manual_queue);
}

// returns false if queue is full
bool  motion_add_move(Move *command)
{
    if (!motion_enabled)
    {
        return false;
    }

    DEBUG_INFO("Adding manual motion command with %f steps and %f feedrate\n", command->x, command->f);
    return queue_push(&manual_queue, command);
}

void motion_test_start()
{
    test_mode = true;
}

void motion_test_end()
{
    test_mode = false;
   // printf("Ending test mode\n");
}

void motion_test_clear()
{
    queue_empty(&test_queue);
}

bool motion_test_is_empty()
{
    return queue_isempty(&test_queue) && motion_position_steps == motion_setpoint_steps;
}

bool motion_test_is_full()
{
    return queue_isfull(&test_queue);
}

bool motion_test_add_move(Move *command)
{
    //("Adding test command with %d steps and %d feedrate\n", command->steps, command->feedrate);
    return queue_push(&test_queue, (void*)command);
}

long motion_get_position()
{
    return motion_position_steps;
}

void motion_set_position(int position)
{
    motion_position_steps = position;
    motion_setpoint_steps = position;
}

long motion_get_setpoint()
{
    return motion_setpoint_steps;
}
#define TEST_QUEUE_EMPTY_PERIOD 1000
static void motion_cog(void *arg)
{
    MachineProfile machine_profile;
    while (!machine_profile_loaded())
    ;
    MachineProfile *profile_ptr = NULL;
    while (!lock_machine_profile_ms(&profile_ptr,1000))
    {
      DEBUG_ERROR("%s","Failed to lock machine profile for motion\n");
    }
    memcpy(&machine_profile, profile_ptr, sizeof(MachineProfile));
    unlock_machine_profile();

    motion_setpoint_steps = 0;
    motion_position_steps = 0;
    test_mode = false;
    queue_init(&manual_queue, manual_buffer, MAX_SIZE_MANUAL, sizeof(Move));
    queue_init(&test_queue, test_buffer, MAX_SIZE_TEST, sizeof(Move));
    motion_enabled = true;
    int queue_last_time = 0;

    // Get machine motion state ptr for read only
    MachineState *machine_state_ptr = get_machine_state_ptr();
    State * state_ptr = &(machine_state_ptr->state);
    MotionMode * motion_mode = &(machine_state_ptr->motionParameters.mode);
    while(true)
    {
        
        Move command;
        StaticQueue *queue = &manual_queue;
        bool test_running = *motion_mode == MODE_TEST_RUNNING;
        set_motion_status(_getms());
        if (test_running)
        {
            queue = &test_queue;
        }

        if (queue_isempty(queue))
        {
            // if the queue is empty, we should wait for a new command
            if (test_running)
            {
                int currentms = _getms();
                if (currentms - queue_last_time > TEST_QUEUE_EMPTY_PERIOD)
                {
                    DEBUG_WARNING("%s","Test queue empty for too long, ending test\n");
                    state_machine_set(PARAM_MOTION_MODE, MODE_TEST); // End of profile
                }
            }
            continue;
        }

        queue_last_time = _getms();

        // Should have a mode for pausing all motion, but for now just disable motion
        // motion enable should not be here as the queue should be cleared once disabled
        // pause should remember only stop new step and directions from executing
        if (!motion_enabled && false)
        {
            continue;
        }

        if (!queue_pop(queue, &command))
            continue;
        DEBUG_INFO("Running command: %d\n", command.g);

        if (motion_enabled)
        {
            switch(command.g)
            {
                case G0_RAPID_MOVE || G1_LINEAR_MOVE:
                {
                    if (abs(command.f) < 0.001)
                    {
                        DEBUG_WARNING("G0/G1 Command has zero feedrate: %d\n",command.f);
                        continue;
                    }
                    int delta_steps = 0;
                    int steps = (int)(command.x * machine_profile.configuration.servoStepPermm);
                    int feedrate = (int)(command.f * machine_profile.configuration.servoStepPermm);
                    if (relative_mode)
                    {
                        delta_steps = steps;
                        motion_setpoint_steps += delta_steps;
                        printf("Relative mode: %d\n", motion_setpoint_steps);
                    }
                    else
                    {
                        motion_setpoint_steps = steps;
                        delta_steps = motion_setpoint_steps - motion_position_steps;
                        printf("Absolute mode: %d\n", motion_setpoint_steps);
                    }

                    bool direction = delta_steps > 0;
                    if (direction)
                    {
                        _pinl(PIN_SERVO_DIR);
                    }
                    else
                    {
                        _pinh(PIN_SERVO_DIR);
                    }

                    delta_steps = abs(delta_steps);

                    if (feedrate <= 0)
                    {
                        DEBUG_WARNING("G0/G1 Command has zero or negative feedrate: %d\n", feedrate);
                        continue;
                    }

                    if (delta_steps == 0)
                    {
                        //DEBUG_WARNING("%s","G0/G1 Command has zero steps\n");
                        continue;
                    }

                    DEBUG_INFO("G0/G1 Command with %d steps and %d feedrate\n", delta_steps, feedrate);
                    
                    // need to use floating point math as 60*clockfreq is greater then max integer
                    // SHOULD CHECK IF GREATER THEN MAX INTEGER (VERY SLOW SPEEDS)
                    uint32_t clkticks_per_step = (1.0/(double)feedrate)*_clockfreq();
                    if (clkticks_per_step > 65535)
                    {
                        DEBUG_INFO("Slow step pulses: %d\n", clkticks_per_step);
                        _pinclear(PIN_SERVO_PUL);
                        // Pulses too slow for hardware
                        for (int i = 0;i<delta_steps;i++)
                        {
                            set_motion_status(_getms());
                            _pinl(PIN_SERVO_PUL);
                            _waitx(clkticks_per_step >> 1);
                            _pinh(PIN_SERVO_PUL);
                            _waitx(clkticks_per_step >> 1);
                            if (direction)
                                motion_position_steps++;
                            else
                                motion_position_steps--;
                            if (*state_ptr != STATE_MOTION)
                            {
                                motion_setpoint_steps = motion_position_steps;
                                DEBUG_NOTIFY("%s","Motion mode changed, stopping motion\n");
                                break;
                            }
                        }
                    }
                    else
                    {
                        //continue;
                        // Fast enough to use hardware
                        uint32_t pulseTiming = ((clkticks_per_step >> 1) << 16) | clkticks_per_step; // Frequency of Pulse in ms
                        _pinstart(PIN_SERVO_PUL, P_PULSE | P_OE, pulseTiming, 0);
                        _wypin( PIN_SERVO_PUL, delta_steps);
                        if (direction)
                            motion_position_steps += delta_steps;
                        else
                            motion_position_steps -= delta_steps;
                        while(_pinr(PIN_SERVO_PUL) == 0);
                        {
                            set_motion_status(_getms());
                            if (*state_ptr != STATE_MOTION)
                            {
                                _pinclear(PIN_SERVO_PUL);
                                MonitorData temp;
                                if (get_monitor_data(&temp, 1000))
                                {
                                    int current_pos_steps = temp.encoderum/machine_profile.configuration.servoStepPermm;
                                    motion_setpoint_steps = current_pos_steps;
                                    motion_position_steps = current_pos_steps;
                                }
                                else
                                {
                                    DEBUG_ERROR("%s","Failed to get monitor data to sync motion\n");
                                }
                                
                                DEBUG_NOTIFY("%s","Motion mode changed, stopping motion\n");
                                break;
                            }
                        }
                    }
                    if (motion_position_steps != motion_setpoint_steps)
                    {
                        DEBUG_ERROR("Position != setpoint : %d != %d\n",motion_position_steps,motion_setpoint_steps);
                    }
                    break;
                }
                case G2_ClOCKWISE_ARC:
                {
                    break;
                }
                case G3_COUNTERCLOCKWISE_ARC:
                {
                    break;
                }
                case G4_DWELL:
                {
                    _waitms(command.p);
                    break;
                }
                case G90_ABSOLUTE:
                {
                    relative_mode = false;
                    break;
                }
                case G91_RELATIVE:
                {
                    relative_mode = true;
                    break;
                }
                case G122:
                {
                    state_machine_set(PARAM_MOTION_MODE, MODE_TEST); // End of profile
                    DEBUG_NOTIFY("%s","Test Complete!\n");
                    break;
                }
            }
        }else
        {
            DEBUG_ERROR("%s","Motion disabled!!!\n");
            motion_setpoint_steps = motion_position_steps;
            queue_empty(&manual_queue);
            queue_empty(&test_queue);
            break;
        }
    }
}

bool motion_begin()
{
    int id = _cogstart_C(motion_cog, NULL, &motion_stack[0], sizeof(long) * MOTION_MEMORY_SIZE);
    if (id != -1)
    {
        return true;
    }
    return false;
}

