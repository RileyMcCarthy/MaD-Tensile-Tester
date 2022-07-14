#ifndef StatusPage_H
#define StatusPage_H
#include "simpletools.h"
#include "RA8876.h"
#include "Error.h"
#include "Style.h"
#include "StateMachine.h"
#include "Images.h"
#include "Monitor.h"
#include "MotionStateWindow.h"
#include "FunctionWindow.h"
#include "Graph.h"
#include "JSON.h"

typedef struct StatusPage_s
{
    bool complete;
    Display *display;
    MachineState *stateMachine;
    MachineProfile *machineProfile;
    MonitorData *data;
    Images *images;

    Module root;
    Module background;
    Module navigationButton;
    Module machineStateWindow;
    Module machineStateImages;
    Module machineStateWindowTitle;
    Module chargePumpText;
    Module chargePumpImage;
    Module switchedPowerText;
    Module switchedPowerImage;
    Module estSwitchText;
    Module estSwitchImage;
    Module esdUpperText;
    Module esdUpperImage;
    Module estLowerText;
    Module estLowerImage;
    Module servoReadyText;
    Module servoReadyImage;
    Module forceGaugeComText;
    Module forceGaugeComImage;
    Module servoComText;
    Module servoComImage;
    Module machineInfoWindow;
    Module machineInfoWindowTitle;
    Module forceText;
    Module forceValue;
    Module positionText;
    Module positionValue;
    Module positionGraphContainer;
    ModuleGraph positionGraph;
    Module forceGraphContainer;
    ModuleGraph forceGraph;
    Module functionWindow;
    FunctionWindow functionWindowData;
    Module motionStateWindow;
    MotionStateWindow motionStateWindowData;

    char forceValueBuffer[20];
    char positionValueBuffer[20];
} StatusPage;

void status_page_init(StatusPage *page, Display *display, MachineState *machineState, MachineProfile *machineProfile, MonitorData *data, Images *images)__fromfile("src/Pages/StatusPage.c");
/**
 * @brief Runs the UI that displays machine information and status
 *
 */
void status_page_run(StatusPage *statusPage)__fromfile("src/Pages/StatusPage.c");

#endif
