#include "Images.h"
#include <stdlib.h>
Images *create_images()
{
    Images *images = (Images *)malloc(sizeof(Images));

    images->keyboardImage = (Image *)malloc(sizeof(Image));
    if (images->keyboardImage == NULL)
    {
        printf("Error: malloc failed\n");
        return NULL;
    }
    char *keyboardName = "keyboard.bin";
    images->keyboardImage->name = (char *)malloc(strlen(keyboardName) + 1);
    strcpy((images->keyboardImage->name), keyboardName);
    images->keyboardImage->page = 2;
    images->keyboardImage->width = 1026;
    images->keyboardImage->height = 284;
    images->keyboardImage->x0 = 0;
    images->keyboardImage->y0 = SCREEN_HEIGHT - images->keyboardImage->height;
    images->keyboardImage->backgroundColor = NULL;
    printf("Name:%s\n", (images->keyboardImage->name));

    images->manualPageImage = (Image *)malloc(sizeof(Image));
    char *manualPageImageName = "manual.bin";
    images->manualPageImage->name = (char *)malloc(strlen(manualPageImageName) + 1);
    strcpy((images->manualPageImage->name), manualPageImageName);
    images->manualPageImage->page = 2;
    images->manualPageImage->width = 200;
    images->manualPageImage->height = 200;
    images->manualPageImage->x0 = 0;
    images->manualPageImage->y0 = 0;
    images->manualPageImage->backgroundColor = 0xf800;

    images->automaticPageImage = (Image *)malloc(sizeof(Image));
    char *automaticPageImageName = "auto.bin";
    images->automaticPageImage->name = (char *)malloc(strlen(automaticPageImageName) + 1);
    strcpy((images->automaticPageImage->name), automaticPageImageName);
    images->automaticPageImage->page = 2;
    images->automaticPageImage->width = 200;
    images->automaticPageImage->height = 200;
    images->automaticPageImage->x0 = 200;
    images->automaticPageImage->y0 = 0;
    images->automaticPageImage->backgroundColor = 0xf800;

    images->statusPageImage = (Image *)malloc(sizeof(Image));
    char *statusPageImageName = "status.bin";
    images->statusPageImage->name = (char *)malloc(strlen(statusPageImageName) + 1);
    strcpy((images->statusPageImage->name), statusPageImageName);
    images->statusPageImage->page = 2;
    images->statusPageImage->width = 200;
    images->statusPageImage->height = 200;
    images->statusPageImage->x0 = 400;
    images->statusPageImage->y0 = 0;
    images->statusPageImage->backgroundColor = 0xf800;

    images->calibratePageImage = (Image *)malloc(sizeof(Image));
    char *calibratePageImageName = "calib.bin";
    images->calibratePageImage->name = (char *)malloc(strlen(calibratePageImageName) + 1);
    strcpy((images->calibratePageImage->name), calibratePageImageName);
    images->calibratePageImage->page = 2;
    images->calibratePageImage->width = 200;
    images->calibratePageImage->height = 200;
    images->calibratePageImage->x0 = 600;
    images->calibratePageImage->y0 = 0;
    images->calibratePageImage->backgroundColor = 0xf800;

    images->filesPageImage = (Image *)malloc(sizeof(Image));
    char *filesPageImageName = "files.bin";
    images->filesPageImage->name = (char *)malloc(strlen(filesPageImageName) + 1);
    strcpy((images->filesPageImage->name), filesPageImageName);
    images->filesPageImage->page = 2;
    images->filesPageImage->width = 200;
    images->filesPageImage->height = 200;
    images->filesPageImage->x0 = 800;
    images->filesPageImage->y0 = 0;
    images->filesPageImage->backgroundColor = 0xf800;

    images->navigationImage = (Image *)malloc(sizeof(Image));
    char *navigationImageName = "nav.bin";
    images->navigationImage->name = (char *)malloc(strlen(navigationImageName) + 1);
    strcpy((images->navigationImage->name), navigationImageName);
    images->navigationImage->page = 2;
    images->navigationImage->width = 100;
    images->navigationImage->height = 100;
    images->navigationImage->x0 = 0;
    images->navigationImage->y0 = 200;
    images->navigationImage->backgroundColor = 0xf800;

    images->successImage = (Image *)malloc(sizeof(Image));
    char *successImageName = "check.bin";
    images->successImage->name = (char *)malloc(strlen(successImageName) + 1);
    strcpy((images->successImage->name), successImageName);
    images->successImage->page = 2;
    images->successImage->width = 25;
    images->successImage->height = 25;
    images->successImage->x0 = 100;
    images->successImage->y0 = 200;
    images->successImage->backgroundColor = 0xf800;

    images->failImage = (Image *)malloc(sizeof(Image));
    char *failImageName = "ex.bin";
    images->failImage->name = (char *)malloc(strlen(failImageName) + 1);
    strcpy((images->failImage->name), failImageName);
    images->failImage->page = 2;
    images->failImage->width = 25;
    images->failImage->height = 25;
    images->failImage->x0 = 100;
    images->failImage->y0 = 225;
    images->failImage->backgroundColor = 0x07e0;
    printf("%d\n", images->failImage->width);
    printf("Name:%s\n", (images->keyboardImage->name));
    printf("Name:%s\n", (images->manualPageImage->name));
    return images;
}

void image_load_assets(Images *images, Display *display)
{
    loading_overlay_display(display, "Loading Image: nav", OVERLAY_TYPE_LOADING);
    display_load_image(display, images->navigationImage);

    loading_overlay_display(display, "Loading Image: manual page", OVERLAY_TYPE_LOADING);
    display_load_image(display, images->manualPageImage);

    loading_overlay_display(display, "Loading Image: automatic page", OVERLAY_TYPE_LOADING);
    display_load_image(display, images->automaticPageImage);

    loading_overlay_display(display, "Loading Image: status page", OVERLAY_TYPE_LOADING);
    display_load_image(display, images->statusPageImage);

    loading_overlay_display(display, "Loading Image: calibrate page", OVERLAY_TYPE_LOADING);
    display_load_image(display, images->calibratePageImage);

    loading_overlay_display(display, "Loading Image: files page", OVERLAY_TYPE_LOADING);
    display_load_image(display, images->filesPageImage);

    loading_overlay_display(display, "Loading Image: check status", OVERLAY_TYPE_LOADING);
    display_load_image(display, images->successImage);

    loading_overlay_display(display, "Loading Image: fail status", OVERLAY_TYPE_LOADING);
    display_load_image(display, images->failImage);
}
