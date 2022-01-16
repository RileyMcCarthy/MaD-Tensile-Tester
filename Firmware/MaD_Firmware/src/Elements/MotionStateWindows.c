#include "MotionStateWindow.h"
#include "style.h"
#define BUTTONCOUNT 3

#define BUTTON_STATUS 0
#define BUTTON_CONDITION 1
#define BUTTON_MODE 2

static void check_buttons(MotionStateWindow *window)
{

    if (display_update_buttons(window->display, window->buttons, BUTTONCOUNT) > 0)
    {
        for (int i = 0; i < BUTTONCOUNT; i++)
        {
            if (window->buttons[i].pressed)
            {
                switch (window->buttons[i].name)
                {
                case BUTTON_STATUS:
                    switch (window->state->motionParameters.status)
                    {
                    case STATUS_DISABLED:
                        state_machine_set(window->state, PARAM_MOTION_STATUS, STATUS_ENABLED);
                        break;
                    case STATUS_ENABLED:
                        state_machine_set(window->state, PARAM_MOTION_STATUS, STATUS_DISABLED);
                        break;
                    }
                    break;
                case BUTTON_CONDITION:
                {
                }
                case BUTTON_MODE:
                {
                    switch (window->state->motionParameters.mode)
                    {
                    case MODE_MANUAL:
                        state_machine_set(window->state, PARAM_MOTION_MODE, MODE_TEST);
                        break;
                    case MODE_TEST:
                        state_machine_set(window->state, PARAM_MOTION_MODE, MODE_MANUAL);
                        break;
                    }
                }
                }
            }
        }
    }
}

