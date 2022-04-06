#ifndef NavigationPage_H
#define NavigationPage_H
#include "simpletools.h"
#include "RA8876.h"
#include "Error.h"
#include "Style.h"
#include "StateMachine.h"
#include "Images.h"
#include "Button.h"

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
    PAGE_CALIBRATION
} Pages;

typedef struct NavigationPage_s
{
    bool complete;
    Display *display;
    Button *buttons;
    Pages newPage;
    Images *images;
} NavigationPage;

NavigationPage *navigation_page_create(Display *display, Images *images);

void navigation_page_destroy(NavigationPage *page);

Pages navigation_page_run(NavigationPage *page);

#endif
