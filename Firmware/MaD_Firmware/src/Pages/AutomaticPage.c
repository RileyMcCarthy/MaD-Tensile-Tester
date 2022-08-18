#include "AutomaticPage.h"
#include "Monitor.h"
#include "Explorer.h"

#define BUTTON_MOTION_MODE 0
#define BUTTON_CONDITION 1
#define BUTTON_NAVIGATION 2

static char runTitleBuffer[] = "Run Test Profile";
static char openTextBuffer[] = "Open";
static char runTextBuffer[] = "Run!";
static char saveBuffer[] = "Save";

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

static void button_save(int id, void *arg)
{
    AutomaticPage *page = (AutomaticPage *)arg;
    printf("Saving profile\n");

    FILE *file = fopen("/sd/raw.txt", "w");
    if (file == NULL)
    {
        printf("Error opening file:%s\n", page->profileNameBuffer);
        return;
    }

    MonitorData *data = monitor_read_data(0); // Read flash from address 0
    while (data->timems != -1)
    {
        fprintf(file, "%d,%d,%d,%f,%f\n", data->timems, data->forceRaw, data->encoderRaw, data->force, data->position);
        // printf("%d,%d,%d,%f,%f\n", data->timems, data->forceRaw, data->encoderRaw, data->force, data->position);
        monitor_read_data(-1); // Read next address in flash
    }
    fclose(file);
    printf("File saved\n");
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
void automatic_page_init(AutomaticPage *page, Display *display, Images *images, MachineState *state, ControlSystem *control, MonitorData *data)
{
    page->display = display;
    page->images = images;
    page->machineState = state;
    page->control = control;
    page->data = data;

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
    module_redraw_callback(selectedProfileNameText, update_profileName);

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
    module_redraw_callback(selectedProfileNumberText, update_profileNumber);

    // Create Open Button
    Module *openButton = &(page->openButton);
    module_init(openButton, runProfile);
    module_set_rectangle_circle(openButton, 100, 50);
    module_set_color(openButton, COLOR65K_LIGHTGREEN, COLOR65K_LIGHTGREEN);
    module_set_padding(openButton, padding, padding);
    module_align_space_even(openButton, 1, 3);
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
    module_align_space_even(runButton, 2, 3);
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

    // Create Off Button
    Module *saveButton = &(page->saveButton);
    module_init(saveButton, runProfile);
    module_set_text(saveButton, saveBuffer);
    module_set_color(saveButton, MAINTEXTCOLOR, COLOR65K_LIGHTGREEN);
    module_set_margin(saveButton, padding, padding);
    module_text_align(saveButton, MODULE_TEXT_ALIGN_INNER_CENTER);
    module_text_align_verticle(saveButton, MODULE_TEXT_ALIGN_VCENTER);
    module_add_border(saveButton, COLOR65K_BLACK, 3);
    module_fit_space_even(saveButton, 3);
    module_align_inner_bottom(saveButton);
    module_align_space_even(saveButton, 3, 3);
    module_touch_callback(saveButton, button_save, FUNC_MANUAL_OFF);

    // Create navigation button
    Module *navigationButton = &(page->navigationButton);
    module_init(navigationButton, background);
    module_set_image(navigationButton, &(page->images->navigationImage));
    module_align_inner_top(navigationButton);
    module_align_inner_right(navigationButton);
    module_touch_callback(navigationButton, button_nav, 0);

    Module *machineInfoWindow = &(page->machineInfoWindow);
    module_init(machineInfoWindow, background);
    module_set_padding(machineInfoWindow, padding, padding);
    module_set_margin(machineInfoWindow, 10, 10);
    module_set_rectangle_circle(machineInfoWindow, 0, 0);
    module_set_padding(machineInfoWindow, padding, padding);
    module_fit_space_even(machineInfoWindow, 3);
    module_fit_height(machineInfoWindow);
    module_set_color(machineInfoWindow, MAINCOLOR, BACKCOLOR);
    module_align_inner_left(machineInfoWindow);
    module_align_inner_top(machineInfoWindow);

    machine_view_init(&(page->machineView), machineInfoWindow, page->data);
}

void automatic_page_run(AutomaticPage *page)
{
    page->complete = false;
    module_draw(page->display, &(page->root));

    while (!page->complete)
    {
        display_update_touch(page->display);
        module_touch_check(&(page->root), page->display, page);
        module_redraw(page->display, &(page->root), page);
    }
}
