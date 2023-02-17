#include <stdbool.h>
typedef struct MotionCommand{
    int steps; // steps to move
    int feedrate; // steps per second
    bool last; // is this the last command in the queue?
} MotionCommand;

void motion_enable();
void motion_disable();
void motion_clear();
bool motion_add_move(int steps, int feedrate);
void motion_test_start();
void motion_test_end();
void motion_test_clear();
bool motion_test_is_empty();
bool motion_test_add_move(MotionCommand *command);
long motion_get_position();
long motion_get_setpoint();
