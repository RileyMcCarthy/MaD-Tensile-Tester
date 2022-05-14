#include "FunctionWindow.h"
#include "Control.h"
#include "style.h"
#define BUTTONCOUNT 7

/*static void check_buttons(FunctionWindow *window)
{
    button_update(window->display);
    if (button_check(window->display, window->buttons, BUTTONCOUNT) > 0)
    {
        for (int i = 0; i < BUTTONCOUNT; i++)
        {
            if (window->buttons[i].pressed)
            {
                state_machine_set(window->state, PARAM_FUNCTION, window->buttons[i].name);
            }
        }
    }
}*/

static void button_set_function(int id, void *arg)
{
    FunctionWindow *window = (FunctionWindow *)arg;
    state_machine_set(window->state, PARAM_FUNCTION, id);
}

static void update_header(Display *display, Module *module, void *arg)
{
    FunctionWindow *window = (FunctionWindow *)arg;

    // Update header string
    char buf[50];
    switch (window->state->function)
    {
    case FUNC_MANUAL_OFF:
        sprintf(buf, "OFF");
        break;
    case FUNC_MANUAL_INCREMENTAL_JOG:
        sprintf(buf, "INCR JOG: %0.1fmm", (float)window->state->functionData / 1000.0);
        break;
    case FUNC_MANUAL_CONTINUOUS_JOG:
        sprintf(buf, "CONT JOG: %dmm", window->state->functionData / 1000);
        break;
    case FUNC_MANUAL_POSITIONAL_MOVE:
        sprintf(buf, "POSN MOVE: %dmm", window->state->functionData / 1000);
        break;
    case FUNC_MANUAL_HOME:
        if (window->state->functionData == HOMING_NONE)
            sprintf(buf, "HOME");
        else if (window->state->functionData == HOMING_SEEKING)
            sprintf(buf, "HOMING");
        else if (window->state->functionData == HOMING_SEEKING_SLOW)
            sprintf(buf, "HOMING SEEKING SLOW");
        else if (window->state->functionData == HOMING_BACKING_OFF)
            sprintf(buf, "HOMING BACKING OFF");
        else if (window->state->functionData == HOMING_COMPLETE)
            sprintf(buf, "HOMING COMPLETE");
        break;
    case FUNC_MANUAL_MOVE_GAUGE_LENGTH:
        sprintf(buf, "GAUGE LENGTH: %dmm", window->state->functionData / 1000);
        break;
    case FUNC_MANUAL_MOVE_FORCE:
        sprintf(buf, "GAUGE FORCE: %dmN", window->state->functionData);
        break;
    default:
        sprintf(buf, "OFF");
        break;
    }
    bool update = false;
    if (strcmp(buf, module->data) != 0)
    {
        module_set_text(module, buf);
        module_draw(display, module);
    }
}

static void update_off(Display *display, Module *module, void *arg)
{
    FunctionWindow *window = (FunctionWindow *)arg;

    int outlineColor = COLOR65K_BLACK;
    int innerColor = COLOR65K_BLACK;
    int textColor = COLOR65K_BLACK;
    printf("%d\n", window->state->function);
    if (window->state->state == STATE_MOTION && window->state->motionParameters.status != STATUS_DISABLED)
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

    if (module->foregroundColor == outlineColor &&
        module->child[0]->backgroundColor == innerColor &&
        module->child[0]->foregroundColor == textColor)
    {
        return;
    }
    module_set_color(module, outlineColor, outlineColor);
    module_set_color(module->child[0], textColor, innerColor);
    module_draw(display, module);
}

static void update_incremental(Display *display, Module *module, void *arg)
{
    FunctionWindow *window = (FunctionWindow *)arg;

    int outlineColor = COLOR65K_BLACK;
    int innerColor = COLOR65K_BLACK;
    int textColor = COLOR65K_BLACK;

    if (window->state->state == STATE_MOTION && window->state->motionParameters.condition != MOTION_MOVING && window->state->motionParameters.status != STATUS_DISABLED)
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

    if (module->foregroundColor == outlineColor &&
        module->child[0]->backgroundColor == innerColor &&
        module->child[0]->foregroundColor == textColor)
    {
        return;
    }
    module_set_color(module, outlineColor, outlineColor);
    module_set_color(module->child[0], textColor, innerColor);
    module_draw(display, module);
}

