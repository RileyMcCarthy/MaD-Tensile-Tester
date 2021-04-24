//status page
#include "StatusPage.h"
#include "Images.h"

#define BUTTONCOUNT 3

void StatusPage::checkButtons(Button *buttons)
{
    if (display->checkButtons(buttons, BUTTONCOUNT) > 0)
    {
        for (int i = 0; i < BUTTONCOUNT; i++)
        {
            if (buttons[i].pressed)
            {
                switch (buttons[i].name)
                {
                case button_names::BUTTON_MACHINE_ENABLE:
                    printf("enabling motion\n");
                    machineState->motion.Status(Motion_Status::STATUS_ENABLED);
                    break;
                case button_names::BUTTON_MACHINE_DISABLE:
                    printf("disabling motion\n");
                    machineState->motion.Status(Motion_Status::STATUS_DISABLED);
                    break;
                case button_names::BUTTON_NAVIGATION:
                    complete = true;
                    break;
                default:
                    break;
                }
            }
        }
    }
}

void StatusPage::run(Ra8876_Lite *the_display, MachineState *the_machineState)
{
    display = the_display;
    machineState = the_machineState;
    complete = false;

    MCP23017 gpio;
    gpioConfig(&gpio);

    display->drawSquareFill(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BACKCOLOR);

    display->setTextParameter1(RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_32, RA8876_SELECT_8859_1);
    display->setTextParameter2(RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
    display->textColor(MAINTEXTCOLOR, BACKCOLOR);

    //display->bteMemoryCopy(PAGE2_START_ADDR, SCREEN_WIDTH, 0, 0, PAGE1_START_ADDR, SCREEN_WIDTH, 0, 0, 100, 50);

    /*Main headers*/
    char buf[50];
    strcpy(buf, "Operational States");
    int operationalStartX = SCREEN_WIDTH / 6 - strlen(buf) * 8;
    int operationalStartY = 40;
    display->putString(operationalStartX, operationalStartY, buf);
    display->drawLine(operationalStartX, operationalStartY + 30, operationalStartX + strlen(buf) * 16, operationalStartY + 30, MAINTEXTCOLOR);
    display->drawSquare(20, 30, SCREEN_WIDTH / 3 - 10, SCREEN_HEIGHT - 20, SECONDARYTEXTCOLOR);

    display->textColor(MAINTEXTCOLOR, BACKCOLOR);
    strcpy(buf, "General Inputs");
    int generalInputsStartY = 40;
    int generalInputsStartX = SCREEN_WIDTH / 2 - strlen(buf) * 8;
    display->putString(generalInputsStartX, generalInputsStartY, buf);
    display->drawLine(generalInputsStartX, generalInputsStartY + 30, generalInputsStartX + strlen(buf) * 16, generalInputsStartY + 30, MAINTEXTCOLOR);
    display->drawSquare(SCREEN_WIDTH / 3 + 10, 30, SCREEN_WIDTH * 2 / 3 - 10, SCREEN_HEIGHT / 2 - 10, SECONDARYTEXTCOLOR);

    /*operational states subheaders*/
    display->setTextParameter1(RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
    display->textColor(MAINTEXTCOLOR, BACKCOLOR);
    strcpy(buf, "Self Check");
    int selfCheckStartY = operationalStartY + 40;
    int selfCheckStartX = operationalStartX;
    display->putString(selfCheckStartX, selfCheckStartY, buf);
    display->drawLine(selfCheckStartX, selfCheckStartY + 22, selfCheckStartX + strlen(buf) * 12, selfCheckStartY + 22, MAINTEXTCOLOR);

    display->textColor(MAINTEXTCOLOR, BACKCOLOR);
    strcpy(buf, "Machine Check");
    int machineCheckStartY = selfCheckStartY + 100;
    int machineCheckStartX = selfCheckStartX;
    display->putString(machineCheckStartX, machineCheckStartY, buf);
    display->drawLine(machineCheckStartX, machineCheckStartY + 22, machineCheckStartX + strlen(buf) * 12, machineCheckStartY + 22, MAINTEXTCOLOR);

    display->textColor(MAINTEXTCOLOR, BACKCOLOR);
    strcpy(buf, "Motion");
    int motionStartY = machineCheckStartY + 220;
    int motionStartX = machineCheckStartX;
    display->putString(motionStartX, motionStartY, buf);
    display->drawLine(motionStartX, motionStartY + 22, motionStartX + strlen(buf) * 12, motionStartY + 22, MAINTEXTCOLOR);

    /*page buttons*/
    Button buttons[BUTTONCOUNT];
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

    //display->drawSquareFill(buttons[0].xmin, buttons[0].ymin, buttons[0].xmax, buttons[0].ymax, COLOR65K_GREEN);
    display->drawSquareFill(buttons[1].xmin, buttons[1].ymin, buttons[1].xmax, buttons[1].ymax, COLOR65K_RED);
    display->bteMemoryCopyImage(navigationImg, SCREEN_WIDTH - navigationImg.width - 5, 5);
    display->textColor(MAINTEXTCOLOR, MAINCOLOR);
    display->bteMemoryCopyImage(button_wide, buttons[0].xmin, buttons[0].ymin);

    // display->loadImage("navigation.bin", 1, 800, 300, 100, 100);

    printf("Status page loaded\n");
    while (!complete)
    {
        checkButtons(buttons);
        display->setTextParameter1(RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1);
        display->setTextParameter2(RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);

        machineState->update();

        display->textColor(MAINTEXTCOLOR, COLOR65K_BLACK);
        strcpy(buf, "Enable");
        display->putString(buttons[0].xmin + 50 - strlen(buf) * 6, buttons[0].ymin + 25 - 12, buf);
        display->textColor(MAINTEXTCOLOR, COLOR65K_BLACK);
        strcpy(buf, "Disable");
        display->putString(buttons[1].xmin + 50 - strlen(buf) * 6, buttons[1].ymin + 25 - 12, buf);

        /*Self Check State*/
        //self check status
        if (machineState->selfCheck.chargePumpOK)
        {
            display->textColor(MAINTEXTCOLOR, BACKCOLOR);
            strcpy(buf, "Self check: COMPLETE");
        }
        else
        {
            display->textColor(COLOR65K_RED, BACKCOLOR);
            strcpy(buf, "Self check: PENDING ");
        }
        display->putString(selfCheckStartX, selfCheckStartY + 30, buf);

        //charge pump
        if (machineState->selfCheck.chargePumpOK)
        {
            display->textColor(MAINTEXTCOLOR, BACKCOLOR);
            strcpy(buf, "Charge Pump: OK  ");
        }
        else
        {
            display->textColor(COLOR65K_RED, BACKCOLOR);
            strcpy(buf, "Charge Pump: FAIL");
        }
        display->putString(selfCheckStartX, selfCheckStartY + 60, buf);

        /*Machine Check State*/
        //switched power enabled
        if (machineState->machineCheck.power)
        {
            display->textColor(MAINTEXTCOLOR, BACKCOLOR);
            strcpy(buf, "Switched Power: ON ");
        }
        else
        {
            display->textColor(COLOR65K_RED, BACKCOLOR);
            strcpy(buf, "Switched Power: OFF");
        }
        display->putString(machineCheckStartX, machineCheckStartY + 30, buf);

        //upper overtravel
        if (machineState->machineCheck.upperLimit)
        {
            display->textColor(MAINTEXTCOLOR, BACKCOLOR);
            strcpy(buf, "Upper Limit Fault: NONE  ");
        }
        else
        {
            display->textColor(COLOR65K_RED, BACKCOLOR);
            strcpy(buf, "Upper Limit Fault: ACTIVE");
        }
        display->putString(machineCheckStartX, machineCheckStartY + 60, buf);

        //ESD
        if (machineState->machineCheck.esd)
        {
            display->textColor(MAINTEXTCOLOR, BACKCOLOR);
            strcpy(buf, "ESD Fault: NONE  ");
        }
        else
        {
            display->textColor(COLOR65K_RED, BACKCOLOR);
            strcpy(buf, "ESD Fault: ACTIVE");
        }
        display->putString(machineCheckStartX, machineCheckStartY + 90, buf);

        //Servo
        if (machineState->machineCheck.servoReady)
        {
            display->textColor(MAINTEXTCOLOR, BACKCOLOR);
            strcpy(buf, "Servo: READY   ");
        }
        else
        {
            display->textColor(COLOR65K_RED, BACKCOLOR);
            strcpy(buf, "Servo: WAITING");
        }
        display->putString(machineCheckStartX, machineCheckStartY + 120, buf);

        //Force Gauge
        if (machineState->machineCheck.forceGaugeResponding)
        {
            display->textColor(MAINTEXTCOLOR, BACKCOLOR);
            strcpy(buf, "Force Gauge: READY   ");
        }
        else
        {
            display->textColor(COLOR65K_RED, BACKCOLOR);
            strcpy(buf, "Force Gauge: WAITING");
        }
        display->putString(machineCheckStartX, machineCheckStartY + 150, buf);

        //machine Ready
        if (machineState->machineCheck.machineReady)
        {
            display->textColor(MAINTEXTCOLOR, BACKCOLOR);
            strcpy(buf, "Machine: READY   ");
        }
        else
        {
            display->textColor(COLOR65K_RED, BACKCOLOR);
            strcpy(buf, "Machine: WAITING");
        }
        display->putString(machineCheckStartX, machineCheckStartY + 180, buf);

        /*Motion State*/
        //motion enabled
        if (machineState->motion.status == Motion_Status::STATUS_ENABLED)
        {
            display->textColor(ENABLEDTEXT, ENABLEDBACK);
            strcpy(buf, "Motion: ENABLED       ");
        }
        else if (machineState->motion.status == Motion_Status::STATUS_DISABLED)
        {
            display->textColor(DISABLEDTEXT, DISABLEDBACK);
            strcpy(buf, "Motion: DISABLED      ");
        }
        else if (machineState->motion.status == Motion_Status::STATUS_RESTRICTED)
        {
            display->textColor(ERRORTEXT, ERRORBACK);
            strcpy(buf, "Motion: RESTRICTED    ");
        }
        display->putString(motionStartX, motionStartY + 30, buf);

        //Motion Status
        switch (machineState->motion.condition)
        {
        case Motion_Condition::MOTION_STOPPED:
            display->textColor(ERRORTEXT, ERRORBACK);
            strcpy(buf, "Condition: STOPPED    ");
            break;
        case Motion_Condition::MOTION_MOVING:
            display->textColor(ACTIVETEXT, ACTIVEBACK);
            strcpy(buf, "Condition: MOVING     ");
            break;
        case Motion_Condition::MOTION_TENSION:
            display->textColor(WARNINGTEXT, WARNINGBACK);
            strcpy(buf, "Condition: TENSION    ");
            break;
        case Motion_Condition::MOTION_COMPRESSION:
            display->textColor(WARNINGTEXT, WARNINGBACK);
            strcpy(buf, "Condition: COMPRESSION");
            break;
        case Motion_Condition::MOTION_UPPER:
            display->textColor(WARNINGTEXT, WARNINGBACK);
            strcpy(buf, "Condition: UPPER      ");
            break;
        case Motion_Condition::MOTION_LOWER:
            display->textColor(WARNINGTEXT, WARNINGBACK);
            strcpy(buf, "Condition: LOWER     ");
            break;
        case Motion_Condition::MOTION_DOOR:
            display->textColor(WARNINGTEXT, WARNINGBACK);
            strcpy(buf, "Condition: DOOR      ");
            break;
        default:
            break;
        }
        display->putString(motionStartX, motionStartY + 60, buf);

        //Motion Mode
        switch (machineState->motion.mode)
        {
        case Motion_Mode::MODE_MANUAL:
            display->textColor(COLOR65K_WHITE, COLOR65K_BLUE);
            strcpy(buf, "Mode: MANUAL          ");
            break;
        case Motion_Mode::MODE_AUTOMATIC:
            display->textColor(COLOR65K_WHITE, COLOR65K_LIGHTBLUE);
            strcpy(buf, "Mode: AUTOMATIC       ");
            break;
        case Motion_Mode::MODE_OVERRIDE:
            display->textColor(ERRORTEXT, ERRORBACK);
            strcpy(buf, "Mode: OVERRIDE        ");
            break;
        default:
            break;
        }
        display->putString(motionStartX, motionStartY + 90, buf);

        /*GPIO*/
        display->textColor(MAINTEXTCOLOR, BACKCOLOR);
        int x = SCREEN_WIDTH / 3 + 20;
        int y = 80;
        for (int i = 0; i < 12; i++)
        {
            sprintf(buf, "GPI_%d: %s", i + 1, (gpi(&gpio, i + 1) ? "HIGH" : "LOW"));
            if (i > 6)
            {
                x = SCREEN_WIDTH * 2 / 3 - strlen(buf) * 12 - 20;
            }
            if (i == 7)
            {
                y = 80;
            }
            display->putString(x, y, buf);
            y += 30;
        }
        // clock.render();
    }
}