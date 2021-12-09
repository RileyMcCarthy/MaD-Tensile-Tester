// status page
#include "StatusPage.h"
#include "Images.h"

#define BUTTONCOUNT 3
#define BUTTON_MACHINE_ENABLE 0
#define BUTTON_MACHINE_DISABLE 1
#define BUTTON_NAVIGATION 2

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

static void drawSuccessIndicator(StatusPage *page, int x, int y)
{
    Image *check = page->images->successImage;
    display_bte_memory_copy_image(page->display, check, x, y);
}

static void drawFailIndicator(StatusPage *page, int x, int y)
{
    Image *ex = page->images->failImage;
    display_bte_memory_copy_image(page->display, ex, x, y);
}

StatusPage *status_page_create(Display *display, MachineState *machineState, Images *images)
{
    StatusPage *page = malloc(sizeof(StatusPage));
    page->display = display;
    page->stateMachine = machineState;
    page->complete = false;
    page->images = images;
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

    int machineStateX0 = 20;
    int machineStateX1 = SCREEN_WIDTH / 3 - 10;
    int machineStateY0 = 20;
    int machineStateY1 = SCREEN_HEIGHT - 20;
    int machineStateWidth = machineStateX1 - machineStateX0;

    /*Section windows*/
    // Machine State window
    display_draw_circle_square_fill(page->display, machineStateX0, machineStateY0, machineStateX1, machineStateY1, 20, 20, MAINCOLOR);

    /*Main headers*/
    char buf[50];
    strcpy(buf, "Machine State");
    int machineStateStartX = machineStateX0 + machineStateWidth / 2 - strlen(buf) * 8;
    int machineStateStartY = 40;
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    display_draw_string(page->display, machineStateX0 + machineStateWidth / 2 - strlen(buf) * 8, machineStateY0 + 20, buf);
    display_draw_line(page->display, machineStateStartX, machineStateStartY + 30, machineStateStartX + strlen(buf) * 16, machineStateStartY + 30, MAINTEXTCOLOR);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);

    /*operational states subheaders*/
    int selfCheckStartY = machineStateStartY + 40;
    int selfCheckStartX = machineStateX0 + 20;
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    strcpy(buf, "Self Check Status");
    display_draw_string(page->display, selfCheckStartX, selfCheckStartY, buf);
    display_draw_line(page->display, selfCheckStartX, selfCheckStartY + 22, selfCheckStartX + strlen(buf) * 12, selfCheckStartY + 22, MAINTEXTCOLOR);

    int selfCheckStateStartY = selfCheckStartY + 30;
    int selfCheckStateStartX = selfCheckStartX + 20;
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    strcpy(buf, "Self check:");
    display_draw_string(page->display, selfCheckStateStartX, selfCheckStateStartY, buf);

    int chargePumpStartX = selfCheckStateStartX;
    int chargePumpStartY = selfCheckStateStartY + 30;
    strcpy(buf, "Charge Pump:");
    display_draw_string(page->display, chargePumpStartX, chargePumpStartY, buf);

    int powerStartX = chargePumpStartX;
    int powerStartY = chargePumpStartY + 30;
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    strcpy(buf, "Switched Power:");
    display_draw_string(page->display, powerStartX, powerStartY, buf);

    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    strcpy(buf, "Machine Check Status");
    int machineCheckStartY = powerStartY + 30;
    int machineCheckStartX = selfCheckStartX;
    display_draw_string(page->display, machineCheckStartX, machineCheckStartY, buf);
    display_draw_line(page->display, machineCheckStartX, machineCheckStartY + 22, machineCheckStartX + strlen(buf) * 12, machineCheckStartY + 22, MAINTEXTCOLOR);

    int esdStartX = machineCheckStartX + 20;
    int esdStartY = machineCheckStartY + 30;
    strcpy(buf, "ESD:");
    display_draw_string(page->display, esdStartX, esdStartY, buf);

    int servoStartX = esdStartX;
    int servoStartY = esdStartY + 30;
    strcpy(buf, "Servo Ready:");
    display_draw_string(page->display, servoStartX, servoStartY, buf);

    int forceStartX = servoStartX;
    int forceStartY = servoStartY + 30;
    strcpy(buf, "Force Gauge:");
    display_draw_string(page->display, forceStartX, forceStartY, buf);

    int dyn4StartX = forceStartX;
    int dyn4StartY = forceStartY + 30;
    strcpy(buf, "DYN4 Ready:");
    display_draw_string(page->display, dyn4StartX, dyn4StartY, buf);

    int machineReadyStartX = dyn4StartX;
    int machineReadyStartY = dyn4StartY + 30;
    strcpy(buf, "Machine Ready:");
    display_draw_string(page->display, machineReadyStartX, machineReadyStartY, buf);

    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    strcpy(buf, "Machine Limits");
    int motionStartY = machineReadyStartY + 30;
    int motionStartX = machineCheckStartX;
    display_draw_string(page->display, motionStartX, motionStartY, buf);
    display_draw_line(page->display, motionStartX, motionStartY + 22, motionStartX + strlen(buf) * 12, motionStartY + 22, MAINTEXTCOLOR);

    int hardUpperStartX = motionStartX + 20;
    int hardUpperStartY = motionStartY + 30;
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    strcpy(buf, "Hard Upper Limit:");
    display_draw_string(page->display, hardUpperStartX, hardUpperStartY, buf);

    int hardLowerStartX = hardUpperStartX;
    int hardLowerStartY = hardUpperStartY + 30;
    strcpy(buf, "Hard Lower Limit:");
    display_draw_string(page->display, hardLowerStartX, hardLowerStartY, buf);

    int softUpperStartX = hardLowerStartX;
    int softUpperStartY = hardLowerStartY + 30;
    strcpy(buf, "Soft Upper Limit:");
    display_draw_string(page->display, softUpperStartX, softUpperStartY, buf);

    int softLowerStartX = softUpperStartX;
    int softLowerStartY = softUpperStartY + 30;
    strcpy(buf, "Soft Lower Limit:");
    display_draw_string(page->display, softLowerStartX, softLowerStartY, buf);

    int forceOverloadStartX = softLowerStartX;
    int forceOverloadStartY = softLowerStartY + 30;
    strcpy(buf, "Soft Lower Limit:");
    display_draw_string(page->display, forceOverloadStartX, forceOverloadStartY, buf);

    /*Motion Status window*/
    int motionStatusX0 = SCREEN_WIDTH / 3 + 10;
    int motionStatusX1 = SCREEN_WIDTH * 2 / 3 - 10;
    int motionStatusY0 = 20;
    int motionStatusY1 = 180;
    int motionStatusWidth = motionStatusX1 - motionStatusX0;

    display_draw_circle_square_fill(page->display, motionStatusX0, motionStatusY0, motionStatusX1, motionStatusY1, 20, 20, MAINCOLOR);

    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_32, RA8876_SELECT_8859_1);
    display_set_text_parameter2(page->display, RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
    display_text_color(page->display, MAINTEXTCOLOR, BACKCOLOR);
    strcpy(buf, "Motion Status");
    int motionStatusStartX = motionStatusX0 + motionStatusWidth / 2 - strlen(buf) * 8;
    int motionStatusStartY = motionStatusY0 + 10;
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    display_draw_string(page->display, motionStatusStartX, motionStatusStartY, buf);
    display_draw_line(page->display, motionStatusStartX, motionStatusStartY + 30, motionStatusStartX + strlen(buf) * 16, motionStatusStartY + 30, MAINTEXTCOLOR);

    int motionStatus1StartX = motionStatusX0 + 20;
    int motionStatus1StartY = motionStatusStartY + 40;
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    strcpy(buf, "Motion:");
    display_draw_string(page->display, motionStatus1StartX, motionStatus1StartY, buf);

    int motionConditionStartX = motionStatus1StartX;
    int motionConditionStartY = motionStatus1StartY + 30;
    strcpy(buf, "Condition:");
    display_draw_string(page->display, motionConditionStartX, motionConditionStartY, buf);

    int motionModeStartX = motionConditionStartX;
    int motionModeStartY = motionConditionStartY + 30;
    strcpy(buf, "Mode:");
    display_draw_string(page->display, motionModeStartX, motionModeStartY, buf);

    /*Machine GPIO*/
    int gpioX0 = SCREEN_WIDTH / 3 + 10;
    int gpioX1 = SCREEN_WIDTH * 2 / 3 - 10;
    int gpioY0 = 200;
    int gpioY1 = SCREEN_HEIGHT - 20;
    int gpioWidth = gpioX1 - gpioX0;
    display_draw_circle_square_fill(page->display, gpioX0, gpioY0, gpioX1, gpioY1, 20, 20, MAINCOLOR);

    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_32, RA8876_SELECT_8859_1);
    display_set_text_parameter2(page->display, RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
    display_text_color(page->display, MAINTEXTCOLOR, BACKCOLOR);
    strcpy(buf, "Machine GPIO");
    int gpioStartX = gpioX0 + gpioWidth / 2 - strlen(buf) * 8;
    int gpioStartY = gpioY0 + 10;
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    display_draw_string(page->display, gpioStartX, gpioStartY, buf);
    display_draw_line(page->display, gpioStartX, gpioStartY + 30, gpioStartX + strlen(buf) * 16, gpioStartY + 30, MAINTEXTCOLOR);

    /*page buttons*/
    Button *buttons = (Button *)malloc(sizeof(Button) * BUTTONCOUNT);
    page->buttons = buttons;
    buttons[0].name = BUTTON_MACHINE_ENABLE;
    buttons[0].xmin = SCREEN_WIDTH / 2 - 10 - 100;
    buttons[0].xmax = buttons[0].xmin + 100;
    buttons[0].ymin = 360;
    buttons[0].ymax = buttons[0].ymin + 50;
    buttons[0].pressed = false;
    buttons[0].debounceTimems = 100;
    buttons[0].lastPress = 0;

    buttons[1].name = BUTTON_MACHINE_DISABLE;
    buttons[1].xmin = SCREEN_WIDTH / 2 + 10;
    buttons[1].xmax = buttons[1].xmin + 100;
    buttons[1].ymin = 360;
    buttons[1].ymax = buttons[1].ymin + 50;
    buttons[1].pressed = false;
    buttons[1].debounceTimems = 100;
    buttons[1].lastPress = 0;

    buttons[2].name = BUTTON_NAVIGATION;
    buttons[2].xmin = SCREEN_WIDTH - 100;
    buttons[2].xmax = buttons[2].xmin + 100;
    buttons[2].ymin = 0;
    buttons[2].ymax = buttons[2].ymin + 100;
    buttons[2].pressed = false;
    buttons[2].debounceTimems = 100;
    buttons[2].lastPress = 0;

    Image *navigationImg = page->images->navigationImage;
    printf("printing:%s,%d\n", navigationImg->name, navigationImg->width);
    display_bte_memory_copy_image(page->display, navigationImg, SCREEN_WIDTH - navigationImg->width - 5, 5);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);

    printf("Status page loaded\n");
    while (!page->complete)
    {
        check_buttons(page);
        display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
        display_set_text_parameter2(page->display, RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);

        display_text_color(page->display, MAINTEXTCOLOR, COLOR65K_BLACK);

        /*Self Check State*/
        // self check status
        if (page->stateMachine->selfCheckParameters.rtcReady)
        {
            drawSuccessIndicator(page, machineStateX0 + machineStateWidth - 50, selfCheckStateStartY);
        }
        else
        {
            drawFailIndicator(page, machineStateX0 + machineStateWidth - 50, selfCheckStateStartY);
        }

        // charge pump
        if (page->stateMachine->selfCheckParameters.chargePumpOK)
        {
            drawSuccessIndicator(page, machineStateX0 + machineStateWidth - 50, chargePumpStartY);
        }
        else
        {
            drawFailIndicator(page, machineStateX0 + machineStateWidth - 50, chargePumpStartY);
        }

        /*Machine Check State*/
        // switched power enabled
        if (page->stateMachine->machineCheckParameters.power)
        {
            drawSuccessIndicator(page, machineStateX0 + machineStateWidth - 50, powerStartY);
        }
        else
        {
            drawFailIndicator(page, machineStateX0 + machineStateWidth - 50, powerStartY);
        }

        // ESD
        if (page->stateMachine->machineCheckParameters.esd)
        {
            drawSuccessIndicator(page, machineStateX0 + machineStateWidth - 50, esdStartY);
        }
        else
        {
            drawFailIndicator(page, machineStateX0 + machineStateWidth - 50, esdStartY);
        }

        // Servo
        if (page->stateMachine->machineCheckParameters.servoReady)
        {
            drawSuccessIndicator(page, machineStateX0 + machineStateWidth - 50, servoStartY);
        }
        else
        {
            drawFailIndicator(page, machineStateX0 + machineStateWidth - 50, servoStartY);
        }

        // Force Gauge
        if (page->stateMachine->machineCheckParameters.forceGaugeResponding)
        {
            drawSuccessIndicator(page, machineStateX0 + machineStateWidth - 50, forceStartY);
        }
        else
        {
            drawFailIndicator(page, machineStateX0 + machineStateWidth - 50, forceStartY);
        }

        if (page->stateMachine->machineCheckParameters.dyn4Responding)
        {
            drawSuccessIndicator(page, machineStateX0 + machineStateWidth - 50, dyn4StartY);
        }
        else
        {
            drawFailIndicator(page, machineStateX0 + machineStateWidth - 50, dyn4StartY);
        }

        // machine Ready
        if (page->stateMachine->machineCheckParameters.machineReady)
        {
            drawSuccessIndicator(page, machineStateX0 + machineStateWidth - 50, machineReadyStartY);
        }
        else
        {
            drawFailIndicator(page, machineStateX0 + machineStateWidth - 50, machineReadyStartY);
        }

        /*Motion State*/
        // motion enabled
        if (page->stateMachine->motionParameters.status == STATUS_ENABLED)
        {
            display_text_color(page->display, ENABLEDTEXT, ENABLEDBACK);
            strcpy(buf, "ENABLED");
        }
        else if (page->stateMachine->motionParameters.status == STATUS_DISABLED)
        {
            display_text_color(page->display, DISABLEDTEXT, DISABLEDBACK);
            strcpy(buf, "DISABLED");
        }
        else if (page->stateMachine->motionParameters.status == STATUS_RESTRICTED)
        {
            display_text_color(page->display, ERRORTEXT, ERRORBACK);
            strcpy(buf, "RESTRICTED");
        }
        display_draw_string(page->display, motionStatusX1 - strlen(buf) * 12 - 20, motionStatus1StartY, buf);

        // Motion Status
        switch (page->stateMachine->motionParameters.condition)
        {
        case MOTION_STOPPED:
            display_text_color(page->display, ERRORTEXT, ERRORBACK);
            strcpy(buf, "STOPPED");
            break;
        case MOTION_MOVING:
            display_text_color(page->display, ACTIVETEXT, ACTIVEBACK);
            strcpy(buf, "MOVING");
            break;
        case MOTION_TENSION:
            display_text_color(page->display, WARNINGTEXT, WARNINGBACK);
            strcpy(buf, "TENSION");
            break;
        case MOTION_COMPRESSION:
            display_text_color(page->display, WARNINGTEXT, WARNINGBACK);
            strcpy(buf, "COMPRESSION");
            break;
        case MOTION_UPPER:
            display_text_color(page->display, WARNINGTEXT, WARNINGBACK);
            strcpy(buf, "UPPER");
            break;
        case MOTION_LOWER:
            display_text_color(page->display, WARNINGTEXT, WARNINGBACK);
            strcpy(buf, "LOWER");
            break;
        case MOTION_DOOR:
            display_text_color(page->display, WARNINGTEXT, WARNINGBACK);
            strcpy(buf, "DOOR");
            break;
        default:
            break;
        }
        display_draw_string(page->display, motionStatusX1 - strlen(buf) * 12 - 20, motionConditionStartY, buf);

        // Motion Mode
        switch (page->stateMachine->motionParameters.mode)
        {
        case MODE_MANUAL:
            display_text_color(page->display, COLOR65K_WHITE, COLOR65K_BLUE);
            strcpy(buf, "MANUAL");
            break;
        case MODE_AUTOMATIC:
            display_text_color(page->display, COLOR65K_WHITE, COLOR65K_LIGHTBLUE);
            strcpy(buf, "AUTOMATIC");
            break;
        case MODE_OVERRIDE:
            display_text_color(page->display, ERRORTEXT, ERRORBACK);
            strcpy(buf, "OVERRIDE ");
            break;
        default:
            break;
        }
        display_draw_string(page->display, motionStatusX1 - strlen(buf) * 12 - 20, motionModeStartY, buf);

        /*GPIO*/
        display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
        int x = gpioX0 + 10;
        int y = gpioStartY + 45;
        for (int i = 0; i < 12; i++)
        {
            sprintf(buf, "GPI_%d: %s", i + 1, (1 ? "HIGH" : "LOW ")); // change to using mcp
            if (i > 6)
            {
                x = gpioX1 - 10 - strlen(buf) * 12;
            }
            if (i == 7)
            {
                y = gpioStartY + 45;
            }
            display_draw_string(page->display, x, y, buf);
            y += 30;
        }
        // clock.render();
    }
}
