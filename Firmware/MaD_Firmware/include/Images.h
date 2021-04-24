
#ifndef Images_H
#define Images_H
#include "Ra8876_Lite.h"
#include "MaD.h"

static Image keyboardImg;
static Image navigationImg;
static Image button_wide;
static Image button_wide_border;
static Image manualImg;
static Image automaticImg;

static void loadAssets()
{
    strcpy(keyboardImg.name, "keyboard.bin");
    keyboardImg.page = 3;
    keyboardImg.width = 1026;
    keyboardImg.height = 284;
    keyboardImg.x0 = 0;
    keyboardImg.y0 = SCREEN_HEIGHT - keyboardImg.height;
    keyboardImg.backgroundColor = NULL;

    strcpy(navigationImg.name, "nav.bin");
    navigationImg.page = 3;
    navigationImg.width = 100;
    navigationImg.height = 100;
    navigationImg.x0 = 0;
    navigationImg.y0 = 0;
    navigationImg.backgroundColor = 0xf800;

    strcpy(button_wide.name, "btnw.bin");
    button_wide.page = 3;
    button_wide.width = 100;
    button_wide.height = 50;
    button_wide.x0 = 150;
    button_wide.y0 = 0;
    button_wide.backgroundColor = 0xf800;

    strcpy(manualImg.name, "manual.bin");
    manualImg.page = 3;
    manualImg.width = 200;
    manualImg.height = 200;
    manualImg.x0 = 300;
    manualImg.y0 = 0;
    manualImg.backgroundColor = 0xf800;

    strcpy(automaticImg.name, "auto.bin");
    automaticImg.page = 3;
    automaticImg.width = 200;
    automaticImg.height = 200;
    automaticImg.x0 = 600;
    automaticImg.y0 = 0;
    automaticImg.backgroundColor = 0xf800;
}

static void loadAssets(Ra8876_Lite *display)
{
    libpropeller::SD sd;
    sd.ClearError();
    printf("Mounting SD...\n");
    sd.Mount(MAD_SD_DO, MAD_SD_CLK, MAD_CD_DI, MAD_CD_CS);
    printf("...Mounted\n");
    if (sd.HasError())
    {

        printf("Error mounting sd: %d\n", sd.GetError());
    }
    display->drawSquareFill(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, COLOR65K_BLUE);
    display->setTextParameter1(RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_32, RA8876_SELECT_8859_1);
    display->setTextParameter2(RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
    display->textColor(COLOR65K_WHITE, COLOR65K_BLUE);
    display->putString(SCREEN_WIDTH / 2 - 11 * 8, 300 - 16, "LOADING...");

    //display->loadImage(keyboardImg, &sd);
    display->loadImage(navigationImg, &sd);
    printf("loading button wide\n");
    display->loadImage(button_wide, &sd);
    printf("loading manual page img\n");
    display->loadImage(manualImg, &sd);
    printf("loading automatic page img\n");
    display->loadImage(automaticImg, &sd);
    sd.Unmount();
}

#endif