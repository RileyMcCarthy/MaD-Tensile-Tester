#include "NavigationPage.h"
#include "Images.h"
#include "StatusPage.h"
#include "ManualPage.h"
#include "Module.h"
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
    page->complete = false;
    page->images = images;
}

void navigation_page_destroy(NavigationPage *page)
{
    free(page->buttons);
    free(page);
}

/**
 * @brief Starts navigation page
 *
 */
Page navigation_page_run(NavigationPage *page)
{
    printf("Starting navigation page\n");
    page->complete = false;

    int padding = 20;
    // Create Background
    Module *root = module_create(NULL);
    Module *background = module_create(root);
    module_set_rectangle(background, SCREEN_WIDTH, SCREEN_HEIGHT);
    module_set_position(background, 0, 0);
    module_set_padding(background, padding, padding);
    module_set_color(background, BACKCOLOR, BACKCOLOR);

    // Creating status page
    Module *statusPage = module_create(background);
    module_set_image(statusPage, page->images->statusPageImage);
    module_align_inner_top(statusPage);
    module_align_space_even(statusPage, 1, 4);
    module_touch_callback(statusPage, check_buttons, PAGE_STATUS);

    Module *statusText = module_create(statusPage);
    module_set_text(statusText, "Status");
    module_set_font(statusText, RA8876_CHAR_HEIGHT_32);
    module_set_color(statusText, COLOR65K_BLACK, BACKCOLOR);
    module_align_below(statusText, statusText->parent);
    module_align_center(statusText);

    // Creating manual page
    Module *manualPage = module_create(background);
    module_set_image(manualPage, page->images->manualPageImage);
    module_align_inner_top(manualPage);
    module_align_space_even(manualPage, 2, 4);
    module_touch_callback(manualPage, check_buttons, PAGE_MANUAL);

    Module *manualText = module_create(manualPage);
    module_set_text(manualText, "Manual");
    module_set_font(manualText, RA8876_CHAR_HEIGHT_32);
    module_set_color(manualText, COLOR65K_BLACK, BACKCOLOR);
    module_align_below(manualText, manualText->parent);
    module_align_center(manualText);

    // Creating Test page
    Module *testPage = module_create(background);
    module_set_image(testPage, page->images->automaticPageImage);
    module_align_inner_top(testPage);
    module_align_space_even(testPage, 3, 4);
    module_touch_callback(testPage, check_buttons, PAGE_AUTOMATIC);

    Module *testText = module_create(testPage);
    module_set_text(testText, "Test");
    module_set_font(testText, RA8876_CHAR_HEIGHT_32);
    module_set_color(testText, COLOR65K_BLACK, BACKCOLOR);
    module_align_below(testText, testText->parent);
    module_align_center(testText);

    // Creating Calibrate page
    Module *calibratePage = module_create(background);
    module_set_image(calibratePage, page->images->calibratePageImage);
    module_align_inner_top(calibratePage);
    module_align_space_even(calibratePage, 4, 4);
    module_touch_callback(calibratePage, check_buttons, PAGE_CALIBRATION);

    Module *calibrateText = module_create(calibratePage);
    module_set_text(calibrateText, "Calibrate");
    module_set_font(calibrateText, RA8876_CHAR_HEIGHT_32);
    module_set_color(calibrateText, COLOR65K_BLACK, BACKCOLOR);
    module_align_below(calibrateText, calibrateText->parent);
    module_align_center(calibrateText);

    // Creating Settings page
    Module *settingPage = module_create(background);
    // module_set_image(settingPage, page->images->calibratePageImage);
    module_set_rectangle_circle(settingPage, 200, 200);
    module_set_color(settingPage, COLOR65K_RED, BACKCOLOR);
    module_align_below(settingPage, statusText);
    module_align_space_even(settingPage, 1, 4);
    module_touch_callback(settingPage, check_buttons, PAGE_SETTINGS);

    Module *settingText = module_create(settingPage);
    module_set_text(settingText, "Settings");
    module_set_font(settingText, RA8876_CHAR_HEIGHT_32);
    module_set_color(settingText, COLOR65K_BLACK, BACKCOLOR);
    module_align_below(settingText, settingText->parent);
    module_align_center(settingText);

    // Creating Test profile page
    Module *tpPage = module_create(background);
    // module_set_image(settingPage, page->images->calibratePageImage);
    module_set_rectangle_circle(tpPage, 200, 200);
    module_set_color(tpPage, COLOR65K_RED, BACKCOLOR);
    module_align_below(tpPage, statusText);
    module_align_space_even(tpPage, 2, 4);
    module_touch_callback(tpPage, check_buttons, PAGE_TEST_PROFILE);

    Module *tpText = module_create(tpPage);
    module_set_text(tpText, "Create Profiles");
    module_set_font(tpText, RA8876_CHAR_HEIGHT_32);
    module_set_color(tpText, COLOR65K_BLACK, BACKCOLOR);
    module_align_below(tpText, tpText->parent);
    module_align_center(tpText);

    module_draw(page->display, root);
    while (!page->complete)
    {
        display_update_touch(page->display);
        module_touch_check(root, page->display, page);
    }
    module_destroy(root);
    return page->newPage;
}
