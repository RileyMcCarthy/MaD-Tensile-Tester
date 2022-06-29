#ifndef SELECTION_H
#define SELECTION_H
#include "RA8876.h"
#include "Module.h"
#include "Button.h"
#include "Style.h"

#define MAX_SELECTIONS 10

typedef struct selection_s
{
    Display *display;

    Module root;
    Module window;
    Module title;
    Module options[MAX_SELECTIONS];
    char optionsBuffer[MAX_SELECTIONS][20];
    int optionCount;
    int selected;
} Selection;

void selection_init(Selection *selection, Display *display);
void selection_add_option(Selection *selection, char *option);
int selection_run(Selection *selection);
#endif
