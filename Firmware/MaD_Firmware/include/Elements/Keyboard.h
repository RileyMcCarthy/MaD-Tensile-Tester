#ifndef Keyboard_H
#define Keyboard_H

#include "simpletools.h"
#include "RA8876.h"
#include "Images.h"
#include "Module.h"

#define KEYBOARDBUTTONCOUNT 47

typedef struct keyboard_s
{
    Display *display;
    Images *images;
    char keyboard_buffer[50];
    char prompt[30];
    bool keyboard_complete;

    Module root;
    Module background;
    Module topbar;
    Module topbarPrompt;
    Module topbarValue;
    Module key[KEYBOARDBUTTONCOUNT];
    Module keyText[KEYBOARDBUTTONCOUNT];
} Keyboard;

Keyboard *keyboard_create(Display *display, Images *images);
void keyboard_destroy(Keyboard *keyboard);

/**
 * @brief The keyboard class contains an ondisplay keyboard that can be used to gather user input
 *
 **/
char *keyboard_get_input(Keyboard *keyboardObj, const char *prompt);

#endif
