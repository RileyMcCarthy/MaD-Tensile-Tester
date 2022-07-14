#ifndef SettingsPage_H
#define SettingsPage_H

#include "simpletools.h"
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
} SettingsPage;

void settings_page_init(SettingsPage *page, Display *display, MachineProfile *machineProfile, Images *images);
void settings_page_destroy(SettingsPage *page);
bool settings_page_run(SettingsPage *page);

#endif
