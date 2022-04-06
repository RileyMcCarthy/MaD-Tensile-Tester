#ifndef AutomaticPage_H
#define AutomaticPage_H
#ifdef __MEMORY_CHECK__
#include "leak_detector_c.h"
#endif
#include <simpletools.h>
#include "RA8876.h"
#include "Error.h"
#include "Style.h"
#include "StateMachine.h"
#include "IOBoard.h"
#include "DYN4.h"
#include "Monitor.h"
#include "stdbool.h"
#include "string.h"
#include "stdio.h"
#include "Images.h"
#include "Button.h"

/**
 * @brief Runs the UI for running tests
 *
 */
typedef struct AutomaticPage_t
{
    bool complete;
    Display *display;
    Images *images;
} AutomaticPage;

AutomaticPage *automatic_page_create(Display *display, Images *images);
void automatic_page_destroy(AutomaticPage *page);
void automatic_page_run(AutomaticPage *page);

#endif
