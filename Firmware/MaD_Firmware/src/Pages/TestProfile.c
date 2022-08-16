#include "TestProfile.h"
#include "Explorer.h"

#define BUTTON_NAVIGATION 0
#define BUTTON_NEW 1
#define BUTTON_OPEN 2

#define BUTTON_QUARTET_COUNT 3
#define BUTTON_QUARTET_NAME 0
#define BUTTON_QUARTET_FUNC 1
#define BUTTON_QUARTET_DWELL 2

#define BUTTON_SET_COUNT 3
#define BUTTON_SET_NAME 0
#define BUTTON_SET_NUMBER 1
#define BUTTON_SET_TYPE 2
#define BUTTON_SET_EXECUTIONS 3
#define BUTTON_SET_QUARTETS 4

#define BUTTON_PROFILE_COUNT 3
#define BUTTON_PROFILE_NAME 0
#define BUTTON_PROFILE_NUMBER 1
#define BUTTON_PROFILE_SETS 2

#define BUTTON_SAMMPMLE_COUNT 3
#define BUTTON_SAMPLE_NAME 0
#define BUTTON_SAMPLE_NUMBER 1
#define BUTTON_SAMPLE_LENGTH 2
#define BUTTON_SAMPLE_STRETCH 3
#define BUTTON_SAMPLE_MAX_VELOCITY 4
#define BUTTON_SAMPLE_MAX_ACCELERATION 5
#define BUTTON_SAMPLE_MAX_JERK 6
#define BUTTON_SAMPLE_MAX_FORCE_TENSILE 7
#define BUTTON_SAMPLE_MAX_FORCE_COMPRESSION 8

#define PROFILE_TYPES 5
#define PROFILE_QUARTET 0
#define PROFILE_SET 1
#define PROFILE_MOTION 2
#define PROFILE_SAMPLE 3
#define PROFILE_TEST 4

static const char openTextBuffer[] = "Open";
static const char newTextBuffer[] = "New";
static const char saveTextBuffer[] = "Save";

static const char *optionNames[] = {"Quartet", "Set", "Motion", "Sample", "Test"};
static const char *extension[] = {".QRT", ".SET", ".MOT", ".SMP", ".TST"};

static bool complete;

static void button_navigation(int id, void *arg)
{
    TestProfilePage *page = (TestProfilePage *)arg;
    complete = true;
}

static void create_filepath(char *dest, const char *path, const char *filename, const char *extension)
{
    strcpy(dest, path);
    strcat(dest, "/");
    strcat(dest, filename);
    strcat(dest, extension);
}

static void button_open(int id, void *arg)
{
    TestProfilePage *page = (TestProfilePage *)arg;

    Explorer *explorer = explorer_create(page->display, 100, 100, EXPLORER_MODE_FILE, "/sd");
    char *filepath = explorer_run(explorer);
    if (strcmp(filepath, "") == 0)
    {
        return;
    }

    for (int i = 0; i < PROFILE_TYPES; i++)
    {
        char *ext = filepath;
        ext += strlen(filepath) - strlen(extension[i]);
        serial_debug("%s\n", ext);
        if (strcmp(ext, extension[i]) == 0)
        {
            serial_debug("Found mode:%d\n", i);
            page->mode = i;
            break;
        }
    }
    serial_debug("Mode:%d\n", page->mode);

    switch (page->mode)
    {
    case PROFILE_QUARTET:
    {
        serial_debug("Quartet from json\n");
        json_to_motion_quartet(filepath, &(page->quartet));
        break;
    }
    case PROFILE_SET:
    {
        json_to_motion_set(filepath, &(page->set));
        break;
    }
    case PROFILE_MOTION:
    {
        json_to_motion_profile(filepath, &(page->profile));
        break;
    }
    case PROFILE_SAMPLE:
    {
        //        json_to_sample_profile(filepath, &(page->sample));
        break;
    }
    case PROFILE_TEST:
    {
        //        json_to_test_profile(filepath, &(page->test));
        break;
    }
    }
}

