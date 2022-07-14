#ifndef MotionStateWindow_h
#define MotionStateWindow_h
#include "simpletools.h"
#include "RA8876.h"
#include "StateMachine.h"
#include "Module.h"

typedef struct MotionStateWindow_t
{
    MachineState *state;

    Module motionStateWindow;
    Module conditionHeader;
    Module statusHeader;
    Module modeHeader;
    Module statusButton;
    Module statusButtonText;
    Module conditionButton;
    Module conditionButtonText;
    Module modeButton;
    Module modeButtonText;

} MotionStateWindow;

void motion_state_window_init(MotionStateWindow *window, Module *container, MachineState *state);
void motion_state_window_update(MotionStateWindow *window);

#endif
