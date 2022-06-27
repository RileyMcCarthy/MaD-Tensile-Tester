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

typedef struct CalibratePage_s
{
    bool complete;
    Display *display;
    Monitor *monitor;
    Button *buttons;
    MachineProfile *machineProfile;
    Images *images;
    int state;

} CalibratePage;

void calibrate_force_page_init(CalibratePage *page, Display *display, Monitor *monitor, MachineProfile *machineProfile, Images *images);
void calibrate_force_page_destroy(CalibratePage *page);
bool calibrate_force_page_run(CalibratePage *page);

#endif
