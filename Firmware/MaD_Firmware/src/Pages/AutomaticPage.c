#include "AutomaticPage.h"
#include "Monitor.h"
#include "Button.h"

#define BUTTON_MOTION_MODE 0
#define BUTTON_CONDITION 1
#define BUTTON_NAVIGATION 2

// Private Functions

static void load_graph(AutomaticPage *page)
{
    display_text_color(page->display, COLOR65K_WHITE, MAINCOLOR);
    display_canvas_image_start_address(page->display, PAGE4_START_ADDR);
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_16, RA8876_SELECT_8859_1);
    display_draw_square_fill(page->display, 492, 32, SCREEN_WIDTH - 32, 32 + 300, MAINCOLOR);
    display_draw_line(page->display, 492 + 5, 32 + 5, 492 + 5, 332 - 5, COLOR65K_WHITE);
    display_draw_line(page->display, 492 + 5, 182, SCREEN_WIDTH - 32 - 5, 182, COLOR65K_WHITE);
    display_draw_string(page->display, 492 + 10, 32 + 5, "Force(N)/Distance(Steps)");
    display_draw_string(page->display, SCREEN_WIDTH - 32 - strlen("Time(s)") * 8, 182 - 20, "Time(s)");
    display_canvas_image_start_address(page->display, PAGE1_START_ADDR);
}
static void draw_graph(AutomaticPage *page)
{
    display_bte_memory_copy(page->display, PAGE4_START_ADDR, SCREEN_WIDTH, 492, 32, PAGE1_START_ADDR, SCREEN_WIDTH, 492, 32, 500, 300);
}

// Public Functions

AutomaticPage *automatic_page_create(Display *display, Images *images)
{
    AutomaticPage *page = (AutomaticPage *)malloc(sizeof(AutomaticPage));
    page->display = display;
    page->complete = false;
    page->images = images;
    return page;
}

void automatic_page_destroy(AutomaticPage *page)
{
    free(page);
}

