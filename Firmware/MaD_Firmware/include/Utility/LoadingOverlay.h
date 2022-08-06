#include <simpletools.h>
#include "RA8876.h"
#include "Style.h"
typedef enum OverlayType_e
{
    OVERLAY_TYPE_REGULAR,
    OVERLAY_TYPE_LOADING,
    OVERLAY_TYPE_ERROR,
} OverlayType;

void loading_overlay_display(Display *display, const char *message, OverlayType type);
