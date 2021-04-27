#ifndef StatusPage_H
#define StatusPage_H
#include "simpletools.h"
#include "Ra8876_Lite.h"
#include "Error.h"
#include "Style.h"
#include "State.h"

/**
 * @brief Runs the UI that displays machine information and status
 * 
 */
class StatusPage
{
public:
    void run(Ra8876_Lite *the_display);

private:
    void checkButtons(Button *buttons);
    bool complete;
    Ra8876_Lite *display;
    enum button_names
    {
        BUTTON_MACHINE_ENABLE,
        BUTTON_MACHINE_DISABLE,
        BUTTON_NAVIGATION
    };
};

#endif