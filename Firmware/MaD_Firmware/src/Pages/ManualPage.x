#include "ManualPage.h"
#include "Images.h"

#define BUTTONCOUNT 3

enum button_names
{
    BUTTON_MOTION_MODE,
    BUTTON_CONDITION,
    BUTTON_NAVIGATION
};

/* Private Function Declarations */
void checkButtons(ManualPage *page);

/**
 * @brief Manual page contains information and controls for running in manual mode
 * 
 */

/**
 * @brief Callback method when Button is pressed
 * 
 * @param b Button that was pressed
 */
static void checkButtons(ManualPage *page)
{
    display->checkButtons(buttons, BUTTONCOUNT);
    for (int i = 0; i < BUTTONCOUNT; i++)
    {
        if (buttons[i].pressed)
        {
            switch (buttons[i].name)
            {
            case BUTTON_MOTION_MODE:
                /* code */
                break;
            case BUTTON_CONDITION:
                /* code */
                break;
            case BUTTON_NAVIGATION:
                complete = true;
                break;
            default:
                break;
            }
        }
    }
}

ManualPage *ManualPage_create(Display *display)
{
    ManualPage *page = (ManualPage *)malloc(sizeof(ManualPage));
    page->display = display;
    page->complete = false;
    return page;
}

void ManualPage_destroy(ManualPage *page)
{
    free(page);
}

/**
 * @brief Starts manual page
 * 
 */
