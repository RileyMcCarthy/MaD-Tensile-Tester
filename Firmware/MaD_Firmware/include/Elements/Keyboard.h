#ifndef Keyboard_H
#define Keyboard_H
#include "simpletools.h"
#include "RA8876.h"

/**
 * @brief The keyboard class contains an ondisplay keyboard that can be used to gather user input
 *
 **/

void keyboard_get_input(Display *display, char *input);

#endif