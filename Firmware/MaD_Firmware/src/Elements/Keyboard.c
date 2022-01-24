#include "keyboard.h"
#define BUTTONCOUNT 47

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
    key_caps,
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
static bool check_buttons(Keyboard *keyboard)
{
    display_update_touch(keyboard->display);

    if (display_update_buttons(keyboard->display, keyboard->keys, BUTTONCOUNT) > 0)
    {
        printf("bvutton pressed\n");
        for (int i = 0; i < BUTTONCOUNT; i++)
        {
            if (keyboard->keys[i].pressed)
            {
                int size = strlen(keyboard->keyboard_buffer) + 2;
                printf("new size:%d\n", size);
                keyboard->keyboard_buffer = (char *)realloc(keyboard->keyboard_buffer, size);
                switch (keyboard->keys[i].name)
                {
                case key_0:
                    strcat(keyboard->keyboard_buffer, "0");
                    break;
                case key_1:
                    strcat(keyboard->keyboard_buffer, "1");
                    break;
                case key_2:
                    strcat(keyboard->keyboard_buffer, "2");
                    break;
                case key_3:
                    strcat(keyboard->keyboard_buffer, "3");
                    break;
                case key_4:
                    strcat(keyboard->keyboard_buffer, "4");
                    break;
                case key_5:
                    strcat(keyboard->keyboard_buffer, "5");
                    break;
                case key_6:
                    strcat(keyboard->keyboard_buffer, "6");
                    break;
                case key_7:
                    strcat(keyboard->keyboard_buffer, "7");
                    break;
                case key_8:
                    strcat(keyboard->keyboard_buffer, "8");
                    break;
                case key_9:
                    strcat(keyboard->keyboard_buffer, "9");
                    break;
                case key_tab:
                    strcat(keyboard->keyboard_buffer, "\t");
                    break;
                case key_q:
                    strcat(keyboard->keyboard_buffer, "Q");
                    break;
                case key_w:
                    strcat(keyboard->keyboard_buffer, "W");
                    break;
                case key_e:
                    strcat(keyboard->keyboard_buffer, "E");
                    break;
                case key_r:
                    strcat(keyboard->keyboard_buffer, "R");
                    break;
                case key_t:
                    strcat(keyboard->keyboard_buffer, "T");
                    break;
                case key_y:
                    strcat(keyboard->keyboard_buffer, "Y");
                    break;
                case key_u:
                    strcat(keyboard->keyboard_buffer, "U");
                    break;
                case key_i:
                    strcat(keyboard->keyboard_buffer, "I");
                    break;
                case key_o:
                    strcat(keyboard->keyboard_buffer, "O");
                    break;
                case key_p:
                    strcat(keyboard->keyboard_buffer, "P");
                    break;
                case key_caps:
                    break;
                case key_a:
                    strcat(keyboard->keyboard_buffer, "A");
                    break;
                case key_s:
                    strcat(keyboard->keyboard_buffer, "S");
                    break;
                case key_d:
                    strcat(keyboard->keyboard_buffer, "D");
                    break;
                case key_f:
                    strcat(keyboard->keyboard_buffer, "F");
                    break;
                case key_g:
                    strcat(keyboard->keyboard_buffer, "G");
                    break;
                case key_h:
                    strcat(keyboard->keyboard_buffer, "H");
                    break;
                case key_j:
                    strcat(keyboard->keyboard_buffer, "J");
                    break;
                case key_k:
                    strcat(keyboard->keyboard_buffer, "K");
                    break;
                case key_l:
                    strcat(keyboard->keyboard_buffer, "L");
                    break;
                case key_del:
                    if (strlen(keyboard->keyboard_buffer) <= 0)
                        break;
                    keyboard->keyboard_buffer[strlen(keyboard->keyboard_buffer) - 1] = '\0';
                    keyboard->keyboard_buffer = (char *)realloc(keyboard->keyboard_buffer, strlen(keyboard->keyboard_buffer) + 1);
                    break;
                case key_shift:
                    break;
                case key_z:
                    strcat(keyboard->keyboard_buffer, "Z");
                    break;
                case key_x:
                    strcat(keyboard->keyboard_buffer, "X");
                    break;
                case key_c:
                    strcat(keyboard->keyboard_buffer, "C");
                    break;
                case key_v:
                    strcat(keyboard->keyboard_buffer, "V");
                    break;
                case key_b:
                    strcat(keyboard->keyboard_buffer, "B");
                    break;
                case key_n:
                    strcat(keyboard->keyboard_buffer, "N");
                    break;
                case key_m:
                    strcat(keyboard->keyboard_buffer, "M");
                    break;
                case key_comma:
                    strcat(keyboard->keyboard_buffer, ",");
                    break;
                case key_period:
                    strcat(keyboard->keyboard_buffer, ".");
                    break;
                case key_cancel:
                    free(keyboard->keyboard_buffer);
                    keyboard->keyboard_buffer = NULL;
                    keyboard->keyboard_complete = true;
                    break;
                case key_left:
                    break;
                case key_space:
                    strcat(keyboard->keyboard_buffer, " ");
                    break;
                case key_right:
                    break;
                case key_submit:
                    keyboard->keyboard_complete = true;
                    break;
                default:
                    break;
                }
                return true;
            }
        }
    }
    return false;
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
static void createKey(Button *button, int name, int xmin, int ymin, float keySize)
{
    button->name = name;
    button->xmin = xmin;
    button->xmax = button->xmin + (int)(54.0 * keySize);
    button->ymin = ymin;
    button->ymax = button->ymin + (int)(54.0 * keySize);
    button->pressed = false;
    button->debounceTimems = 100;
    button->lastPress = 0;
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
char *keyboard_get_input(Keyboard *keyboardObj, const char *prompt)
{
    Image *keyboard = keyboardObj->images->keyboardImage;
    int topBarSize = 32;
    keyboardObj->keyboard_buffer = (char *)malloc(sizeof(char));
    strcpy(keyboardObj->keyboard_buffer, "");

    display_text_color(keyboardObj->display, COLOR65K_WHITE, COLOR65K_BLUE);
    display_set_text_parameter1(keyboardObj->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_32, RA8876_SELECT_8859_1);
    display_set_text_parameter2(keyboardObj->display, RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);

    display_bte_memory_copy(keyboardObj->display, PAGE1_START_ADDR, SCREEN_WIDTH, keyboard->x0, keyboard->y0 - topBarSize, PAGE3_START_ADDR, SCREEN_WIDTH, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    for (int i = 0; i < keyboard->height; i += 10)
    {
        display_bte_memory_copy_image(keyboardObj->display, keyboard, keyboard->x0, SCREEN_HEIGHT - i);
    }
    display_bte_memory_copy_image(keyboardObj->display, keyboard, keyboard->x0, keyboard->y0);
    Button keys[BUTTONCOUNT];
    createKey(&keys[0], key_1, keyboard->x0 + 823, keyboard->y0 + 148, 1);
    createKey(&keys[1], key_2, keyboard->x0 + 891, keyboard->y0 + 148, 1);
    createKey(&keys[2], key_3, keyboard->x0 + 958, keyboard->y0 + 148, 1);
    createKey(&keys[3], key_4, keyboard->x0 + 823, keyboard->y0 + 81, 1);
    createKey(&keys[4], key_5, keyboard->x0 + 891, keyboard->y0 + 81, 1);
    createKey(&keys[5], key_6, keyboard->x0 + 958, keyboard->y0 + 81, 1);
    createKey(&keys[6], key_7, keyboard->x0 + 823, keyboard->y0 + 14, 1);
    createKey(&keys[7], key_8, keyboard->x0 + 891, keyboard->y0 + 14, 1);
    createKey(&keys[8], key_9, keyboard->x0 + 958, keyboard->y0 + 14, 1);
    createKey(&keys[9], key_0, keyboard->x0 + 823, keyboard->y0 + 216, 3.5);

    createKey(&keys[10], key_tab, keyboard->x0 + 13, keyboard->y0 + 14, 1.25);
    createKey(&keys[11], key_q, keyboard->x0 + 94, keyboard->y0 + 14, 1);
    createKey(&keys[12], key_w, keyboard->x0 + 162, keyboard->y0 + 14, 1);
    createKey(&keys[13], key_e, keyboard->x0 + 229, keyboard->y0 + 14, 1);
    createKey(&keys[14], key_r, keyboard->x0 + 297, keyboard->y0 + 14, 1);
    createKey(&keys[15], key_t, keyboard->x0 + 364, keyboard->y0 + 14, 1);
    createKey(&keys[16], key_y, keyboard->x0 + 432, keyboard->y0 + 14, 1);
    createKey(&keys[17], key_u, keyboard->x0 + 499, keyboard->y0 + 14, 1);
    createKey(&keys[18], key_i, keyboard->x0 + 567, keyboard->y0 + 14, 1);
    createKey(&keys[19], key_o, keyboard->x0 + 634, keyboard->y0 + 14, 1);
    createKey(&keys[20], key_p, keyboard->x0 + 702, keyboard->y0 + 14, 1);

    createKey(&keys[21], key_caps, keyboard->x0 + 702, keyboard->y0 + 14, 1.5);
    createKey(&keys[22], key_a, keyboard->x0 + 121, keyboard->y0 + 81, 1);
    createKey(&keys[23], key_s, keyboard->x0 + 189, keyboard->y0 + 81, 1);
    createKey(&keys[24], key_d, keyboard->x0 + 256, keyboard->y0 + 81, 1);
    createKey(&keys[25], key_f, keyboard->x0 + 324, keyboard->y0 + 81, 1);
    createKey(&keys[26], key_g, keyboard->x0 + 391, keyboard->y0 + 81, 1);
    createKey(&keys[27], key_h, keyboard->x0 + 459, keyboard->y0 + 81, 1);
    createKey(&keys[28], key_j, keyboard->x0 + 526, keyboard->y0 + 81, 1);
    createKey(&keys[29], key_k, keyboard->x0 + 594, keyboard->y0 + 81, 1);
    createKey(&keys[30], key_l, keyboard->x0 + 661, keyboard->y0 + 81, 1);
    createKey(&keys[31], key_del, keyboard->x0 + 729, keyboard->y0 + 81, 1.5);

    createKey(&keys[32], key_shift, keyboard->x0 + 13, keyboard->y0 + 148, 2.25);
    createKey(&keys[33], key_z, keyboard->x0 + 162, keyboard->y0 + 148, 1);
    createKey(&keys[34], key_x, keyboard->x0 + 229, keyboard->y0 + 148, 1);
    createKey(&keys[35], key_c, keyboard->x0 + 297, keyboard->y0 + 148, 1);
    createKey(&keys[36], key_v, keyboard->x0 + 364, keyboard->y0 + 148, 1);
    createKey(&keys[37], key_b, keyboard->x0 + 432, keyboard->y0 + 148, 1);
    createKey(&keys[38], key_n, keyboard->x0 + 499, keyboard->y0 + 148, 1);
    createKey(&keys[39], key_m, keyboard->x0 + 567, keyboard->y0 + 148, 1);
    createKey(&keys[40], key_comma, keyboard->x0 + 634, keyboard->y0 + 148, 1);
    createKey(&keys[41], key_period, keyboard->x0 + 702, keyboard->y0 + 148, 1);

    createKey(&keys[42], key_cancel, keyboard->x0 + 14, keyboard->y0 + 216, 2.75);
    createKey(&keys[43], key_left, keyboard->x0 + 175, keyboard->y0 + 216, 1.25);
    createKey(&keys[44], key_space, keyboard->x0 + 256, keyboard->y0 + 216, 6.25);
    createKey(&keys[45], key_right, keyboard->x0 + 607, keyboard->y0 + 216, 1.25);
    createKey(&keys[46], key_submit, keyboard->x0 + 702, keyboard->y0 + 216, 2);

    keyboardObj->keys = keys;

    keyboardObj->keyboard_complete = false;
    bool initial = true;
    display_update_buttons(keyboardObj->display, keyboardObj->keys, BUTTONCOUNT); // Clear button presses
    while (!keyboardObj->keyboard_complete)
    {
        if (check_buttons(keyboardObj) || initial)
        {
            initial = false;
            char *temp = malloc(sizeof(char) * (strlen(keyboardObj->keyboard_buffer) + strlen(prompt) + 2));
            strcpy(temp, prompt);
            strcat(temp, keyboardObj->keyboard_buffer);
            display_draw_square_fill(keyboardObj->display, keyboard->x0, keyboard->y0 - topBarSize, keyboard->x0 + keyboard->width, keyboard->y0, COLOR65K_GRAYSCALE30);
            display_text_color(keyboardObj->display, COLOR65K_WHITE, COLOR65K_GRAYSCALE30);
            display_draw_string(keyboardObj->display, SCREEN_WIDTH / 2 - strlen(temp) * 8, keyboard->y0 - topBarSize, temp);
            free(temp);
        }
    }
    display_bte_memory_copy(keyboardObj->display, PAGE3_START_ADDR, SCREEN_WIDTH, 0, 0, PAGE1_START_ADDR, SCREEN_WIDTH, keyboard->x0, keyboard->y0 - topBarSize, SCREEN_WIDTH, SCREEN_HEIGHT);

    if (keyboardObj->keyboard_buffer == NULL)
    {
        return NULL;
    }
    char *toReturn = (char *)malloc(sizeof(char) * (strlen(keyboardObj->keyboard_buffer) + 1));
    strcpy(toReturn, keyboardObj->keyboard_buffer);
    return toReturn;
}