void ManualPage_run(ManualPage *page)
{
    printf("Manual page running\n");
    page->display->drawSquareFill(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BACKCOLOR);
    page->display->setTextParameter1(RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_32, RA8876_SELECT_8859_1);
    page->display->setTextParameter2(RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X2, RA8876_TEXT_HEIGHT_ENLARGEMENT_X2);
    page->display->textColor(MAINTEXTCOLOR, BACKCOLOR);

    //connect motor
    printf("loaded dyn4\n");
    //Setup NavKey
    i2cNavKey navkey(0b0010010);
    NavKey *navkey = NavKey_create(28, 29, 0b0010010);
    NavKey_begin(navkey, i2cNavKey::INT_DATA | i2cNavKey::WRAP_ENABLE | i2cNavKey::DIRE_RIGHT | i2cNavKey::IPUP_ENABLE);
    NavKey_reset(navkey);

    writeCounter(navkey, (int32_t)0);  /* Reset the counter value */
    writeMax(navkey, (int32_t)10000);  /* Set the maximum threshold*/
    writeMin(navkey, (int32_t)-10000); /* Set the minimum threshold */
    writeStep(navkey, (int32_t)100);   /* Set the step to 1*/

    writeDoublePushPeriod(navkey, 30); /*Set a period for the double push of 300ms */

    char buf[50];
    strcpy(buf, "Manual");
    int titlex = SCREEN_WIDTH / 2 - strlen(buf) * 16;
    int titley = 10;
    putString(navkey->display, titlex, titley, buf);

    setTextParameter1(navkey->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_32, RA8876_SELECT_8859_1);
    setTextParameter2(navkey->display, RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);

    textColor(navkey->display, MAINTEXTCOLOR, BACKCOLOR);
    strcpy(buf, "Information");
    titley = 120;
    titlex = SCREEN_WIDTH / 6 - strlen(buf) * 8;
    putString(navkey->display, titlex, titley, buf);
    drawLine(navkey->display, titlex, titley + 30, titlex + strlen(buf) * 16, titley + 30, MAINTEXTCOLOR);

    textColor(navkey->display, MAINTEXTCOLOR, BACKCOLOR);
    strcpy(buf, "Control");
    titley = 300;
    titlex = SCREEN_WIDTH / 6 - strlen(buf) * 8;
    putString(navkey->display, titlex, titley, buf);
    drawLine(navkey->display, titlex, titley + 30, titlex + strlen(buf) * 16, titley + 30, MAINTEXTCOLOR);
    printf("creating buttons\n");
    Button buttons[BUTTONCOUNT];
    buttons[0].name = BUTTON_MOTION_MODE;
    buttons[0].xmin = SCREEN_WIDTH / 6 - 110;
    buttons[0].xmax = buttons[0].xmin + 100;
    buttons[0].ymin = 350;
    buttons[0].ymax = buttons[0].ymin + 50;
    buttons[0].pressed = false;
    buttons[0].lastPress = 0;

    buttons[1].name = BUTTON_CONDITION;
    buttons[1].xmin = SCREEN_WIDTH / 6 + 10;
    buttons[1].xmax = buttons[1].xmin + 100;
    buttons[1].ymin = 350;
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

    drawSquareFill(navkey->display, buttons[0].xmin, buttons[0].ymin, buttons[0].xmax, buttons[0].ymax, COLOR65K_GREEN);
    drawSquareFill(navkey->display, buttons[1].xmin, buttons[1].ymin, buttons[1].xmax, buttons[1].ymax, COLOR65K_RED);
    bteMemoryCopyImage(navkey->display, navigationImg, SCREEN_WIDTH - navigationImg.width - 5, 5);
    bool initialRender = true;
    printf("Manual page finished laoding\n");
    struct MachineState_t previousState;
    while (!complete)
    {
        struct MachineState_t currentState;
        navkey.updateStatus();
        checkButtons(buttons);

        setTextParameter1(navkey->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_16, RA8876_SELECT_8859_1);
        setTextParameter2(navkey->display, RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
        int textcolor = 0;
        int backcolor = 0;
        int bordercolor = 0;
        if ((previousState.motionParameters.status != currentState.motionParameters.status) || initialRender)
        {
            switch (currentState.motionParameters.status)
            {
            case STATUS_DISABLED:
                strcpy(buf, "DISABLED");
                textcolor = COLOR65K_WHITE;
                backcolor = COLOR65K_BLACK;
                bordercolor = COLOR65K_GREEN;
                break;
            case STATUS_ENABLED:
                strcpy(buf, "ENABLED");
                textcolor = COLOR65K_BLACK;
                backcolor = COLOR65K_WHITE;
                bordercolor = COLOR65K_GREEN;
                break;
            case STATUS_RESTRICTED:
                strcpy(buf, "RESTRICTED");
                textcolor = COLOR65K_WHITE;
                backcolor = COLOR65K_RED;
                bordercolor = COLOR65K_DARKRED;
                break;
            default:
                break;
            }

            display->drawSquareFill(buttons[0].xmin, buttons[0].ymin, buttons[0].xmax, buttons[0].ymax, bordercolor);
            display->drawSquareFill(buttons[0].xmin + 5, buttons[0].ymin + 5, buttons[0].xmax - 5, buttons[0].ymax - 5, backcolor);
            display->textColor(textcolor, backcolor);
            display->putString(buttons[0].xmin + 50 - strlen(buf) * 4, buttons[0].ymin + 25 - 8, buf);
        }
        display->textColor(MAINTEXTCOLOR, BACKCOLOR);
        int position = navkey.readCounterInt();
        //dyn4_send_command(dyn4, 0x01, position);
        sprintf(buf, "Relitive Position: %d", position);
        display->putString(SCREEN_WIDTH / 6 - strlen(buf) * 6, 160, buf);
        strcpy(buf, "Relative Force: 0.00");
        display->putString(SCREEN_WIDTH / 6 - strlen(buf) * 6, 190, buf);
        strcpy(buf, "Absolute Position: 0.00");
        display->putString(SCREEN_WIDTH / 6 - strlen(buf) * 6, 230, buf);
        strcpy(buf, "Absolute Force: 0.00");
        display->putString(SCREEN_WIDTH / 6 - strlen(buf) * 6, 260, buf);
        // clock.render();
        initialRender = false;
        previousState = currentState;
    }
}