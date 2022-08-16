#ifndef FunctionWindow_h
#define FunctionWindow_h
#include "RA8876.h"
#include "StateMachine.h"
#include "Module.h"

typedef struct FunctionWindow_t
{
    MachineState *state;

    char functionHeaderBuffer[30];

    Module functionWindow;
    Module functionHeader;
    Module buttonArea;
    Module offButton;
    Module incrementalButton;
    Module continuousButton;
    Module positionalMoveButton;
    Module homeButton;
    Module gaugeForceButton;
    Module gaugeLengthButton;

} FunctionWindow;

void function_window_init(FunctionWindow *window, Module *container, MachineState *state);
void function_window_update(FunctionWindow *window);

#endif
