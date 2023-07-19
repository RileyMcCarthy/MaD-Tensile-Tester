#ifndef MOTION_H
#define MOTION_H
#include <stdbool.h>
#include <stdint.h>

typedef struct Move {
    uint8_t g; // Gcode command
    double x; // Position in mm
    double f; // Feedrate in mm/min
    int p; // ms to pause motion
} Move;

void motion_enable();
void motion_disable();
void motion_clear();
bool motion_add_move(Move *command);
void motion_test_start();
void motion_test_end();
void motion_test_clear();
bool motion_test_is_empty();
bool motion_test_add_move(Move *command);
long motion_get_position();
long motion_get_setpoint();
#endif
