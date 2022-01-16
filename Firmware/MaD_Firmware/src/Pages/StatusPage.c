// status page
#include "StatusPage.h"
#include "Images.h"

#define BUTTONCOUNT 2
#define BUTTON_MACHINE 0
#define BUTTON_NAVIGATION 1

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
    display_draw_square_fill(page->display, x, y, x + check->width - 1, y + check->height - 1, MAINCOLOR);
    display_bte_memory_copy_image(page->display, check, x, y);
}

static void drawFailIndicator(StatusPage *page, int x, int y)
{
    Image *ex = page->images->failImage;
    display_draw_square_fill(page->display, x, y, x + ex->width - 1, y + ex->height - 1, MAINCOLOR);
    display_bte_memory_copy_image(page->display, ex, x, y);
}

StatusPage *status_page_create(Display *display, MachineState *machineState, MonitorData *data, Images *images)
{
    StatusPage *page = malloc(sizeof(StatusPage));
    page->display = display;
    page->stateMachine = machineState;
    page->complete = false;
    page->data = data;
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
    printf("Status page running\n");
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
    char buf[100];
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

    int chargePumpStartY = selfCheckStartY + 30;
    int chargePumpStartX = selfCheckStartX + 20;
    strcpy(buf, "Charge Pump:");
    display_draw_string(page->display, chargePumpStartX, chargePumpStartY, buf);

    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    strcpy(buf, "Machine Check Status");
    int machineCheckStartX = selfCheckStartX;
    int machineCheckStartY = chargePumpStartY + 30;
    display_draw_string(page->display, machineCheckStartX, machineCheckStartY, buf);
    display_draw_line(page->display, machineCheckStartX, machineCheckStartY + 22, machineCheckStartX + strlen(buf) * 12, machineCheckStartY + 22, MAINTEXTCOLOR);

    int powerStartX = machineCheckStartX + 20;
    int powerStartY = machineCheckStartY + 30;
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    strcpy(buf, "Switched Power:");
    display_draw_string(page->display, powerStartX, powerStartY, buf);

    int overTravelStartX = powerStartX;
    int overTravelStartY = powerStartY + 30;
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    strcpy(buf, "ESD Limits:");
    display_draw_string(page->display, overTravelStartX, overTravelStartY, buf);

    int esdStartX = overTravelStartX;
    int esdStartY = overTravelStartY + 30;
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
    strcpy(buf, "DYN4:");
    display_draw_string(page->display, dyn4StartX, dyn4StartY, buf);

    int dyn5StartX = dyn4StartX;
    int rtcStartY = dyn4StartY + 30;
    strcpy(buf, "RTC:");

    strcpy(buf, "Motion Status");
    int motionStatusStartX = dyn4StartX;
    int motionStatusStartY = dyn4StartY + 30;
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    display_draw_string(page->display, motionStatusStartX, motionStatusStartY, buf);
    display_draw_line(page->display, motionStatusStartX, motionStatusStartY + 22, motionStatusStartX + strlen(buf) * 12, motionStatusStartY + 22, MAINTEXTCOLOR);

    int motionStatus1StartX = forceStartX;
    int motionStatus1StartY = motionStatusStartY + 30;
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

    int travelLimitStartX = motionModeStartX;
    int travelLimitStartY = motionModeStartY + 30;
    strcpy(buf, "Travel Limits:");
    display_draw_string(page->display, travelLimitStartX, travelLimitStartY, buf);

    int softLimitStartX = travelLimitStartX;
    int softLimitStartY = travelLimitStartY + 30;
    strcpy(buf, "Soft Limits:");
    display_draw_string(page->display, softLimitStartX, softLimitStartY, buf);

    /*Motion Status window*/
    int motionStatusX0 = SCREEN_WIDTH / 3 + 10;
    int motionStatusX1 = SCREEN_WIDTH * 2 / 3 - 10;
    int motionStatusY0 = 20;
    int motionStatusY1 = SCREEN_HEIGHT - 20;
    int motionStatusWidth = motionStatusX1 - motionStatusX0;

    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_32, RA8876_SELECT_8859_1);
    display_set_text_parameter2(page->display, RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
    display_text_color(page->display, MAINTEXTCOLOR, BACKCOLOR);
    display_draw_circle_square_fill(page->display, motionStatusX0, motionStatusY0, motionStatusX1, motionStatusY1, 20, 20, MAINCOLOR);

    strcpy(buf, "Values");
    int valuesStartX = motionStatusX0 + motionStatusWidth / 2 - strlen(buf) * 8;
    int valuesStartY = motionStatusY0 + 20;
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    display_draw_string(page->display, valuesStartX, valuesStartY, buf);
    display_draw_line(page->display, valuesStartX, valuesStartY + 30, valuesStartX + strlen(buf) * 16, valuesStartY + 30, MAINTEXTCOLOR);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);

    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_32, RA8876_SELECT_8859_1);
    display_set_text_parameter2(page->display, RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
    display_text_color(page->display, MAINTEXTCOLOR, BACKCOLOR);

    int positionStartY = valuesStartY + 40;
    int positionStartX = motionStatusX0 + 20;
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    strcpy(buf, "Position:");
    display_draw_string(page->display, positionStartX, positionStartY, buf);

    int forceValStartY = positionStartY + 30;
    int forceValStartX = positionStartX;
    strcpy(buf, "Force:");
    display_draw_string(page->display, forceValStartX, forceValStartY, buf);

    int forceGraphStartY = forceValStartY + 50;
    int forceGraphStartX = forceValStartX;
    int forceGraphWidth = motionStatusWidth - 40;
    Graph *forceTimeGraph = graph_create(forceGraphStartX, forceGraphStartY, forceGraphWidth, 100, 2.5, 0, "N", "F vs t");
    graph_add_marker(forceTimeGraph, 2);

    int positionGraphStartY = forceGraphStartY + 150;
    int positionGraphStartX = forceGraphStartX;
    int positionGraphWidth = motionStatusWidth - 40;
    Graph *positionTimeGraph = graph_create(positionGraphStartX, positionGraphStartY, positionGraphWidth, 100, 200, 0, "mm", "P vs t");
    graph_add_marker(positionTimeGraph, 150);

    /*page buttons*/
    Button *buttons = (Button *)malloc(sizeof(Button) * BUTTONCOUNT);
    page->buttons = buttons;
    buttons[0].name = BUTTON_MACHINE;
    buttons[0].xmin = SCREEN_WIDTH - 100 - 20;
    buttons[0].xmax = buttons[0].xmin + 100;
    buttons[0].ymin = SCREEN_HEIGHT - 50 - 20;
    buttons[0].ymax = buttons[0].ymin + 50;
    buttons[0].pressed = false;
    buttons[0].debounceTimems = 100;
    buttons[0].lastPress = 0;

    buttons[1].name = BUTTON_NAVIGATION;
    buttons[1].xmin = SCREEN_WIDTH - 100;
    buttons[1].xmax = buttons[1].xmin + 100;
    buttons[1].ymin = 0;
    buttons[1].ymax = buttons[1].ymin + 100;
    buttons[1].pressed = false;
    buttons[1].debounceTimems = 100;
    buttons[1].lastPress = 0;

    Image *navigationImg = page->images->navigationImage;
    display_bte_memory_copy_image(page->display, navigationImg, SCREEN_WIDTH - navigationImg->width - 5, 5);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);

    MotionStateWindow *motionStateWindow = motion_state_window_create(page->display, page->stateMachine, 690, SCREEN_HEIGHT - 90 - 20);

    float *forceLog = __builtin_alloca(sizeof(float) * (forceGraphWidth));
    for (int i = 0; i < forceGraphWidth; i++)
    {
        forceLog[i] = 0;
    }
    printf("Status page loaded\n");
    MachineState previousState = *(page->stateMachine);
    bool initial = true;

    while (!page->complete)
    {
        check_buttons(page);
        display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
        display_set_text_parameter2(page->display, RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);

        display_text_color(page->display, MAINTEXTCOLOR, COLOR65K_BLACK);

        MachineState currentState = *(page->stateMachine);

        /*Self Check State*/
        // charge pump
        if (currentState.selfCheckParameters.chargePump != previousState.selfCheckParameters.chargePump || initial)
            if (currentState.selfCheckParameters.chargePump)
            {
                drawSuccessIndicator(page, machineStateX0 + machineStateWidth - 50, chargePumpStartY);
            }
            else
            {
                drawFailIndicator(page, machineStateX0 + machineStateWidth - 50, chargePumpStartY);
            }

        /*Machine Check State*/
        // switched power enabled
        if (currentState.machineCheckParameters.switchedPower != previousState.machineCheckParameters.switchedPower || initial)
            if (page->stateMachine->machineCheckParameters.switchedPower)
            {
                drawSuccessIndicator(page, machineStateX0 + machineStateWidth - 50, powerStartY);
            }
            else
            {
                drawFailIndicator(page, machineStateX0 + machineStateWidth - 50, powerStartY);
            }

        // Over Travel Limits
        if (currentState.machineCheckParameters.esdTravelLimit != previousState.machineCheckParameters.esdTravelLimit || initial)
            if (page->stateMachine->machineCheckParameters.esdTravelLimit == MOTION_LIMIT_OK)
            {
                drawSuccessIndicator(page, machineStateX0 + machineStateWidth - 50, overTravelStartY);
            }
            else
            {
                drawFailIndicator(page, machineStateX0 + machineStateWidth - 50, overTravelStartY);
            }

        // ESD
        if (currentState.machineCheckParameters.esdSwitch != previousState.machineCheckParameters.esdSwitch || initial)
            if (page->stateMachine->machineCheckParameters.esdSwitch)
            {
                drawSuccessIndicator(page, machineStateX0 + machineStateWidth - 50, esdStartY);
            }
            else
            {
                drawFailIndicator(page, machineStateX0 + machineStateWidth - 50, esdStartY);
            }

        // Servo
        if (currentState.machineCheckParameters.servoOK != previousState.machineCheckParameters.servoOK || initial)
            if (page->stateMachine->machineCheckParameters.servoOK)
            {
                drawSuccessIndicator(page, machineStateX0 + machineStateWidth - 50, servoStartY);
            }
            else
            {
                drawFailIndicator(page, machineStateX0 + machineStateWidth - 50, servoStartY);
            }

        // Force Gauge
        if (currentState.machineCheckParameters.forceGaugeCom != previousState.machineCheckParameters.forceGaugeCom || initial)
            if (page->stateMachine->machineCheckParameters.forceGaugeCom)
            {
                drawSuccessIndicator(page, machineStateX0 + machineStateWidth - 50, forceStartY);
            }
            else
            {
                drawFailIndicator(page, machineStateX0 + machineStateWidth - 50, forceStartY);
            }

        // RTC
        /* if (currentState.machineCheckParameters.rtcOK != previousState.machineCheckParameters.rtcOK || initial)
             if (page->stateMachine->machineCheckParameters.rtcOK)
             {
                 drawSuccessIndicator(page, machineStateX0 + machineStateWidth - 50, rtcStartY);
             }
             else
             {
                 drawFailIndicator(page, machineStateX0 + machineStateWidth - 50, rtcStartY);
             }
 */
        // DYN4
        if (currentState.machineCheckParameters.servoCom != previousState.machineCheckParameters.servoCom || initial)
            if (page->stateMachine->machineCheckParameters.servoCom)
            {
                drawSuccessIndicator(page, machineStateX0 + machineStateWidth - 50, dyn4StartY);
            }
            else
            {
                drawFailIndicator(page, machineStateX0 + machineStateWidth - 50, dyn4StartY);
            }

        /*Motion State*/
        // motion enabled
        if (currentState.motionParameters.status != previousState.motionParameters.status || initial)
        {
            display_draw_square_fill(page->display, machineStateX1 - 11 * 12 - 20, motionStatus1StartY, machineStateX1 - 20, motionStatus1StartY + 24, MAINCOLOR);

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
            else if (page->stateMachine->motionParameters.status == STATUS_SAMPLE_LIMIT)
            {
                display_text_color(page->display, ERRORTEXT, ERRORBACK);
                strcpy(buf, "SMPL LIMIT");
            }
            else if (page->stateMachine->motionParameters.status == STATUS_MACHINE_LIMIT)
            {
                display_text_color(page->display, ERRORTEXT, ERRORBACK);
                strcpy(buf, "MACH LIMIT");
            }
            else if (page->stateMachine->motionParameters.status == STATUS_FAULTED)
            {
                display_text_color(page->display, ERRORTEXT, ERRORBACK);
                strcpy(buf, "FAULTED");
            }
            display_draw_string(page->display, machineStateX1 - strlen(buf) * 12 - 20, motionStatus1StartY, buf);
        }

        // Motion Status
        if (currentState.motionParameters.condition != previousState.motionParameters.condition || initial)
        {
            display_draw_square_fill(page->display, machineStateX1 - 12 * 12 - 20, motionConditionStartY, machineStateX1 - 20, motionConditionStartY + 24, MAINCOLOR);

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
            display_draw_string(page->display, machineStateX1 - strlen(buf) * 12 - 20, motionConditionStartY, buf);
        }
        // Motion Mode
        if (currentState.motionParameters.mode != previousState.motionParameters.mode || initial)
        {
            display_draw_square_fill(page->display, machineStateX1 - 10 * 12 - 20, motionModeStartY, machineStateX1 - 20, motionModeStartY + 24, MAINCOLOR);

            switch (page->stateMachine->motionParameters.mode)
            {
            case MODE_MANUAL:
                display_text_color(page->display, COLOR65K_WHITE, COLOR65K_BLUE);
                strcpy(buf, "MANUAL");
                break;
            case MODE_TEST:
                display_text_color(page->display, COLOR65K_WHITE, COLOR65K_LIGHTBLUE);
                strcpy(buf, "AUTOMATIC");
                break;
            default:
                break;
            }
            display_draw_string(page->display, machineStateX1 - strlen(buf) * 12 - 20, motionModeStartY, buf);
        }

        /*Machine Toggle Button*/

        motion_state_window_update(motionStateWindow);

        /*Values*/
        // printf("Position: %d\n", page->data->position);
        sprintf(buf, "%dmm", page->data->position);
        display_draw_square_fill(page->display, motionStatusX1 - 12 * 12 - 20, positionStartY, motionStatusX1 - 20, positionStartY + 24, MAINCOLOR);
        display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
        display_draw_string(page->display, motionStatusX1 - strlen(buf) * 12 - 20, positionStartY, buf);

        // printf("Force: %f\n", page->data->force);
        sprintf(buf, "%0.3fN", page->data->force);
        display_draw_square_fill(page->display, motionStatusX1 - 12 * 12 - 20, forceValStartY, motionStatusX1 - 20, forceValStartY + 24, MAINCOLOR);
        display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
        display_draw_string(page->display, motionStatusX1 - strlen(buf) * 12 - 20, forceValStartY, buf);
        // display_draw_square_fill(page->display, forceGraphStartX - 1, forceGraphStartY - 1, forceGraphStartX + forceGraphWidth, forceGraphStartY + 100, MAINCOLOR);
        graph_draw(forceTimeGraph, page->display, page->data->force);
        graph_draw(positionTimeGraph, page->display, page->data->position);
        _waitms(10);
        initial = false;
        previousState = currentState;
    }
    graph_destroy(forceTimeGraph);
}