static void update_continuous(Display *display, Module *module, void *arg)
{
    FunctionWindow *window = (FunctionWindow *)arg;

    int outlineColor = COLOR65K_BLACK;
    int innerColor = COLOR65K_BLACK;
    int textColor = COLOR65K_BLACK;

    if (window->state->state == STATE_MOTION && window->state->motionParameters.condition != MOTION_MOVING && window->state->motionParameters.status != STATUS_DISABLED)
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

    if (module->foregroundColor == outlineColor &&
        module->child[0]->backgroundColor == innerColor &&
        module->child[0]->foregroundColor == textColor)
    {
        return;
    }
    module_set_color(module, outlineColor, outlineColor);
    module_set_color(module->child[0], textColor, innerColor);
    module_draw(display, module);
}

static void update_positional(Display *display, Module *module, void *arg)
{
    FunctionWindow *window = (FunctionWindow *)arg;

    int outlineColor = COLOR65K_BLACK;
    int innerColor = COLOR65K_BLACK;
    int textColor = COLOR65K_BLACK;

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

    if (module->foregroundColor == outlineColor &&
        module->child[0]->backgroundColor == innerColor &&
        module->child[0]->foregroundColor == textColor)
    {
        return;
    }
    module_set_color(module, outlineColor, outlineColor);
    module_set_color(module->child[0], textColor, innerColor);
    module_draw(display, module);
}

static void update_home(Display *display, Module *module, void *arg)
{
    FunctionWindow *window = (FunctionWindow *)arg;

    int outlineColor = COLOR65K_BLACK;
    int innerColor = COLOR65K_BLACK;
    int textColor = COLOR65K_BLACK;

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

    if (module->foregroundColor == outlineColor &&
        module->child[0]->backgroundColor == innerColor &&
        module->child[0]->foregroundColor == textColor)
    {
        return;
    }
    module_set_color(module, outlineColor, outlineColor);
    module_set_color(module->child[0], textColor, innerColor);
    module_draw(display, module);
}

static void update_length(Display *display, Module *module, void *arg)
{
    FunctionWindow *window = (FunctionWindow *)arg;

    int outlineColor = COLOR65K_BLACK;
    int innerColor = COLOR65K_BLACK;
    int textColor = COLOR65K_BLACK;

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
    if (module->foregroundColor == outlineColor &&
        module->child[0]->backgroundColor == innerColor &&
        module->child[0]->foregroundColor == textColor)
    {
        return;
    }
    module_set_color(module, outlineColor, outlineColor);
    module_set_color(module->child[0], textColor, innerColor);
    module_draw(display, module);
}

static void update_force(Display *display, Module *module, void *arg)
{
    FunctionWindow *window = (FunctionWindow *)arg;

    int outlineColor = COLOR65K_BLACK;
    int innerColor = COLOR65K_BLACK;
    int textColor = COLOR65K_BLACK;

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

    if (module->foregroundColor == outlineColor &&
        module->child[0]->backgroundColor == innerColor &&
        module->child[0]->foregroundColor == textColor)
    {
        return;
    }
    module_set_color(module, outlineColor, outlineColor);
    module_set_color(module->child[0], textColor, innerColor);
    module_draw(display, module);
}

