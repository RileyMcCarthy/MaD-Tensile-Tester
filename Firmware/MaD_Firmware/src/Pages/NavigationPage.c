#include "NavigationPage.h"
#include "Images.h"
#include "StatusPage.h"
#include "ManualPage.h"
#include "Module.h"

static char statusTextBuffer[] = "Status";
static char manualTextBuffer[] = "Manual";
static char testTextBuffer[] = "Test";
static char calibrateTextBuffer[] = "Calibrate";
static char settingsTextBuffer[] = "Settings";
static char tpTextBuffer[] = "Create Profiles";

/**
 * @brief Navigation page shows all available pages to access.
 *
 */

/**
 * @brief Number of buttons on the page
 *
 */
#define BUTTONCOUNT 5

/**
 * @brief Enum for name of buttons on page
 *
 */

/**
 * @brief Callback method when Button is pressed
 *
 * @param button Button that was pressed
 */
static void check_buttons(int id, void *arg)
{
    NavigationPage *page = (NavigationPage *)arg;
    page->newPage = id;
    page->complete = true;
}

void navigation_page_init(NavigationPage *page, Display *display, Images *images)
{
    page->display = display;
    page->images = images;

    int padding = 20;
    // Create Background
    Module *root = &(page->root);
    module_init(root, NULL);

    Module *background = &(page->background);
    module_init(background, root);
    module_set_rectangle(background, SCREEN_WIDTH, SCREEN_HEIGHT);
    module_set_position(background, 0, 0);
    module_set_padding(background, 50, 50);
    module_set_color(background, BACKCOLOR, BACKCOLOR);
    module_animate_draw(background, module_animation_switch_page_down);

    // Creating status page
    Module *statusPage = &(page->statusPage);
    module_init(statusPage, background);
    module_set_image(statusPage, &(page->images->statusPageImage));
    module_align_inner_top(statusPage);
    module_align_space_even(statusPage, 1, 4);
    module_touch_callback(statusPage, check_buttons, PAGE_STATUS);

    Module *statusText = &(page->statusText);
    module_init(statusText, statusPage);
    module_set_text(statusText, statusTextBuffer);
    module_text_font(statusText, RA8876_CHAR_HEIGHT_32);
    module_text_fit(statusText);
    module_set_color(statusText, MAINTEXTCOLOR, BACKCOLOR);
    module_align_below(statusText, statusText->parent);
    module_align_center(statusText);

    // Creating manual page
    Module *manualPage = &(page->manualPage);
    module_init(manualPage, background);
    module_set_image(manualPage, &(page->images->manualPageImage));
    module_align_inner_top(manualPage);
    module_align_space_even(manualPage, 2, 4);
    module_touch_callback(manualPage, check_buttons, PAGE_MANUAL);

    Module *manualText = &(page->manualText);
    module_init(manualText, manualPage);
    module_set_text(manualText, manualTextBuffer);
    module_text_font(manualText, RA8876_CHAR_HEIGHT_32);
    module_text_fit(manualText);
    module_set_color(manualText, MAINTEXTCOLOR, BACKCOLOR);
    module_align_below(manualText, manualText->parent);
    module_align_center(manualText);

    // Creating Test page
    Module *testPage = &(page->testPage);
    module_init(testPage, background);
    module_set_image(testPage, &(page->images->automaticPageImage));
    module_align_inner_top(testPage);
    module_align_space_even(testPage, 3, 4);
    module_touch_callback(testPage, check_buttons, PAGE_AUTOMATIC);

    Module *testText = &(page->testText);
    module_init(testText, testPage);
    module_set_text(testText, testTextBuffer);
    module_text_font(testText, RA8876_CHAR_HEIGHT_32);
    module_text_fit(testText);
    module_set_color(testText, MAINTEXTCOLOR, BACKCOLOR);
    module_align_below(testText, testText->parent);
    module_align_center(testText);

    // Creating Calibrate page
    Module *calibratePage = &(page->calibratePage);
    module_init(calibratePage, background);
    module_set_image(calibratePage, &(page->images->calibratePageImage));
    module_align_inner_top(calibratePage);
    module_align_space_even(calibratePage, 4, 4);
    module_touch_callback(calibratePage, check_buttons, PAGE_CALIBRATION);

    Module *calibrateText = &(page->calibrateText);
    module_init(calibrateText, calibratePage);
    module_set_text(calibrateText, calibrateTextBuffer);
    module_text_font(calibrateText, RA8876_CHAR_HEIGHT_32);
    module_text_fit(calibrateText);
    module_set_color(calibrateText, MAINTEXTCOLOR, BACKCOLOR);
    module_align_below(calibrateText, calibrateText->parent);
    module_align_center(calibrateText);

    // Creating Settings page
    Module *settingPage = &(page->settingPage);
    module_init(settingPage, background);
    // module_set_image(settingPage, page->images->calibratePageImage);
    module_set_rectangle_circle(settingPage, 200, 200);
    module_set_color(settingPage, SECONDARYCOLOR, BACKCOLOR);
    module_align_inner_bottom(settingPage);
    module_align_space_even(settingPage, 1, 4);
    module_touch_callback(settingPage, check_buttons, PAGE_SETTINGS);

    Module *settingText = &(page->settingText);
    module_init(settingText, settingPage);
    module_set_text(settingText, settingsTextBuffer);
    module_text_font(settingText, RA8876_CHAR_HEIGHT_32);
    module_text_fit(settingText);
    module_set_color(settingText, MAINTEXTCOLOR, BACKCOLOR);
    module_align_below(settingText, settingText->parent);
    module_align_center(settingText);

    // Creating Test profile page
    Module *tpPage = &(page->tpPage);
    module_init(tpPage, background);
    // module_set_image(settingPage, page->images->calibratePageImage);
    module_set_rectangle_circle(tpPage, 200, 200);
    module_set_color(tpPage, SECONDARYCOLOR, BACKCOLOR);
    module_align_inner_bottom(tpPage);
    module_align_space_even(tpPage, 2, 4);
    module_touch_callback(tpPage, check_buttons, PAGE_TEST_PROFILE);

    Module *tpText = &(page->tpText);
    module_init(tpText, tpPage);
    module_set_text(tpText, tpTextBuffer);
    module_text_font(tpText, RA8876_CHAR_HEIGHT_32);
    module_text_fit(tpText);
    module_set_color(tpText, MAINTEXTCOLOR, BACKCOLOR);
    module_align_below(tpText, tpText->parent);
    module_align_center(tpText);
}

/**
 * @brief Starts navigation p
 */
Page navigation_page_run(NavigationPage *page)
{
    printf("Starting navigation page\n");
    page->complete = false;

    module_draw(page->display, &(page->root));
    printf("done drawing page\n");
    while (!page->complete)
    {
        display_update_touch(page->display);
        module_touch_check(&(page->root), page->display, page);
    }
    return page->newPage;
}
