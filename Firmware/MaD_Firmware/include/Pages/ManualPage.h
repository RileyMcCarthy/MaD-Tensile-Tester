#ifndef ManualPage_H
#define ManualPage_H
#include "simpletools.h"
#include "Ra8876_Lite.h"
#include "Error.h"
#include "Style.h"
#include "State.h"
#include "DYN4.h"
#include "Motion.h"
/**
 * @brief Runs the UI for manually controlling the machine
 * 
 */

typedef struct ManualPage_t
{
    bool complete;
    Display *display;
    Button *buttons;
} ManualPage;

ManualPage *ManualPage_Create(Display *display);
void ManualPage_Destroy(ManualPage *page);
void ManualPage_run(ManualPage *page);

#endif