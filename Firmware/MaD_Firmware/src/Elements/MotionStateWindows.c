#include "MotionStateWindow.h"
#include "style.h"
#define BUTTONCOUNT 3

#define BUTTON_STATUS 0
#define BUTTON_CONDITION 1
#define BUTTON_MODE 2

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
    char buf[50];
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

    if (module->foregroundColor == statusOutlineColor &&
        module->child[0]->backgroundColor == statusInnerColor &&
        module->child[0]->foregroundColor == statusTextColor)
    {
        return;
    }
    module_set_color(module, statusOutlineColor, statusOutlineColor);
    module_set_color(module->child[0], statusTextColor, statusInnerColor);
    module_set_text(module->child[0], buf);
    module_draw(display, module);
}

void motion_state_window_create(Module *container, MachineState *state)
{
    MotionStateWindow *window = (MotionStateWindow *)malloc(sizeof(MotionStateWindow));
    window->state = state;

    module_set_window(container, window);

    int padding = 8;

    // Create edit window
    Module *motionStateWindow = module_create(container);
    module_set_rectangle_circle(motionStateWindow, 0, 0);
    module_fit_height(motionStateWindow);
    module_fit_width(motionStateWindow);
    module_set_padding(motionStateWindow, 0, 0);
    module_set_color(motionStateWindow, MAINCOLOR, BACKCOLOR);
    module_align_inner_left(motionStateWindow);
    module_align_inner_top(motionStateWindow);

    // Create Status Header
    Module *statusHeader = module_create(motionStateWindow);
    module_set_padding(statusHeader, padding, padding);
    module_set_text(statusHeader, "Status");
    module_set_font(statusHeader, RA8876_CHAR_HEIGHT_24);
    module_set_color(statusHeader, COLOR65K_BLACK, statusHeader->parent->foregroundColor);
    module_align_space_even(statusHeader, 1, 3);
    module_align_inner_top(statusHeader);

    // Create Condition Header
    Module *conditionHeader = module_create(motionStateWindow);
    module_set_padding(conditionHeader, padding, padding);
    module_set_text(conditionHeader, "Condition");
    module_set_font(conditionHeader, RA8876_CHAR_HEIGHT_24);
    module_set_color(conditionHeader, COLOR65K_BLACK, conditionHeader->parent->foregroundColor);
    module_align_space_even(conditionHeader, 2, 3);
    module_align_inner_top(conditionHeader);

    // Create Mode Header
    Module *modeHeader = module_create(motionStateWindow);
    module_set_padding(modeHeader, padding, padding);
    module_set_text(modeHeader, "Mode");
    module_set_font(modeHeader, RA8876_CHAR_HEIGHT_24);
    module_set_color(modeHeader, COLOR65K_BLACK, statusHeader->parent->foregroundColor);
    module_align_space_even(modeHeader, 3, 3);
    module_align_inner_top(modeHeader);

    // Create status Button
    Module *statusButton = module_create(motionStateWindow);
    module_set_rectangle_circle(statusButton, 0, 0);
    module_set_color(statusButton, COLOR65K_LIGHTGREEN, COLOR65K_LIGHTGREEN);
    module_set_padding(statusButton, padding, padding);
    module_fit_space_even(statusButton, 3);
    module_fit_below(statusButton, statusHeader);
    module_align_space_even(statusButton, 1, 3);
    module_touch_callback(statusButton, button_callback, BUTTON_STATUS);
    module_update_callback(statusButton, update_status);

    // Create status Button Text
    Module *statusButtonText = module_create(statusButton);
    module_set_padding(statusButtonText, 0, 0);
    module_set_text(statusButtonText, "");
    module_text_max_char(statusButtonText, 8);
    module_set_font(statusButtonText, RA8876_CHAR_HEIGHT_24);
    module_set_color(statusButtonText, COLOR65K_BLACK, statusButtonText->parent->foregroundColor);
    module_text_align(statusButtonText, MODULE_TEXT_ALIGN_CENTER);
    module_align_middle(statusButtonText);

    // Create Condition Button
    Module *conditionButton = module_create(motionStateWindow);
    module_set_rectangle_circle(conditionButton, 0, 0);
    module_set_color(conditionButton, COLOR65K_LIGHTGREEN, COLOR65K_LIGHTGREEN);
    module_set_padding(conditionButton, padding, padding);
    module_fit_space_even(conditionButton, 3);
    module_fit_below(conditionButton, statusHeader);
    module_align_space_even(conditionButton, 2, 3);

    // Create condition Button Text
    Module *conditionButtonText = module_create(conditionButton);
    module_set_padding(conditionButtonText, 0, 0);
    module_set_text(conditionButtonText, "");
    module_text_max_char(conditionButtonText, 8);
    module_set_font(conditionButtonText, RA8876_CHAR_HEIGHT_24);
    module_set_color(conditionButtonText, COLOR65K_BLACK, conditionButtonText->parent->foregroundColor);
    module_text_align(conditionButtonText, MODULE_TEXT_ALIGN_CENTER);
    module_align_middle(conditionButtonText);

    // Create Mode Button
    Module *modeButton = module_create(motionStateWindow);
    module_set_rectangle_circle(modeButton, 0, 0);
    module_set_color(modeButton, COLOR65K_LIGHTGREEN, COLOR65K_LIGHTGREEN);
    module_set_padding(modeButton, padding, padding);
    module_fit_space_even(modeButton, 3);
    module_fit_below(modeButton, statusHeader);
    module_align_space_even(modeButton, 3, 3);
    module_touch_callback(modeButton, button_callback, BUTTON_MODE);

    // Create Mode Button Text
    Module *modeButtonText = module_create(modeButton);
    module_set_padding(modeButtonText, 0, 0);
    module_set_text(modeButtonText, "");
    module_text_max_char(modeButtonText, 8);
    module_set_font(modeButtonText, RA8876_CHAR_HEIGHT_24);
    module_set_color(modeButtonText, COLOR65K_BLACK, modeButtonText->parent->foregroundColor);
    module_text_align(modeButtonText, MODULE_TEXT_ALIGN_CENTER);
    module_align_middle(modeButtonText);

    // Update Button Text and Color
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

    module_set_text(conditionButtonText, buf);
    module_set_font(conditionButtonText, RA8876_CHAR_HEIGHT_24);
    module_align_center(conditionButtonText);
    module_align_inner_top(conditionButtonText);
    module_set_color(conditionButton, conditionOutlineColor, conditionOutlineColor);
    module_set_color(conditionButtonText, conditionTextColor, conditionInnerColor);

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
    case MODE_TEST_RUNNING:
    {
        strcpy(buf, " RUN  ");
        conditionOutlineColor = COLOR65K_MAGENTA;
        conditionInnerColor = COLOR65K_MAGENTA;
        conditionTextColor = COLOR65K_BLACK;
        break;
    }
    default:
        break;
    }

    module_set_text(modeButtonText, buf);
    module_set_font(modeButtonText, RA8876_CHAR_HEIGHT_24);
    module_align_center(modeButtonText);
    module_align_inner_top(modeButtonText);
    module_set_color(modeButton, conditionOutlineColor, conditionOutlineColor);
    module_set_color(modeButtonText, conditionTextColor, conditionInnerColor);
}
void motion_state_window_destroy(MotionStateWindow *window)
{
    free(window);
}
/*void motion_state_window_update(MotionStateWindow *window)
{
    check_buttons(window);

    if (window->state->motionParameters.status == window->lastState.motionParameters.status &&
        window->state->motionParameters.condition == window->lastState.motionParameters.condition &&
        window->state->motionParameters.mode == window->lastState.motionParameters.mode &&
        window->state->state == window->lastState.state)
        return;
    display_draw_circle_square_fill(window->display, window->x, window->y, window->x + 320, window->y + 90, 20, 20, MAINCOLOR);

    display_set_text_parameter1(window->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display_set_text_parameter2(window->display, RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);

    display_text_color(window->display, MAINTEXTCOLOR, MAINCOLOR);
    display_draw_string(window->display, window->buttons[0].xmin + 20, window->y + 5, "Status");
    display_draw_string(window->display, window->buttons[1].xmin, window->y + 5, "Condition");
    display_draw_string(window->display, window->buttons[2].xmin + 30, window->y + 5, "Mode");
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
case MODE_TEST_RUNNING:
{
    strcpy(buf, " RUN  ");
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
* /
