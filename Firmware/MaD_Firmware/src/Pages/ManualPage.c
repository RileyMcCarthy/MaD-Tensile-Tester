#include "ManualPage.h"
#include "Images.h"

#define BUTTONCOUNT 3

#define BUTTON_MOTION_MODE 0
#define BUTTON_CONDITION 1
#define BUTTON_NAVIGATION 2

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
    display_update_buttons(page->display, page->buttons, BUTTONCOUNT);
    for (int i = 0; i < BUTTONCOUNT; i++)
    {
        if (page->buttons[i].pressed)
        {
            switch (page->buttons[i].name)
            {
            case BUTTON_MOTION_MODE:
                /* code */
                break;
            case BUTTON_CONDITION:
                /* code */
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

ManualPage *manual_page_create(Display *display, MachineState *machineState, Images *images)
{
    ManualPage *page = (ManualPage *)malloc(sizeof(ManualPage));
    page->display = display;
    page->complete = false;
    page->machineState = machineState;
    page->images = images;
    return page;
}

void manual_page_destroy(ManualPage *page)
{
    free(page->buttons);
    free(page);
}

/**
 * @brief Starts manual page
 *
 */
void manual_page_run(ManualPage *page)
{
    printf("Manual page running\n");
    display_draw_square_fill(page->display, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BACKCOLOR);
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_32, RA8876_SELECT_8859_1);
    display_set_text_parameter2(page->display, RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X2, RA8876_TEXT_HEIGHT_ENLARGEMENT_X2);
    display_text_color(page->display, MAINTEXTCOLOR, BACKCOLOR);

    char buf[50];
    strcpy(buf, "Manual");
    int titlex = SCREEN_WIDTH / 2 - strlen(buf) * 16;
    int titley = 10;
    display_draw_string(page->display, titlex, titley, buf);

    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_32, RA8876_SELECT_8859_1);
    display_set_text_parameter2(page->display, RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);

    display_text_color(page->display, MAINTEXTCOLOR, BACKCOLOR);
    strcpy(buf, "Information");
    titley = 120;
    titlex = SCREEN_WIDTH / 6 - strlen(buf) * 8;
    display_draw_string(page->display, titlex, titley, buf);
    display_draw_line(page->display, titlex, titley + 30, titlex + strlen(buf) * 16, titley + 30, MAINTEXTCOLOR);

    display_text_color(page->display, MAINTEXTCOLOR, BACKCOLOR);
    strcpy(buf, "Control");
    titley = 300;
    titlex = SCREEN_WIDTH / 6 - strlen(buf) * 8;
    display_draw_string(page->display, titlex, titley, buf);
    display_draw_line(page->display, titlex, titley + 30, titlex + strlen(buf) * 16, titley + 30, MAINTEXTCOLOR);

    printf("creating buttons\n");
    Button *buttons = (Button *)malloc(sizeof(Button) * BUTTONCOUNT);
    page->buttons = buttons;
    buttons[0].name = BUTTON_MOTION_MODE;
    buttons[0].xmin = SCREEN_WIDTH / 6 - 110;
    buttons[0].xmax = buttons[0].xmin + 100;
    buttons[0].ymin = 350;
    buttons[0].ymax = buttons[0].ymin + 50;
    buttons[0].pressed = false;
    buttons[0].debounceTimems = 200;
    buttons[0].lastPress = 0;

    buttons[1].name = BUTTON_CONDITION;
    buttons[1].xmin = SCREEN_WIDTH / 6 + 10;
    buttons[1].xmax = buttons[1].xmin + 100;
    buttons[1].ymin = 350;
    buttons[1].ymax = buttons[1].ymin + 50;
    buttons[1].pressed = false;
    buttons[1].debounceTimems = 200;
    buttons[1].lastPress = 0;

    buttons[2].name = BUTTON_NAVIGATION;
    buttons[2].xmin = SCREEN_WIDTH - 100;
    buttons[2].xmax = buttons[2].xmin + 100;
    buttons[2].ymin = 0;
    buttons[2].ymax = buttons[2].ymin + 100;
    buttons[2].pressed = false;
    buttons[2].debounceTimems = 200;
    buttons[2].lastPress = 0;

    display_draw_square_fill(page->display, buttons[0].xmin, buttons[0].ymin, buttons[0].xmax, buttons[0].ymax, COLOR65K_GREEN);
    display_draw_square_fill(page->display, buttons[1].xmin, buttons[1].ymin, buttons[1].xmax, buttons[1].ymax, COLOR65K_RED);
    Image *navigationImg = page->images->navigationImage;
    display_bte_memory_copy_image(page->display, navigationImg, SCREEN_WIDTH - navigationImg->width - 5, 5);
    bool initialRender = true;
    printf("Manual page finished loading\n");
    MachineState previousState = *(page->machineState);
    while (!page->complete)
    {
        MachineState currentState = *(page->machineState);
        checkButtons(page);

        display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_16, RA8876_SELECT_8859_1);
        display_set_text_parameter2(page->display, RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
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

            display_draw_square_fill(page->display, buttons[0].xmin, buttons[0].ymin, buttons[0].xmax, buttons[0].ymax, bordercolor);
            display_draw_square_fill(page->display, buttons[0].xmin + 5, buttons[0].ymin + 5, buttons[0].xmax - 5, buttons[0].ymax - 5, backcolor);
            display_text_color(page->display, textcolor, backcolor);
            display_draw_string(page->display, buttons[0].xmin + 50 - strlen(buf) * 4, buttons[0].ymin + 25 - 8, buf);
        }
        display_text_color(page->display, MAINTEXTCOLOR, BACKCOLOR);
        // clock.render();
        initialRender = false;
        previousState = currentState;
    }
}