void function_window_create(Module *container, MachineState *state)
{
    FunctionWindow *window = (FunctionWindow *)malloc(sizeof(FunctionWindow));
    window->state = state;

    int padding = 8;

    module_set_window(container, window);

    // Create edit window
    Module *functionWindow = module_create(container);
    module_set_rectangle_circle(functionWindow, 0, 0);
    module_fit_height(functionWindow);
    module_fit_width(functionWindow);
    module_set_padding(functionWindow, 0, 0);
    module_set_color(functionWindow, MAINCOLOR, BACKCOLOR);
    module_align_inner_left(functionWindow);
    module_align_inner_top(functionWindow);

    // Create Function Header
    Module *functionHeader = module_create(functionWindow);
    module_set_padding(functionHeader, padding, padding);
    module_set_text(functionHeader, "Off");
    module_text_max_char(functionHeader, 20);
    module_set_font(functionHeader, RA8876_CHAR_HEIGHT_24);
    module_set_color(functionHeader, COLOR65K_BLACK, functionHeader->parent->foregroundColor);
    module_text_align(functionHeader, MODULE_TEXT_ALIGN_CENTER); // used for dynamic updating of alignment
    module_align_inner_top(functionHeader);
    module_update_callback(functionHeader, update_header);

    // Create Button Area
    Module *buttonArea = module_create(functionWindow);
    module_set_padding(buttonArea, 0, 0);
    module_fit_width(buttonArea);
    module_fit_below(buttonArea, functionHeader);
    module_align_inner_left(buttonArea);
    module_align_below(buttonArea, functionHeader);

    // Create Off Button
    Module *offButton = module_create(buttonArea);
    module_set_rectangle_circle(offButton, 0, 0);
    module_set_color(offButton, COLOR65K_LIGHTGREEN, COLOR65K_LIGHTGREEN);
    module_set_padding(offButton, padding, padding);
    module_fit_space_even(offButton, 3);
    module_fit_space_even_verticle(offButton, 3);
    module_align_space_even(offButton, 1, 3);
    module_align_inner_top(offButton);
    module_touch_callback(offButton, button_set_function, FUNC_MANUAL_OFF);
    module_update_callback(offButton, update_off);

    // Create Off Button Text
    Module *offButtonText = module_create(offButton);
    module_set_padding(offButtonText, 0, 0);
    module_set_text(offButtonText, "Off");
    module_set_font(offButtonText, -1);
    module_set_color(offButtonText, COLOR65K_BLACK, offButtonText->parent->foregroundColor);
    module_align_center(offButtonText);
    module_align_middle(offButtonText);

    // Create Incremental Jog Button
    Module *incrementalButton = module_create(buttonArea);
    module_copy(incrementalButton, offButton);
    module_align_space_even(incrementalButton, 2, 3);
    module_touch_callback(incrementalButton, button_set_function, FUNC_MANUAL_INCREMENTAL_JOG);
    module_update_callback(incrementalButton, update_incremental);

    // Create Incremental Jog Button Text
    Module *incrementalButtonText = module_create(incrementalButton);
    module_copy(incrementalButtonText, offButtonText);
    module_set_text(incrementalButtonText, "INCR");
    module_set_font(incrementalButtonText, -1);
    module_align_center(incrementalButtonText);
    module_align_middle(incrementalButtonText);

    // Create Continuous Jog Button
    Module *continuousButton = module_create(buttonArea);
    module_copy(continuousButton, offButton);
    module_align_space_even(continuousButton, 3, 3);
    module_touch_callback(continuousButton, button_set_function, FUNC_MANUAL_CONTINUOUS_JOG);
    module_update_callback(continuousButton, update_continuous);

    // Create Continuous Jog Button Text
    Module *continuousButtonText = module_create(continuousButton);
    module_copy(continuousButtonText, offButtonText);
    module_set_text(continuousButtonText, "CONT");
    module_set_font(continuousButtonText, -1);
    module_align_center(continuousButtonText);
    module_align_middle(continuousButtonText);

    // Create Position Move Button
    Module *positionalMoveButton = module_create(buttonArea);
    module_copy(positionalMoveButton, offButton);
    module_align_space_even(positionalMoveButton, 1, 3);
    module_align_below(positionalMoveButton, offButton);
    module_touch_callback(positionalMoveButton, button_set_function, FUNC_MANUAL_POSITIONAL_MOVE);
    module_update_callback(positionalMoveButton, update_positional);

    // Create Position Jog Button Text
    Module *positionButtonText = module_create(positionalMoveButton);
    module_copy(positionButtonText, offButtonText);
    module_set_text(positionButtonText, "POSN");
    module_set_font(positionButtonText, -1);
    module_align_center(positionButtonText);
    module_align_middle(positionButtonText);

    // Create Home Button
    Module *homeButton = module_create(buttonArea);
    module_copy(homeButton, positionalMoveButton);
    module_align_space_even(homeButton, 2, 3);
    module_touch_callback(homeButton, button_set_function, FUNC_MANUAL_HOME);
    module_update_callback(homeButton, update_home);

    // Create Home Jog Button Text
    Module *homeButtonText = module_create(homeButton);
    module_copy(homeButtonText, offButtonText);
    module_set_text(homeButtonText, "HOME");
    module_set_font(homeButtonText, -1);
    module_align_center(homeButtonText);
    module_align_middle(homeButtonText);

    // Create Gauge Force Button
    Module *gaugeForceButton = module_create(buttonArea);
    module_copy(gaugeForceButton, homeButton);
    module_align_space_even(gaugeForceButton, 3, 3);
    module_touch_callback(gaugeForceButton, button_set_function, FUNC_MANUAL_MOVE_FORCE);
    module_update_callback(gaugeForceButton, update_force);

    // Create Gauge Force Button Text
    Module *gaugeForceButtonText = module_create(gaugeForceButton);
    module_copy(gaugeForceButtonText, offButtonText);
    module_set_text(gaugeForceButtonText, "GA-L");
    module_set_font(gaugeForceButtonText, -1);
    module_align_center(gaugeForceButtonText);
    module_align_middle(gaugeForceButtonText);

    // Create Gauge Length Button
    Module *gaugeLengthButton = module_create(buttonArea);
    module_copy(gaugeLengthButton, offButton);
    module_align_space_even(gaugeLengthButton, 2, 3);
    module_align_below(gaugeLengthButton, positionalMoveButton);
    module_touch_callback(gaugeLengthButton, button_set_function, FUNC_MANUAL_MOVE_GAUGE_LENGTH);
    module_update_callback(gaugeLengthButton, update_length);

    // Create Gauge Length Button Text
    Module *gaugeLengthButtonText = module_create(gaugeLengthButton);
    module_copy(gaugeLengthButtonText, offButtonText);
    module_set_text(gaugeLengthButtonText, "GA-F");
    module_set_font(gaugeLengthButtonText, -1);
    module_align_center(gaugeLengthButtonText);
    module_align_middle(gaugeLengthButtonText);
}

