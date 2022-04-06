#ifndef BUTTON_H
#define BUTTON_H
#include "RA8876.h"
#include "Module.h"
typedef struct Button_s
{
    int lastPress;
    bool pressed;
    uint16_t xmin, xmax;
    uint16_t ymin, ymax;
    int name; // use enumeration
} Button;

Button *button_create(int xmin, int xmax, int ymin, int ymax, int debounceTimems, int name);
void button_initialize(Button *button, int xmin, int xmax, int ymin, int ymax, int debounceTimems, int name);
void button_initialize_from_module(Button *button, Module *module, int debounceTimems, int name);
void button_update(Display *display);
int button_check(Display *display, Button *buttons, const int amount);
#endif
