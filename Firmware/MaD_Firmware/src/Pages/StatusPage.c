// status page
#include "StatusPage.h"
#include "Images.h"

#define BUTTONCOUNT 3

enum button_names
{
    BUTTON_MACHINE_ENABLE,
    BUTTON_MACHINE_DISABLE,
    BUTTON_NAVIGATION
};

static void check_buttons(StatusPage *page)
{
    if (display_update_buttons(page->display, page->buttons, BUTTONCOUNT) > 0)
    {
        for (int i = 0; i < BUTTONCOUNT; i++)
        {
            if (page->buttons[i].pressed)
            {
                switch (page->buttons[i].name)
                {
                case BUTTON_MACHINE_ENABLE:
                    printf("enabling motion\n");
                    state_machine_set_status(page->stateMachine, STATUS_ENABLED);
                    break;
                case BUTTON_MACHINE_DISABLE:
                    printf("disabling motion\n");
                    state_machine_set_status(page->stateMachine, STATUS_DISABLED);
                    break;
                case BUTTON_NAVIGATION:
                    page->complete = true;
                    break;
                default:
                    break;
                }
            }
        }
    }
}

StatusPage *status_page_create(Display *display, MachineState *machineState)
{
    StatusPage *page = malloc(sizeof(StatusPage));
    page->display = display;
    page->stateMachine = machineState;
    page->complete = false;
    return page;
}

void status_page_destroy(StatusPage *page)
{
    free(page->buttons);
    free(page);
}

