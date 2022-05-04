#ifndef Keyboard_H
#define Keyboard_H

#include "simpletools.h"
#include "RA8876.h"
#include "Images.h"
#include "Button.h"
#include "Module.h"

typedef struct keyboard_s
{
    Display *display;
    Images *images;
    Button *keys;
    char *keyboard_buffer;
    bool keyboard_complete;
} Keyboard;

Keyboard *keyboard_create(Display *display, Images *images);
void keyboard_destroy(Keyboard *keyboard);

/**
 * @brief The keyboard class contains an ondisplay keyboard that can be used to gather user input
 *
 **/
char *keyboard_get_input(Keyboard *keyboardObj, const char *prompt);

#endif
