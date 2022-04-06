#include "Button.h"

Button *button_create(int xmin, int xmax, int ymin, int ymax, int debounceTimems, int name)
{
    Button *button = (Button *)malloc(sizeof(Button));
    button->lastPress = 0;
    button->pressed = false;
    button->xmin = xmin;
    button->xmax = xmax;
    button->ymin = ymin;
    button->ymax = ymax;
    button->name = name;
    return button;
}

void button_initialize(Button *button, int xmin, int xmax, int ymin, int ymax, int debounceTimems, int name)
{
    button->lastPress = 0;
    button->pressed = false;
    button->xmin = xmin;
    button->xmax = xmax;
    button->ymin = ymin;
    button->ymax = ymax;
    button->name = name;
}

void button_initialize_from_module(Button *button, Module *module, int debounceTimems, int name)
{
    button_initialize(button, module->x, module->x + module->w, module->y, module->y + module->h, debounceTimems, name);
}

void button_update(Display *display)
{
    display_update_touch(display);
}

int button_check(Display *display, Button *buttons, const int amount)
{
    int amountPressed = 0;

    for (int i = 0; i < amount; i++)
    {
        buttons[i].pressed = false;
        if (((SCREEN_WIDTH - display->location[0].x) > buttons[i].xmin) && ((SCREEN_WIDTH - display->location[0].x) < buttons[i].xmax))
        {
            if (((SCREEN_HEIGHT - display->location[0].y) > buttons[i].ymin) && ((SCREEN_HEIGHT - display->location[0].y) < buttons[i].ymax))
            {
                if (display->locationCount > 0)
                {
                    buttons[i].pressed = true;
                    amountPressed++;
                }
            }
        }
    }
    return amountPressed;
}