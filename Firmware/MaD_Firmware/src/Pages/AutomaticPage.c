#include "AutomaticPage.h"
#include "Monitor.h"
#include "Button.h"

#define BUTTON_MOTION_MODE 0
#define BUTTON_CONDITION 1
#define BUTTON_NAVIGATION 2

// Private Functions

static void button_run(int id, void *arg)
{
    AutomaticPage *page = (AutomaticPage *)arg;
    state_machine_set(page->state, PARAM_MOTION_MODE, MODE_TEST);
    state_machine_set(page->state, PARAM_MOTION_MODE, MODE_TEST_RUNNING);
    printf("New state: %d\n", page->state->motionParameters.mode);
}

static void button_nav(int id, void *arg)
{
    AutomaticPage *page = (AutomaticPage *)arg;
    page->complete = true;
}

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

AutomaticPage *automatic_page_create(Display *display, Images *images, MachineState *state)
{
    AutomaticPage *page = (AutomaticPage *)malloc(sizeof(AutomaticPage));
    page->state = state;
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
    int padding = 20;
    // Create Background
    Module *root = module_create(NULL);
    Module *background = module_create(root);
    module_set_rectangle_circle(background, SCREEN_WIDTH, SCREEN_HEIGHT);
    module_set_position(background, 0, 0);
    module_set_padding(background, padding, padding);
    module_set_color(background, BACKCOLOR, BACKCOLOR);

    // Create edit window
    Module *runProfile = module_create(background);
    module_set_rectangle_circle(runProfile, SCREEN_WIDTH / 3, 0);
    module_fit_height(runProfile);
    module_set_padding(runProfile, padding, padding);
    module_set_color(runProfile, MAINCOLOR, BACKCOLOR);
    module_align_center(runProfile);
    module_align_inner_top(runProfile);

    // Create Run Button
    Module *runButton = module_create(runProfile);
    module_set_rectangle_circle(runButton, 100, 50);
    module_set_color(runButton, COLOR65K_LIGHTGREEN, COLOR65K_LIGHTGREEN);
    module_set_padding(runButton, padding, padding);
    module_align_center(runButton);
    module_align_inner_bottom(runButton);
    module_touch_callback(runButton, button_run, 0);

    // Create Nav Button
    Module *navButton = module_create(background);
    module_set_rectangle_circle(navButton, 100, 100);
    module_set_color(navButton, COLOR65K_LIGHTGREEN, COLOR65K_LIGHTGREEN);
    module_set_padding(navButton, padding, padding);
    module_align_inner_right(navButton);
    module_align_inner_top(navButton);
    module_touch_callback(navButton, button_nav, 0);

    while (!page->complete)
    {
        module_draw(page->display, root);
        do
        {
            display_update_touch(page->display);
        } while (module_touch_check(root, page->display, page) == 0);
    }
}