static void button_new(int id, void *arg)
{
    TestProfilePage *page = (TestProfilePage *)arg;

    selection_init(&(page->selection), page->display);
    for (int i = 0; i < PROFILE_TYPES; i++)
    {
        selection_add_option(&(page->selection), optionNames[i]);
    }

    int newmode = selection_run(&(page->selection));

    Keyboard *keyboard = keyboard_create(page->display, page->images);
    if (keyboard == NULL)
    {
        serial_debug("TestProfile Button new Keyboard could not allocate memory\n");
        return;
    }
    char *filename = keyboard_get_input(keyboard, "Enter file name: ");

    if (strcmp(filename, "") == 0)
    {
        return;
    }

    Explorer *explorer = explorer_create(page->display, 100, 100, EXPLORER_MODE_DIRECTORY, "/sd");
    if (explorer == NULL)
    {
        serial_debug("Testprofile.c explorer could not allocate memory\n");
        return;
    }

    char *newpath = explorer_run(explorer);

    if (strcmp(newpath, "") == 0)
    {
        return;
    }

    page->mode = newmode;
    switch (page->mode)
    {
    case PROFILE_QUARTET:
    {
        motion_quartet_init(&(page->quartet));
        create_filepath(page->quartet.name, newpath, filename, extension[PROFILE_QUARTET]);
        break;
    }
    case PROFILE_SET:
    {
        motion_set_init(&(page->set));
        create_filepath(page->set.name, newpath, filename, extension[PROFILE_SET]);
        break;
    }
    case PROFILE_SAMPLE:
    {
        sample_profile_init(&(page->sample));
        create_filepath(page->sample.name, newpath, filename, extension[PROFILE_SAMPLE]);
        break;
    }
    case PROFILE_MOTION:
    {
        motion_profile_init(&(page->profile));
        create_filepath(page->profile.name, newpath, filename, extension[PROFILE_MOTION]);
        break;
    }
    }
}

static void button_simulate(int id, void *arg)
{
    TestProfilePage *page = (TestProfilePage *)arg;
    serial_debug("Simulate mode: %d\n", page->mode);

    RunMotionProfile *run = &(page->runProfile);
    bool *profileComplete = NULL;
    double (*getPosition)(double t, RunMotionProfile *run, MotionQuartet *quartet) = NULL;
    void *profile = NULL;

    switch (page->mode)
    {
    case PROFILE_QUARTET:
    {
        profileComplete = &(run->quartetComplete);
        getPosition = position_quartet;
        profile = &(page->quartet);
        // motion_quartet_simulate(&(page->quartet));
        break;
    }
    case PROFILE_SET:
    {
        // motion_set_simulate(&(page->set));
        break;
    }
    case PROFILE_MOTION:
    {
        // motion_profile_simulate(&(page->profile));
        break;
    }
    case PROFILE_TEST:
    {
        // test_profile_simulate(&(page->test));
        break;
    }
    }

    // can make this modular for all types by using pointers for functions and flags
    run_motion_profile_init(run); // Create new RunMotionProfile structure
    double t = 0;
    int max = getPosition(0, run, profile);
    int min = getPosition(0, run, profile);
    while (!(*profileComplete))
    {
        double position = getPosition(t, run, profile);
        if (position > max)
        {
            max = position;
        }
        if (position < min)
        {
            min = position;
        }
        t += 0.001;
    }
    serial_debug("Max: %d Min: %d\n", max, min);
    int maxData = module_graph_get_max_data(&(page->graphPositionContainer));
    serial_debug("MaxData: %d\n", maxData);
    double dt = t / (double)(maxData);
    module_graph_set_range(&(page->graphPositionContainer), max, -1 * max);

    t = 0;
    run_motion_profile_init(run); // Create new RunMotionProfile structure
    while (!(*profileComplete))
    {
        double position = getPosition(t, run, profile);
        serial_debug("Position, Time: %f,%f\n", position, t);
        page->graphValue = position;
        module_redraw(page->display, &(page->graphPositionContainer), &position);
        t += dt;
    }
}

