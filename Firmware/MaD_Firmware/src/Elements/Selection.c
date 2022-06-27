#include "Selection.h"

#define SELECTION_WIDTH 400

static char selectBuffer[] = "Select";

typedef struct selection_s
{
    Display *display;
    int selected;
} Selection;

static void check_buttons(int id, void *arg)
{
    Selection *selection = (Selection *)arg;
    selection->selected = id;
}

int selection_run(Display *display, int x, int y, char **options, int amount)
{
    // Copy current page to memory
    display_bte_memory_copy(display, PAGE1_START_ADDR, SCREEN_WIDTH, 0, 0, PAGE3_START_ADDR, SCREEN_WIDTH, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    int padding = 10;
    // Create Background
    Module *root = module_create(NULL);

    // Create Window
    Module *window = module_create(root);
    module_set_rectangle_circle(window, SELECTION_WIDTH, 0);
    module_set_position(window, x, y);
    module_set_padding(window, padding, padding);
    module_set_color(window, COLOR65K_LIGHTBLUE, COLOR65K_BLUE);

    // Create Title
    Module *title = module_create(window);
    module_set_padding(title, padding, padding);
    module_set_text(title, selectBuffer);
    module_text_font(title, RA8876_CHAR_HEIGHT_32);
    module_set_color(title, COLOR65K_WHITE, title->parent->foregroundColor);
    module_align_inner_top(title);
    module_align_center(title);
    module_text_underline(title);

    Module **optionModule = (Module **)malloc(sizeof(Module *) * amount);
    Module *below = title;
    for (int i = 0; i < amount; i++)
    {
        optionModule[i] = module_create(window);
        module_set_padding(optionModule[i], padding, padding);
        module_set_rectangle(optionModule[i], 0, 40);
        module_fit_width(optionModule[i]);
        module_align_inner_left(optionModule[i]);
        module_align_below(optionModule[i], below);
        module_set_color(optionModule[i], COLOR65K_GREEN, optionModule[i]->parent->foregroundColor);
        module_touch_callback(optionModule[i], check_buttons, i);

        Module *text = module_create(optionModule[i]);
        module_set_text(text, options[i]);
        module_text_font(text, RA8876_CHAR_HEIGHT_24);
        module_set_padding(text, padding, padding);
        module_set_color(text, COLOR65K_BLACK, text->parent->foregroundColor);
        module_align_center(text);
        module_align_middle(text);
        below = optionModule[i];
    }
    window->h = below->y + below->h + below->py;
    module_draw(display, root);

    // Clear any unwanted presses
    _waitms(100);
    display_update_touch(display);

    Selection *selection = (Selection *)malloc(sizeof(Selection));
    selection->display = display;
    selection->selected = -1;
    do
    {
        display_update_touch(display);
    } while (module_touch_check(root, display, selection) == 0);
    printf("selected: %d\n", selection->selected);
    // Paste memory page to screen
    display_bte_memory_copy(display, PAGE3_START_ADDR, SCREEN_WIDTH, 0, 0, PAGE1_START_ADDR, SCREEN_WIDTH, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    _waitms(200);
    button_update(display); // Clear any presses
    module_destroy(root);
    int ret = selection->selected;
    free(selection);
    return ret;
}