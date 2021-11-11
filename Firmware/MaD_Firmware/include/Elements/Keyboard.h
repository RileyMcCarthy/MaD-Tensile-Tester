#ifndef Keyboard_H
#define Keyboard_H
#ifdef __MEMORY_CHECK__
#include "leak_detector_c.h"
#endif
#include "simpletools.h"
#include "RA8876.h"
#include "Images.h"

/**
 * @brief The keyboard class contains an ondisplay keyboard that can be used to gather user input
 *
 **/

char *keyboard_get_input(Display *display, const char *prompt);

#endif