static void button_save(int id, void *arg)
{
    TestProfilePage *page = (TestProfilePage *)arg;
    switch (page->mode)
    {
    case PROFILE_QUARTET:
    {
        serial_debug("Saving quartet\n");
        motion_quartet_to_json(&(page->quartet), page->quartet.name);
        break;
    }
    case PROFILE_SET:
    {
        serial_debug("Saving set\n");
        motion_set_to_json(&(page->set), page->set.name);
        break;
    }
    case PROFILE_MOTION:
    {
        serial_debug("Saving motion\n");
        motion_profile_to_json(&(page->profile), page->profile.name);
        break;
    }
    case PROFILE_TEST:
    {
        serial_debug("Saving test\n");
        test_profile_to_json(&(page->test), page->test.name);
        break;
    }
    }
    // button_simulate(id, arg);
}
static void update_filename(char *filepath, const char *newName, const char *extension)
{
    char *name = strrchr(filepath, '/') + 1;
    strcpy(name, newName);
    strcat(name, extension);
}

static void button_quartet(int id, void *arg)
{
    serial_debug("BUTTON_QUARTET_id:%d\n", id);
    TestProfilePage *page = (TestProfilePage *)arg;
    switch (id)
    {
    case BUTTON_QUARTET_NAME:
    {
        Keyboard *keyboard = keyboard_create(page->display, page->images);
        char *filename = keyboard_get_input(keyboard, "Enter file name: ");
        if (strcmp(filename, "") == 0)
        {
            return;
        }

        update_filename(page->quartet.name, filename, extension[PROFILE_QUARTET]);
        break;
    }
    case BUTTON_QUARTET_FUNC:
    {
        selection_init(&(page->selection), page->display);
        for (int i = 0; i < FUNCTION_COUNT; i++)
        {
            get_function_info(&(page->info), i);
            serial_debug("function:%d,name:%s\n", i, page->info.name);
            selection_add_option(&(page->selection), page->info.name);
        }
        page->quartet.function = selection_run(&(page->selection));
        break;
    }
    case BUTTON_QUARTET_DWELL:
    {
        Keyboard *keyboard = keyboard_create(page->display, page->images);
        char *dwell = keyboard_get_input(keyboard, "Dwell: ");
        page->quartet.dwell = atof(dwell);
        break;
    }
    default:
        break;
    }
}

static void button_quartet_parameters(int id, void *arg)
{
    TestProfilePage *page = (TestProfilePage *)arg;

    get_function_info(&(page->info), page->quartet.function);
    Keyboard *keyboard = keyboard_create(page->display, page->images);
    char prompt[30];
    strcpy(prompt, page->info.args[id]);
    strcat(prompt, ": ");
    char *param = keyboard_get_input(keyboard, prompt);
    page->quartet.parameters[id] = atof(param);
}

static void button_set(int id, void *arg)
{
    TestProfilePage *page = (TestProfilePage *)arg;
    switch (id)
    {
    case BUTTON_SET_NAME:
    {
        Keyboard *keyboard = keyboard_create(page->display, page->images);
        char *filename = keyboard_get_input(keyboard, "Enter file name: ");
        if (strcmp(filename, "") == 0)
        {
            return;
        }

        update_filename(page->set.name, filename, extension[PROFILE_SET]);
        break;
    }
    case BUTTON_SET_NUMBER:
    {
        Keyboard *keyboard = keyboard_create(page->display, page->images);
        char *number = keyboard_get_input(keyboard, "Number: ");
        page->set.number = atoi(number);
        break;
    }
    case BUTTON_SET_TYPE:
    {
        Keyboard *keyboard = keyboard_create(page->display, page->images);
        char *type = keyboard_get_input(keyboard, "Enter file name: ");
        if (strcmp(type, "") == 0)
        {
            return;
        }
        strncpy(page->set.type, type, MAX_MOTION_PROFILE_SET_TYPE);
        break;
    }
    case BUTTON_SET_EXECUTIONS:
    {
        Keyboard *keyboard = keyboard_create(page->display, page->images);
        char *executions = keyboard_get_input(keyboard, "Executions: ");
        page->set.executions = atoi(executions);
        break;
    }
    }
}

void button_set_add(int id, void *arg)
{
    TestProfilePage *page = (TestProfilePage *)arg;

    Explorer *explorer = explorer_create(page->display, 100, 100, EXPLORER_MODE_FILE, "/sd");
    char *filepath = explorer_run(explorer);
    if (strcmp(filepath, "") == 0)
    {
        return;
    }

    json_to_motion_quartet(filepath, &(page->set.quartets[page->set.quartetCount]));
    page->set.quartetCount++;
}

