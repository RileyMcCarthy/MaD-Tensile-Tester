#include "AutomaticPage.h"
#include "Monitor.h"
#include "Button.h"
#include "Explorer.h"

#define BUTTON_MOTION_MODE 0
#define BUTTON_CONDITION 1
#define BUTTON_NAVIGATION 2

// Private Functions
static bool complete;
static void button_run(int id, void *arg)
{
    AutomaticPage *page = (AutomaticPage *)arg;
    // state_machine_set(page->state, PARAM_MOTION_MODE, MODE_TEST);
    state_machine_set(page->state, PARAM_MOTION_MODE, MODE_TEST_RUNNING);
    // printf("New state: %d\n", page->state->motionParameters.mode);
}

static void button_nav(int id, void *arg)
{
    complete = true;
}

static void button_open(int id, void *arg)
{
    AutomaticPage *page = (AutomaticPage *)arg;
    Explorer *explorer = explorer_create(page->display, 100, 100, EXPLORER_MODE_FILE, "/sd");
    char *filepath = explorer_run(explorer);
    if (filepath == NULL)
    {
        return;
    }
    // printf("Filepath: %s\n", filepath);
    json_to_motion_profile(filepath, &(page->control->motionProfile));
    json_print_motion_profile(&(page->control->motionProfile));
    explorer_destroy(explorer);
    free(filepath);
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

void automatic_page_init(AutomaticPage *page, Display *display, Images *images, MachineState *state, Control *control)
{
    page->state = state;
    page->display = display;
    page->images = images;
    page->state = state;
    page->control = control;
}

void automatic_page_destroy(AutomaticPage *page)
{
    free(page);
}

static void update_profileName(Display *display, Module *module, void *arg)
{
    // printf("Updating profile name\n");
    AutomaticPage *page = (AutomaticPage *)arg;

    char buf[50];
    sprintf(buf, "Name: %s", page->control->motionProfile.name);

    if (strcmp(buf, module_get_text(module)) == 0)
        return;
    // printf("past check\n");
    module_set_text(module, buf);
    module_draw(display, module);
}

static void update_profileNumber(Display *display, Module *module, void *arg)
{
    AutomaticPage *page = (AutomaticPage *)arg;

    char buf[50];
    sprintf(buf, "Number: %d", page->control->motionProfile.number);

    if (strcmp(buf, module_get_text(module)) == 0)
        return;

    module_set_text(module, buf);
    module_draw(display, module);
}

void automatic_page_run(AutomaticPage *page)
{
    complete = false;
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

    // Create run title
    Module *runTitle = module_create(runProfile);
    module_set_padding(runTitle, padding, padding);
    module_set_text(runTitle, "Run Test Profile");
    module_set_font(runTitle, RA8876_CHAR_HEIGHT_32);
    module_set_color(runTitle, COLOR65K_BLACK, MAINCOLOR);
    module_align_inner_top(runTitle);
    module_align_center(runTitle);
    module_add_underline(runTitle);

    Module *selectedProfileNameText = module_create(runProfile);
    module_set_text(selectedProfileNameText, "Name:");
    module_set_font(selectedProfileNameText, RA8876_CHAR_HEIGHT_24);
    module_text_max_char(selectedProfileNameText, 20);
    module_text_align(selectedProfileNameText, MODULE_TEXT_ALIGN_INNER_CENTER);
    module_align_below(selectedProfileNameText, runTitle);
    module_set_color(selectedProfileNameText, COLOR65K_BLACK, selectedProfileNameText->parent->foregroundColor);
    module_update_callback(selectedProfileNameText, update_profileName);

    Module *selectedProfileNumberText = module_create(runProfile);
    module_set_text(selectedProfileNumberText, "Number:");
    module_text_max_char(selectedProfileNumberText, 20);
    module_set_font(selectedProfileNumberText, RA8876_CHAR_HEIGHT_24);
    module_text_align(selectedProfileNumberText, MODULE_TEXT_ALIGN_INNER_CENTER);
    module_align_below(selectedProfileNumberText, selectedProfileNameText);
    module_set_color(selectedProfileNumberText, COLOR65K_BLACK, selectedProfileNumberText->parent->foregroundColor);
    module_update_callback(selectedProfileNumberText, update_profileNumber);

    // Create Open Button
    Module *openButton = module_create(runProfile);
    module_set_rectangle_circle(openButton, 100, 50);
    module_set_color(openButton, COLOR65K_LIGHTGREEN, COLOR65K_LIGHTGREEN);
    module_set_padding(openButton, padding, padding);
    module_align_space_even(openButton, 1, 2);
    module_align_inner_bottom(openButton);
    module_touch_callback(openButton, button_open, 0);

    // Create Open Text
    Module *openText = module_create(openButton);
    module_set_text(openText, "Open");
    module_set_font(openText, RA8876_CHAR_HEIGHT_32);
    module_align_center(openText);
    module_align_middle(openText);
    module_set_color(openText, SECONDARYTEXTCOLOR, openText->parent->foregroundColor);

    // Create Run Button
    Module *runButton = module_create(runProfile);
    module_set_rectangle_circle(runButton, 100, 50);
    module_set_color(runButton, COLOR65K_LIGHTGREEN, COLOR65K_LIGHTGREEN);
    module_set_padding(runButton, padding, padding);
    module_align_space_even(runButton, 2, 2);
    module_align_inner_bottom(runButton);
    module_touch_callback(runButton, button_run, 0);

    // Create Run Text
    Module *runText = module_create(runButton);
    module_set_text(runText, "Run!");
    module_set_font(runText, RA8876_CHAR_HEIGHT_32);
    module_align_center(runText);
    module_align_middle(runText);
    module_set_color(runText, SECONDARYTEXTCOLOR, runText->parent->foregroundColor);

    // Create Nav Button
    Module *navButton = module_create(background);
    module_set_rectangle_circle(navButton, 100, 100);
    module_set_color(navButton, COLOR65K_LIGHTGREEN, COLOR65K_LIGHTGREEN);
    module_set_padding(navButton, padding, padding);
    module_align_inner_right(navButton);
    module_align_inner_top(navButton);
    module_touch_callback(navButton, button_nav, 0);
    module_draw(page->display, root);
    while (!complete)
    {
        module_touch_check(root, page->display, page);
        module_update_check(page->display, root, page);
        // module_draw(page->display, root);
        do
        {
        } while (display_update_touch(page->display) == 0);
    }
    module_destroy(root);
}
