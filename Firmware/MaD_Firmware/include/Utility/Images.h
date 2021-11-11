
#ifndef Images_H
#define Images_H
#ifdef __MEMORY_CHECK__
#include "leak_detector_c.h"
#endif
#include "RA8876.h"
#include "LoadingOverlay.h"

typedef struct assets_s
{
    Image keyboard;
    Image navigation;
    Image button_wide;
    Image button_wide_border;
    Image manual;
    Image automatic;
} Assets;

/**
 * @brief Creates Image Objects in preparation for loading
 * 
 */
void image_create_assets();

/**
 * @brief Loads images from MaD SD card to display SDRAM
 * 
 */

void image_load_assets(Display *display, Assets *assets);

#endif
