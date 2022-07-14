#ifndef FunctionWindow_h
#define FunctionWindow_h
#include "simpletools.h"
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

void function_window_init(FunctionWindow *window, Module *container, MachineState *state)__fromfile("src/Elements/FunctionWindow.c");
void function_window_update(FunctionWindow *window)__fromfile("src/Elements/FunctionWindow.c");

#endif
