#ifndef FunctionWindow_h
#define FunctionWindow_h
#include "simpletools.h"
#include "RA8876.h"
#include "StateMachine.h"

typedef struct FunctionWindow_t
{
    Display *display;
    Button *buttons;
    int x, y;
    MachineState *state;
    MachineState lastState;
} FunctionWindow;

FunctionWindow *function_window_create(Display *display, MachineState *state, int x, int y);
void function_window_destroy(FunctionWindow *window);
void function_window_update(FunctionWindow *window);

#endif