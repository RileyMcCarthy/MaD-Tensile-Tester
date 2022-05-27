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
#include "Images.h"
#include "Button.h"

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
    Images *images;
} ManualPage;

void manual_page_init(ManualPage *page, Display *display, MachineState *machineState, Images *images);
void manual_page_destroy(ManualPage *page);
void manual_page_run(ManualPage *page);

#endif
