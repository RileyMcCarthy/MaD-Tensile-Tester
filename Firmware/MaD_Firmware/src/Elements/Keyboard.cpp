#include "Keyboard.h"
#include "Images.h"
#define BUTTONCOUNT 47

/**
 * @brief Keyboard page, renders a keyboard to get user input.
 * 
 */

/**
 * @brief Update page from button inputs
 * 
 * @param key The Button that was pressed
 */
void Keyboard::checkButtons(Button *keys)
{
    if (display->checkButtons(keys, BUTTONCOUNT) > 0)
    {
        for (int i = 0; i < BUTTONCOUNT; i++)
        {
            if (keys[i].pressed)
            {
                switch (keys[i].name)
                {
                case key_0:
                    strcat(buffer, "0");
                    break;
                case key_1:
                    strcat(buffer, "1");
                    break;
                case key_2:
                    strcat(buffer, "2");
                    break;
                case key_3:
                    strcat(buffer, "3");
                    break;
                case key_4:
                    strcat(buffer, "4");
                    break;
                case key_5:
                    strcat(buffer, "5");
                    break;
                case key_6:
                    strcat(buffer, "6");
                    break;
                case key_7:
                    strcat(buffer, "7");
                    break;
                case key_8:
                    strcat(buffer, "8");
                    break;
                case key_9:
                    strcat(buffer, "9");
                    break;
                case key_tab:
                    strcat(buffer, "\t");
                    break;
                case key_q:
                    strcat(buffer, "Q");
                    break;
                case key_w:
                    strcat(buffer, "W");
                    break;
                case key_e:
                    strcat(buffer, "E");
                    break;
                case key_r:
                    strcat(buffer, "R");
                    break;
                case key_t:
                    strcat(buffer, "T");
                    break;
                case key_y:
                    strcat(buffer, "Y");
                    break;
                case key_u:
                    strcat(buffer, "U");
                    break;
                case key_i:
                    strcat(buffer, "I");
                    break;
                case key_o:
                    strcat(buffer, "O");
                    break;
                case key_p:
                    strcat(buffer, "P");
                    break;
                case key_caps:
                    break;
                case key_a:
                    strcat(buffer, "A");
                    break;
                case key_s:
                    strcat(buffer, "S");
                    break;
                case key_d:
                    strcat(buffer, "D");
                    break;
                case key_f:
                    strcat(buffer, "F");
                    break;
                case key_g:
                    strcat(buffer, "G");
                    break;
                case key_h:
                    strcat(buffer, "H");
                    break;
                case key_j:
                    strcat(buffer, "J");
                    break;
                case key_k:
                    strcat(buffer, "K");
                    break;
                case key_l:
                    strcat(buffer, "L");
                    break;
                case key_del:
                    if (strlen(buffer) <= 0)
                        break;
                    buffer[strlen(buffer) - 1] = '\0';
                    break;
                case key_shift:
                    break;
                case key_z:
                    strcat(buffer, "Z");
                    break;
                case key_x:
                    strcat(buffer, "X");
                    break;
                case key_c:
                    strcat(buffer, "C");
                    break;
                case key_v:
                    strcat(buffer, "V");
                    break;
                case key_b:
                    strcat(buffer, "B");
                    break;
                case key_n:
                    strcat(buffer, "N");
                    break;
                case key_m:
                    strcat(buffer, "M");
                    break;
                case key_comma:
                    strcat(buffer, ",");
                    break;
                case key_period:
                    strcat(buffer, ".");
                    break;
                case key_cancel:
                    buffer = NULL;
                    complete = true;
                    break;
                case key_left:
                    break;
                case key_space:
                    strcat(buffer, " ");
                    break;
                case key_right:
                    break;
                case key_submit:
                    complete = true;
                    break;
                default:
                    break;
                }
            }
        }
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
static Button createKey(int name, int xmin, int ymin, float keySize)
{
    Button key;
    key.name = name;
    key.xmin = xmin;
    key.xmax = key.xmin + (int)(54.0 * keySize);
    key.ymin = ymin;
    key.ymax = key.ymin + (int)(54.0 * keySize);
    key.pressed = false;
    key.lastPress = 0;
    return key;
}

/**
 * @brief Gets user input using on display keyboard
 * 
 * @param input String buffer for user input
 */
void Keyboard::getKeyboardInput(Ra8876_Lite *the_display, char *input)
{
    display = the_display;
    complete = false;
    buffer = input;
    strcpy(buffer, "");
    int width = 1026;
    int height = 284;
    int startx = 0;
    int starty = SCREEN_HEIGHT - height;

    display->textColor(COLOR65K_WHITE, COLOR65K_BLUE);
    display->setTextParameter1(RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_32, RA8876_SELECT_8859_1);
    display->setTextParameter2(RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
    for (int i = 0; i < height; i += 10)
    {
        display->bteMemoryCopyImage(keyboardImg, startx, SCREEN_HEIGHT - i);
    }
    display->bteMemoryCopyImage(keyboardImg, startx, starty);

    Button keys[BUTTONCOUNT];
    keys[0] = createKey(key_1, startx + 823, starty + 148, 1);
    keys[1] = createKey(key_2, startx + 891, starty + 148, 1);
    keys[2] = createKey(key_3, startx + 958, starty + 148, 1);
    keys[3] = createKey(key_4, startx + 823, starty + 81, 1);
    keys[4] = createKey(key_5, startx + 891, starty + 81, 1);
    keys[5] = createKey(key_6, startx + 958, starty + 81, 1);
    keys[6] = createKey(key_7, startx + 823, starty + 14, 1);
    keys[7] = createKey(key_8, startx + 891, starty + 14, 1);
    keys[8] = createKey(key_9, startx + 958, starty + 14, 1);
    keys[9] = createKey(key_0, startx + 823, starty + 216, 3.5);

    keys[10] = createKey(key_tab, startx + 13, starty + 14, 1.25);
    keys[11] = createKey(key_q, startx + 94, starty + 14, 1);
    keys[12] = createKey(key_w, startx + 162, starty + 14, 1);
    keys[13] = createKey(key_e, startx + 229, starty + 14, 1);
    keys[14] = createKey(key_r, startx + 297, starty + 14, 1);
    keys[15] = createKey(key_t, startx + 364, starty + 14, 1);
    keys[16] = createKey(key_y, startx + 432, starty + 14, 1);
    keys[17] = createKey(key_u, startx + 499, starty + 14, 1);
    keys[18] = createKey(key_i, startx + 567, starty + 14, 1);
    keys[19] = createKey(key_o, startx + 634, starty + 14, 1);
    keys[20] = createKey(key_p, startx + 702, starty + 14, 1);

    keys[21] = createKey(key_caps, startx + 702, starty + 14, 1.5);
    keys[22] = createKey(key_a, startx + 121, starty + 81, 1);
    keys[23] = createKey(key_s, startx + 189, starty + 81, 1);
    keys[24] = createKey(key_d, startx + 256, starty + 81, 1);
    keys[25] = createKey(key_f, startx + 324, starty + 81, 1);
    keys[26] = createKey(key_g, startx + 391, starty + 81, 1);
    keys[27] = createKey(key_h, startx + 459, starty + 81, 1);
    keys[28] = createKey(key_j, startx + 526, starty + 81, 1);
    keys[29] = createKey(key_k, startx + 594, starty + 81, 1);
    keys[30] = createKey(key_l, startx + 661, starty + 81, 1);
    keys[31] = createKey(key_del, startx + 729, starty + 81, 1.5);

    keys[32] = createKey(key_shift, startx + 13, starty + 148, 2.25);
    keys[33] = createKey(key_z, startx + 162, starty + 148, 1);
    keys[34] = createKey(key_x, startx + 229, starty + 148, 1);
    keys[35] = createKey(key_c, startx + 297, starty + 148, 1);
    keys[36] = createKey(key_v, startx + 364, starty + 148, 1);
    keys[37] = createKey(key_b, startx + 432, starty + 148, 1);
    keys[38] = createKey(key_n, startx + 499, starty + 148, 1);
    keys[39] = createKey(key_m, startx + 567, starty + 148, 1);
    keys[40] = createKey(key_comma, startx + 634, starty + 148, 1);
    keys[41] = createKey(key_period, startx + 702, starty + 148, 1);

    keys[42] = createKey(key_cancel, startx + 14, starty + 216, 2.75);
    keys[43] = createKey(key_left, startx + 175, starty + 216, 1.25);
    keys[44] = createKey(key_space, startx + 256, starty + 216, 6.25);
    keys[45] = createKey(key_right, startx + 607, starty + 216, 1.25);
    keys[46] = createKey(key_submit, startx + 702, starty + 216, 2);
    complete = false;
    while (!complete)
    {
        checkButtons(keys);
        display->drawSquareFill(startx, starty - 32, startx + width, starty, COLOR65K_GRAYSCALE30);
        display->textColor(COLOR65K_WHITE, COLOR65K_GRAYSCALE30);
        display->putString(SCREEN_WIDTH / 2 - strlen(buffer) * 16, starty - 32, buffer);
        pause(100);
    }
}