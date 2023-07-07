#include "Utility/Motion.h"
#include "Main/MaD.h"
#include <stdlib.h>
#include "StaticQueue.h"
#include <propeller.h>
#include "Utility/Debug.h"
#include "Memory/MachineProfile.h"

#define MOTION_MEMORY_SIZE 30000
static long motion_stack[MOTION_MEMORY_SIZE];

static bool motion_enabled = false;
static long motion_setpoint_steps = 0;
long motion_position_steps = 0;
static long motion_feedrate_steps_per_second = 0;

#define MAX_SIZE_MANUAL 100
#define MAX_SIZE_TEST 10

static Move manual_buffer[MAX_SIZE_MANUAL];
static Move test_buffer[MAX_SIZE_TEST];

static StaticQueue manual_queue;
static StaticQueue test_queue;

static bool test_mode = false;

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
bool motion_add_move(Move *command)
{
    if (!motion_enabled)
    {
        return false;
    }

    DEBUG_INFO("Adding manual motion command with %d steps and %d feedrate\n", command->x, command->f);
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

long motion_get_setpoint()
{
    return motion_setpoint_steps;
}

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
    
    int steps_per_mm = 10;//machine_profile.configuration.NEEDTOMAKEVARFOROUTPUTSTEPS;

    motion_setpoint_steps = 0;
    test_mode = false;
    queue_init(&manual_queue, manual_buffer, MAX_SIZE_MANUAL, sizeof(Move));
    queue_init(&test_queue, test_buffer, MAX_SIZE_TEST, sizeof(Move));
    motion_enabled = true;
    while(true)
    {
        Move command;
        StaticQueue *queue = &manual_queue;
        if (test_mode)
        {
            queue = &test_queue;
        }

        if (queue_isempty(queue))
        {
            // if the queue is empty, we should wait for a new command
            continue;
        }

        // Should have a mode for pausing all motion, but for now just disable motion
        // motion enable should not be here as the queue should be cleared once disabled
        // pause should remember only stop new step and directions from executing
        if (!motion_enabled && false)
        {
            continue;
        }

        if (!queue_pop(queue, &command))
            continue;

        //printf("Running motion command with %d steps and %d feedrate\n", command.steps, command.feedrate);
        int delayus = 1000000 / (command.f * steps_per_mm);
        int steps = command.x * steps_per_mm;
        if (command.g == 0)
         {
             motion_setpoint_steps += steps;
         }
         else if (command.g == 1)
         {
             motion_setpoint_steps -= steps;
         }
         else
         {
             DEBUG_ERROR("%s","Invalid motion command g value!!!\n");
         }

        while (motion_position_steps != motion_setpoint_steps) //while we haven't reached the setpoint
        {
            if (motion_enabled)
            {
                if (command.g == 0)
                {
                    _pinl(PIN_SERVO_DIR);
                    motion_position_steps++;
                }
                else if (command.g == 1)
                {
                    _pinh(PIN_SERVO_DIR);
                    motion_position_steps--;
                }
                _pinl(PIN_SERVO_PUL);
                _waitus(delayus/2);
                _pinh(PIN_SERVO_PUL);
                _waitus(delayus/2);
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

