#include "keyboard.h"
#include "Module.h"

#define BUTTONCOUNT 47

#define KEY_UNIT_WIDTH 54
#define KEY_UNIT_HEIGHT 54

static enum keys {
    key_0,
    key_1,
    key_2,
    key_3,
    key_4,
    key_5,
    key_6,
    key_7,
    key_8,
    key_9,
    key_tab,
    key_q,
    key_w,
    key_e,
    key_r,
    key_t,
    key_y,
    key_u,
    key_i,
    key_o,
    key_p,
    key_minus,
    key_a,
    key_s,
    key_d,
    key_f,
    key_g,
    key_h,
    key_j,
    key_k,
    key_l,
    key_del,
    key_shift,
    key_z,
    key_x,
    key_c,
    key_v,
    key_b,
    key_n,
    key_m,
    key_comma,
    key_period,
    key_cancel,
    key_left,
    key_space,
    key_right,
    key_submit
};

static char *key_name[] = {
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "tab",
    "q",
    "w",
    "e",
    "r",
    "t",
    "y",
    "u",
    "i",
    "o",
    "p",
    "-",
    "a",
    "s",
    "d",
    "f",
    "g",
    "h",
    "j",
    "k",
    "l",
    "del",
    "shift",
    "z",
    "x",
    "c",
    "v",
    "b",
    "n",
    "m",
    ",",
    ".",
    "cancel",
    "<",
    "space",
    ">",
    "submit"};

/**
 * @brief Keyboard page, renders a keyboard to get user input.
 *
 */

/**
 * @brief Update page from button inputs
 *
 * @param key The Button that was pressed
 * @returns if submit was pressed
 */
static bool key_pressed(int id, void *arg)
{
    Keyboard *keyboard = (Keyboard *)arg;
    keyboard->keyboard_buffer = (char *)realloc(keyboard->keyboard_buffer, strlen(keyboard->keyboard_buffer) + 2);
    printf("%s\n", keyboard->keyboard_buffer);
    switch (id)
    {
    case key_del:
        if (strlen(keyboard->keyboard_buffer) <= 0)
            break;
        keyboard->keyboard_buffer[strlen(keyboard->keyboard_buffer) - 1] = '\0';
        keyboard->keyboard_buffer = (char *)realloc(keyboard->keyboard_buffer, strlen(keyboard->keyboard_buffer) + 1);
        break;
    case key_cancel:
    {
        free(keyboard->keyboard_buffer);
        keyboard->keyboard_buffer = NULL;
        keyboard->keyboard_complete = true;
        break;
    }
    case key_left:
        break;
    case key_right:
        break;
    case key_submit:
        keyboard->keyboard_complete = true;
        break;
    default:
        strcat(keyboard->keyboard_buffer, key_name[id]);
        break;
    }
}

/**
 * @brief Create a Button object
 *
 * @param name Enum for button name
 * @param xmin top left position of button
 * @param ymin top right position of button
 * @param keySize Unit size of key see:
 * @image html https://cdn.shopify.com/s/files/1/0059/0630/1017/files/Keychron-C1-hot-swappable-wired-type-c-mechanical-keyboard-tenkeyless-layout-for-Mac-Windows-iOS-Keycap-Size_2048x2048.jpg?v=1608368111
 * @return Button struct
 */
static Module *createKey(Module *background, int name, Module *left, Module *above, float keySize)
{
    Module *key = module_create(background);
    module_set_rectangle_circle(key, KEY_UNIT_WIDTH * keySize, KEY_UNIT_HEIGHT);
    module_set_color(key, COLOR65K_BLUE);
    module_set_padding(key, 14, 14);
    module_touch_callback(key, key_pressed, name);

    if (left == NULL)
    {
        module_align_inner_left(key);
    }
    else
    {
        module_align_right(key, left);
    }
    if (above == NULL)
    {
        module_align_inner_top(key);
    }
    else
    {
        module_align_below(key, above);
    }

    Module *text = module_create(key);
    module_set_text(text, key_name[name]);
    module_set_font(text, RA8876_CHAR_HEIGHT_32);
    module_set_color(text, COLOR65K_WHITE, text->parent->foregroundColor);
    module_align_center(text);
    module_align_middle(text);
    return key;
}

Keyboard *keyboard_create(Display *display, Images *images)
{
    Keyboard *keyboard = (Keyboard *)malloc(sizeof(Keyboard));
    keyboard->display = display;
    keyboard->images = images;
    keyboard->keyboard_buffer = NULL;
    keyboard->keyboard_complete = false;
    return keyboard;
}
void keyboard_destroy(Keyboard *keyboard)
{
    free(keyboard->keyboard_buffer);
    free(keyboard);
}

/**
 * @brief Gets user input using on display keyboard
 *
 * @param input String keyboard_buffer for user input
 */
