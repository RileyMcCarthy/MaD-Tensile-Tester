#ifndef CalibrateForcePage_H
#define CalibrateForcePage_H
#include "RA8876.h"
#include "Error.h"
#include "Style.h"
#include "StateMachine.h"
#include "ForceGauge.h"
#include "Keyboard.h"
#include "JSON.h"
#include "Monitor.h"

typedef struct CalibratePage_s
{
    bool complete;
    Display *display;
    Monitor *monitor;
    MachineProfile *machineProfile;
    Images *images;

    Module root;
    Module background;
    Module navigation;
    Module forceWindow;
    Module forceHeader;
    Module forceCalStatus;
    Module adcValue;
    Module zeroValue;
    Module slopeValue;

    Module machineSettingsHeader;
    Module msZeroValue;
    Module msSlopeValue;
    Module forceValue;

    Module actionButton;

    char forceCalStatusBuffer[15];
    char adcValueBuffer[15];
    char zeroValueBuffer[15];
    char slopeValueBuffer[15];
    char msZeroValueBuffer[15];
    char msSlopeValueBuffer[15];
    char forceValueBuffer[15];
    char actionButtonBuffer[15];

    int state;

} CalibratePage;

void calibrate_force_page_init(CalibratePage *page, Display *display, Monitor *monitor, MachineProfile *machineProfile, Images *images);
void calibrate_force_page_destroy(CalibratePage *page);
bool calibrate_force_page_run(CalibratePage *page);

#endif