void button_set_remove(int id, void *arg)
{
    TestProfilePage *page = (TestProfilePage *)arg;

    memmove(&(page->set.quartets[id]), &(page->set.quartets[id + 1]), sizeof(MotionQuartet) * (page->set.quartetCount - id - 1));
    page->set.quartetCount--;
}

static void button_profile(int id, void *arg)
{
    TestProfilePage *page = (TestProfilePage *)arg;
    switch (id)
    {
    case BUTTON_PROFILE_NAME:
    {
        Keyboard *keyboard = keyboard_create(page->display, page->images);
        char *filename = keyboard_get_input(keyboard, "Enter file name: ");
        if (strcmp(filename, "") == 0)
        {
            return;
        }

        update_filename(page->profile.name, filename, extension[PROFILE_MOTION]);
        break;
    }
    case BUTTON_PROFILE_NUMBER:
    {
        Keyboard *keyboard = keyboard_create(page->display, page->images);
        char *number = keyboard_get_input(keyboard, "Number: ");
        page->profile.number = atoi(number);
        break;
    }
    }
}

void button_profile_add(int id, void *arg)
{
    TestProfilePage *page = (TestProfilePage *)arg;

    Explorer *explorer = explorer_create(page->display, 100, 100, EXPLORER_MODE_FILE, "/sd");
    char *filepath = explorer_run(explorer);
    if (strcmp(filepath, "") == 0)
    {
        return;
    }

    json_to_motion_set(filepath, &(page->profile.sets[page->profile.setCount]));
    page->profile.setCount++;
}

void button_profile_remove(int id, void *arg)
{
    TestProfilePage *page = (TestProfilePage *)arg;

    memmove(&(page->profile.sets[id]), &(page->profile.sets[id + 1]), sizeof(MotionQuartet) * (page->profile.setCount - id - 1));
    page->profile.setCount--;
}

static void button_sample(int id, void *arg)
{
    TestProfilePage *page = (TestProfilePage *)arg;
    switch (id)
    {
    case BUTTON_SAMPLE_NAME:
    {
        Keyboard *keyboard = keyboard_create(page->display, page->images);
        char *filename = keyboard_get_input(keyboard, "Enter file name: ");
        if (strcmp(filename, "") == 0)
        {
            return;
        }

        update_filename(page->sample.name, filename, extension[PROFILE_SAMPLE]);
        break;
    }
    case BUTTON_SAMPLE_NUMBER:
    {
        Keyboard *keyboard = keyboard_create(page->display, page->images);
        char *number = keyboard_get_input(keyboard, "Number: ");
        page->sample.number = atoi(number);
        break;
    }
    case BUTTON_SAMPLE_STRETCH:
    {
        Keyboard *keyboard = keyboard_create(page->display, page->images);
        char *number = keyboard_get_input(keyboard, "Stretch: ");
        page->sample.stretchMax = atof(number);
        break;
    }
    case BUTTON_SAMPLE_MAX_VELOCITY:
    {
        Keyboard *keyboard = keyboard_create(page->display, page->images);
        char *number = keyboard_get_input(keyboard, "Max Velocity: ");
        page->sample.maxVelocity = atof(number);
        break;
    }
    case BUTTON_SAMPLE_MAX_ACCELERATION:
    {
        Keyboard *keyboard = keyboard_create(page->display, page->images);
        char *number = keyboard_get_input(keyboard, "Max Acceleration: ");
        page->sample.maxAcceleration = atof(number);
        break;
    }
    case BUTTON_SAMPLE_MAX_JERK:
    {
        Keyboard *keyboard = keyboard_create(page->display, page->images);
        char *number = keyboard_get_input(keyboard, "Max Jerk: ");
        page->sample.maxJerk = atof(number);
        break;
    }
    case BUTTON_SAMPLE_MAX_FORCE_TENSILE:
    {
        Keyboard *keyboard = keyboard_create(page->display, page->images);
        char *number = keyboard_get_input(keyboard, "Max Tensile: ");
        page->sample.maxForceTensile = atof(number);
        break;
    }
    case BUTTON_SAMPLE_MAX_FORCE_COMPRESSION:
    {
        Keyboard *keyboard = keyboard_create(page->display, page->images);
        char *number = keyboard_get_input(keyboard, "Max Compression: ");
        page->sample.maxForceCompression = atof(number);
        break;
    }
    }
}

