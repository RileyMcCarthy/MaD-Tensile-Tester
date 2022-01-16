#ifndef MotionStateWindow_h
#define MotionStateWindow_h
#include "simpletools.h"
#include "RA8876.h"
#include "StateMachine.h"

typedef struct MotionStateWindow_t
{
    Display *display;
    Button *buttons;
    int x, y;
    MachineState *state;
    MachineState lastState;
} MotionStateWindow;

MotionStateWindow *motion_state_window_create(Display *display, MachineState *state, int x, int y);
void motion_state_window_destroy(MotionStateWindow *window);
void motion_state_window_update(MotionStateWindow *window);

#endif