char *keyboard_get_input(Keyboard *keyboard, const char *prompt)
{
    printf("keyboard running:%s\n", prompt);

    // Create Empty Buffer
    keyboard->keyboard_buffer = (char *)malloc(sizeof(char));
    strcpy(keyboard->keyboard_buffer, "");

    int padding = 10;
    int topBarSize = 40;

    // Copy existing screen contents to memory
    display_bte_memory_copy(keyboard->display, PAGE1_START_ADDR, SCREEN_WIDTH, 0, 0, PAGE3_START_ADDR, SCREEN_WIDTH, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Create Background
    Module *root = module_create(NULL);
    Module *background = module_create(root);
    module_set_rectangle(background, SCREEN_WIDTH, 283.5);
    module_set_position(background, 0, SCREEN_HEIGHT - 283.5);
    module_set_padding(background, padding, padding);
    module_set_color(background, COLOR65K_DARKBLUE, COLOR65K_DARKBLUE);

    Module *topBar = module_create(root);
    module_set_rectangle(topBar, SCREEN_WIDTH, topBarSize);
    module_set_position(topBar, 0, SCREEN_HEIGHT - 283.5 - topBarSize);
    module_set_color(topBar, COLOR65K_BLACK, COLOR65K_BLACK);

    Module *topBarText = module_create(topBar);
    module_set_text(topBarText, prompt);
    module_set_font(topBarText, RA8876_CHAR_HEIGHT_32);
    module_align_center(topBarText);
    module_align_middle(topBarText);
    module_set_color(topBarText, COLOR65K_WHITE, COLOR65K_BLACK);

    Module *tab = createKey(background, key_tab, NULL, NULL, 1.25);
    Module *q = createKey(background, key_q, tab, NULL, 1);
    Module *w = createKey(background, key_w, q, NULL, 1);
    Module *e = createKey(background, key_e, w, NULL, 1);
    Module *r = createKey(background, key_r, e, NULL, 1);
    Module *t = createKey(background, key_t, r, NULL, 1);
    Module *y = createKey(background, key_y, t, NULL, 1);
    Module *u = createKey(background, key_u, y, NULL, 1);
    Module *i = createKey(background, key_i, u, NULL, 1);
    Module *o = createKey(background, key_o, i, NULL, 1);
    Module *p = createKey(background, key_p, o, NULL, 1);

    Module *minus = createKey(background, key_minus, NULL, tab, 1.5);
    Module *a = createKey(background, key_a, minus, tab, 1);
    Module *s = createKey(background, key_s, a, tab, 1);
    Module *d = createKey(background, key_d, s, tab, 1);
    Module *f = createKey(background, key_f, d, tab, 1);
    Module *g = createKey(background, key_g, f, tab, 1);
    Module *h = createKey(background, key_h, g, tab, 1);
    Module *j = createKey(background, key_j, h, tab, 1);
    Module *k = createKey(background, key_k, j, tab, 1);
    Module *l = createKey(background, key_l, k, tab, 1);
    Module *del = createKey(background, key_del, l, tab, 1.5);

    Module *shift = createKey(background, key_shift, NULL, del, 2.25);
    Module *z = createKey(background, key_z, shift, del, 1);
    Module *x = createKey(background, key_x, z, del, 1);
    Module *c = createKey(background, key_c, x, del, 1);
    Module *v = createKey(background, key_v, c, del, 1);
    Module *b = createKey(background, key_b, v, del, 1);
    Module *n = createKey(background, key_n, b, del, 1);
    Module *m = createKey(background, key_m, n, del, 1);
    Module *comma = createKey(background, key_comma, m, del, 1);
    Module *period = createKey(background, key_period, comma, del, 1);

    Module *cancel = createKey(background, key_cancel, NULL, period, 2.75);
    Module *left = createKey(background, key_left, cancel, period, 1.25);
    Module *space = createKey(background, key_space, left, period, 3);
    Module *right = createKey(background, key_right, space, period, 1.25);
    Module *submit = createKey(background, key_submit, right, period, 2);

    Module *seven = createKey(background, key_7, del, NULL, 1);
    Module *eight = createKey(background, key_8, seven, NULL, 1);
    Module *nine = createKey(background, key_9, eight, NULL, 1);

    Module *four = createKey(background, key_4, del, nine, 1);
    Module *five = createKey(background, key_5, four, nine, 1);
    Module *six = createKey(background, key_6, five, nine, 1);

    Module *one = createKey(background, key_1, del, five, 1);
    Module *two = createKey(background, key_2, one, five, 1);
    Module *three = createKey(background, key_3, two, five, 1);

    createKey(background, key_0, del, three, 3.5);

    keyboard->keyboard_complete = false;

    module_draw(keyboard->display, root);
    while (!keyboard->keyboard_complete)
    {

        module_draw(keyboard->display, topBar);
        do
        {
            display_update_touch(keyboard->display);
        } while (module_touch_check(root, keyboard->display, keyboard) == 0);

        char *temp = malloc(sizeof(char) * (strlen(keyboard->keyboard_buffer) + strlen(prompt) + 2));
        strcpy(temp, prompt);
        strcat(temp, keyboard->keyboard_buffer);
        module_set_text(topBarText, temp);
        module_set_font(topBarText, RA8876_CHAR_HEIGHT_32);
        module_align_center(topBarText);
        module_align_middle(topBarText);
        free(temp);
    }
    display_bte_memory_copy(keyboard->display, PAGE3_START_ADDR, SCREEN_WIDTH, 0, 0, PAGE1_START_ADDR, SCREEN_WIDTH, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    if (keyboard->keyboard_buffer == NULL)
    {
        return NULL;
    }

    char *toReturn = (char *)malloc(sizeof(char) * (strlen(keyboard->keyboard_buffer) + 1));
    strcpy(toReturn, keyboard->keyboard_buffer);
    module_destroy(root);
    return toReturn;
}
