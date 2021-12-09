
#ifndef Images_H
#define Images_H
#include "RA8876.h"
#include "LoadingOverlay.h"
#include "simpletools.h"
#include <stdlib.h>

typedef struct images_s
{
    Image *keyboardImage;
    Image *navigationImage;
    Image *manualPageImage;
    Image *statusPageImage;
    Image *automaticPageImage;
    Image *calibratePageImage;
    Image *filesPageImage;
    Image *successImage;
    Image *failImage;
    Display *display;
} Images;

Images *create_images();
/**
 * @brief Loads images from MaD SD card to display SDRAM
 *
 */
void image_load_assets(Images *imagesPtr, Display *display);

#endif
