#include "FunctionWindow.h"

#include "style.h"
#define BUTTONCOUNT 7

static void check_buttons(FunctionWindow *window)
{
    if (display_update_buttons(window->display, window->buttons, BUTTONCOUNT) > 0)
    {
        for (int i = 0; i < BUTTONCOUNT; i++)
        {
            if (window->buttons[i].pressed)
            {
                state_machine_set(window->state, PARAM_FUNCTION, window->buttons[i].name);
            }
        }
    }
}

FunctionWindow *function_window_create(Display *display, MachineState *state, int x, int y)
{
    FunctionWindow *window = (FunctionWindow *)malloc(sizeof(FunctionWindow));
    window->display = display;
    window->state = state;
    window->x = x;
    window->y = y;
    Button *buttons = (Button *)malloc(sizeof(Button) * BUTTONCOUNT);
    window->buttons = buttons;

    buttons[0].name = FUNC_MANUAL_OFF;
    buttons[0].xmin = window->x + 5;
    buttons[0].xmax = buttons[0].xmin + 100;
    buttons[0].ymin = window->y + 35;
    buttons[0].ymax = buttons[0].ymin + 50;
    buttons[0].pressed = false;
    buttons[0].debounceTimems = 100;
    buttons[0].lastPress = 0;

    buttons[1].name = FUNC_MANUAL_INCREMENTAL_JOG;
    buttons[1].xmin = buttons[0].xmax + 5;
    buttons[1].xmax = buttons[1].xmin + 100;
    buttons[1].ymin = buttons[0].ymin;
    buttons[1].ymax = buttons[1].ymin + 50;
    buttons[1].pressed = false;
    buttons[1].debounceTimems = 100;
    buttons[1].lastPress = 0;

    buttons[2].name = FUNC_MANUAL_CONTINUOUS_JOG;
    buttons[2].xmin = buttons[1].xmax + 5;
    buttons[2].xmax = buttons[2].xmin + 100;
    buttons[2].ymin = buttons[0].ymin;
    buttons[2].ymax = buttons[2].ymin + 50;
    buttons[2].pressed = false;
    buttons[2].debounceTimems = 100;
    buttons[2].lastPress = 0;

    buttons[3].name = FUNC_MANUAL_POSITIONAL_MOVE;
    buttons[3].xmin = buttons[0].xmin;
    buttons[3].xmax = buttons[3].xmin + 100;
    buttons[3].ymin = buttons[0].ymax + 5;
    buttons[3].ymax = buttons[3].ymin + 50;
    buttons[3].pressed = false;
    buttons[3].debounceTimems = 100;
    buttons[3].lastPress = 0;

    buttons[4].name = FUNC_MANUAL_HOME;
    buttons[4].xmin = buttons[3].xmax + 5;
    buttons[4].xmax = buttons[4].xmin + 100;
    buttons[4].ymin = buttons[3].ymin;
    buttons[4].ymax = buttons[4].ymin + 50;
    buttons[4].pressed = false;
    buttons[4].debounceTimems = 100;
    buttons[4].lastPress = 0;
    buttons[5].name = FUNC_MANUAL_MOVE_GAUGE_LENGTH;
    buttons[5].xmin = buttons[4].xmax + 5;
    buttons[5].xmax = buttons[5].xmin + 100;
    buttons[5].ymin = buttons[4].ymin;
    buttons[5].ymax = buttons[5].ymin + 50;
    buttons[5].pressed = false;
    buttons[5].debounceTimems = 100;
    buttons[5].lastPress = 0;

    buttons[6].name = FUNC_MANUAL_MOVE_FORCE;
    buttons[6].xmin = buttons[0].xmin;
    buttons[6].xmax = buttons[6].xmin + 100;
    buttons[6].ymin = buttons[5].ymax + 5;
    buttons[6].ymax = buttons[6].ymin + 50;
    buttons[6].pressed = false;
    buttons[6].debounceTimems = 100;
    buttons[6].lastPress = 0;
    return window;
}
void function_window_destroy(FunctionWindow *window)
{
    free(window->buttons);
    free(window);
}
void function_window_update(FunctionWindow *window)
{
    check_buttons(window);

    if (window->state->motionParameters.status == window->lastState.motionParameters.status &&
        window->state->motionParameters.condition == window->lastState.motionParameters.condition &&
        window->state->motionParameters.mode == window->lastState.motionParameters.mode &&
        window->state->state == window->lastState.state &&
        window->state->function == window->lastState.function &&
        window->state->functionData == window->lastState.functionData)
        return;

    display_draw_circle_square_fill(window->display, window->x, window->y, window->x + 320, window->y + 200, 20, 20, MAINCOLOR);

    display_text_color(window->display, MAINTEXTCOLOR, MAINCOLOR);

    char buf[50];
    switch (window->state->function)
    {
    case FUNC_MANUAL_OFF:
        sprintf(buf, "OFF");
        break;
    case FUNC_MANUAL_INCREMENTAL_JOG:
        sprintf(buf, "INCREMENTAL JOG: %dmm", window->state->functionData);
        break;
    case FUNC_MANUAL_CONTINUOUS_JOG:
        sprintf(buf, "CONTINUOUS JOG: %dmm", window->state->functionData);
        break;
    case FUNC_MANUAL_POSITIONAL_MOVE:
        sprintf(buf, "POSITIONAL MOVE: %dmm", window->state->functionData);
        break;
    case FUNC_MANUAL_HOME:
        if (window->state->functionData == (int)false && window->state->motionParameters.condition == MOTION_STOPPED)
            sprintf(buf, "HOME", window->state->functionData);
        else if (window->state->functionData == (int)false && window->state->motionParameters.condition == MOTION_MOVING)
            sprintf(buf, "HOMING", window->state->functionData);
        else if (window->state->functionData == (int)true)
            sprintf(buf, "HOMING COMPLETE", window->state->functionData);
        break;
    case FUNC_MANUAL_MOVE_GAUGE_LENGTH:
        sprintf(buf, "MOVE GAUGE LENGTH: %dmm", window->state->functionData);
        break;
    case FUNC_MANUAL_MOVE_FORCE:
        sprintf(buf, "MOVE GAUGE FORCE: %dmN", window->state->functionData);
        break;
    default:
        sprintf(buf, "OFF");
        break;
    }
    display_draw_string(window->display, window->x + 10, window->y + 10, buf);

    /*Machine Toggle Button*/
    int outlineColor = COLOR65K_BLACK;
    int innerColor = COLOR65K_BLACK;
    int textColor = COLOR65K_BLACK;

    strcpy(buf, "OFF");
    if (window->state->state == STATE_MOTION && window->state->motionParameters.status == STATUS_ENABLED)
    {
        if (window->state->function == FUNC_MANUAL_OFF)
        {
            outlineColor = COLOR65K_BLUE;
            innerColor = COLOR65K_BLACK;
            textColor = COLOR65K_WHITE;
        }
        else
        {
            outlineColor = COLOR65K_GREEN;
            innerColor = COLOR65K_BLACK;
            textColor = COLOR65K_WHITE;
        }
    }
    else
    {
        outlineColor = COLOR65K_ORANGE;
        innerColor = COLOR65K_BLACK;
        textColor = COLOR65K_WHITE;
    }

    display_draw_circle_square_fill(window->display, window->buttons[0].xmin, window->buttons[0].ymin, window->buttons[0].xmax, window->buttons[0].ymax, 20, 20, outlineColor);
    display_text_color(window->display, textColor, innerColor);
    display_draw_string(window->display, window->buttons[0].xmin + (window->buttons[0].xmax - window->buttons[0].xmin) / 2 - strlen(buf) * 6, window->buttons[0].ymin + (window->buttons[0].ymax - window->buttons[0].ymin) / 2 - 12, buf);

    strcpy(buf, "INCR");
    if (window->state->state == STATE_MOTION && window->state->motionParameters.condition == MOTION_STOPPED && window->state->motionParameters.status == STATUS_ENABLED)
    {
        if (window->state->function == FUNC_MANUAL_INCREMENTAL_JOG)
        {
            outlineColor = COLOR65K_BLUE;
            innerColor = COLOR65K_BLACK;
            textColor = COLOR65K_WHITE;
        }
        else
        {
            outlineColor = COLOR65K_GREEN;
            innerColor = COLOR65K_BLACK;
            textColor = COLOR65K_WHITE;
        }
    }
    else
    {
        outlineColor = COLOR65K_ORANGE;
        innerColor = COLOR65K_BLACK;
        textColor = COLOR65K_WHITE;
    }
    display_draw_circle_square_fill(window->display, window->buttons[1].xmin, window->buttons[1].ymin, window->buttons[1].xmax, window->buttons[1].ymax, 20, 20, outlineColor);
    display_text_color(window->display, textColor, innerColor);
    display_draw_string(window->display, window->buttons[1].xmin + (window->buttons[1].xmax - window->buttons[1].xmin) / 2 - strlen(buf) * 6, window->buttons[1].ymin + (window->buttons[1].ymax - window->buttons[1].ymin) / 2 - 12, buf);

    strcpy(buf, "CONT");
    if (window->state->state == STATE_MOTION && window->state->motionParameters.condition == MOTION_STOPPED && window->state->motionParameters.status == STATUS_ENABLED)
    {
        if (window->state->function == FUNC_MANUAL_CONTINUOUS_JOG)
        {
            outlineColor = COLOR65K_BLUE;
            innerColor = COLOR65K_BLACK;
            textColor = COLOR65K_WHITE;
        }
        else
        {
            outlineColor = COLOR65K_GREEN;
            innerColor = COLOR65K_BLACK;
            textColor = COLOR65K_WHITE;
        }
    }
    else
    {
        outlineColor = COLOR65K_ORANGE;
        innerColor = COLOR65K_BLACK;
        textColor = COLOR65K_WHITE;
    }
    display_draw_circle_square_fill(window->display, window->buttons[2].xmin, window->buttons[2].ymin, window->buttons[2].xmax, window->buttons[2].ymax, 20, 20, outlineColor);
    display_text_color(window->display, textColor, innerColor);
    display_draw_string(window->display, window->buttons[2].xmin + (window->buttons[2].xmax - window->buttons[2].xmin) / 2 - strlen(buf) * 6, window->buttons[2].ymin + (window->buttons[2].ymax - window->buttons[2].ymin) / 2 - 12, buf);

    strcpy(buf, "POS");
    if (window->state->state == STATE_MOTION && window->state->motionParameters.condition == MOTION_STOPPED && window->state->motionParameters.status == STATUS_ENABLED)
    {
        if (window->state->function == FUNC_MANUAL_POSITIONAL_MOVE)
        {
            outlineColor = COLOR65K_BLUE;
            innerColor = COLOR65K_BLACK;
            textColor = COLOR65K_WHITE;
        }
        else
        {
            outlineColor = COLOR65K_GREEN;
            innerColor = COLOR65K_BLACK;
            textColor = COLOR65K_WHITE;
        }
    }
    else
    {
        outlineColor = COLOR65K_ORANGE;
        innerColor = COLOR65K_BLACK;
        textColor = COLOR65K_WHITE;
    }
    display_draw_circle_square_fill(window->display, window->buttons[3].xmin, window->buttons[3].ymin, window->buttons[3].xmax, window->buttons[3].ymax, 20, 20, outlineColor);
    display_text_color(window->display, textColor, innerColor);
    display_draw_string(window->display, window->buttons[3].xmin + (window->buttons[3].xmax - window->buttons[3].xmin) / 2 - strlen(buf) * 6, window->buttons[3].ymin + (window->buttons[3].ymax - window->buttons[3].ymin) / 2 - 12, buf);

    strcpy(buf, "HOME");
    if (window->state->state == STATE_MOTION && window->state->motionParameters.condition == MOTION_STOPPED && window->state->motionParameters.status == STATUS_ENABLED)
    {
        if (window->state->function == FUNC_MANUAL_HOME)
        {
            outlineColor = COLOR65K_BLUE;
            innerColor = COLOR65K_BLACK;
            textColor = COLOR65K_WHITE;
        }
        else
        {
            outlineColor = COLOR65K_GREEN;
            innerColor = COLOR65K_BLACK;
            textColor = COLOR65K_WHITE;
        }
    }
    else
    {
        outlineColor = COLOR65K_ORANGE;
        innerColor = COLOR65K_BLACK;
        textColor = COLOR65K_WHITE;
    }
    display_draw_circle_square_fill(window->display, window->buttons[4].xmin, window->buttons[4].ymin, window->buttons[4].xmax, window->buttons[4].ymax, 20, 20, outlineColor);
    display_text_color(window->display, textColor, innerColor);
    display_draw_string(window->display, window->buttons[4].xmin + (window->buttons[4].xmax - window->buttons[4].xmin) / 2 - strlen(buf) * 6, window->buttons[4].ymin + (window->buttons[4].ymax - window->buttons[4].ymin) / 2 - 12, buf);

    strcpy(buf, "GA-L");
    if (window->state->state == STATE_MOTION && window->state->motionParameters.condition == MOTION_STOPPED && window->state->motionParameters.status == STATUS_ENABLED)
    {
        if (window->state->function == FUNC_MANUAL_MOVE_GAUGE_LENGTH)
        {
            outlineColor = COLOR65K_BLUE;
            innerColor = COLOR65K_BLACK;
            textColor = COLOR65K_WHITE;
        }
        else
        {
            outlineColor = COLOR65K_GREEN;
            innerColor = COLOR65K_BLACK;
            textColor = COLOR65K_WHITE;
        }
    }
    else
    {
        outlineColor = COLOR65K_ORANGE;
        innerColor = COLOR65K_BLACK;
        textColor = COLOR65K_WHITE;
    }
    display_draw_circle_square_fill(window->display, window->buttons[5].xmin, window->buttons[5].ymin, window->buttons[5].xmax, window->buttons[5].ymax, 20, 20, outlineColor);
    display_text_color(window->display, textColor, innerColor);
    display_draw_string(window->display, window->buttons[5].xmin + (window->buttons[5].xmax - window->buttons[5].xmin) / 2 - strlen(buf) * 6, window->buttons[5].ymin + (window->buttons[5].ymax - window->buttons[5].ymin) / 2 - 12, buf);

    strcpy(buf, "GA-F");
    if (window->state->state == STATE_MOTION && window->state->motionParameters.condition == MOTION_STOPPED && window->state->motionParameters.status == STATUS_ENABLED)
    {
        if (window->state->function == FUNC_MANUAL_MOVE_FORCE)
        {
            outlineColor = COLOR65K_BLUE;
            innerColor = COLOR65K_BLACK;
            textColor = COLOR65K_WHITE;
        }
        else
        {
            outlineColor = COLOR65K_GREEN;
            innerColor = COLOR65K_BLACK;
            textColor = COLOR65K_WHITE;
        }
    }
    else
    {
        outlineColor = COLOR65K_ORANGE;
        innerColor = COLOR65K_BLACK;
        textColor = COLOR65K_WHITE;
    }

    display_draw_circle_square_fill(window->display, window->buttons[6].xmin, window->buttons[6].ymin, window->buttons[6].xmax, window->buttons[6].ymax, 20, 20, outlineColor);
    display_text_color(window->display, textColor, innerColor);
    display_draw_string(window->display, window->buttons[6].xmin + (window->buttons[6].xmax - window->buttons[6].xmin) / 2 - strlen(buf) * 6, window->buttons[6].ymin + (window->buttons[6].ymax - window->buttons[6].ymin) / 2 - 12, buf);

    window->lastState = *window->state;
}
