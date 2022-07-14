#ifndef NavigationPage_H
#define NavigationPage_H
#include "simpletools.h"
#include "RA8876.h"
#include "Error.h"
#include "Style.h"
#include "StateMachine.h"
#include "Images.h"
#include "Module.h"

/**
 * @brief Enumeration representing the pages available to select from navigation
 *
 */

typedef enum Pages_e
{
    PAGE_STATUS,
    PAGE_MANUAL,
    PAGE_AUTOMATIC,
    PAGE_SETTINGS,
    PAGE_CALIBRATION,
    PAGE_TEST_PROFILE
} Page;

typedef struct NavigationPage_s
{
    bool complete;
    Display *display;
    Images *images;
    Page newPage;

    Module root;
    Module background;
    Module statusPage;
    Module statusText;
    Module manualPage;
    Module manualText;
    Module testPage;
    Module testText;
    Module calibratePage;
    Module calibrateText;
    Module settingPage;
    Module settingText;
    Module tpPage;
    Module tpText;
} NavigationPage;

void navigation_page_init(NavigationPage *page, Display *display, Images *images);

void navigation_page_destroy(NavigationPage *page);

Page navigation_page_run(NavigationPage *page);

#endif