MotionStateWindow *motion_state_window_create(Display *display, MachineState *state, int x, int y)
{
    MotionStateWindow *window = (MotionStateWindow *)malloc(sizeof(MotionStateWindow));
    window->display = display;
    window->state = state;
    window->x = x;
    window->y = y;
    Button *buttons = (Button *)malloc(sizeof(Button) * BUTTONCOUNT);
    window->buttons = buttons;
    buttons[0].name = BUTTON_STATUS;
    buttons[0].xmin = window->x + 5;
    buttons[0].xmax = buttons[0].xmin + 100;
    buttons[0].ymin = window->y + 35;
    buttons[0].ymax = buttons[0].ymin + 50;
    buttons[0].pressed = false;
    buttons[0].debounceTimems = 100;
    buttons[0].lastPress = 0;

    buttons[1].name = BUTTON_CONDITION;
    buttons[1].xmin = buttons[0].xmax + 5;
    buttons[1].xmax = buttons[1].xmin + 100;
    buttons[1].ymin = buttons[0].ymin;
    buttons[1].ymax = buttons[1].ymin + 50;
    buttons[1].pressed = false;
    buttons[1].debounceTimems = 100;
    buttons[1].lastPress = 0;

    buttons[2].name = BUTTON_MODE;
    buttons[2].xmin = buttons[1].xmax + 5;
    buttons[2].xmax = buttons[2].xmin + 100;
    buttons[2].ymin = buttons[0].ymin;
    buttons[2].ymax = buttons[2].ymin + 50;
    buttons[2].pressed = false;
    buttons[2].debounceTimems = 100;
    buttons[2].lastPress = 0;
    return window;
}
void motion_state_window_destroy(MotionStateWindow *window)
{
    free(window->buttons);
    free(window);
}
motion_state_window_update(MotionStateWindow *window)
{
    check_buttons(window);

    if (window->state->motionParameters.status == window->lastState.motionParameters.status &&
        window->state->motionParameters.condition == window->lastState.motionParameters.condition &&
        window->state->motionParameters.mode == window->lastState.motionParameters.mode &&
        window->state->state == window->lastState.state)
        return;
    display_draw_circle_square_fill(window->display, window->x, window->y, window->x + 320, window->y + 90, 20, 20, MAINCOLOR);

    display_text_color(window->display, MAINTEXTCOLOR, MAINCOLOR);
    display_draw_string(window->display, window->buttons[0].xmin + 20, window->y + 5, "Status");
    display_draw_string(window->display, window->buttons[1].xmin, window->y + 5, "Condition");
    display_draw_string(window->display, window->buttons[2].xmin + 30, window->y + 5, "Mode");
    /*Machine Toggle Button*/
    char buf[50];
    int statusOutlineColor = COLOR65K_BLACK;
    int statusInnerColor = COLOR65K_BLACK;
    int statusTextColor = COLOR65K_BLACK;
    int conditionOutlineColor = COLOR65K_BLACK;
    int conditionInnerColor = COLOR65K_BLACK;
    int conditionTextColor = COLOR65K_BLACK;
    int modeOutlineColor = COLOR65K_BLACK;
    int modeInnerColor = COLOR65K_BLACK;
    int modeTextColor = COLOR65K_BLACK;
    switch (window->state->motionParameters.status)
    {
    case STATUS_DISABLED:
    {
        strcpy(buf, "DISABLED");
        if (window->state->state == STATE_MOTION)
        {
            statusOutlineColor = COLOR65K_GREEN;
            statusInnerColor = COLOR65K_BLACK;
            statusTextColor = COLOR65K_WHITE;
        }
        else
        {
            statusOutlineColor = COLOR65K_BROWN;
            statusInnerColor = COLOR65K_BLACK;
            statusTextColor = COLOR65K_YELLOW;
        }
        break;
    }
    case STATUS_ENABLED:
    {
        strcpy(buf, "ENABLED");
        statusOutlineColor = COLOR65K_GREEN;
        statusInnerColor = COLOR65K_WHITE;
        statusTextColor = COLOR65K_BLACK;
        break;
    }
    case STATUS_SAMPLE_LIMIT:
    {
        strcpy(buf, "SMP LIM");
        statusOutlineColor = COLOR65K_BLUE;
        statusInnerColor = COLOR65K_YELLOW;
        statusTextColor = COLOR65K_BLACK;
        break;
    }
    case STATUS_MACHINE_LIMIT:
    {
        strcpy(buf, "MCH LIM");
        statusOutlineColor = COLOR65K_BLUE;
        statusInnerColor = COLOR65K_YELLOW;
        statusTextColor = COLOR65K_BLACK;
        break;
    }
    case STATUS_FAULTED:
    {
        strcpy(buf, "FAULTED");
        statusOutlineColor = COLOR65K_BLUE;
        statusInnerColor = COLOR65K_RED;
        statusTextColor = COLOR65K_WHITE;
        break;
    }
    default:
        break;
    }
    display_draw_circle_square_fill(window->display, window->buttons[0].xmin, window->buttons[0].ymin, window->buttons[0].xmax, window->buttons[0].ymax, 20, 20, statusOutlineColor);
    display_text_color(window->display, statusTextColor, statusInnerColor);
    display_draw_string(window->display, window->buttons[0].xmin + (window->buttons[0].xmax - window->buttons[0].xmin) / 2 - strlen(buf) * 6, window->buttons[0].ymin + (window->buttons[0].ymax - window->buttons[0].ymin) / 2 - 12, buf);

    switch (window->state->motionParameters.condition)
    {
    case MOTION_LENGTH:
    {
        strcpy(buf, "LENGTH ");
        conditionOutlineColor = COLOR65K_MAGENTA;
        conditionInnerColor = COLOR65K_MAGENTA;
        conditionTextColor = COLOR65K_BLACK;
        break;
    }
    case MOTION_FORCE:
    {
        strcpy(buf, " FORCE ");
        conditionOutlineColor = COLOR65K_MAGENTA;
        conditionInnerColor = COLOR65K_MAGENTA;
        conditionTextColor = COLOR65K_BLACK;
        break;
    }
    case MOTION_TENSION:
    {
        strcpy(buf, "TENSION");
        conditionOutlineColor = COLOR65K_YELLOW;
        conditionInnerColor = COLOR65K_YELLOW;
        conditionTextColor = COLOR65K_BLACK;
        break;
    }
    case MOTION_COMPRESSION:
    {
        strcpy(buf, "COMPRES");
        conditionOutlineColor = COLOR65K_YELLOW;
        conditionInnerColor = COLOR65K_YELLOW;
        conditionTextColor = COLOR65K_BLACK;
        break;
    }
    case MOTION_UPPER:
    {
        strcpy(buf, " UPPER ");
        conditionOutlineColor = COLOR65K_YELLOW;
        conditionInnerColor = COLOR65K_YELLOW;
        conditionTextColor = COLOR65K_BLACK;
        break;
    }
    case MOTION_LOWER:
    {
        strcpy(buf, " LOWER ");
        conditionOutlineColor = COLOR65K_YELLOW;
        conditionInnerColor = COLOR65K_YELLOW;
        conditionTextColor = COLOR65K_BLACK;
        break;
    }
    case MOTION_DOOR:
    {
        strcpy(buf, " DOOR  ");
        conditionOutlineColor = COLOR65K_YELLOW;
        conditionInnerColor = COLOR65K_YELLOW;
        conditionTextColor = COLOR65K_BLACK;
        break;
    }
    case MOTION_STOPPED:
    {
        strcpy(buf, "STOPPED");
        conditionOutlineColor = COLOR65K_RED;
        conditionInnerColor = COLOR65K_RED;
        conditionTextColor = COLOR65K_WHITE;
        break;
    }
    case MOTION_MOVING:
    {
        strcpy(buf, "MOVING ");
        conditionOutlineColor = COLOR65K_LIGHTMAGENTA;
        conditionInnerColor = COLOR65K_LIGHTMAGENTA;
        conditionTextColor = COLOR65K_BLACK;
        break;
    }
    default:
        break;
    }
    display_draw_circle_square_fill(window->display, window->buttons[1].xmin, window->buttons[1].ymin, window->buttons[1].xmax, window->buttons[1].ymax, 20, 20, conditionOutlineColor);
    display_text_color(window->display, conditionTextColor, conditionInnerColor);
    display_draw_string(window->display, window->buttons[1].xmin + (window->buttons[1].xmax - window->buttons[1].xmin) / 2 - strlen(buf) * 6, window->buttons[1].ymin + (window->buttons[1].ymax - window->buttons[1].ymin) / 2 - 12, buf);

    switch (window->state->motionParameters.mode)
    {
    case MODE_MANUAL:
    {
        strcpy(buf, "MANUAL");
        conditionOutlineColor = COLOR65K_MAGENTA;
        conditionInnerColor = COLOR65K_MAGENTA;
        conditionTextColor = COLOR65K_BLACK;
        break;
    }
    case MODE_TEST:
    {
        strcpy(buf, " TEST ");
        conditionOutlineColor = COLOR65K_MAGENTA;
        conditionInnerColor = COLOR65K_MAGENTA;
        conditionTextColor = COLOR65K_BLACK;
        break;
    }
    default:
        break;
    }
    display_draw_circle_square_fill(window->display, window->buttons[2].xmin, window->buttons[2].ymin, window->buttons[2].xmax, window->buttons[2].ymax, 20, 20, conditionOutlineColor);
    display_text_color(window->display, conditionTextColor, conditionInnerColor);
    display_draw_string(window->display, window->buttons[2].xmin + (window->buttons[2].xmax - window->buttons[2].xmin) / 2 - strlen(buf) * 6, window->buttons[2].ymin + (window->buttons[2].ymax - window->buttons[2].ymin) / 2 - 12, buf);
    window->lastState = *window->state;
}
