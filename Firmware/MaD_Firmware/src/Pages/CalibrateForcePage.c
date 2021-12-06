#include "CalibrateForcePage.h"

#define BUTTONCOUNT 2

enum button_names
{
    BUTTON_NAVIGATION
};

static void check_buttons(CalibrateForcePage *page)
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

CalibrateForcePage *calibrate_force_page_create(Display *display, MachineState *machineState)
{
    CalibrateForcePage *page = malloc(sizeof(CalibrateForcePage));
    page->display = display;
    page->complete = false;
    return page;
}

void calibrate_force_page_destroy(CalibrateForcePage *page)
{
    free(page->buttons);
    free(page);
}

void calibrate_force_page_run(CalibrateForcePage *page)
{
    printf("Force Calibration page running\n");

    // Reset machine state to stop data aquisition and motion
    bool isMachineReady = machineState->machineCheckParameters.machineReady;
    machineState->machineCheckParameters.machineReady = false;

    display_draw_square_fill(page->display, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BACKCOLOR);
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_32, RA8876_SELECT_8859_1);
    display_set_text_parameter2(page->display, RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X2, RA8876_TEXT_HEIGHT_ENLARGEMENT_X2);
    display_text_color(page->display, MAINTEXTCOLOR, BACKCOLOR);
    page->complete = false;

    char buf[50];
    strcpy(buf, "Force Calibration");
    int titlex = SCREEN_WIDTH / 2 - strlen(buf) * 16;
    int titley = 10;
    display_draw_string(page->display, titlex, titley, buf);

    display_text_color(page->display, MAINTEXTCOLOR, BACKCOLOR);

    int calibrateX0 = 20;
    int calibrateX1 = SCREEN_WIDTH / 3 - 10;
    int calibrateY0 = 20;
    int calibrateY1 = SCREEN_HEIGHT - 20;
    int calibrateWidth = calibrateX1 - calibrateX0;

    /*Section windows*/
    // calibrate window
    display_draw_circle_square_fill(page->display, calibrateX0, calibrateY0, calibrateX1, calibrateY1, 20, 20, MAINCOLOR);

    /*Main headers*/
    char buf[50];
    strcpy(buf, "Calibrate");
    int calibrateStartX = calibrateX0 + calibrateWidth / 2 - strlen(buf) * 8;
    int calibrateStartY = calibrateY0 + 20;
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
    display_draw_string(page->display, calibrateStartX, calibrateStartY, buf);
    display_draw_line(page->display, calibrateStartX, calibrateStartY + 30, calibrateStartX + strlen(buf) * 16, calibrateStartY + 30, MAINTEXTCOLOR);
    display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);

    state = 0;

    while (!page->complete)
    {
        check_buttons(page);
        int actionStartX;
        int actionStartY;
        int buttonStartX;
        int buttonStartY;
        if (state == 0)
        {
            strcpy(buf, "Press start to begin...");
            actionStartX = calibrateX0 + calibrateWidth / 2 - strlen(buf) * 8;
            actionStartY = calibrateStartY + 30;
            display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
            display_draw_string(page->display, actionStartX, actionStartY, buf);

            strcpy(buf, "Start!");
            buttonStartX = calibrateX0 + 100;
            buttonStartY = calibrateY0 - 10 - 50;
            display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
            display_draw_string(page->display, buttonStartX, buttonStartY, buf);
        }

        strcpy(buf, "ADC Reading: 02192112");
        int readingStartX = calibrateX0 + calibrateWidth / 2 - 50;
        int readingStartY = calibrateStartY + 30;
        display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
        display_draw_string(page->display, readingStartX - strlen(buf) * 8 + 50, readingStartY, buf);

        display_draw_circle_square_fill(page->display, buttonStartX, buttonStartY, buttonStartX + 100, buttonStartY + 50, 20, 20, MAINCOLOR);
    }

    machineState->machineCheckParameters.machineReady = isMachineReady;
}