void function_window_destroy(FunctionWindow *window)
{
    free(window);
}
/*void function_window_update(FunctionWindow *window)
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

    display_set_text_parameter1(window->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display_set_text_parameter2(window->display, RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);

    display_text_color(window->display, MAINTEXTCOLOR, MAINCOLOR);

    char buf[50];
    switch (window->state->function)
    {
    case FUNC_MANUAL_OFF:
        sprintf(buf, "OFF");
        break;
    case FUNC_MANUAL_INCREMENTAL_JOG:
        sprintf(buf, "INCREMENTAL JOG: %0.1fmm", (float)window->state->functionData / 1000.0);
        break;
    case FUNC_MANUAL_CONTINUOUS_JOG:
        sprintf(buf, "CONTINUOUS JOG: %dmm", window->state->functionData / 1000);
        break;
    case FUNC_MANUAL_POSITIONAL_MOVE:
        sprintf(buf, "POSITIONAL MOVE: %dmm", window->state->functionData / 1000);
        break;
    case FUNC_MANUAL_HOME:
        if (window->state->functionData == HOMING_NONE)
            sprintf(buf, "HOME");
        else if (window->state->functionData == HOMING_SEEKING)
            sprintf(buf, "HOMING");
        else if (window->state->functionData == HOMING_SEEKING_SLOW)
            sprintf(buf, "HOMING SEEKING SLOW");
        else if (window->state->functionData == HOMING_BACKING_OFF)
            sprintf(buf, "HOMING BACKING OFF");
        else if (window->state->functionData == HOMING_COMPLETE)
            sprintf(buf, "HOMING COMPLETE");
        break;
    case FUNC_MANUAL_MOVE_GAUGE_LENGTH:
        sprintf(buf, "MOVE GAUGE LENGTH: %dmm", window->state->functionData / 1000);
        break;
    case FUNC_MANUAL_MOVE_FORCE:
        sprintf(buf, "MOVE GAUGE FORCE: %dmN", window->state->functionData);
        break;
    default:
        sprintf(buf, "OFF");
        break;
    }
    display_draw_string(window->display, window->x + 10, window->y + 10, buf);

    int outlineColor = COLOR65K_BLACK;
    int innerColor = COLOR65K_BLACK;
    int textColor = COLOR65K_BLACK;

    strcpy(buf, "OFF");
    if (window->state->state == STATE_MOTION && window->state->motionParameters.status != STATUS_DISABLED)
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
    if (window->state->state == STATE_MOTION && window->state->motionParameters.condition != MOTION_MOVING && window->state->motionParameters.status != STATUS_DISABLED)
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
    if (window->state->state == STATE_MOTION && window->state->motionParameters.condition != MOTION_MOVING && window->state->motionParameters.status != STATUS_DISABLED)
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
* /
