#ifndef CalibrateForcePage_H
#define CalibrateForcePage_H
#include "simpletools.h"
#include "RA8876.h"
#include "Error.h"
#include "Style.h"
#include "StateMachine.h"
#include "ForceGauge.h"
#include "Keyboard.h"
#include "JSON.h"
#include "Monitor.h"
#include "Button.h"

typedef struct CalibrateForcePage_s
{
    bool complete;
    Display *display;
    Monitor *monitor;
    ForceGauge *forceGauge;
    Button *buttons;
    MachineProfile *machineProfile;
    Images *images;
    int state;
} CalibrateForcePage;

CalibrateForcePage *calibrate_force_page_create(Display *display, Monitor *monitor, ForceGauge *forceGauge, Images *images);
void calibrate_force_page_destroy(CalibrateForcePage *page);
bool calibrate_force_page_run(CalibrateForcePage *page);

#endif
