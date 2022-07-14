#include "FunctionWindow.h"
#include "ControlSystem.h"
#include "style.h"
#define BUTTONCOUNT 7

static char offBuffer[] = "Off";
static char incrementalBuffer[] = "INCR";
static char contBuffer[] = "CONT";
static char posnBuffer[] = "POSN";
static char homeBuffer[] = "HOME";
static char gaugeLengthBuffer[] = "GA-L";
static char gaugeForceBuffer[] = "GA-F";

static void button_set_function(int id, void *arg)
{
    FunctionWindow *window = (FunctionWindow *)arg;
    state_machine_set(window->state, PARAM_FUNCTION, id);
}

static void update_header(Display *display, Module *module, void *arg)
{
    FunctionWindow *window = (FunctionWindow *)arg;
    char lastHeader[30];
    strcpy(lastHeader, window->functionHeaderBuffer);
    // Update header string
    switch (window->state->function)
    {
    case FUNC_MANUAL_OFF:
        sprintf(window->functionHeaderBuffer, "OFF");
        break;
    case FUNC_MANUAL_INCREMENTAL_JOG:
        sprintf(window->functionHeaderBuffer, "INCR JOG: %0.1fmm", (double)window->state->functionData / 1000.0);
        break;
    case FUNC_MANUAL_CONTINUOUS_JOG:
        sprintf(window->functionHeaderBuffer, "CONT JOG: %dmm", window->state->functionData / 1000);
        break;
    case FUNC_MANUAL_POSITIONAL_MOVE:
        sprintf(window->functionHeaderBuffer, "POSN MOVE: %dmm", window->state->functionData / 1000);
        break;
    case FUNC_MANUAL_HOME:
        if (window->state->functionData == HOMING_NONE)
            sprintf(window->functionHeaderBuffer, "HOME");
        else if (window->state->functionData == HOMING_SEEKING)
            sprintf(window->functionHeaderBuffer, "HOMING");
        else if (window->state->functionData == HOMING_SEEKING_SLOW)
            sprintf(window->functionHeaderBuffer, "HOMING SEEKING SLOW");
        else if (window->state->functionData == HOMING_BACKING_OFF)
            sprintf(window->functionHeaderBuffer, "HOMING BACKING OFF");
        else if (window->state->functionData == HOMING_COMPLETE)
            sprintf(window->functionHeaderBuffer, "HOMING COMPLETE");
        break;
    case FUNC_MANUAL_MOVE_GAUGE_LENGTH:
        sprintf(window->functionHeaderBuffer, "GAUGE LENGTH: %dmm", window->state->functionData / 1000);
        break;
    case FUNC_MANUAL_MOVE_FORCE:
        sprintf(window->functionHeaderBuffer, "GAUGE FORCE: %dmN", window->state->functionData);
        break;
    default:
        sprintf(window->functionHeaderBuffer, "OFF");
        break;
    }
    bool update = false;
    if (strcmp(window->functionHeaderBuffer, lastHeader) != 0)
    {
        module_draw(display, module);
    }
}

static void update_off(Display *display, Module *module, void *arg)
{
    FunctionWindow *window = (FunctionWindow *)arg;

    int outlineColor = COLOR65K_BLACK;
    int innerColor = COLOR65K_BLACK;
    int textColor = COLOR65K_BLACK;
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
    module_set_color(module, textColor, innerColor);
    module->borderColor = outlineColor;
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
    module_set_color(module, textColor, innerColor);
    module->borderColor = outlineColor;
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
    module_set_color(module, textColor, innerColor);
    module->borderColor = outlineColor;
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
    module_set_color(module, textColor, innerColor);
    module->borderColor = outlineColor;
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
    module_set_color(module, textColor, innerColor);
    module->borderColor = outlineColor;
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
    module_set_color(module, textColor, innerColor);
    module->borderColor = outlineColor;
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
    module_set_color(module, textColor, innerColor);
    module->borderColor = outlineColor;
    module_draw(display, module);
}

