#ifndef AutomaticPage_H
#define AutomaticPage_H
#include "simpletools.h"
#include "Ra8876_Lite.h"
#include "Error.h"
#include "Style.h"
#include "State.h"
#include "IOBoard.h"
#include "DYN4.h"
#include "Motion.h"

class AutomaticPage
{
public:
    void run(Ra8876_Lite *p_display, struct mailbox_t *motionMailbox, MachineState *machineState);
    void checkButtons(Button *buttons);
    static void motionCOG(void *par);
    void loadGraph(Ra8876_Lite *display);
    void drawGraph(Ra8876_Lite *display);

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