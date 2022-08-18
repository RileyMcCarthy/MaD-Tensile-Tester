#include "CalibrateForcePage.h"
#include <stdlib.h>
#include "Monitor.h"

#define BUTTONCOUNT 2
#define BUTTON_NAVIGATION 0
#define BUTTON_FORCE_NAVIGATION 1

static char forceCalibrateBuffer[] = "Force Calibrate";
static char machineSettingsBuffer[] = "Machine Settings";

static bool complete;

void calibrate_force_page_init(CalibratePage *page, Display *display, Monitor *monitor, MachineProfile *machineProfile, Images *images)
{
    page->display = display;
    page->monitor = monitor;
    page->machineProfile = machineProfile;
    page->images = images;

    Module *root = &page->root;
    module_init(root,NULL);

    Module *background = &page->background;
    module_init(background, root);
    module_set_rectangle_circle(background, SCREEN_WIDTH, SCREEN_HEIGHT);
    module_set_position(background, 0, 0);
    module_set_padding(background, 10, 10);
    module_set_color(background, BACKCOLOR, BACKCOLOR);
    module_animate_draw(background, module_animation_switch_page_up);

    // Create navigation button
    Module *navigationButton = &(page->navigation);
    module_init(navigationButton, background);
    module_set_image(navigationButton, &(page->images->navigationImage));
    module_align_inner_top(navigationButton);
    module_align_inner_right(navigationButton);
    module_touch_callback(navigationButton, button_navigation, 0);

    // Create force calibrate window
    Module *forceWindow = &(page->forceWindow);
    module_init(forceWindow, background);
    module_set_padding(forceWindow, 10, 10);
    module_add_border(forceWindow, MAINTEXTCOLOR, 1);
    module_set_margin(forceWindow, 10, 10);
    module_set_rectangle_circle(forceWindow, 0, 0);
    module_fit_space_even(forceWindow, 3);
    module_fit_height(forceWindow);
    module_set_color(forceWindow, MAINCOLOR, BACKCOLOR);
    module_align_center(forceWindow);
    module_align_inner_top(forceWindow);

    // Create machine state window title
    Module *forceTitle = &(page->forceHeader);
    module_init(forceTitle, forceWindow);
    module_set_text(forceTitle, forceCalibrateBuffer);
    module_set_margin(forceTitle, 8, 8);
    module_text_font(forceTitle, RA8876_CHAR_HEIGHT_32);
    module_text_fit(forceTitle);
    module_set_color(forceTitle, MAINTEXTCOLOR, MAINCOLOR);
    module_align_inner_top(forceTitle);
    module_align_center(forceTitle);
    module_text_underline(forceTitle);

    // Force Status
    Module *forceCalStatus = &(page->forceCalStatus);
    module_init(forceCalStatus, forceWindow);
    module_set_margin(forceCalStatus, 8, 8);
    module_set_text(forceCalStatus, page->forceCalStatusBuffer);
    module_text_font(forceCalStatus, RA8876_CHAR_HEIGHT_24);
    module_set_color(forceCalStatus, MAINTEXTCOLOR, MAINCOLOR);
    module_align_below(forceCalStatus, forceTitle);
    module_align_inner_left(forceCalStatus);

    // ADC Value
    Module *adcValue = &(page->adcValue);
    module_init(adcValue, forceWindow);
    module_set_margin(adcValue, 8, 8);
    module_set_text(adcValue, page->adcValueBuffer);
    module_text_font(adcValue, RA8876_CHAR_HEIGHT_24);
    module_set_color(adcValue, MAINTEXTCOLOR, MAINCOLOR);
    module_align_below(adcValue, forceCalStatus);
    module_align_inner_left(adcValue);

    // Zero Value
    Module *zeroValue = &(page->zeroValue);
    module_init(zeroValue, forceWindow);
    module_set_margin(zeroValue, 8, 8);
    module_set_text(zeroValue, page->zeroValueBuffer);
    module_text_font(zeroValue, RA8876_CHAR_HEIGHT_24);
    module_set_color(zeroValue, MAINTEXTCOLOR, MAINCOLOR);
    module_align_below(zeroValue, adcValue);
    module_align_inner_left(zeroValue);

    // Slope Value
    Module *slopeValue = &(page->slopeValue);
    module_init(slopeValue, forceWindow);
    module_set_margin(slopeValue, 8, 8);
    module_set_text(slopeValue, page->slopeValueBuffer);
    module_text_font(slopeValue, RA8876_CHAR_HEIGHT_24);
    module_set_color(slopeValue, MAINTEXTCOLOR, MAINCOLOR);
    module_align_below(slopeValue, zeroValue);
    module_align_inner_left(slopeValue);

    // Slope Value
    Module *slopeValue = &(page->slopeValue);
    module_init(slopeValue, forceWindow);
    module_set_margin(slopeValue, 8, 8);
    module_set_text(slopeValue, page->slopeValueBuffer);
    module_text_font(slopeValue, RA8876_CHAR_HEIGHT_24);
    module_set_color(slopeValue, MAINTEXTCOLOR, MAINCOLOR);
    module_align_below(slopeValue, zeroValue);
    module_align_inner_left(slopeValue);

    // Create machine state window title
    Module *machineSettingsHeader = &(page->machineSettingsHeader);
    module_init(machineSettingsHeader, forceWindow);
    module_set_text(machineSettingsHeader, machineSettingsBuffer);
    module_set_margin(machineSettingsHeader, 8, 8);
    module_text_font(machineSettingsHeader, RA8876_CHAR_HEIGHT_32);
    module_text_fit(machineSettingsHeader);
    module_set_color(machineSettingsHeader, MAINTEXTCOLOR, MAINCOLOR);
    module_align_below(machineSettingsHeader,slopeValue);
    module_align_center(machineSettingsHeader);
    module_text_underline(machineSettingsHeader);

    // msZero Value
    Module *msZeroValue = &(page->msZeroValue);
    module_init(msZeroValue, forceWindow);
    module_set_margin(msZeroValue, 8, 8);
    module_set_text(msZeroValue, page->msZeroValueBuffer);
    module_text_font(msZeroValue, RA8876_CHAR_HEIGHT_24);
    module_set_color(msZeroValue, MAINTEXTCOLOR, MAINCOLOR);
    module_align_below(msZeroValue, machineSettingsHeader);
    module_align_inner_left(msZeroValue);

    // msSlope Value
    Module *msSlopeValue = &(page->msSlopeValue);
    module_init(msSlopeValue, forceWindow);
    module_set_margin(msSlopeValue, 8, 8);
    module_set_text(msSlopeValue, page->msZeroValueBuffer);
    module_text_font(msSlopeValue, RA8876_CHAR_HEIGHT_24);
    module_set_color(msSlopeValue, MAINTEXTCOLOR, MAINCOLOR);
    module_align_below(msSlopeValue, msZeroValue);
    module_align_inner_left(msSlopeValue);

    // Force Value
    Module *forceValue = &(page->forceValue);
    module_init(forceValue, forceWindow);
    module_set_margin(forceValue, 8, 8);
    module_set_text(forceValue, page->msSlopeValueBuffer);
    module_text_font(forceValue, RA8876_CHAR_HEIGHT_24);
    module_set_color(forceValue, MAINTEXTCOLOR, MAINCOLOR);
    module_align_below(forceValue, msSlopeValue);
    module_align_inner_left(forceValue);

    // Action button
    Module *actionButton = &(page->actionButton);
    module_init(actionButton, forceWindow);
    module_set_rectangle_circle(actionButton, 50, 100);
    module_set_color(actionButton, COLOR65K_LIGHTGREEN, COLOR65K_LIGHTGREEN);
    module_add_border(actionButton, COLOR65K_RED, 3);
    module_set_margin(actionButton, 10, 10);
    module_fit_below(actionButton, forceValue);
    module_align_center(actionButton);
    module_touch_callback(actionButton, button_callback, BUTTON_STATUS);

}

