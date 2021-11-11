
#ifndef Images_H
#define Images_H
#ifdef __MEMORY_CHECK__
#include "leak_detector_c.h"
#endif
#include "RA8876.h"
#include "LoadingOverlay.h"

Image image_get_keyboard();
Image image_get_navigation();
Image image_get_manual();
Image image_get_automatic();
Image image_get_success();
Image image_get_fails()
    /**
 * @brief Loads images from MaD SD card to display SDRAM
 * 
 */

    void image_load_assets(Display *display);

#endif
