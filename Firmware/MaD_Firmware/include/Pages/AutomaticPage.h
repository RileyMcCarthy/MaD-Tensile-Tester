#ifndef AutomaticPage_H
#define AutomaticPage_H

#include "RA8876.h"
#include "Error.h"
#include "Style.h"
#include "StateMachine.h"
#include "IOBoard.h"
#include "Monitor.h"
#include "stdbool.h"
#include "string.h"
#include "stdio.h"
#include "Images.h"
#include "ControlSystem.h"
#include "StateMachine.h"
#include "Module.h"
#include "MachineView.h"
/**
 * @brief Runs the UI for running tests
 *
 */
typedef struct AutomaticPage_t
{
    Display *display;
    Images *images;
    MachineState *machineState;
    ControlSystem *control;
    bool complete;

    Module root;
    Module background;
    Module runProfile;
    Module runTitle;
    Module selectedProfileNameText;
    Module selectedProfileNumberText;
    Module selectedProfileNumberText;
    Module openButton;
    Module openText;
    Module runButton;
    Module runText;
    Module saveButton;
    Module navigationButton;

    Module machineInfoWindow;
    MachineView machineView;

    char profileNameBuffer[50];
    char profileNumberBuffer[50];
    MonitorData *data;
} AutomaticPage;

void automatic_page_init(AutomaticPage *page, Display *display, Images *images, MachineState *state, ControlSystem *control, MonitorData *data);
void automatic_page_destroy(AutomaticPage *page);
void automatic_page_run(AutomaticPage *page);

#endif