void status_page_run(StatusPage *page)
{
    display_draw_square_fill(page->display, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BACKCOLOR);

    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_32, RA8876_SELECT_8859_1);
    display_set_text_parameter2(page->display, RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
    display_text_color(page->display, MAINTEXTCOLOR, BACKCOLOR);

    // display->bteMemoryCopy(page->display,PAGE2_START_ADDR, SCREEN_WIDTH, 0, 0, PAGE1_START_ADDR, SCREEN_WIDTH, 0, 0, 100, 50);

    /*Main headers*/
    char buf[50];
    strcpy(buf, "Operational States");
    int operationalStartX = SCREEN_WIDTH / 6 - strlen(buf) * 8;
    int operationalStartY = 40;
    display_draw_string(page->display, operationalStartX, operationalStartY, buf);
    display_draw_line(page->display, operationalStartX, operationalStartY + 30, operationalStartX + strlen(buf) * 16, operationalStartY + 30, MAINTEXTCOLOR);
    display_draw_square(page->display, 20, 30, SCREEN_WIDTH / 3 - 10, SCREEN_HEIGHT - 20, SECONDARYTEXTCOLOR);

    display_text_color(page->display, MAINTEXTCOLOR, BACKCOLOR);
    strcpy(buf, "General Inputs");
    int generalInputsStartY = 40;
    int generalInputsStartX = SCREEN_WIDTH / 2 - strlen(buf) * 8;
    display_draw_string(page->display, generalInputsStartX, generalInputsStartY, buf);
    display_draw_line(page->display, generalInputsStartX, generalInputsStartY + 30, generalInputsStartX + strlen(buf) * 16, generalInputsStartY + 30, MAINTEXTCOLOR);
    display_draw_square(page->display, SCREEN_WIDTH / 3 + 10, 30, SCREEN_WIDTH * 2 / 3 - 10, SCREEN_HEIGHT / 2 - 10, SECONDARYTEXTCOLOR);

    /*operational states subheaders*/
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display_text_color(page->display, MAINTEXTCOLOR, BACKCOLOR);
    strcpy(buf, "Self Check");
    int selfCheckStartY = operationalStartY + 40;
    int selfCheckStartX = operationalStartX;
    display_draw_string(page->display, selfCheckStartX, selfCheckStartY, buf);
    display_draw_line(page->display, selfCheckStartX, selfCheckStartY + 22, selfCheckStartX + strlen(buf) * 12, selfCheckStartY + 22, MAINTEXTCOLOR);

    display_text_color(page->display, MAINTEXTCOLOR, BACKCOLOR);
    strcpy(buf, "Machine Check");
    int machineCheckStartY = selfCheckStartY + 100;
    int machineCheckStartX = selfCheckStartX;
    display_draw_string(page->display, machineCheckStartX, machineCheckStartY, buf);
    display_draw_line(page->display, machineCheckStartX, machineCheckStartY + 22, machineCheckStartX + strlen(buf) * 12, machineCheckStartY + 22, MAINTEXTCOLOR);

    display_text_color(page->display, MAINTEXTCOLOR, BACKCOLOR);
    strcpy(buf, "Motion");
    int motionStartY = machineCheckStartY + 220;
    int motionStartX = machineCheckStartX;
    display_draw_string(page->display, motionStartX, motionStartY, buf);
    display_draw_line(page->display, motionStartX, motionStartY + 22, motionStartX + strlen(buf) * 12, motionStartY + 22, MAINTEXTCOLOR);

    /*page buttons*/
    Button *buttons = (Button *)malloc(sizeof(Button) * BUTTONCOUNT);
    page->buttons = buttons;
    buttons[0].name = BUTTON_MACHINE_ENABLE;
    buttons[0].xmin = SCREEN_WIDTH / 2 - 10 - 100;
    buttons[0].xmax = buttons[0].xmin + 100;
    buttons[0].ymin = 360;
    buttons[0].ymax = buttons[0].ymin + 50;
    buttons[0].pressed = false;
    buttons[0].lastPress = 0;

    buttons[1].name = BUTTON_MACHINE_DISABLE;
    buttons[1].xmin = SCREEN_WIDTH / 2 + 10;
    buttons[1].xmax = buttons[1].xmin + 100;
    buttons[1].ymin = 360;
    buttons[1].ymax = buttons[1].ymin + 50;
    buttons[1].pressed = false;
    buttons[1].lastPress = 0;

    buttons[2].name = BUTTON_NAVIGATION;
    buttons[2].xmin = SCREEN_WIDTH - 100;
    buttons[2].xmax = buttons[2].xmin + 100;
    buttons[2].ymin = 0;
    buttons[2].ymax = buttons[2].ymin + 100;
    buttons[2].pressed = false;
    buttons[2].lastPress = 0;

    display_draw_square_fill(page->display, buttons[1].xmin, buttons[1].ymin, buttons[1].xmax, buttons[1].ymax, COLOR65K_RED);
    Image navigationImg; //= image_get(IMAGE_NAVIGATION);
    display_bte_memory_copy_image(page->display, navigationImg, SCREEN_WIDTH - navigationImg.width - 5, 5);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    Image button_wide; // = image_get(IMAGE_BUTTON_WIDE);
    display_bte_memory_copy_image(page->display, button_wide, buttons[0].xmin, buttons[0].ymin);

    printf("Status page loaded\n");
    while (!page->complete)
    {
        check_buttons(page);
        display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
        display_set_text_parameter2(page->display, RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);

        display_text_color(page->display, MAINTEXTCOLOR, COLOR65K_BLACK);
        strcpy(buf, "Enable");
        display_draw_string(page->display, buttons[0].xmin + 50 - strlen(buf) * 6, buttons[0].ymin + 25 - 12, buf);
        display_text_color(page->display, MAINTEXTCOLOR, COLOR65K_BLACK);
        strcpy(buf, "Disable");
        display_draw_string(page->display, buttons[1].xmin + 50 - strlen(buf) * 6, buttons[1].ymin + 25 - 12, buf);

        /*Self Check State*/
        // self check status
        if (page->stateMachine->selfCheckParameters.chargePumpOK)
        {
            display_text_color(page->display, MAINTEXTCOLOR, BACKCOLOR);
            strcpy(buf, "Self check: COMPLETE");
        }
        else
        {
            display_text_color(page->display, COLOR65K_RED, BACKCOLOR);
            strcpy(buf, "Self check: PENDING ");
        }
        display_draw_string(page->display, selfCheckStartX, selfCheckStartY + 30, buf);

        // charge pump
        if (page->stateMachine->selfCheckParameters.chargePumpOK)
        {
            display_text_color(page->display, MAINTEXTCOLOR, BACKCOLOR);
            strcpy(buf, "Charge Pump: OK  ");
        }
        else
        {
            display_text_color(page->display, COLOR65K_RED, BACKCOLOR);
            strcpy(buf, "Charge Pump: FAIL");
        }
        display_draw_string(page->display, selfCheckStartX, selfCheckStartY + 60, buf);

        /*Machine Check State*/
        // switched power enabled
        if (page->stateMachine->machineCheckParameters.power)
        {
            display_text_color(page->display, MAINTEXTCOLOR, BACKCOLOR);
            strcpy(buf, "Switched Power: ON ");
        }
        else
        {
            display_text_color(page->display, COLOR65K_RED, BACKCOLOR);
            strcpy(buf, "Switched Power: OFF");
        }
        display_draw_string(page->display, machineCheckStartX, machineCheckStartY + 30, buf);

        // upper overtravel
        if (page->stateMachine->motionParameters.hardUpperLimit)
        {
            display_text_color(page->display, MAINTEXTCOLOR, BACKCOLOR);
            strcpy(buf, "Hard Upper Limit Fault: NONE  ");
        }
        else
        {
            display_text_color(page->display, COLOR65K_RED, BACKCOLOR);
            strcpy(buf, "Hard Upper Limit Fault: ACTIVE");
        }
        display_draw_string(page->display, machineCheckStartX, machineCheckStartY + 60, buf);

        // ESD
        if (page->stateMachine->machineCheckParameters.esd)
        {
            display_text_color(page->display, MAINTEXTCOLOR, BACKCOLOR);
            strcpy(buf, "ESD Fault: NONE  ");
        }
        else
        {
            display_text_color(page->display, COLOR65K_RED, BACKCOLOR);
            strcpy(buf, "ESD Fault: ACTIVE");
        }
        display_draw_string(page->display, machineCheckStartX, machineCheckStartY + 90, buf);

        // Servo
        if (page->stateMachine->machineCheckParameters.servoReady)
        {
            display_text_color(page->display, MAINTEXTCOLOR, BACKCOLOR);
            strcpy(buf, "Servo: READY   ");
        }
        else
        {
            display_text_color(page->display, COLOR65K_RED, BACKCOLOR);
            strcpy(buf, "Servo: WAITING");
        }
        display_draw_string(page->display, machineCheckStartX, machineCheckStartY + 120, buf);

        // Force Gauge
        if (page->stateMachine->machineCheckParameters.forceGaugeResponding)
        {
            display_text_color(page->display, MAINTEXTCOLOR, BACKCOLOR);
            strcpy(buf, "Force Gauge: READY   ");
        }
        else
        {
            display_text_color(page->display, COLOR65K_RED, BACKCOLOR);
            strcpy(buf, "Force Gauge: WAITING");
        }
        display_draw_string(page->display, machineCheckStartX, machineCheckStartY + 150, buf);

        // machine Ready
        if (page->stateMachine->machineCheckParameters.machineReady)
        {
            display_text_color(page->display, MAINTEXTCOLOR, BACKCOLOR);
            strcpy(buf, "Machine: READY   ");
        }
        else
        {
            display_text_color(page->display, COLOR65K_RED, BACKCOLOR);
            strcpy(buf, "Machine: WAITING");
        }
        display_draw_string(page->display, machineCheckStartX, machineCheckStartY + 180, buf);

        /*Motion State*/
        // motion enabled
        if (page->stateMachine->motionParameters.status == STATUS_ENABLED)
        {
            display_text_color(page->display, ENABLEDTEXT, ENABLEDBACK);
            strcpy(buf, "Motion: ENABLED       ");
        }
        else if (page->stateMachine->motionParameters.status == STATUS_DISABLED)
        {
            display_text_color(page->display, DISABLEDTEXT, DISABLEDBACK);
            strcpy(buf, "Motion: DISABLED      ");
        }
        else if (page->stateMachine->motionParameters.status == STATUS_RESTRICTED)
        {
            display_text_color(page->display, ERRORTEXT, ERRORBACK);
            strcpy(buf, "Motion: RESTRICTED    ");
        }
        display_draw_string(page->display, motionStartX, motionStartY + 30, buf);

        // Motion Status
        switch (page->stateMachine->motionParameters.condition)
        {
        case MOTION_STOPPED:
            display_text_color(page->display, ERRORTEXT, ERRORBACK);
            strcpy(buf, "Condition: STOPPED    ");
            break;
        case MOTION_MOVING:
            display_text_color(page->display, ACTIVETEXT, ACTIVEBACK);
            strcpy(buf, "Condition: MOVING     ");
            break;
        case MOTION_TENSION:
            display_text_color(page->display, WARNINGTEXT, WARNINGBACK);
            strcpy(buf, "Condition: TENSION    ");
            break;
        case MOTION_COMPRESSION:
            display_text_color(page->display, WARNINGTEXT, WARNINGBACK);
            strcpy(buf, "Condition: COMPRESSION");
            break;
        case MOTION_UPPER:
            display_text_color(page->display, WARNINGTEXT, WARNINGBACK);
            strcpy(buf, "Condition: UPPER      ");
            break;
        case MOTION_LOWER:
            display_text_color(page->display, WARNINGTEXT, WARNINGBACK);
            strcpy(buf, "Condition: LOWER     ");
            break;
        case MOTION_DOOR:
            display_text_color(page->display, WARNINGTEXT, WARNINGBACK);
            strcpy(buf, "Condition: DOOR      ");
            break;
        default:
            break;
        }
        display_draw_string(page->display, motionStartX, motionStartY + 60, buf);

        // Motion Mode
        switch (page->stateMachine->motionParameters.mode)
        {
        case MODE_MANUAL:
            display_text_color(page->display, COLOR65K_WHITE, COLOR65K_BLUE);
            strcpy(buf, "Mode: MANUAL          ");
            break;
        case MODE_AUTOMATIC:
            display_text_color(page->display, COLOR65K_WHITE, COLOR65K_LIGHTBLUE);
            strcpy(buf, "Mode: AUTOMATIC       ");
            break;
        case MODE_OVERRIDE:
            display_text_color(page->display, ERRORTEXT, ERRORBACK);
            strcpy(buf, "Mode: OVERRIDE        ");
            break;
        default:
            break;
        }
        display_draw_string(page->display, motionStartX, motionStartY + 90, buf);

        /*GPIO*/
        display_text_color(page->display, MAINTEXTCOLOR, BACKCOLOR);
        int x = SCREEN_WIDTH / 3 + 20;
        int y = 80;
        for (int i = 0; i < 12; i++)
        {
            sprintf(buf, "GPI_%d: %s", i + 1, (1 ? "HIGH" : "LOW")); // change to using mcp
            if (i > 6)
            {
                x = SCREEN_WIDTH * 2 / 3 - strlen(buf) * 12 - 20;
            }
            if (i == 7)
            {
                y = 80;
            }
            display_draw_string(page->display, x, y, buf);
            y += 30;
        }
        // clock.render();
    }
}
