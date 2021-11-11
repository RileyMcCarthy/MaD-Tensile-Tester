#include "Images.h"

Assets *image_create_assets()
{
    Assets *assets = (Assets *)malloc(sizeof(Assets));

    strcpy(assets->keyboard.name, "keyboard.bin");
    assets->keyboard.page = 3;
    assets->keyboard.width = 1026;
    assets->keyboard.height = 284;
    assets->keyboard.x0 = 0;
    assets->keyboard.y0 = SCREEN_HEIGHT - assets->keyboard.height;
    assets->keyboard.backgroundColor = NULL;

    strcpy(assets->navigation.name, "nav.bin");
    assets->navigation.page = 3;
    assets->navigation.width = 100;
    assets->navigation.height = 100;
    assets->navigation.x0 = 0;
    assets->navigation.y0 = 0;
    assets->navigation.backgroundColor = 0xf800;

    strcpy(assets->button_wide.name, "btnw.bin");
    assets->button_wide.page = 3;
    assets->button_wide.width = 100;
    assets->button_wide.height = 50;
    assets->button_wide.x0 = 150;
    assets->button_wide.y0 = 0;
    assets->button_wide.backgroundColor = 0xf800;

    strcpy(assets->manual.name, "manual.bin");
    assets->manual.page = 3;
    assets->manual.width = 200;
    assets->manual.height = 200;
    assets->manual.x0 = 300;
    assets->manual.y0 = 0;
    assets->manual.backgroundColor = 0xf800;

    strcpy(assets->automatic.name, "auto.bin");
    assets->automatic.page = 3;
    assets->automatic.width = 200;
    assets->automatic.height = 200;
    assets->automatic.x0 = 600;
    assets->automatic.y0 = 0;
    assets->automatic.backgroundColor = 0xf800;
    return assets;
}

void image_load_assets(Display *display, Assets *assets)
{
    //display->loadImage(keyboardImg, &sd);
    display_load_image(display, assets->navigation);
    printf("loading button wide\n");
    loading_overlay_display(display, "Loading Image: Button Wide", OVERLAY_TYPE_LOADING);
    display_load_image(display, assets->button_wide);

    printf("loading manual page img\n");
    loading_overlay_display(display, "Loading Image: manual page", OVERLAY_TYPE_LOADING);
    display_load_image(display, assets->manual);

    printf("loading automatic page img\n");
    loading_overlay_display(display, "Loading Image: automatic page", OVERLAY_TYPE_LOADING);
    display_load_image(display, assets->automatic);
}