void function_window_init(FunctionWindow *window, Module *container, MachineState *state)
{
    window->state = state;

    int padding = 8;

    module_set_window(container, window);

    // Create edit window
    Module *functionWindow = &(window->functionWindow);
    module_init(functionWindow,container);
    module_set_rectangle_circle(functionWindow, 0, 0);
    module_add_border(functionWindow, COLOR65K_WHITE, 1);
    module_fit_height(functionWindow);
    module_fit_width(functionWindow);
    module_set_padding(functionWindow, 5, 5);
    module_set_color(functionWindow, MAINCOLOR, BACKCOLOR);
    module_align_inner_left(functionWindow);
    module_align_inner_top(functionWindow);

    // Create Function Header
    Module *functionHeader = &(window->functionHeader);
    module_init(functionHeader, functionWindow);
    module_set_padding(functionHeader, padding, padding);
    module_set_margin(functionHeader, padding, padding);
    module_set_text(functionHeader, window->functionHeaderBuffer);
    module_text_font(functionHeader, RA8876_CHAR_HEIGHT_24);
    module_set_color(functionHeader, MAINTEXTCOLOR, functionHeader->parent->foregroundColor);
    module_text_align(functionHeader, MODULE_TEXT_ALIGN_INNER_CENTER); // used for dynamic updating of alignment
    module_align_inner_top(functionHeader);
    module_update_callback(functionHeader, update_header);

    // Create Button Area
    Module *buttonArea = &(window->buttonArea);
    module_init(buttonArea, functionWindow);
    module_set_padding(buttonArea, 0, 0);
    module_fit_width(buttonArea);
    module_fit_below(buttonArea, functionHeader);
    module_align_inner_left(buttonArea);
    module_align_below(buttonArea, functionHeader);

    // Create Off Button
    Module *offButton = &(window->offButton);
    module_init(offButton, buttonArea);
    module_set_text(offButton, offBuffer);
    module_set_color(offButton, MAINTEXTCOLOR, COLOR65K_LIGHTGREEN);
    module_set_margin(offButton, padding, padding);
    module_text_align(offButton, MODULE_TEXT_ALIGN_INNER_CENTER);
    module_text_align_verticle(offButton, MODULE_TEXT_ALIGN_VCENTER);
    module_add_border(offButton, COLOR65K_BLACK, 3);
    module_fit_space_even(offButton, 3);
    module_fit_space_even_verticle(offButton, 3);
    module_align_space_even(offButton, 1, 3);
    module_align_inner_top(offButton);
    module_touch_callback(offButton, button_set_function, FUNC_MANUAL_OFF);
    module_update_callback(offButton, update_off);

    // Create Incremental Jog Button
    Module *incrementalButton = &(window->incrementalButton);
    module_init(incrementalButton, buttonArea);
    module_copy(incrementalButton, offButton);
    module_set_text(incrementalButton, incrementalBuffer);
    module_text_align(incrementalButton, MODULE_TEXT_ALIGN_INNER_CENTER);
    module_text_align_verticle(incrementalButton, MODULE_TEXT_ALIGN_VCENTER);
    module_add_border(incrementalButton, COLOR65K_BLACK, 3);
    module_fit_space_even(incrementalButton, 3);
    module_fit_space_even_verticle(incrementalButton, 3);
    module_align_space_even(incrementalButton, 2, 3);
    module_touch_callback(incrementalButton, button_set_function, FUNC_MANUAL_INCREMENTAL_JOG);
    module_update_callback(incrementalButton, update_incremental);

    // Create Continuous Jog Button
    Module *continuousButton = &(window->continuousButton);
    module_init(continuousButton, buttonArea);
    module_copy(continuousButton, offButton);
    module_set_text(continuousButton, contBuffer);
    module_text_align(continuousButton, MODULE_TEXT_ALIGN_INNER_CENTER);
    module_text_align_verticle(continuousButton, MODULE_TEXT_ALIGN_VCENTER);
    module_add_border(continuousButton, COLOR65K_BLACK, 3);
    module_fit_space_even(continuousButton, 3);
    module_fit_space_even_verticle(continuousButton, 3);
    module_align_space_even(continuousButton, 3, 3);
    module_touch_callback(continuousButton, button_set_function, FUNC_MANUAL_CONTINUOUS_JOG);
    module_update_callback(continuousButton, update_continuous);

    // Create Position Move Button
    Module *positionalMoveButton = &(window->positionalMoveButton);
    module_init(positionalMoveButton, buttonArea);
    module_copy(positionalMoveButton, offButton);
    module_set_text(positionalMoveButton, posnBuffer);
    module_text_align(positionalMoveButton, MODULE_TEXT_ALIGN_INNER_CENTER);
    module_text_align_verticle(positionalMoveButton, MODULE_TEXT_ALIGN_VCENTER);
    module_add_border(positionalMoveButton, COLOR65K_BLACK, 3);
    module_fit_space_even(positionalMoveButton, 3);
    module_fit_space_even_verticle(positionalMoveButton, 3);
    module_align_space_even(positionalMoveButton, 1, 3);
    module_align_below(positionalMoveButton, offButton);
    module_touch_callback(positionalMoveButton, button_set_function, FUNC_MANUAL_POSITIONAL_MOVE);
    module_update_callback(positionalMoveButton, update_positional);

    // Create Home Button
    Module *homeButton = &(window->homeButton);
    module_init(homeButton, buttonArea);
    module_copy(homeButton, positionalMoveButton);
    module_set_text(homeButton, homeBuffer);
    module_text_align(homeButton, MODULE_TEXT_ALIGN_INNER_CENTER);
    module_text_align_verticle(homeButton, MODULE_TEXT_ALIGN_VCENTER);
    module_add_border(homeButton, COLOR65K_BLACK, 3);
    module_fit_space_even(homeButton, 3);
    module_fit_space_even_verticle(homeButton, 3);
    module_align_space_even(homeButton, 2, 3);
    module_touch_callback(homeButton, button_set_function, FUNC_MANUAL_HOME);
    module_update_callback(homeButton, update_home);

    // Create Gauge Force Button
    Module *gaugeForceButton = &(window->gaugeForceButton);
    module_init(gaugeForceButton, buttonArea);
    module_copy(gaugeForceButton, homeButton);
    module_set_text(gaugeForceButton, gaugeLengthBuffer);
    module_text_align(gaugeForceButton, MODULE_TEXT_ALIGN_INNER_CENTER);
    module_text_align_verticle(gaugeForceButton, MODULE_TEXT_ALIGN_VCENTER);
    module_add_border(gaugeForceButton, COLOR65K_BLACK, 3);
    module_fit_space_even(gaugeForceButton, 3);
    module_fit_space_even_verticle(gaugeForceButton, 3);
    module_align_space_even(gaugeForceButton, 3, 3);
    module_touch_callback(gaugeForceButton, button_set_function, FUNC_MANUAL_MOVE_FORCE);
    module_update_callback(gaugeForceButton, update_force);

    // Create Gauge Length Button
    Module *gaugeLengthButton = &(window->gaugeLengthButton);
    module_init(gaugeLengthButton, buttonArea);
    module_copy(gaugeLengthButton, offButton);
    module_set_text(gaugeLengthButton, gaugeForceBuffer);
    module_text_align(gaugeLengthButton, MODULE_TEXT_ALIGN_INNER_CENTER);
    module_text_align_verticle(gaugeLengthButton, MODULE_TEXT_ALIGN_VCENTER);
    module_add_border(gaugeLengthButton, COLOR65K_BLACK, 3);
    module_fit_space_even(gaugeLengthButton, 3);
    module_fit_space_even_verticle(gaugeLengthButton, 3);
    module_align_space_even(gaugeLengthButton, 2, 3);
    module_align_below(gaugeLengthButton, positionalMoveButton);
    module_touch_callback(gaugeLengthButton, button_set_function, FUNC_MANUAL_MOVE_GAUGE_LENGTH);
    module_update_callback(gaugeLengthButton, update_length);
}
