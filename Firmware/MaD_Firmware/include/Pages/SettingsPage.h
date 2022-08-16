#ifndef SettingsPage_H
#define SettingsPage_H

#include "RA8876.h"
#include "Error.h"
#include "Style.h"
#include "Images.h"
#include "JSON.h"
#include "Keyboard.h"
#include <stdbool.h>

typedef struct SettingsPage_s
{
    bool complete;
    MachineProfile *machineProfile;
    Display *display;
    Images *images;

    Module root;
    Module background;
    Module navigationButton;
    Module window;
    Module windowTitle;
    Module name;
    Module number;

    Module configurationTitle;
    Module motor;
    Module maxTorque;
    Module maxRPM;
    Module gearDiameter;
    Module inertia;
    Module staticTorque;
    Module load;
    Module encoder;
    Module encoderStepsRev;
    Module forceGauge;
    Module forceStepMilliNewton;
    Module forceZero;

    Module performanceTitle;
    Module minPosition;
    Module maxPosition;
    Module maxVelocity;
    Module maxAcceleration;
    Module maxTensile;
    Module maxCompressive;
    Module forceNeutral;

} SettingsPage;

void settings_page_init(SettingsPage *page, Display *display, MachineProfile *machineProfile, Images *images);
bool settings_page_run(SettingsPage *page);

#endif
