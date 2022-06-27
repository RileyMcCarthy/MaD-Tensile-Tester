#include "CalibrateForcePage.h"
#include <stdlib.h>
#include "Monitor.h"

#define BUTTONCOUNT 2
#define BUTTON_NAVIGATION 0
#define BUTTON_FORCE_NAVIGATION 1

static bool complete;

typedef struct ForceCalibration_s
{
    int zero;
    float slope;
} ForceCalibration;

static void check_buttons(CalibratePage *page)
{
    button_update(page->display);

    if (button_check(page->display, page->buttons, BUTTONCOUNT) > 0)
    {
        for (int i = 0; i < BUTTONCOUNT; i++)
        {
            if (page->buttons[i].pressed)
            {
                switch (page->buttons[i].name)
                {
                case BUTTON_NAVIGATION:
                    complete = true;
                    break;
                case BUTTON_FORCE_NAVIGATION:
                    page->state++;
                    break;
                default:
                    break;
                }
            }
        }
    }
}

void calibrate_force_page_init(CalibratePage *page, Display *display, Monitor *monitor, MachineProfile *machineProfile, Images *images)
{
    page->display = display;
    page->monitor = monitor;
    page->machineProfile = machineProfile;
    page->images = images;
}

// returns 1 if someting is updated
bool calibrate_force_page_run(CalibratePage *page)
{
    bool infoUpdated = false;
    printf("Force Calibration page running\n");

    display_draw_square_fill(page->display, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BACKCOLOR);
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_32, RA8876_SELECT_8859_1);
    display_set_text_parameter2(page->display, RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X2, RA8876_TEXT_HEIGHT_ENLARGEMENT_X2);
    display_text_color(page->display, MAINTEXTCOLOR, BACKCOLOR);
    complete = false;

    char buf[50];
    strcpy(buf, "Calibrate");
    int titlex = SCREEN_WIDTH / 2 - strlen(buf) * 16;
    int titley = 10;
    display_draw_string(page->display, titlex, titley, buf);

    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_32, RA8876_SELECT_8859_1);
    display_set_text_parameter2(page->display, RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
    display_text_color(page->display, MAINTEXTCOLOR, BACKCOLOR);

    int calibrateX0 = 20;
    int calibrateX1 = SCREEN_WIDTH / 3 - 10;
    int calibrateY0 = 100;
    int calibrateY1 = SCREEN_HEIGHT - 20;
    int calibrateWidth = calibrateX1 - calibrateX0;

    /*Section windows*/
    // calibrate window
    display_draw_circle_square_fill(page->display, calibrateX0, calibrateY0, calibrateX1, calibrateY1, 20, 20, MAINCOLOR);

    /*Main headers*/
    strcpy(buf, "Force");
    int calibrateStartX = calibrateX0 + calibrateWidth / 2 - strlen(buf) * 8;
    int calibrateStartY = calibrateY0 + 20;
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    display_draw_string(page->display, calibrateStartX, calibrateStartY, buf);
    display_draw_line(page->display, calibrateStartX, calibrateStartY + 30, calibrateStartX + strlen(buf) * 16, calibrateStartY + 30, MAINTEXTCOLOR);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);

    strcpy(buf, "Machine Settings");
    int machineStartX = calibrateX0 + calibrateWidth / 2 - strlen(buf) * 8;
    int machineStartY = calibrateY0 + 200;
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    display_draw_string(page->display, machineStartX, machineStartY, buf);
    display_draw_line(page->display, machineStartX, machineStartY + 30, machineStartX + strlen(buf) * 16, machineStartY + 30, MAINTEXTCOLOR);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);

    Button *buttons = (Button *)malloc(sizeof(Button) * BUTTONCOUNT);
    page->buttons = buttons;

    buttons[1].name = BUTTON_NAVIGATION;
    buttons[1].xmin = SCREEN_WIDTH - 100;
    buttons[1].xmax = buttons[1].xmin + 100;
    buttons[1].ymin = 0;
    buttons[1].ymax = buttons[1].ymin + 100;
    buttons[1].pressed = false;
    buttons[1].lastPress = 0;

    buttons[0].name = BUTTON_FORCE_NAVIGATION;
    buttons[0].xmin = calibrateX0 + calibrateWidth / 2 - 100;
    buttons[0].xmax = buttons[0].xmin + 200;
    buttons[0].ymin = calibrateY1 - 30 - 100;
    buttons[0].ymax = buttons[0].ymin + 100;
    buttons[0].pressed = false;
    buttons[0].lastPress = 0;

    Image navigationImg = page->images->navigationImage;
    display_bte_memory_copy_image(page->display, &navigationImg, SCREEN_WIDTH - navigationImg.width - 5, 5);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);

    page->state = 0;
    int lastState = -1;
    ForceCalibration forceCalibration;
    int currentMills = 0;
    int lastMills = 0;
    int currentForce = 0;
    while (!complete)
    {
        if (page->state > 10)
        {
            page->state = 0;
        }
        currentMills = _getms();
        if ((currentMills - lastMills) > 100)
        {
            // while (page->monitor->writingData)
            //     ;
            lastMills = currentMills;
            currentForce = page->monitor->data.forceRaw;
        }
        check_buttons(page);
        int actionStartX;
        int actionStartY;
        if (page->state == 0)
        {
            if (page->state != lastState)
            {
                forceCalibration.slope = 0;
                forceCalibration.zero = 0;
                strcpy(buf, "  Press start...  ");
                actionStartX = calibrateX0 + calibrateWidth / 2 - strlen(buf) * 8;
                actionStartY = calibrateStartY + 35;
                display_text_color(page->display, CLICKABLE, MAINCOLOR);
                display_draw_string(page->display, actionStartX, actionStartY, buf);

                display_draw_circle_square_fill(page->display, buttons[0].xmin, buttons[0].ymin, buttons[0].xmax, buttons[0].ymax, 20, 20, CLICKABLE);
                strcpy(buf, "Start!");
                display_text_color(page->display, MAINTEXTCOLOR, CLICKABLE);
                display_draw_string(page->display, buttons[0].xmin + (buttons[0].xmax - buttons[0].xmin) / 2 - strlen(buf) * 8, buttons[0].ymin + (buttons[0].ymax - buttons[0].ymin) / 2 - 16, buf);
            }
        }
        else if (page->state == 1)
        {
            if (page->state != lastState)
            {
                strcpy(buf, " Set Force to zero");
                actionStartX = calibrateX0 + calibrateWidth / 2 - strlen(buf) * 8;
                actionStartY = calibrateStartY + 35;
                display_text_color(page->display, CLICKABLE, MAINCOLOR);
                display_draw_string(page->display, actionStartX, actionStartY, buf);

                display_draw_circle_square_fill(page->display, buttons[0].xmin, buttons[0].ymin, buttons[0].xmax, buttons[0].ymax, 20, 20, CLICKABLE);
                strcpy(buf, "Next");
                display_text_color(page->display, MAINTEXTCOLOR, CLICKABLE);
                display_draw_string(page->display, buttons[0].xmin + (buttons[0].xmax - buttons[0].xmin) / 2 - strlen(buf) * 8, buttons[0].ymin + (buttons[0].ymax - buttons[0].ymin) / 2 - 16, buf);
            }
        }
        else if (page->state == 2)
        {
            if (page->state != lastState)
            {
                forceCalibration.zero = currentForce;
                strcpy(buf, "   Set new force  ");
                actionStartX = calibrateX0 + calibrateWidth / 2 - strlen(buf) * 8;
                actionStartY = calibrateStartY + 35;
                display_text_color(page->display, CLICKABLE, MAINCOLOR);
                display_draw_string(page->display, actionStartX, actionStartY, buf);

                display_draw_circle_square_fill(page->display, buttons[0].xmin, buttons[0].ymin, buttons[0].xmax, buttons[0].ymax, 20, 20, CLICKABLE);
                strcpy(buf, "Select");
                display_text_color(page->display, MAINTEXTCOLOR, CLICKABLE);
                display_draw_string(page->display, buttons[0].xmin + (buttons[0].xmax - buttons[0].xmin) / 2 - strlen(buf) * 8, buttons[0].ymin + (buttons[0].ymax - buttons[0].ymin) / 2 - 16, buf);
            }
        }
        else if (page->state == 3)
        {
            if (page->state != lastState)
            {
                int force = currentForce;
                Keyboard *keyboard = keyboard_create(page->display, page->images);
                char *forceValue = keyboard_get_input(keyboard, "Force:");
                float forceFloat = atof(forceValue);
                printf("force value:%s\n", forceValue);
                if (forceValue == NULL || forceFloat > 5 || forceFloat < -5)
                {
                    page->state--;
                }
                else
                {
                    forceCalibration.slope = (float)(force - forceCalibration.zero) / (1000 * forceFloat); // steps/mN
                    strcpy(buf, "Update new values?");
                    actionStartX = calibrateX0 + calibrateWidth / 2 - strlen(buf) * 8;
                    actionStartY = calibrateStartY + 35;
                    display_text_color(page->display, CLICKABLE, MAINCOLOR);
                    display_draw_string(page->display, actionStartX, actionStartY, buf);

                    display_draw_circle_square_fill(page->display, buttons[0].xmin, buttons[0].ymin, buttons[0].xmax, buttons[0].ymax, 20, 20, CLICKABLE);
                    strcpy(buf, "Update!");
                    display_text_color(page->display, MAINTEXTCOLOR, CLICKABLE);
                    display_draw_string(page->display, buttons[0].xmin + (buttons[0].xmax - buttons[0].xmin) / 2 - strlen(buf) * 8, buttons[0].ymin + (buttons[0].ymax - buttons[0].ymin) / 2 - 16, buf);
                }
                free(forceValue);
            }
        }
        else if (page->state == 4)
        {
            page->machineProfile->configuration.forceGaugeScaleFactor = forceCalibration.slope;
            page->machineProfile->configuration.forceGaugeZeroFactor = forceCalibration.zero;
            page->state = 10;
            infoUpdated = true;
        }
        int force = currentForce;
        sprintf(buf, "ADC: %d", force);
        int readingStartX = calibrateX0 + 10;
        int readingStartY = actionStartY + 30;
        display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
        display_draw_string(page->display, readingStartX, readingStartY, buf);

        sprintf(buf, "Zero: %d", forceCalibration.zero);
        int zeroPointStartX = readingStartX;
        int zeroPointStartY = readingStartY + 30;
        display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
        display_draw_string(page->display, zeroPointStartX, zeroPointStartY, buf);

        sprintf(buf, "Slope: %0.3f", forceCalibration.slope);
        int slopePointStartX = readingStartX;
        int slopePointStartY = zeroPointStartY + 30;
        display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
        display_draw_string(page->display, slopePointStartX, slopePointStartY, buf);

        sprintf(buf, "Zero: %d", page->machineProfile->configuration.forceGaugeZeroFactor);
        int forceZeroStartX = calibrateX0 + 10;
        int forceZeroStartY = machineStartY + 35;
        display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
        display_draw_string(page->display, forceZeroStartX, forceZeroStartY, buf);

        sprintf(buf, "Slope: %0.3f", page->machineProfile->configuration.forceGaugeScaleFactor); // need to add it machine configuration
        int forceSlopeStartX = calibrateX0 + 10;
        int forceSlopeStartY = forceZeroStartY + 30;
        display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
        display_draw_string(page->display, forceSlopeStartX, forceSlopeStartY, buf);

        sprintf(buf, "Force: %0.3fN", ((float)(force - page->machineProfile->configuration.forceGaugeZeroFactor) / (page->machineProfile->configuration.forceGaugeScaleFactor * (float)1000))); // need to add it machine configuration
        int forceRealStartX = calibrateX0 + 10;
        int forceRealStartY = forceSlopeStartY + 30;
        display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
        display_draw_string(page->display, forceRealStartX, forceRealStartY, buf);

        lastState = page->state;
        _waitms(100);
    }

    return infoUpdated;
}
