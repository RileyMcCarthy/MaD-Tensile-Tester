#include "Motion.h"
#include "MaD.h"
#include <stdlib.h>
#include <propeller2.h>
#include "StaticQueue.h"

#define MOTION_MEMORY_SIZE 30000
static long motion_stack[MOTION_MEMORY_SIZE];

static bool motion_enabled = false;
static long motion_setpoint_steps = 0;
long motion_position_steps = 0;
static long motion_feedrate_steps_per_second = 0;

#define MAX_SIZE_MANUAL 100
#define MAX_SIZE_TEST 10

static MotionCommand manual_buffer[MAX_SIZE_MANUAL];
static MotionCommand test_buffer[MAX_SIZE_TEST];

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
bool motion_add_move(int steps, int feedrate)
{
    if (!motion_enabled)
    {
        return false;
    }
    MotionCommand command;
    command.steps = steps;
    command.feedrate = feedrate;
    printf("Adding motion command with %d steps and %d feedrate\n", command.steps, command.feedrate);
    return queue_push(&manual_queue, &command);
}

void motion_test_start()
{
    test_mode = true;
}

void motion_test_end()
{
    test_mode = false;
    printf("Ending test mode\n");
}

void motion_test_clear()
{
    queue_empty(&test_queue);
}

bool motion_test_is_empty()
{
    return queue_isempty(&test_queue);
}

bool motion_test_is_full()
{
    return queue_isfull(&test_queue);
}

bool motion_test_add_move(MotionCommand *command)
{
    printf("Adding test command with %d steps and %d feedrate\n", command->steps, command->feedrate);
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
    test_mode = false;
    queue_init(&manual_queue, manual_buffer, MAX_SIZE_MANUAL, sizeof(MotionCommand));
    queue_init(&test_queue, test_buffer, MAX_SIZE_TEST, sizeof(MotionCommand));
    motion_enabled = true;
    while(true)
    {
        MotionCommand command;
        StaticQueue *queue = &manual_queue;
        if (test_mode)
        {
            queue = &test_queue;
        }
        if (queue_isempty(queue))
        {
            // if the queue is empty, we should wait for a new command
            if (test_mode)
                printf("Waiting for new motion command, ERROR if this is printed in test mode\n");
            continue;
        }
        if (!queue_pop(queue, &command))
            continue;

        printf("Running motion command with %d steps and %d feedrate\n", command.steps, command.feedrate);
        int delayus = 1000000 / command.feedrate;
        motion_setpoint_steps += command.steps;
        while (motion_position_steps != motion_setpoint_steps) //while we haven't reached the setpoint
        {
            if (motion_enabled)
            {
                if (motion_setpoint_steps > motion_position_steps)
                {
                    _pinl(PIN_SERVO_DIR);
                    motion_position_steps++;
                }
                else if (motion_setpoint_steps < motion_position_steps)
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
                motion_setpoint_steps = motion_position_steps;
                queue_empty(&manual_queue);
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

