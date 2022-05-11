#ifndef MotionStateWindow_h
#define MotionStateWindow_h
#include "simpletools.h"
#include "RA8876.h"
#include "StateMachine.h"
#include "Module.h"

typedef struct MotionStateWindow_t
{
    MachineState *state;
} MotionStateWindow;

void motion_state_window_create(Module *container, MachineState *state);
void motion_state_window_destroy(MotionStateWindow *window);
void motion_state_window_update(MotionStateWindow *window);

#endif