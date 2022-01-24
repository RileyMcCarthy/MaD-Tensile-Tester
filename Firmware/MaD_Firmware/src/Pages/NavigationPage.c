#include "NavigationPage.h"
#include "Images.h"
#include "StatusPage.h"
#include "ManualPage.h"
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
static void check_buttons(NavigationPage *page)
{
    display_update_touch(page->display);

    if (display_update_buttons(page->display, page->buttons, BUTTONCOUNT) > 0)
    {
        for (int i = 0; i < BUTTONCOUNT; i++)
        {
            if (page->buttons[i].pressed)
            {
                page->complete = true;
                page->newPage = (Pages)page->buttons[i].name;
            }
        }
    }
}

NavigationPage *navigation_page_create(Display *display, Images *images)
{
    NavigationPage *page = malloc(sizeof(NavigationPage));
    page->display = display;
    page->complete = false;
    page->images = images;
    return page;
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
Pages navigation_page_run(NavigationPage *page)
{
    printf("Starting navigation page\n");
    page->complete = false;

    display_draw_square_fill(page->display, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BACKCOLOR);
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_32, RA8876_SELECT_8859_1);
    display_set_text_parameter2(page->display, RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X2, RA8876_TEXT_HEIGHT_ENLARGEMENT_X2);
    display_text_color(page->display, MAINTEXTCOLOR, BACKCOLOR);

    int buttonSize = 200;

    char buf[50];
    strcpy(buf, "Navigation Menu");
    int titlex = SCREEN_WIDTH / 2 - strlen(buf) * 16;
    int titley = 30;
    display_draw_string(page->display, titlex, titley, buf);

    printf("creating buttons\n");
    Button *buttons = (Button *)malloc(sizeof(Button) * BUTTONCOUNT);
    page->buttons = buttons;

    buttons[0].name = PAGE_STATUS;
    buttons[0].xmin = SCREEN_WIDTH / 6 - buttonSize / 2;
    buttons[0].xmax = buttons[0].xmin + buttonSize;
    buttons[0].ymin = 120;
    buttons[0].ymax = buttons[0].ymin + buttonSize;
    buttons[0].pressed = false;
    buttons[0].debounceTimems = 100;
    buttons[0].lastPress = 0;

    buttons[1].name = PAGE_MANUAL;
    buttons[1].xmin = buttons[0].xmax + 30;
    buttons[1].xmax = buttons[1].xmin + buttonSize;
    buttons[1].ymin = 120;
    buttons[1].ymax = buttons[1].ymin + buttonSize;
    buttons[1].pressed = false;
    buttons[1].debounceTimems = 100;
    buttons[1].lastPress = 0;

    buttons[2].name = PAGE_AUTOMATIC;
    buttons[2].xmin = buttons[1].xmax + 30;
    buttons[2].xmax = buttons[2].xmin + buttonSize;
    buttons[2].ymin = 120;
    buttons[2].ymax = buttons[2].ymin + buttonSize;
    buttons[2].pressed = false;
    buttons[2].debounceTimems = 100;
    buttons[2].lastPress = 0;

    buttons[3].name = PAGE_CALIBRATION;
    buttons[3].xmin = buttons[2].xmax + 30;
    buttons[3].xmax = buttons[3].xmin + buttonSize;
    buttons[3].ymin = 120;
    buttons[3].ymax = buttons[2].ymin + buttonSize;
    buttons[3].pressed = false;
    buttons[3].debounceTimems = 100;
    buttons[3].lastPress = 0;

    buttons[4].name = PAGE_SETTINGS;
    buttons[4].xmin = buttons[0].xmin;
    buttons[4].xmax = buttons[4].xmin + buttonSize;
    buttons[4].ymin = buttons[0].ymax + 30;
    buttons[4].ymax = buttons[4].ymin + buttonSize;
    buttons[4].pressed = false;
    buttons[4].debounceTimems = 100;
    buttons[4].lastPress = 0;

    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_32, RA8876_SELECT_8859_1);
    display_set_text_parameter2(page->display, RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);

    Image *statusImage = page->images->statusPageImage;

    display_bte_memory_copy_image(page->display, statusImage, buttons[0].xmin, buttons[0].ymin);

    Image *manualImg = page->images->manualPageImage;

    display_bte_memory_copy_image(page->display, manualImg, buttons[1].xmin, buttons[1].ymin);

    Image *automaticImg = page->images->automaticPageImage;

    display_bte_memory_copy_image(page->display, automaticImg, buttons[2].xmin, buttons[2].ymin);

    Image *calibrateImg = page->images->calibratePageImage;

    display_bte_memory_copy_image(page->display, calibrateImg, buttons[3].xmin, buttons[3].ymin);

    display_draw_circle_square_fill(page->display, buttons[4].xmin, buttons[4].ymin, buttons[4].xmax, buttons[4].ymax, COLOR65K_BLACK);
    display_draw_string(page->display, buttons[4].xmin + 10, buttons[4].ymin + 10, "Settings");

    while (!page->complete)
    {
        check_buttons(page);
    }
    return page->newPage;
}
