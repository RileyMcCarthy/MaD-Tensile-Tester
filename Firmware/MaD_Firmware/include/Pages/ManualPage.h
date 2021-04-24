#ifndef ManualPage_H
#define ManualPage_H
#include "simpletools.h"
#include "Ra8876_Lite.h"
#include "Error.h"
#include "Style.h"
#include "State.h"
#include "DYN4.h"

class ManualPage
{
public:
    void run(Ra8876_Lite *the_display, DYN4 *dyn4, MachineState *machineState);
    void checkButtons(Button *buttons);

private:
    bool complete;
    Ra8876_Lite *display;
    enum button_names
    {
        BUTTON_MOTION_MODE,
        BUTTON_CONDITION,
        BUTTON_NAVIGATION
    };
};

#endif