void automatic_page_run(AutomaticPage *page)
{

    // dyn4 = p_dyn4;
    // forceGauge = p_forceGauge;
    // sd = p_sd;

    // Init display with blue background
    display_canvas_image_start_address(page->display, PAGE1_START_ADDR);
    display_canvas_image_width(page->display, SCREEN_WIDTH);
    display_draw_square_fill(page->display, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BACKCOLOR);

    // clock.begin(&display);

    display_active_window_xy(page->display, 0, 0);
    display_active_window_wh(page->display, SCREEN_WIDTH, SCREEN_HEIGHT);
    display_draw_square_fill(page->display, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, COLOR65K_BLUE);
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_32, RA8876_SELECT_8859_1);
    display_set_text_parameter2(page->display, RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
    display_text_color(page->display, COLOR65K_WHITE, COLOR65K_BLUE);
    display_draw_string(page->display, SCREEN_WIDTH / 2 - 11 * 8, 300 - 16, "LOADING...");
    // display->loadImage("keyboard.bin", 3, 0, SCREEN_HEIGHT - 284, 1026, 284); //loading images adds another 10k bytes (maybe make own sd card system? or fopen)
    load_graph(page);

    display_draw_square_fill(page->display, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BACKCOLOR);
    // setup real time clock

    // Clock
    display_text_color(page->display, COLOR65K_WHITE, COLOR65K_BLUE);
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_32, RA8876_SELECT_8859_1);
    display_set_text_parameter2(page->display, RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
    char forceVal[100] = {10};
    display_draw_square_fill(page->display, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BACKCOLOR);
    // display->bteMemoryCopy(PAGE2_START_ADDR, SCREEN_WIDTH, (SCREEN_WIDTH - 500), 0, PAGE1_START_ADDR, SCREEN_WIDTH, (SCREEN_WIDTH - 500), 0, 500, 300);
    char temp[20];
    int samples = 0;
    int lastForce = 0;
    int lastPosition = 0;

    display_draw_square_fill(page->display, 32, 32, 32 + 428, SCREEN_HEIGHT - 32, MAINCOLOR);
    display_text_color(page->display, COLOR65K_WHITE, MAINCOLOR);
    int titlex = 246 - strlen(temp) * 8;
    int titley = 47;
    display_draw_string(page->display, titlex, titley, temp);
    display_draw_line(page->display, titlex, titley + 35, titlex + strlen(temp) * 16, titley + 35, COLOR65K_WHITE);

    titlex = 246 - strlen("Controls") * 8;
    titley = 47 + 240;
    display_draw_string(page->display, titlex, titley, "Controls");
    display_draw_line(page->display, titlex, titley + 35, titlex + strlen("Controls") * 16, titley + 35, COLOR65K_WHITE);

    display_draw_square_fill(page->display, 492, 364, SCREEN_WIDTH - 32, SCREEN_HEIGHT - 32, MAINCOLOR);
    titlex = 742 - strlen("Stats") * 8;
    titley = 375;
    display_text_color(page->display, COLOR65K_WHITE, MAINCOLOR);
    display_draw_string(page->display, titlex, titley, "Stats");
    display_draw_line(page->display, titlex, titley + 35, titlex + strlen("Stats") * 16, titley + 35, COLOR65K_WHITE);

    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_16, RA8876_SELECT_8859_1);
    display_draw_string(page->display, 55, 47 + 50, "Mode: Sinusoidal");
    display_draw_string(page->display, 55, 47 + 70, "Cycles: 20");
    display_draw_string(page->display, 55, 47 + 90, "Sample Rate: 800sps");

    int width = 50;
    int padding = 10;
    int color1 = COLOR65K_GRAYSCALE20;
    int color2 = COLOR65K_WHITE;
    Button keys[5];
    keys[0].xmin = 100;
    keys[0].xmax = keys[0].xmin + width;
    keys[0].ymin = 350;
    keys[0].ymax = keys[0].ymin + width;
    display_draw_square_fill(page->display, keys[0].xmin, keys[0].ymin, keys[0].xmax, keys[0].ymax, color1);
    display_draw_triangle_fill(page->display, keys[0].xmin + padding, keys[0].ymax - padding, keys[0].xmax - padding, keys[0].ymax - padding, keys[0].xmin + width / 2, keys[0].ymin + padding, color2);

    keys[1].xmin = keys[0].xmin;
    keys[1].xmax = keys[1].xmin + width;
    keys[1].ymin = keys[0].ymin + width * 2 + padding * 2;
    keys[1].ymax = keys[1].ymin + width;
    display_draw_square_fill(page->display, keys[1].xmin, keys[1].ymin, keys[1].xmax, keys[1].ymax, color1);
    display_draw_triangle_fill(page->display, keys[1].xmin + padding, keys[1].ymin + padding, keys[1].xmax - padding, keys[1].ymin + padding, keys[1].xmin + width / 2, keys[1].ymax - padding, color2);

    keys[2].xmin = keys[0].xmin;
    keys[2].xmax = keys[2].xmin + width;
    keys[2].ymin = keys[0].ymin + width + padding;
    keys[2].ymax = keys[2].ymin + width;
    display_draw_square_fill(page->display, keys[2].xmin, keys[2].ymin, keys[2].xmax, keys[2].ymax, color1);
    display_text_color(page->display, color2, color1);
    display_set_text_parameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_32, RA8876_SELECT_8859_1);
    display_draw_string(page->display, keys[2].xmin + 17, keys[2].ymin + 9, "H");

    draw_graph(page);
    float total = 0;
    int max = 0;
    int min = 0;
    // motionMailbox->dyn4->send_command(0x0c, 1000);
    while (1)
    {
        // navkey.updateStatus();
        // clock.render();
        display_text_color(page->display, COLOR65K_WHITE, MAINCOLOR);
        char pos[15];
        // sprintf(pos, "Position: %dmm   ", navkey.readCounterInt());

        display_draw_string(page->display, 160, 350, pos);
        // int tempForce = force.getForce();
        // int tempForce = motionMailbox->force;
        int tempForce = 4;
        int tempPosition = 23;
        if (tempForce != -1)
        {
            samples++;
            total += tempForce;
            if (tempForce > max)
            {
                max = tempForce;
            }
            else if (tempForce < min)
            {
                min = tempForce;
            }
            display_draw_square_fill(page->display, SCREEN_WIDTH - strlen(forceVal) * 8 - 8, 0, SCREEN_WIDTH, 35, COLOR65K_BLUE);
            sprintf(forceVal, "Force: %d.%d ", tempForce / 1000, tempForce - (tempForce / 1000));
            display_draw_string(page->display, 575, 420, forceVal);
            int avg = total / samples;
            sprintf(forceVal, "Average: %d.%d ", avg / 1000, avg - (avg / 1000));
            display_draw_string(page->display, 575, 450, forceVal);
            sprintf(forceVal, "Min: %d.%d ", min / 1000, min - (min / 1000));
            display_draw_string(page->display, 575, 480, forceVal);
            sprintf(forceVal, "Max: %d.%d ", max / 1000, max - (max / 1000));
            display_draw_string(page->display, 575, 510, forceVal);
            display_draw_line(page->display, 498 + samples - 1, 182 + lastForce / 34, 498 + samples, 182 + tempForce / 34, COLOR65K_GREEN);    // force is -5000 to 5000 with 300px resolution
            display_draw_line(page->display, 498 + samples - 1, 182 + lastPosition / 5, 498 + samples, 182 + tempPosition / 5, COLOR65K_BLUE); // force is -5000 to 5000 with 300px resolution
            if (samples >= 445)
            {
                draw_graph(page);
                samples = 0;
            }
            lastForce = tempForce;
            lastPosition = tempPosition;
        }
    }
}
