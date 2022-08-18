#include "Selection.h"

#define SELECTION_WIDTH 400

static char selectBuffer[] = "Select";

static void check_buttons(int id, void *arg)
{
    Selection *selection = (Selection *)arg;
    selection->selected = id;
}

void selection_init(Selection *selection, Display *display)
{
    int x = SCREEN_WIDTH / 2 - SELECTION_WIDTH / 2;
    int y = 100;
    int padding = 10;

    // Create Background
    Module *root = &(selection->root);
    module_init(root, NULL);

    // Create Window
    Module *window = &(selection->window);
    module_init(window, root);
    module_set_rectangle_circle(window, SELECTION_WIDTH, 0);
    module_set_position(window, x, y);
    module_set_padding(window, padding, padding);
    module_set_color(window, SECONDARYCOLOR, COLOR65K_BLUE);
    module_add_border(window, SECONDARYTEXTCOLOR, 1);

    // Create Title
    Module *title = &(selection->title);
    module_init(title, window);
    module_set_margin(title, padding, padding);
    module_set_text(title, selectBuffer);
    module_text_font(title, RA8876_CHAR_HEIGHT_32);
    module_text_fit(title);
    module_set_color(title, SECONDARYTEXTCOLOR, title->parent->foregroundColor);
    module_align_inner_top(title);
    module_align_center(title);
    module_text_underline(title);

    Module *below = title;
    for (int i = 0; i < MAX_SELECTIONS; i++)
    {
        Module *option = &(selection->options[i]);
        module_init(option, window);
        module_set_text(option, selection->optionsBuffer[i]);
        module_text_font(option, RA8876_CHAR_HEIGHT_24);
        module_set_size(option, 0, 40);
        module_fit_width(option);
        module_set_margin(option, padding, padding);
        module_set_color(option, SECONDARYTEXTCOLOR, option->parent->foregroundColor);
        module_align_inner_left(option);
        module_align_below(option, below);
        module_add_border(option, SECONDARYTEXTCOLOR, 1);
        module_text_align(option, MODULE_TEXT_ALIGN_INNER_CENTER);
        module_text_align_verticle(option, MODULE_TEXT_ALIGN_VCENTER);
        module_set_visable(option, false);
        module_touch_callback(option, check_buttons, i);
        below = option;
    }
    window->h = 100;
    selection->optionCount = 0;
    selection->selected = -1;
    selection->display = display;
}

void selection_add_option(Selection *selection, const char *option)
{
    printf("adding option\n");
    Module *newOption = &(selection->options[selection->optionCount]);
    strncpy(selection->optionsBuffer[selection->optionCount], option, 20);
    module_set_visable(newOption, true);
    selection->optionCount++;
    selection->window.h = newOption->y + newOption->h + newOption->py;
    printf("done adding option\n");
}

int selection_run(Selection *selection)
{
    // Copy current page to memory
    display_bte_memory_copy(selection->display, PAGE1_START_ADDR, SCREEN_WIDTH, 0, 0, PAGE3_START_ADDR, SCREEN_WIDTH, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    module_draw(selection->display, &(selection->root));

    do
    {
        display_update_touch(selection->display);
    } while (module_touch_check(&(selection->root), selection->display, selection) == 0);
    printf("selected: %d\n", selection->selected);

    // Paste memory page to screen
    display_bte_memory_copy(selection->display, PAGE3_START_ADDR, SCREEN_WIDTH, 0, 0, PAGE1_START_ADDR, SCREEN_WIDTH, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    return selection->selected;
}
