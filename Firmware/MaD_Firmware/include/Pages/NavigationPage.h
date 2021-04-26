#ifndef NavigationPage_H
#define NavigationPage_H
#include "simpletools.h"
#include "Ra8876_Lite.h"
#include "Error.h"
#include "Style.h"
#include "State.h"

/**
 * @brief Enumeration representing the pages available to select from navigation
 * 
 */

enum Pages
{
    PAGE_STATUS,
    PAGE_MANUAL,
    PAGE_AUTOMATIC,
    PAGE_SETTINGS
};

/**
 * @brief Runs the UI that lets the user select which page to run
 * 
 */
class NavigationPage
{
public:
    Pages run(Ra8876_Lite *the_display);
    void checkButtons(Button *buttons);

private:
    bool complete;
    Ra8876_Lite *display;
    Pages newPage;
};

#endif