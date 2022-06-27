#ifndef FunctionWindow_h
#define FunctionWindow_h
#include "simpletools.h"
#include "RA8876.h"
#include "StateMachine.h"
#include "Button.h"
#include "Module.h"

typedef struct FunctionWindow_t
{
    MachineState *state;

    char functionHeaderBuffer[30];

} FunctionWindow;

void function_window_create(Module *container, MachineState *state);
void function_window_destroy(FunctionWindow *window);
void function_window_update(FunctionWindow *window);

#endif