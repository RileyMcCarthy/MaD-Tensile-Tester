#include "AutomaticPage.h"
#include "Monitor.h"
#include "Explorer.h"

#define BUTTON_MOTION_MODE 0
#define BUTTON_CONDITION 1
#define BUTTON_NAVIGATION 2

static char runTitleBuffer[] = "Run Test Profile";
static char openTextBuffer[] = "Open";
static char runTextBuffer[] = "Run!";

// Private functions
static void button_run(int id, void *arg)
{
    AutomaticPage *page = (AutomaticPage *)arg;
    state_machine_set(page->machineState, PARAM_MOTION_MODE, MODE_TEST_RUNNING);
    // printf("New state: %d\n", page->state->motionParameters.mode);
}

static void button_nav(int id, void *arg)
{
    AutomaticPage *page = (AutomaticPage *)arg;
    page->complete = true;
}

static void button_open(int id, void *arg)
{
    AutomaticPage *page = (AutomaticPage *)arg;
    Explorer *explorer = explorer_create(page->display, 100, 100, EXPLORER_MODE_FILE, "/sd");
    explorer_run(explorer);
    printf("Filepath: %s\n", explorer->pathBuffer);
    if (strcmp(explorer->pathBuffer, "") == 0)
    {
        return;
    }
    printf("Filepath: %s\n", explorer->pathBuffer);
    json_to_motion_profile(explorer->pathBuffer, &(page->control->motionProfile));
    json_print_motion_profile(&(page->control->motionProfile));
    explorer_destroy(explorer);
}

static void update_profileName(Display *dis, Module *module, void *arg)
{
    AutomaticPage *page = (AutomaticPage *)arg;
    char buf[50];
    sprintf(buf, "Name: %s", page->control->motionProfile.name);

    if (strcmp(buf, page->profileNameBuffer) == 0)
        return;

    strcpy(page->profileNameBuffer, buf);
    module_text_update(module, page->profileNameBuffer);
    module_draw(page->display, module);
}

static void update_profileNumber(Display *dis, Module *module, void *arg)
{
    AutomaticPage *page = (AutomaticPage *)arg;
    char buf[50];
    sprintf(buf, "Number: %d", page->control->motionProfile.number);

    if (strcmp(buf, page->profileNumberBuffer) == 0)
        return;

    strcpy(page->profileNumberBuffer, buf);
    module_text_update(module, page->profileNumberBuffer);
    module_draw(page->display, module);
}

