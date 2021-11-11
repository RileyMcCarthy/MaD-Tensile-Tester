#ifndef ManualPage_H
#define ManualPage_H
#include "simpletools.h"
#include "RA8876.h"
#include "Error.h"
#include "Style.h"
#include "StateMachine.h"
#include "DYN4.h"
#include "Monitor.h"
#include "i2cNavKey.h"
#ifdef __MEMORY_CHECK__
#include "leak_detector_c.h"
#endif
/**
 * @brief Runs the UI for manually controlling the machine
 *
 */
typedef struct ManualPage_t
{
    bool complete;
    Display *display;
    Button *buttons;
    MachineState *machineState;
} ManualPage;

ManualPage *manual_page_create(Display *display, MachineState *machineState);
void manual_page_destroy(ManualPage *page);
void manual_page_run(ManualPage *page);

#endif
