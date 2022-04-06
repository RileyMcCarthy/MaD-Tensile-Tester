#ifndef StatusPage_H
#define StatusPage_H
#include "simpletools.h"
#include "RA8876.h"
#include "Error.h"
#include "Style.h"
#include "StateMachine.h"
#include "Images.h"
#include "Monitor.h"
#include "Graph.h"
#include "MotionStateWindow.h"
#include "FunctionWindow.h"
#include "Button.h"

typedef struct StatusPage_s
{
    bool complete;
    Display *display;
    MachineState *stateMachine;
    MonitorData *data;
    Button *buttons;
    Images *images;
} StatusPage;

StatusPage *status_page_create(Display *display, MachineState *machineState, MonitorData *data, Images *images);

void status_page_destroy(StatusPage *statusPage);

/**
 * @brief Runs the UI that displays machine information and status
 *
 */
void status_page_run(StatusPage *statusPage);

#endif