// Public Functions
void automatic_page_init(AutomaticPage *page, Display *display, Images *images, MachineState *state, ControlSystem *control)
{
    page->display = display;
    page->images = images;
    page->machineState = state;
    page->control = control;

    int padding = 20;

    // Create Background
    Module *root = &(page->root);
    module_init(root, NULL);

    Module *background = &(page->background);
    module_init(background, root);
    module_set_rectangle_circle(background, SCREEN_WIDTH, SCREEN_HEIGHT);
    module_set_position(background, 0, 0);
    module_set_padding(background, padding, padding);
    module_set_color(background, BACKCOLOR, BACKCOLOR);
    module_animate_draw(background, module_animation_switch_page_up);

    // Create edit window
    Module *runProfile = &(page->runProfile);
    module_init(runProfile, background);
    module_set_rectangle_circle(runProfile, SCREEN_WIDTH / 3, 0);
    module_fit_height(runProfile);
    module_set_padding(runProfile, padding, padding);
    module_set_margin(runProfile, 10, 10);
    module_set_color(runProfile, MAINCOLOR, BACKCOLOR);
    module_align_center(runProfile);
    module_align_inner_top(runProfile);

    // Create run title
    Module *runTitle = &(page->runTitle);
    module_init(runTitle, runProfile);
    module_set_margin(runTitle, 5, 5);
    module_set_text(runTitle, runTitleBuffer);
    module_text_font(runTitle, RA8876_CHAR_HEIGHT_32);
    module_text_fit(runTitle);
    module_text_underline(runTitle);
    module_set_color(runTitle, COLOR65K_BLACK, MAINCOLOR);
    module_align_inner_top(runTitle);
    module_align_center(runTitle);

    Module *selectedProfileNameText = &(page->selectedProfileNameText);
    module_init(selectedProfileNameText, runProfile);
    module_set_margin(selectedProfileNameText, 5, 5);
    module_set_text(selectedProfileNameText, page->profileNameBuffer);
    module_text_font(selectedProfileNameText, RA8876_CHAR_HEIGHT_24);
    module_text_fit(selectedProfileNameText);
    module_fit_width(selectedProfileNameText);
    module_text_align(selectedProfileNameText, MODULE_TEXT_ALIGN_INNER_CENTER);
    module_align_below(selectedProfileNameText, runTitle);
    module_align_center(selectedProfileNameText);
    module_set_color(selectedProfileNameText, COLOR65K_BLACK, selectedProfileNameText->parent->foregroundColor);
    module_update_callback(selectedProfileNameText, update_profileName);

    Module *selectedProfileNumberText = &(page->selectedProfileNumberText);
    module_init(selectedProfileNumberText, runProfile);
    module_set_text(selectedProfileNumberText, page->profileNumberBuffer);
    module_text_font(selectedProfileNumberText, RA8876_CHAR_HEIGHT_24);
    module_text_fit(selectedProfileNumberText);
    module_text_align(selectedProfileNumberText, MODULE_TEXT_ALIGN_INNER_CENTER);
    module_fit_width(selectedProfileNumberText);
    module_align_center(selectedProfileNumberText);
    module_align_below(selectedProfileNumberText, selectedProfileNameText);
    module_set_color(selectedProfileNumberText, COLOR65K_BLACK, selectedProfileNumberText->parent->foregroundColor);
    module_update_callback(selectedProfileNumberText, update_profileNumber);

    // Create Open Button
    Module *openButton = &(page->openButton);
    module_init(openButton, runProfile);
    module_set_rectangle_circle(openButton, 100, 50);
    module_set_color(openButton, COLOR65K_LIGHTGREEN, COLOR65K_LIGHTGREEN);
    module_set_padding(openButton, padding, padding);
    module_align_space_even(openButton, 1, 2);
    module_align_inner_bottom(openButton);
    module_touch_callback(openButton, button_open, 0);

    // Create Open Text
    Module *openText = &(page->openText);
    module_init(openText, openButton);
    module_set_text(openText, openTextBuffer);
    module_text_font(openText, RA8876_CHAR_HEIGHT_32);
    module_text_fit(openText);
    module_align_center(openText);
    module_align_middle(openText);
    module_set_color(openText, SECONDARYTEXTCOLOR, openText->parent->foregroundColor);

    // Create Run Button
    Module *runButton = &(page->runButton);
    module_init(runButton, runProfile);
    module_set_rectangle_circle(runButton, 100, 50);
    module_set_color(runButton, COLOR65K_LIGHTGREEN, COLOR65K_LIGHTGREEN);
    module_set_padding(runButton, padding, padding);
    module_align_space_even(runButton, 2, 2);
    module_align_inner_bottom(runButton);
    module_touch_callback(runButton, button_run, 0);

    // Create Run Text
    Module *runText = &(page->runText);
    module_init(runText, runButton);
    module_set_text(runText, runTextBuffer);
    module_text_font(runText, RA8876_CHAR_HEIGHT_32);
    module_text_fit(runText);
    module_align_center(runText);
    module_align_middle(runText);
    module_set_color(runText, SECONDARYTEXTCOLOR, runText->parent->foregroundColor);

    // Create Nav Button
    Module *navButton = &(page->navButton);
    module_init(navButton, background);
    module_set_rectangle_circle(navButton, 100, 100);
    module_set_color(navButton, COLOR65K_LIGHTGREEN, COLOR65K_LIGHTGREEN);
    module_set_padding(navButton, padding, padding);
    module_align_inner_right(navButton);
    module_align_inner_top(navButton);
    module_touch_callback(navButton, button_nav, 0);
}

void automatic_page_run(AutomaticPage *page)
{
    page->complete = false;
    module_draw(page->display, &(page->root));

    while (!page->complete)
    {
        module_touch_check(&(page->root), page->display, page);
        module_update_check(page->display, &(page->root), page);
        while (display_update_touch(page->display) == 0)
            ;
    }
}
