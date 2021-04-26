#ifndef Keyboard_H
#define Keyboard_H
#include "simpletools.h"
#include "Ra8876_Lite.h"

/**
 * @brief The keyboard class contains an ondisplay keyboard that can be used to gather user input
 *
 **/
class Keyboard
{
public:
    void getKeyboardInput(Ra8876_Lite *display, char *input);
    void checkButtons(Button *keys);

private:
    bool complete;
    char *buffer;
    Ra8876_Lite *display;
    enum keys
    {
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
};

#endif