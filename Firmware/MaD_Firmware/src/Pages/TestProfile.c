#include "TestProfile.h"
#include "Explorer.h"

#define BUTTONCOUNT 3
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

#define PROFILE_TYPES 4
#define PROFILE_QUARTET 0
#define PROFILE_SET 1
#define PROFILE_MOTION 2
#define PROFILE_TEST 3

static const char openTextBuffer[] = "Open";
static const char newTextBuffer[] = "New";
static const char saveTextBuffer[] = "Save";

static const char *optionNames[] = {"Quartet", "Set", "Motion", "Test"};
static const char *extension[] = {".QRT", ".SET", ".MOT", ".TST"};

static bool complete;

static void button_navigation(int id, void *arg)
{
    TestProfilePage *page = (TestProfilePage *)arg;
    complete = true;
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

    char *filename = strrchr(filepath, '/') + 1;
    strcpy(page->filename, filename);
    strncpy(page->path, filepath, strlen(filepath) - strlen(filename));

    for (int i = 0; i < PROFILE_TYPES; i++)
    {
        char *ext = filepath;
        ext += strlen(filepath) - strlen(extension[i]);
        printf("%s\n", ext);
        if (strcmp(ext, extension[i]) == 0)
        {
            printf("Found mode:%d\n", i);
            page->mode = i;
            break;
        }
    }
    printf("Mode:%d\n", page->mode);

    switch (page->mode)
    {
    case PROFILE_QUARTET:
    {
        printf("Quartet from json\n");
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
    case PROFILE_TEST:
    {
        json_to_test_profile(filepath, &(page->test));
        break;
    }
    }
    explorer_destroy(explorer);
}

static void button_new(int id, void *arg)
{
    TestProfilePage *page = (TestProfilePage *)arg;

    int newmode = selection_run(page->display, 100, 100, optionNames, PROFILE_TYPES);

    Keyboard *keyboard = keyboard_create(page->display, page->images);
    if (keyboard == NULL)
    {
        printf("TestProfile Button new Keyboard could not allocate memory\n");
        return;
    }
    char *filename = keyboard_get_input(keyboard, "Enter file name: ");

    if (strcmp(filename, "") == 0)
    {
        return;
    }

    strcpy(page->filename, filename);
    keyboard_destroy(keyboard);

    Explorer *explorer = explorer_create(page->display, 100, 100, EXPLORER_MODE_DIRECTORY, "/sd");
    if (explorer == NULL)
    {
        printf("Testprofile.c explorer could not allocate memory\n");
    }

    char *newpath = explorer_run(explorer);

    if (strcmp(newpath, "") == 0)
    {
        return;
    }

    page->mode = newmode;
    strcpy(page->path, newpath);
    explorer_destroy(explorer);
}

static void button_save(int id, void *arg)
{
    TestProfilePage *page = (TestProfilePage *)arg;
    char *filepath = (char *)malloc(strlen(page->path) + strlen(page->filename) + strlen(extension[page->mode]) + 3);
    if (filepath == NULL)
    {
        printf("Testprofile.c filepath could not allocate memory\n");
    }
    strcpy(filepath, page->path);
    strcat(filepath, "/");
    strcat(filepath, page->filename);
    strcat(filepath, extension[page->mode]);
    printf("Saving %s to %s: %s\n", page->filename, page->path, filepath);
    motion_quartet_to_json(&(page->quartet), filepath);
    free(filepath);
}

static void button_simulate(int id, void *arg)
{
    TestProfilePage *page = (TestProfilePage *)arg;
    switch (page->mode)
    {
    case PROFILE_QUARTET:
    {
        motion_quartet_simulate(&(page->quartet));
        break;
    }
    case PROFILE_SET:
    {
        motion_set_simulate(&(page->set));
        break;
    }
    case PROFILE_MOTION:
    {
        motion_profile_simulate(&(page->profile));
        break;
    }
    case PROFILE_TEST:
    {
        test_profile_simulate(&(page->test));
        break;
    }
    }

    double max_velocity_rpm = 12.21 * 1 * (48 + 3)(48 + 3) / 16; //((float)value / 1000.0) * (60.0 / 80.0);              // um/s to rpm
    double max_acceleration_rpms = 30 * 635.78 * 1;
    double v_max = (0.08) * (max_velocity_rpm) / (60.0);
    double a_max = (0.08) * (max_acceleration_rpms) / (60.0);
    SetPoint *setpoint = create_empty_setpoint();
    double t = 0;
    while (abs(d_max - setpoint->x) > error)
    {
        // simulate_profile(setpoint, t, d_max, v_max, a_max, sigmoid, d_max, sr, error);
        printf("%f,%f,%f,%f\n", t + 0.0386, setpoint->x, setpoint->v, setpoint->a);
        t += error * 4;
    }
}
static void button_quartet(int id, void *arg)
{
    printf("BUTTON_QUARTET_id:%d\n", id);
    TestProfilePage *page = (TestProfilePage *)arg;
    switch (id)
    {
    case BUTTON_QUARTET_NAME:
    {
        Keyboard *keyboard = keyboard_create(page->display, page->images);
        char *filename = keyboard_get_input(keyboard, "Enter file name: ");

        strcpy(page->quartet.name, filename);

        strcpy(page->filename, filename);
        strcat(page->filename, ".qrt");
        keyboard_destroy(keyboard);
        break;
    }
    case BUTTON_QUARTET_FUNC:
    {
        char *optionNames[FUNCTION_COUNT];
        for (int i = 0; i < FUNCTION_COUNT; i++)
        {
            get_function_info(&(page->info), i);
            printf("function:%d,name:%s\n", i, page->info.name);
            optionNames[i] = (char *)malloc(strlen(page->info.name) + 1);
            strncpy(optionNames[i], page->info.name, FUNCTION_MAX_NAME_LENGTH);
        }
        page->quartet.function = selection_run(page->display, 300, 150, optionNames, FUNCTION_COUNT);
        for (int i = 0; i < FUNCTION_COUNT; i++)
        {
            free(optionNames[i]);
        }
        break;
    }
    case BUTTON_QUARTET_DWELL:
    {
        Keyboard *keyboard = keyboard_create(page->display, page->images);
        char *dwell = keyboard_get_input(keyboard, "Dwell: ");
        page->quartet.dwell = atof(dwell);
        keyboard_destroy(keyboard);
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
    char *prompt[30];
    strcpy(prompt, page->info.args[id]);
    strcat(prompt, ": ");
    char *param = keyboard_get_input(keyboard, prompt);
    page->quartet.parameters[id] = atof(param);
    keyboard_destroy(keyboard);
}

static void button_set(int id, void *arg)
{
    /*TestProfilePage *page = (TestProfilePage *)arg;
    switch (id)
    {
    case BUTTON_SET_NAME:
    {
        Keyboard *keyboard = keyboard_create(page->display, page->images);
        char *filename = keyboard_get_input(keyboard, "Enter file name: ");
        keyboard_destroy(keyboard);
        free(page->set->name);
        strcpy(page->set->name, page->path);
        strcat(page->set->name, "/");
        strcat(page->set->name, filename);
        strcat(page->set->name, ".set");
        free(filename);
        break;
    }
    case BUTTON_SET_EXECUTIONS:
    {
        Keyboard *keyboard = keyboard_create(page->display, page->images);
        char *executions = keyboard_get_input(keyboard, "Executions: ");
        page->set->executions = atoi(executions);
        keyboard_destroy(keyboard);
        free(executions);
        break;
    }
    case BUTTON_SET_QUARTETS:
    {
        char *optionNames[] = {"Add", "Remove"};
        int newmode = selection_run(page->display, 100, 100, optionNames, 2);
        if (newmode == 0) // Add
        {
            Explorer *explorer = explorer_create(page->display, 100, 100, EXPLORER_MODE_FILE, "/sd");
            if (explorer == NULL)
            {
                printf("Testprofile.c explorer could not allocate memory\n");
            }

            char *newfile = explorer_run(explorer);
            explorer_destroy(explorer);

            if (newfile != NULL)
            {
                page->set->quartetCount++;
                json_to_motion_quartet(newfile, &(page->set->quartets[page->set->quartetCount - 1]));
            }
            else
            {
                return;
            }
        }
        else if (newmode == 1) // Remove
        {
            char **optionNames = (char **)malloc(sizeof(char *) * page->set->quartetCount);
            for (int i = 0; i < page->set->quartetCount; i++)
            {
                optionNames[i] = (char *)malloc(strlen(page->set->quartets[i].name) + 1);
                strcpy(optionNames[i], page->set->quartets[i]);
            }
            int index = selection_run(page->display, 100, 100, optionNames, 2);

            // Free option names
            for (int i = 0; i < page->set->quartetCount; i++)
            {
                free(optionNames[i]);
            }
            free(optionNames);

            // Remove index for quartets
            free(&(page->set->quartets[index]));
            for (int i = index; i < page->set->quartetCount - 1; i++)
            {
                page->set->quartets[i] = page->set->quartets[i + 1];
            }
            page->set->quartetCount--;
        }
        else
        {
            printf("something is wrong:%d\n", newmode);
        }
    }
    }*/
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
    module_set_graph(graphPositionContainer, &(page->graphPosition), "Position Vs. Time", "mm");
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

void test_profile_page_destroy(TestProfilePage *page)
{
    free(page);
}
void test_profile_page_run(TestProfilePage *page)
{
    complete = false;
    printf("Test profile page running\n");
    int padding = 20;

    module_draw(page->display, &(page->root));
    int lastMode = -1;
    while (!complete)
    {
        printf("Mode:%d\n", page->mode);
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
                strcpy(page->quartet.name, page->filename);
                printf("Quartet name:%s\n", page->quartet.name);
            }

            printf("Quartet\n");
            strcpy(page->editWindowTitleBuffer, "Quartet");

            Module *nameModule = &(page->profileParameters[0]);
            sprintf(page->profileParametersBuffer[0], "Name: %s", page->quartet.name);
            module_touch_callback(nameModule, button_quartet, BUTTON_QUARTET_NAME);
            module_set_visable(nameModule, true);
            printf("Name:%s\n", page->profileParametersBuffer[0]);

            Module *funcModule = &(page->profileParameters[1]);
            get_function_info(&(page->info), page->quartet.function);
            sprintf(page->profileParametersBuffer[1], "Function: %s", page->info.name);
            module_touch_callback(funcModule, button_quartet, BUTTON_QUARTET_FUNC);
            module_set_visable(funcModule, true);

            int paramIndex = 2;
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
            /*    char *name;              // Motion set Filename, user defined
    int number;              // Motion set number, autogenerated as incrment in file system
    char *type;              // descriptive purpose of the set (ie. precondition, test, etc)
    int executions;          // Number of times to execute the motion set
    int quartetCount;        // Number of motion quartets in the set
    MotionQuartet *quartets; // List of quartets to execute (max 10)*/
            if (lastMode != PROFILE_SET)
            {
                strcpy(page->set.name, page->filename);
            }
            module_set_text(&(page->editWindowTitle), "Edit Set");
            module_text_font(&(page->editWindowTitle), RA8876_CHAR_HEIGHT_32);
            module_align_center(&(page->editWindowTitle));
            module_text_underline(&(page->editWindowTitle));

            printf("Set\n");
            strcpy(page->editWindowTitleBuffer, "Set");

            Module *nameModule = &(page->profileParameters[0]);
            sprintf(page->profileParametersBuffer[0], "Name: %s", page->set.name);
            module_touch_callback(nameModule, button_set, BUTTON_SET_NAME);
            module_set_visable(nameModule, true);

            Module *numberModule = &(page->profileParameters[1]);
            sprintf(page->profileParametersBuffer[1], "Number: %d", page->set.number);
            module_touch_callback(nameModule, button_set, BUTTON_SET_NUMBER);
            module_set_visable(nameModule, true);

            Module *typeModule = &(page->profileParameters[2]);
            sprintf(page->profileParametersBuffer[2], "Type: %d", page->set.type);
            module_touch_callback(nameModule, button_set, BUTTON_SET_TYPE);
            module_set_visable(nameModule, true);

            /*Module *nameModule = module_create(subroot);
            sprintf(page->nameBuffer, "Name: %s", page->set->name);
            module_set_text(nameModule, page->nameBuffer);
            module_text_font(nameModule, RA8876_CHAR_HEIGHT_24);
            module_align_inner_left(nameModule);
            module_align_inner_top(nameModule);
            module_set_color(nameModule, SECONDARYTEXTCOLOR, nameModule->parent->foregroundColor);
            module_touch_callback(nameModule, button_set, BUTTON_SET_NAME);

            Module *funcModule = module_create(subroot);
            module_copy(funcModule, nameModule);*/
            break;
        }
        case PROFILE_MOTION:
        {
            break;
        }
        case PROFILE_TEST:
        {
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
