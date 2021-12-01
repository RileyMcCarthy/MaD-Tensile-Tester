#include "Images.h"

static Image keyboardImage;
static Image navigationImage;
static Image manualPageImage;
static Image statusPageImage;
static Image automaticPageImage;
static Image successImage;
static Image failImage;

Image *image_get_keyboard()
{
    return &keyboardImage;
}

Image *image_get_navigation()
{
    return &navigationImage;
}

Image *image_get_manual()
{
    return &manualPageImage;
}
Image *image_get_status()
{
    return &statusPageImage;
}

Image *image_get_automatic()
{
    return &automaticPageImage;
}

Image *image_get_success()
{
    return &successImage;
}

Image *image_get_fail()
{
    return &failImage;
}

static void image_create_assets()
{

    strcpy(keyboardImage.name, "keyboard.bin");
    keyboardImage.page = 3;
    keyboardImage.width = 1026;
    keyboardImage.height = 284;
    keyboardImage.x0 = 0;
    keyboardImage.y0 = SCREEN_HEIGHT - keyboardImage.height;
    keyboardImage.backgroundColor = NULL;

    strcpy(navigationImage.name, "nav.bin");
    navigationImage.page = 3;
    navigationImage.width = 100;
    navigationImage.height = 100;
    navigationImage.x0 = 0;
    navigationImage.y0 = 0;
    navigationImage.backgroundColor = 0xf800;

    strcpy(manualPageImage.name, "manual.bin");
    manualPageImage.page = 3;
    manualPageImage.width = 200;
    manualPageImage.height = 200;
    manualPageImage.x0 = 300;
    manualPageImage.y0 = 0;
    manualPageImage.backgroundColor = 0xf800;

    strcpy(automaticPageImage.name, "auto.bin");
    automaticPageImage.page = 3;
    automaticPageImage.width = 200;
    automaticPageImage.height = 200;
    automaticPageImage.x0 = 600;
    automaticPageImage.y0 = 0;
    automaticPageImage.backgroundColor = 0xf800;

    strcpy(statusPageImage.name, "status.bin");
    statusPageImage.page = 3;
    statusPageImage.width = 200;
    statusPageImage.height = 200;
    statusPageImage.x0 = 0;
    statusPageImage.y0 = 300;
    statusPageImage.backgroundColor = 0xf800;

    strcpy(successImage.name, "check.bin");
    successImage.page = 3;
    successImage.width = 25;
    successImage.height = 25;
    successImage.x0 = 900;
    successImage.y0 = 0;
    successImage.backgroundColor = 0xf800;

    strcpy(failImage.name, "ex.bin");
    failImage.page = 3;
    failImage.width = 25;
    failImage.height = 25;
    failImage.x0 = 930;
    failImage.y0 = 0;
    failImage.backgroundColor = 0x07e0;
}

void image_load_assets(Display *display)
{
    image_create_assets();
    // display->loadImage(keyboardImg, &sd);
    display_load_image(display, navigationImage);
    printf("loading button wide\n");

    loading_overlay_display(display, "Loading Image: manual page", OVERLAY_TYPE_LOADING);
    display_load_image(display, manualPageImage);

    loading_overlay_display(display, "Loading Image: automatic page", OVERLAY_TYPE_LOADING);
    display_load_image(display, automaticPageImage);

    loading_overlay_display(display, "Loading Image: status page", OVERLAY_TYPE_LOADING);
    display_load_image(display, statusPageImage);

    loading_overlay_display(display, "Loading Image: check status", OVERLAY_TYPE_LOADING);
    display_load_image(display, successImage);

    loading_overlay_display(display, "Loading Image: fail status", OVERLAY_TYPE_LOADING);
    display_load_image(display, failImage);
}