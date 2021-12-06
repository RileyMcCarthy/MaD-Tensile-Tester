#ifndef CalibrateForcePage_H
#define CalibrateForcePage_H
#include "simpletools.h"
#include "RA8876.h"
#include "Error.h"
#include "Style.h"
#include "StateMachine.h"

typedef struct CalibrateForcePage_s
{
    bool complete;
    Display *display;
    MachineState *stateMachine;
    Button *buttons;
    int state;
} CalibrateForcePage;

CalibrateForcePage *calibrate_force_page_create(Display *display, MachineState *machineState);
void calibrate_force_page_destroy(CalibrateForcePage *page);
void calibrate_force_page_run(CalibrateForcePage *page)

#endif