// returns 1 if someting is updated
bool calibrate_force_page_run(CalibratePage *page)
{
    bool infoUpdated = false;
    complete = false;
    printf("Force Calibration page running\n");

    module_draw(page->display, &(page->root));

    page->state = 0;
    int lastState = -1;
    int slope;
    int zero;
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
                slope = 0;
                zero = 0;
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
                zero = currentForce;
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
                double forceReal = atof(forceValue);
                printf("force value:%s\n", forceValue);
                if (forceValue == NULL || forceReal > 5 || forceReal < -5)
                {
                    page->state--;
                }
                else
                {
                    slope = (double)(force - zero) / (1000 * forceReal); // steps/mN
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
            page->machineProfile->configuration.forceGaugeScaleFactor = slope;
            page->machineProfile->configuration.forceGaugeZeroFactor = zero;
            page->state = 10;
            infoUpdated = true;
        }
        int force = currentForce;
        sprintf(buf, "ADC: %d", force);
        int readingStartX = calibrateX0 + 10;
        int readingStartY = actionStartY + 30;
        display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
        display_draw_string(page->display, readingStartX, readingStartY, buf);

        sprintf(buf, "Zero: %d", zero);
        int zeroPointStartX = readingStartX;
        int zeroPointStartY = readingStartY + 30;
        display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
        display_draw_string(page->display, zeroPointStartX, zeroPointStartY, buf);

        sprintf(buf, "Slope: %0.3f", slope);
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

        sprintf(buf, "Force: %0.3fN", ((double)(force - page->machineProfile->configuration.forceGaugeZeroFactor) / (page->machineProfile->configuration.forceGaugeScaleFactor * (double)1000))); // need to add it machine configuration
        int forceRealStartX = calibrateX0 + 10;
        int forceRealStartY = forceSlopeStartY + 30;
        display_text_color(page->display, MAINTEXTCOLOR, MAINCOLOR);
        display_draw_string(page->display, forceRealStartX, forceRealStartY, buf);

        lastState = page->state;
        _waitms(100);
    }

    return infoUpdated;
}
