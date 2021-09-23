#ifndef AutomaticPage_H
#define AutomaticPage_H
#include "simpletools.h"
#include "RA8876.h"
#include "Error.h"
#include "Style.h"
#include "State.h"
#include "IOBoard.h"
#include "DYN4.h"
#include "Motion.h"
#include "stdbool.h"
#include "string.h"
#include "stdio.h"
/**
 * @brief Runs the UI for running tests
 * 
 */
typedef struct AutomaticPage_t
{
    bool complete;
    Display *display;
} AutomaticPage;

AutomaticPage *automatic_page_create(Display *display);
void automatic_page_destroy(AutomaticPage *page);
void automatic_page_run(AutomaticPage *page);

#endif