void test_profile_page_init(TestProfilePage *page, Display *display, Images *images)
{
    page->display = display;
    page->images = images;
    page->mode = -1;
    strcpy(page->path, "/sd");
    strcpy(page->filename, "");
    int padding = 10;

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
    Module *editWindow = &(page->editWindow);
    module_init(editWindow, background);
    module_set_rectangle_circle(editWindow, SCREEN_WIDTH / 3, 0);
    module_fit_height(editWindow);
    module_set_padding(editWindow, padding, padding);
    module_set_color(editWindow, MAINCOLOR, BACKCOLOR);
    module_add_border(editWindow, MAINTEXTCOLOR, 1);
    module_align_inner_left(editWindow);
    module_align_inner_top(editWindow);
    module_set_margin(editWindow, 10, 10);

    // Create Open Button
    Module *openButton = &(page->openButton);
    module_init(openButton, editWindow);
    module_set_rectangle_circle(openButton, 100, 50);
    module_set_color(openButton, COLOR65K_LIGHTGREEN, COLOR65K_LIGHTGREEN);
    module_set_padding(openButton, padding, padding);
    module_align_space_even(openButton, 1, 2);
    module_align_inner_top(openButton);
    module_touch_callback(openButton, button_open, 0);

    // Create Open Text
    Module *openText = &(page->openText);
    module_init(openText, openButton);
    module_set_text(openText, openTextBuffer);
    module_text_font(openText, -1);
    module_text_fit(openText);
    module_align_center(openText);
    module_align_middle(openText);
    module_set_color(openText, SECONDARYTEXTCOLOR, openText->parent->foregroundColor);

    // Create New Button
    Module *newButton = &(page->newButton);
    module_init(newButton, editWindow);
    module_copy(newButton, openButton);
    module_align_space_even(newButton, 2, 2);
    module_touch_callback(newButton, button_new, 0);

    // Create New Text
    Module *newText = &(page->newText);
    module_init(newText, newButton);
    module_copy(newText, openText);
    module_set_text(newText, newTextBuffer);
    module_text_font(newText, -1);
    module_text_fit(newText);
    module_align_center(newText);

    // Create Save Button
    Module *saveButton = &(page->saveButton);
    module_init(saveButton, editWindow);
    module_copy(saveButton, openButton);
    module_align_center(saveButton);
    module_align_inner_bottom(saveButton);
    module_touch_callback(saveButton, button_save, 0);

    // Create New Text
    Module *saveText = &(page->saveText);
    module_init(saveText, saveButton);
    module_copy(saveText, openText);
    module_set_text(saveText, saveTextBuffer);
    module_text_font(saveText, -1);
    module_text_fit(saveText);
    module_align_center(saveText);
    module_align_middle(saveText);

    // Create navigation button
    Module *navigationButton = &(page->navigationButton);
    module_init(navigationButton, background);
    module_set_image(navigationButton, &(page->images->navigationImage));
    module_align_inner_top(navigationButton);
    module_align_inner_right(navigationButton);
    module_touch_callback(navigationButton, button_navigation, 0);

    Module *graphPositionContainer = &(page->graphPositionContainer);
    module_init(graphPositionContainer, background);
    module_set_margin(graphPositionContainer, 10, 10);
    module_set_size(graphPositionContainer, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - padding * 3);
    module_align_right(graphPositionContainer, editWindow);
    module_align_inner_top(graphPositionContainer);
    module_set_color(graphPositionContainer, graphPositionContainer->parent->foregroundColor, graphPositionContainer->parent->backgroundColor);
    module_set_graph(graphPositionContainer, &(page->graphPosition), "Position Vs. Time", "mm", &(page->graphValue));
    module_graph_set_range(graphPositionContainer, 5, -5);

    Module *subroot = &(page->subroot);
    module_init(subroot, editWindow);
    module_align_below(subroot, newButton);
    module_fit_below(subroot, newButton);
    module_fit_width(subroot);
    module_align_inner_left(subroot);
    module_set_color(subroot, subroot->parent->foregroundColor, subroot->parent->backgroundColor);
    module_set_padding(subroot, 0, 0);

    // Create edit window title
    Module *editWindowTitle = &(page->editWindowTitle);
    module_init(editWindowTitle, subroot);
    module_set_margin(editWindowTitle, 0, 10);
    module_set_text(editWindowTitle, page->editWindowTitleBuffer);
    module_text_font(editWindowTitle, RA8876_CHAR_HEIGHT_32);
    module_text_fit(editWindowTitle);
    module_fit_width(editWindowTitle);
    module_text_align(editWindowTitle, MODULE_TEXT_ALIGN_INNER_LEFT);
    module_set_color(editWindowTitle, MAINTEXTCOLOR, MAINCOLOR);
    module_align_inner_top(editWindowTitle);
    module_align_center(editWindowTitle);
    // module_text_underline(editWindowTitle);

    Module *below = editWindowTitle;
    for (int i = 0; i < 20; i++)
    {
        Module *param = &(page->profileParameters[i]);
        module_init(param, subroot);
        strcpy(page->profileParametersBuffer[i], "");
        module_set_text(param, page->profileParametersBuffer[i]);
        module_text_font(param, RA8876_CHAR_HEIGHT_24);
        module_set_size(param, 0, 32);
        module_fit_width(param);
        module_set_radius(param, 5);
        module_text_align(param, MODULE_TEXT_ALIGN_INNER_LEFT);
        module_text_align_verticle(param, MODULE_TEXT_ALIGN_VCENTER);
        module_align_below(param, below);
        module_align_inner_left(param);
        module_set_color(param, SECONDARYTEXTCOLOR, SECONDARYCOLOR);
        module_touch_callback(param, button_quartet, BUTTON_QUARTET_NAME);
        module_add_border(param, COLOR65K_BLACK, 2);
        module_set_margin(param, 10, 10);
        module_set_padding(param, 10, 0);
        module_set_visable(param, false);
        below = param;
    }
}

