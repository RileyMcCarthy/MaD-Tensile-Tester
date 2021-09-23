#ifndef StatusPage_H
#define StatusPage_H
#include "simpletools.h"
#include "RA8876.h"
#include "Error.h"
#include "Style.h"
#include "State.h"

typedef struct StatusPage_s
{
    bool complete;
    Display *display;
} StatusPage;

StatusPage *status_page_create(Display *display);

void status_page_destroy(StatusPage *statusPage);

/**
 * @brief Runs the UI that displays machine information and status
 * 
 */
StatusPage *status_page_run(StatusPage *statusPage);

#endif