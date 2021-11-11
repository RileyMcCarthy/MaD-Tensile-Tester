
#include "LoadingOverlay.h"

void loading_overlay_display(Display *display, const char *message, OverlayType type)
{
    display_set_text_parameter1(display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_32, RA8876_SELECT_8859_1);
    display_set_text_parameter2(display, RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
    display_text_color(display, COLOR65K_WHITE, COLOR65K_BLUE);
    display_draw_square_fill(display, SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT / 2 - 100, SCREEN_WIDTH / 2 + 250, SCREEN_HEIGHT / 2 + 100, SECONDARYTEXTCOLOR);
    display_draw_string(display, SCREEN_WIDTH / 2 - strlen(message) * 8, SCREEN_HEIGHT / 2 - 16, message);
    printf("-----------------\nTEMP DISPLAY: %s\n-----------------\n", message);
}