void test_profile_page_run(TestProfilePage *page)
{
    complete = false;
    serial_debug("Test profile page running\n");
    module_draw(page->display, &(page->root));
    int lastMode = -1;
    while (!complete)
    {
        serial_debug("Mode:%d\n", page->mode);
        if (page->mode != lastMode)
        {
            for (int i = 0; i < 20; i++)
            {
                module_set_visable(page->subroot.child[i], false);
            }
        }
        switch (page->mode)
        {
        case PROFILE_QUARTET:
        {
            if (lastMode != PROFILE_QUARTET)
            {
                strcpy(page->editWindowTitleBuffer, "Quartet");
                module_draw(page->display, &(page->root));
                serial_debug("Quartet name:%s\n", page->quartet.name);
            }

            serial_debug("Quartet\n");

            int paramIndex = 0;

            Module *nameModule = &(page->profileParameters[paramIndex]);
            sprintf(page->profileParametersBuffer[paramIndex], "Name: %s", page->quartet.name);
            module_touch_callback(nameModule, button_quartet, BUTTON_QUARTET_NAME);
            module_set_visable(nameModule, true);
            serial_debug("Name:%s\n", page->profileParametersBuffer[paramIndex]);

            paramIndex++;
            Module *funcModule = &(page->profileParameters[paramIndex]);
            get_function_info(&(page->info), page->quartet.function);
            sprintf(page->profileParametersBuffer[paramIndex], "Function: %s", page->info.name);
            module_touch_callback(funcModule, button_quartet, BUTTON_QUARTET_FUNC);
            module_set_visable(funcModule, true);

            paramIndex++;
            for (int i = 0; i < page->info.args_count; i++) //[distance, strain rate, error]
            {
                Module *paramModule = &(page->profileParameters[paramIndex]);
                sprintf(page->profileParametersBuffer[paramIndex], "    %s: %0.3f", page->info.args[i], page->quartet.parameters[i]);
                module_touch_callback(paramModule, button_quartet_parameters, i);
                module_set_visable(paramModule, true);
                paramIndex++;
            }

            Module *dwellModule = &(page->profileParameters[paramIndex]);
            sprintf(page->profileParametersBuffer[paramIndex], "Dwell (ms): %0.3f", page->quartet.dwell);
            module_touch_callback(dwellModule, button_quartet, BUTTON_QUARTET_DWELL);
            module_set_visable(dwellModule, true);
            break;
        }
        case PROFILE_SET:
        {
            if (lastMode != PROFILE_SET)
            {
                strcpy(page->editWindowTitleBuffer, "Set");
                module_draw(page->display, &(page->root));
                // strcpy(page->set.name, page->filename);
            }
            serial_debug("Set\n");
            int paramIndex = 0;

            Module *nameModule = &(page->profileParameters[paramIndex]);
            sprintf(page->profileParametersBuffer[paramIndex], "Name: %s", page->set.name);
            module_touch_callback(nameModule, button_set, BUTTON_SET_NAME);
            module_set_visable(nameModule, true);

            paramIndex++;
            Module *numberModule = &(page->profileParameters[paramIndex]);
            sprintf(page->profileParametersBuffer[paramIndex], "Number: %d", page->set.number);
            module_touch_callback(numberModule, button_set, BUTTON_SET_NUMBER);
            module_set_visable(numberModule, true);

            paramIndex++;
            Module *typeModule = &(page->profileParameters[paramIndex]);
            sprintf(page->profileParametersBuffer[paramIndex], "Type: %s", page->set.type);
            module_touch_callback(typeModule, button_set, BUTTON_SET_TYPE);
            module_set_visable(typeModule, true);

            paramIndex++;
            Module *executionsModule = &(page->profileParameters[paramIndex]);
            sprintf(page->profileParametersBuffer[paramIndex], "Executions: %d", page->set.executions);
            module_touch_callback(executionsModule, button_set, BUTTON_SET_EXECUTIONS);
            module_set_visable(executionsModule, true);

            for (int i = 0; i < page->set.quartetCount; i++)
            {
                paramIndex++;
                Module *quartetModule = &(page->profileParameters[paramIndex]);
                sprintf(page->profileParametersBuffer[paramIndex], "%s", page->set.quartets[i].name);
                module_touch_callback(quartetModule, button_set_remove, i);
                module_set_visable(quartetModule, true);
            }

            paramIndex++;
            Module *quartetModule = &(page->profileParameters[paramIndex]);
            sprintf(page->profileParametersBuffer[paramIndex], "Add Quartet");
            module_touch_callback(quartetModule, button_set_add, 0);
            module_set_visable(quartetModule, true);
            break;
        }
        case PROFILE_MOTION:
        {
            if (lastMode != PROFILE_MOTION)
            {
                strcpy(page->editWindowTitleBuffer, "Motion Profile");
                module_draw(page->display, &(page->root));
                // strcpy(page->set.name, page->filename);
            }
            serial_debug("Motion Profile\n");
            int paramIndex = 0;

            Module *nameModule = &(page->profileParameters[paramIndex]);
            sprintf(page->profileParametersBuffer[paramIndex], "Name: %s", page->profile.name);
            module_touch_callback(nameModule, button_profile, BUTTON_PROFILE_NAME);
            module_set_visable(nameModule, true);

            paramIndex++;
            Module *numberModule = &(page->profileParameters[paramIndex]);
            sprintf(page->profileParametersBuffer[paramIndex], "Number: %d", page->profile.number);
            module_touch_callback(numberModule, button_profile, BUTTON_PROFILE_NUMBER);
            module_set_visable(numberModule, true);

            for (int i = 0; i < page->profile.setCount; i++)
            {
                paramIndex++;
                Module *setModule = &(page->profileParameters[paramIndex]);
                sprintf(page->profileParametersBuffer[paramIndex], "%s", page->profile.sets[i].name);
                module_touch_callback(setModule, button_profile_remove, i);
                module_set_visable(setModule, true);
            }

            paramIndex++;
            Module *setModule = &(page->profileParameters[paramIndex]);
            sprintf(page->profileParametersBuffer[paramIndex], "Add Set");
            module_touch_callback(setModule, button_profile_add, 0);
            module_set_visable(setModule, true);
            break;
        }
        case PROFILE_SAMPLE:
        {
            if (lastMode != PROFILE_SAMPLE)
            {
                strcpy(page->editWindowTitleBuffer, "Sample Profile");
                module_draw(page->display, &(page->root));
                // strcpy(page->set.name, page->filename);
            }
            serial_debug("Sample Profile\n");
            int paramIndex = 0;

            Module *nameModule = &(page->profileParameters[paramIndex]);
            sprintf(page->profileParametersBuffer[paramIndex], "Name: %s", page->sample.name);
            module_touch_callback(nameModule, button_sample, BUTTON_SAMPLE_NAME);
            module_set_visable(nameModule, true);

            paramIndex++;
            Module *numberModule = &(page->profileParameters[paramIndex]);
            sprintf(page->profileParametersBuffer[paramIndex], "Number: %d", page->sample.number);
            module_touch_callback(numberModule, button_sample, BUTTON_SAMPLE_NUMBER);
            module_set_visable(numberModule, true);

            paramIndex++;
            Module *lengthModule = &(page->profileParameters[paramIndex]);
            sprintf(page->profileParametersBuffer[paramIndex], "Length: %f", page->sample.length);
            module_touch_callback(lengthModule, button_sample, BUTTON_SAMPLE_LENGTH);
            module_set_visable(lengthModule, true);

            paramIndex++;
            Module *stretchModule = &(page->profileParameters[paramIndex]);
            sprintf(page->profileParametersBuffer[paramIndex], "Stretch: %f", page->sample.number);
            module_touch_callback(stretchModule, button_sample, BUTTON_SAMPLE_STRETCH);
            module_set_visable(stretchModule, true);

            paramIndex++;
            Module *maxVelocityModule = &(page->profileParameters[paramIndex]);
            sprintf(page->profileParametersBuffer[paramIndex], "Max Velocity: %f", page->sample.maxVelocity);
            module_touch_callback(maxVelocityModule, button_sample, BUTTON_SAMPLE_MAX_VELOCITY);
            module_set_visable(maxVelocityModule, true);

            paramIndex++;
            Module *maxAccelerationModule = &(page->profileParameters[paramIndex]);
            sprintf(page->profileParametersBuffer[paramIndex], "Max Acceleration: %f", page->sample.maxAcceleration);
            module_touch_callback(maxAccelerationModule, button_sample, BUTTON_SAMPLE_MAX_ACCELERATION);
            module_set_visable(maxAccelerationModule, true);

            paramIndex++;
            Module *maxJerkModule = &(page->profileParameters[paramIndex]);
            sprintf(page->profileParametersBuffer[paramIndex], "Max Jerk: %f", page->sample.maxJerk);
            module_touch_callback(maxJerkModule, button_sample, BUTTON_SAMPLE_MAX_JERK);
            module_set_visable(maxJerkModule, true);

            paramIndex++;
            Module *maxForceTensileModule = &(page->profileParameters[paramIndex]);
            sprintf(page->profileParametersBuffer[paramIndex], "Max Tensile: %f", page->sample.maxForceTensile);
            module_touch_callback(maxForceTensileModule, button_sample, BUTTON_SAMPLE_MAX_FORCE_TENSILE);
            module_set_visable(maxForceTensileModule, true);

            paramIndex++;
            Module *maxForceCompressionModule = &(page->profileParameters[paramIndex]);
            sprintf(page->profileParametersBuffer[paramIndex], "Max Compression: %f", page->sample.maxForceCompression);
            module_touch_callback(maxForceCompressionModule, button_sample, BUTTON_SAMPLE_MAX_FORCE_COMPRESSION);
            module_set_visable(maxForceCompressionModule, true);
            break;
        }
        default:
            break;
        }
        lastMode = page->mode;
        module_draw(page->display, &(page->subroot));
        do
        {
            display_update_touch(page->display);
        } while (module_touch_check(&(page->root), page->display, page) == 0);
    }
}
