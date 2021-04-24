#ifndef StatusPage_H
#define StatusPage_H
#include "simpletools.h"
#include "Ra8876_Lite.h"
#include "Error.h"
#include "Style.h"
#include "State.h"

class StatusPage
{
public:
    void run(Ra8876_Lite *the_display, MachineState *machineState);
    void checkButtons(Button *buttons);

private:
    bool complete;
    Ra8876_Lite *display;
    MachineState *machineState;
    enum button_names
    {
        BUTTON_MACHINE_ENABLE,
        BUTTON_MACHINE_DISABLE,
        BUTTON_NAVIGATION
    };
};

#endif