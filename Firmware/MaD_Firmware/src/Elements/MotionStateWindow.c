#include "MotionStateWindow.h"
#include "style.h"
#define BUTTONCOUNT 3

#define BUTTON_STATUS 0
#define BUTTON_CONDITION 1
#define BUTTON_MODE 2

static char disabledStr[] = "DISABLD";
static char enabledStr[] = "ENABLED";
static char smplimStr[] = "SMP LIM";
static char mchlimStr[] = "MCH LIM";
static char faultedStr[] = "FAULTED";

static char lengthStr[] = "LENGTH";
static char forceStr[] = "FORCE";
static char tensionStr[] = "TENSION";
static char compresStr[] = "COMPRES";
static char upperStr[] = "UPPER";
static char lowerStr[] = "LOWER";
static char doorStr[] = "DOOR";
static char stoppedStr[] = "STOPPED";
static char movingStr[] = "MOVING";

static char manualStr[] = "MANUAL";
static char testStr[] = "TEST";
static char runStr[] = "RUN";

static char statusStr[] = "Status";
static char conditionStr[] = "Condition";
static char modeStr[] = "Mode";

static void button_callback(int id, void *arg)
{
    MotionStateWindow *window = (MotionStateWindow *)arg;
    switch (id)
    {
    case BUTTON_STATUS:
        switch (window->state->motionParameters.status)
        {
        case STATUS_DISABLED:
            printf("Status: Disabled");
            state_machine_set(window->state, PARAM_MOTION_STATUS, STATUS_ENABLED);
            break;
        case STATUS_ENABLED:
            printf("Status: enabled");
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

static void update_status(Display *display, Module *module, void *arg)
{
    MotionStateWindow *window = (MotionStateWindow *)arg;

    int statusOutlineColor = COLOR65K_BLACK;
    int statusInnerColor = COLOR65K_BLACK;
    int statusTextColor = COLOR65K_BLACK;
    char *newString = NULL;
    switch (window->state->motionParameters.status)
    {
    case STATUS_DISABLED:
    {
        newString = disabledStr;
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
        newString = enabledStr;
        statusOutlineColor = COLOR65K_GREEN;
        statusInnerColor = COLOR65K_WHITE;
        statusTextColor = COLOR65K_BLACK;
        break;
    }
    case STATUS_SAMPLE_LIMIT:
    {

        newString = smplimStr;
        statusOutlineColor = COLOR65K_BLUE;
        statusInnerColor = COLOR65K_YELLOW;
        statusTextColor = COLOR65K_BLACK;
        break;
    }
    case STATUS_MACHINE_LIMIT:
    {
        newString = mchlimStr;
        statusOutlineColor = COLOR65K_BLUE;
        statusInnerColor = COLOR65K_YELLOW;
        statusTextColor = COLOR65K_BLACK;
        break;
    }
    case STATUS_FAULTED:
    {
        newString = faultedStr;
        statusOutlineColor = COLOR65K_BLUE;
        statusInnerColor = COLOR65K_RED;
        statusTextColor = COLOR65K_WHITE;
        break;
    }
    default:
        break;
    }

    if (module->borderColor == statusOutlineColor &&
        module->child[0]->backgroundColor == statusInnerColor &&
        module->child[0]->foregroundColor == statusTextColor)
    {
        return;
    }

    module->borderColor = statusOutlineColor;
    module_set_color(module, statusInnerColor, statusInnerColor);
    module_set_color(module->child[0], statusTextColor, statusInnerColor);
    module_text_update(module->child[0], newString);
    module_draw(display, module);
}

static void update_condition(Display *display, Module *module, void *arg)
{
    MotionStateWindow *window = (MotionStateWindow *)arg;

    int conditionOutlineColor = COLOR65K_BLACK;
    int conditionInnerColor = COLOR65K_BLACK;
    int conditionTextColor = COLOR65K_BLACK;
    char *newString = NULL;

    switch (window->state->motionParameters.condition)
    {
    case MOTION_LENGTH:
    {
        newString = lengthStr;
        conditionOutlineColor = COLOR65K_MAGENTA;
        conditionInnerColor = COLOR65K_MAGENTA;
        conditionTextColor = COLOR65K_BLACK;
        break;
    }
    case MOTION_FORCE:
    {
        newString = forceStr;
        conditionOutlineColor = COLOR65K_MAGENTA;
        conditionInnerColor = COLOR65K_MAGENTA;
        conditionTextColor = COLOR65K_BLACK;
        break;
    }
    case MOTION_TENSION:
    {
        newString = tensionStr;
        conditionOutlineColor = COLOR65K_YELLOW;
        conditionInnerColor = COLOR65K_YELLOW;
        conditionTextColor = COLOR65K_BLACK;
        break;
    }
    case MOTION_COMPRESSION:
    {
        newString = compresStr;
        conditionOutlineColor = COLOR65K_YELLOW;
        conditionInnerColor = COLOR65K_YELLOW;
        conditionTextColor = COLOR65K_BLACK;
        break;
    }
    case MOTION_UPPER:
    {
        newString = upperStr;
        conditionOutlineColor = COLOR65K_YELLOW;
        conditionInnerColor = COLOR65K_YELLOW;
        conditionTextColor = COLOR65K_BLACK;
        break;
    }
    case MOTION_LOWER:
    {
        newString = lowerStr;
        conditionOutlineColor = COLOR65K_YELLOW;
        conditionInnerColor = COLOR65K_YELLOW;
        conditionTextColor = COLOR65K_BLACK;
        break;
    }
    case MOTION_DOOR:
    {
        newString = doorStr;
        conditionOutlineColor = COLOR65K_YELLOW;
        conditionInnerColor = COLOR65K_YELLOW;
        conditionTextColor = COLOR65K_BLACK;
        break;
    }
    case MOTION_STOPPED:
    {
        newString = stoppedStr;
        conditionOutlineColor = COLOR65K_RED;
        conditionInnerColor = COLOR65K_RED;
        conditionTextColor = COLOR65K_WHITE;
        break;
    }
    case MOTION_MOVING:
    {
        newString = movingStr;
        conditionOutlineColor = COLOR65K_LIGHTMAGENTA;
        conditionInnerColor = COLOR65K_LIGHTMAGENTA;
        conditionTextColor = COLOR65K_BLACK;
        break;
    }
    default:
        break;
    }
    if (module->foregroundColor == conditionOutlineColor &&
        module->child[0]->backgroundColor == conditionInnerColor &&
        module->child[0]->foregroundColor == conditionTextColor)
    {
        return;
    }
    module_set_color(module, conditionOutlineColor, conditionOutlineColor);
    module_set_color(module->child[0], conditionTextColor, conditionInnerColor);
    module_text_update(module->child[0], newString);
    module_draw(display, module);
}

static void update_mode(Display *display, Module *module, void *arg)
{
    MotionStateWindow *window = (MotionStateWindow *)arg;

    int conditionOutlineColor = COLOR65K_BLACK;
    int conditionInnerColor = COLOR65K_BLACK;
    int conditionTextColor = COLOR65K_BLACK;
    char *newString = NULL;

    switch (window->state->motionParameters.mode)
    {
    case MODE_MANUAL:
    {
        newString = manualStr;
        conditionOutlineColor = COLOR65K_MAGENTA;
        conditionInnerColor = COLOR65K_MAGENTA;
        conditionTextColor = COLOR65K_BLACK;
        break;
    }
    case MODE_TEST:
    {
        newString = testStr;
        conditionOutlineColor = COLOR65K_MAGENTA;
        conditionInnerColor = COLOR65K_MAGENTA;
        conditionTextColor = COLOR65K_BLACK;
        break;
    }
    case MODE_TEST_RUNNING:
    {
        newString = runStr;
        conditionOutlineColor = COLOR65K_MAGENTA;
        conditionInnerColor = COLOR65K_MAGENTA;
        conditionTextColor = COLOR65K_BLACK;
        break;
    }
    default:
        break;
    }
    if (module->foregroundColor == conditionOutlineColor &&
        module->child[0]->backgroundColor == conditionInnerColor &&
        module->child[0]->foregroundColor == conditionTextColor &&
        strcmp(newString, module_text_get(module->child[0])) == 0)
    {
        return;
    }
    module_set_color(module, conditionOutlineColor, conditionOutlineColor);
    module_set_color(module->child[0], conditionTextColor, conditionInnerColor);
    module_text_update(module->child[0], newString);
    module_draw(display, module);
}

void motion_state_window_init(MotionStateWindow *window, Module *container, MachineState *state)
{
    window->state = state;

    module_set_window(container, window);

    int padding = 10;

    // Create edit window
    Module *motionStateWindow = &(window->motionStateWindow);
    module_init(motionStateWindow, container);
    module_set_rectangle_circle(motionStateWindow, 0, 0);
    module_add_border(motionStateWindow, COLOR65K_WHITE, 1);
    module_fit_height(motionStateWindow);
    module_fit_width(motionStateWindow);
    module_set_padding(motionStateWindow, 10, 10);
    module_set_color(motionStateWindow, MAINCOLOR, BACKCOLOR);
    module_align_inner_left(motionStateWindow);
    module_align_inner_top(motionStateWindow);

    // Create Condition Header
    Module *conditionHeader = &(window->conditionHeader);
    module_init(conditionHeader, motionStateWindow);
    module_set_margin(conditionHeader, padding, padding);
    module_set_text(conditionHeader, conditionStr);
    module_text_font(conditionHeader, RA8876_CHAR_HEIGHT_24);
    module_text_fit(conditionHeader);
    module_set_color(conditionHeader, MAINTEXTCOLOR, conditionHeader->parent->foregroundColor);
    module_align_center(conditionHeader);
    module_align_inner_top(conditionHeader);

    // Create Status Header
    Module *statusHeader = &(window->statusHeader);
    module_init(statusHeader, motionStateWindow);
    module_set_margin(statusHeader, padding, padding);
    module_set_text(statusHeader, statusStr);
    module_text_font(statusHeader, RA8876_CHAR_HEIGHT_24);
    module_text_fit(statusHeader);
    module_set_color(statusHeader, MAINTEXTCOLOR, statusHeader->parent->foregroundColor);
    module_align_left(statusHeader, conditionHeader);
    module_align_inner_top(statusHeader);

    // Create Mode Header
    Module *modeHeader = &(window->modeHeader);
    module_init(modeHeader, motionStateWindow);
    module_set_margin(modeHeader, padding, padding);
    module_set_text(modeHeader, modeStr);
    module_text_font(modeHeader, RA8876_CHAR_HEIGHT_24);
    module_text_fit(modeHeader);
    module_set_color(modeHeader, MAINTEXTCOLOR, statusHeader->parent->foregroundColor);
    module_align_right(modeHeader, conditionHeader);
    module_align_inner_top(modeHeader);

    // Create status Button
    Module *statusButton = &(window->statusButton);
    module_init(statusButton, motionStateWindow);
    module_set_rectangle_circle(statusButton, 0, 0);
    module_set_color(statusButton, COLOR65K_LIGHTGREEN, COLOR65K_LIGHTGREEN);
    module_add_border(statusButton, COLOR65K_RED, 3);
    module_set_margin(statusButton, padding, padding);
    module_fit_space_even(statusButton, 3);
    module_fit_below(statusButton, statusHeader);
    module_align_space_even(statusButton, 1, 3);
    module_touch_callback(statusButton, button_callback, BUTTON_STATUS);
    module_redraw_callback(statusButton, update_status);

    // Create status Button Text
    Module *statusButtonText = &(window->statusButtonText);
    module_init(statusButtonText, statusButton);
    module_set_text(statusButtonText, disabledStr);
    module_text_font(statusButtonText, RA8876_CHAR_HEIGHT_24);
    module_text_fit(statusButtonText);
    module_fit_width(statusButtonText);
    module_set_color(statusButtonText, MAINTEXTCOLOR, statusButtonText->parent->foregroundColor);
    module_text_align(statusButtonText, MODULE_TEXT_ALIGN_INNER_CENTER);
    module_align_inner_left(statusButtonText);
    module_align_middle(statusButtonText);

    // Create Condition Button
    Module *conditionButton = &(window->conditionButton);
    module_init(conditionButton, motionStateWindow);
    module_set_rectangle_circle(conditionButton, 0, 0);
    module_set_color(conditionButton, COLOR65K_LIGHTGREEN, COLOR65K_LIGHTGREEN);
    module_set_margin(conditionButton, padding, padding);
    module_set_padding(conditionButton, 2, 2);
    module_fit_space_even(conditionButton, 3);
    module_fit_below(conditionButton, statusHeader);
    module_align_space_even(conditionButton, 2, 3);
    module_redraw_callback(conditionButton, update_condition);

    // Create condition Button Text
    Module *conditionButtonText = &(window->conditionButtonText);
    module_init(conditionButtonText, conditionButton);
    module_set_text(conditionButtonText, stoppedStr);
    module_text_font(conditionButtonText, RA8876_CHAR_HEIGHT_24);
    module_text_fit(conditionButtonText);
    module_fit_width(conditionButtonText);
    module_set_color(conditionButtonText, MAINTEXTCOLOR, conditionButtonText->parent->foregroundColor);
    module_text_align(conditionButtonText, MODULE_TEXT_ALIGN_INNER_CENTER);
    module_align_inner_left(conditionButtonText);
    module_align_middle(conditionButtonText);

    // Create Mode Button
    Module *modeButton = &(window->modeButton);
    module_init(modeButton, motionStateWindow);
    module_set_rectangle_circle(modeButton, 0, 0);
    module_set_color(modeButton, COLOR65K_LIGHTGREEN, COLOR65K_LIGHTGREEN);
    module_set_margin(modeButton, padding, padding);
    module_fit_space_even(modeButton, 3);
    module_fit_below(modeButton, statusHeader);
    module_align_space_even(modeButton, 3, 3);
    module_touch_callback(modeButton, button_callback, BUTTON_MODE);
    module_redraw_callback(modeButton, update_mode);

    // Create Mode Button Text
    Module *modeButtonText = &(window->modeButtonText);
    module_init(modeButtonText, modeButton);
    module_set_text(modeButtonText, manualStr);
    module_text_font(modeButtonText, RA8876_CHAR_HEIGHT_24);
    module_text_fit(modeButtonText);
    module_fit_width(modeButtonText);
    module_set_color(modeButtonText, MAINTEXTCOLOR, modeButtonText->parent->foregroundColor);
    module_text_align(modeButtonText, MODULE_TEXT_ALIGN_INNER_CENTER);
    module_align_inner_left(modeButtonText);
    module_align_middle(modeButtonText);
}
