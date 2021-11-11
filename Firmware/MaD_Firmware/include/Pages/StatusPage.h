#ifndef StatusPage_H
#define StatusPage_H
#include "simpletools.h"
#include "RA8876.h"
#include "Error.h"
#include "Style.h"
#include "StateMachine.h"
#ifdef __MEMORY_CHECK__
#include "leak_detector_c.h"
#endif
typedef struct StatusPage_s
{
    bool complete;
    Display *display;
    MachineState *stateMachine;
    Button *buttons;
} StatusPage;

StatusPage *status_page_create(Display *display, MachineState *machineState);

void status_page_destroy(StatusPage *statusPage);

/**
 * @brief Runs the UI that displays machine information and status
 * 
 */
StatusPage *status_page_run(StatusPage *statusPage);

#endif
