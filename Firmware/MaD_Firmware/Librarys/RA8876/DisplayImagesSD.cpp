#include "Ra8876_Lite.h"
#include <stdint.h>
//#include "simpletools.h"
#include "Ra8876_Lite.h"
#include "pattern6.h"
#define GT9271_INT 7
#define CLK 28
#define DATA 29
#define RA8876_XNSCS 10
#define RA8876_XNRESET 9

Ra8876_Lite ra8876lite(RA8876_XNSCS, RA8876_XNRESET, CLK, DATA, GT9271_INT, 8); //int xnscs, int xnreset, int clk, int data, int GT9271_INT
void loop();
void DrawImageTest()
{
    print("starting\n");
    sd_mount(22, 23, 24, 25);
    if (!ra8876lite.begin())
    {
        while (1)
        {
        }
    }
    ra8876lite.displayOn(true);

    while (1)
    {
        loop();
    }
}

void loop()
{
    unsigned short i;
    /***bte memory copy demo***/
    //clear page1
    ra8876lite.canvasImageStartAddress(PAGE1_START_ADDR);
    ra8876lite.canvasImageWidth(SCREEN_WIDTH);

    ra8876lite.activeWindowXY(0, 0);
    ra8876lite.activeWindowWH(SCREEN_WIDTH, SCREEN_HEIGHT);
    ra8876lite.drawSquareFill(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, COLOR65K_BLUE);

    //clear page2
    ra8876lite.canvasImageStartAddress(PAGE2_START_ADDR);
    ra8876lite.drawSquareFill(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, COLOR65K_RED);

    //write picture to page2

    //write string to page1
    ra8876lite.canvasImageStartAddress(PAGE1_START_ADDR);
    ra8876lite.textColor(COLOR65K_WHITE, COLOR65K_BLACK);
    ra8876lite.setTextParameter1(RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1); //cch
    ra8876lite.setTextParameter2(RA8876_TEXT_FULL_ALIGN_ENABLE, RA8876_TEXT_CHROMA_KEY_ENABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
    ra8876lite.putString(0, 0, "BTE memory copy,copy page2 picture to page1 display");
    //copy page2 picture to page1
    ra8876lite.bteMemoryCopy(PAGE2_START_ADDR, SCREEN_WIDTH, 50, 50, PAGE1_START_ADDR, SCREEN_WIDTH, 50, 50, 128, 128);
    ra8876lite.bteMemoryCopy(PAGE2_START_ADDR, SCREEN_WIDTH, 50, 50, PAGE1_START_ADDR, SCREEN_WIDTH, (50 + 128), 50, 128, 128);
    ra8876lite.bteMemoryCopy(PAGE2_START_ADDR, SCREEN_WIDTH, 50, 50, PAGE1_START_ADDR, SCREEN_WIDTH, (50 + 128 + 128), 50, 128, 128);
    //ra8876lite.mainImageStartAddress(PAGE2_START_ADDR);
    pause(3000);
    //while(1);
    /***bte memory copy with rop demo***/
    //write string to page1
    ra8876lite.canvasImageStartAddress(PAGE1_START_ADDR);
    ra8876lite.putString(0, 178, "BTE memory copy with ROP, copy page2 picture to page1 display");
    ra8876lite.bteMemoryCopyWithROP(PAGE2_START_ADDR, SCREEN_WIDTH, 50, 50, PAGE1_START_ADDR, SCREEN_WIDTH, 50, 228,
                                    PAGE1_START_ADDR, SCREEN_WIDTH, 50, 228, 128, 128, RA8876_BTE_ROP_CODE_1);
    ra8876lite.bteMemoryCopyWithROP(PAGE2_START_ADDR, SCREEN_WIDTH, 50, 50, PAGE1_START_ADDR, SCREEN_WIDTH, (50 + 128), 228,
                                    PAGE1_START_ADDR, SCREEN_WIDTH, (50 + 128), 228, 128, 128, RA8876_BTE_ROP_CODE_2);
    ra8876lite.bteMemoryCopyWithROP(PAGE2_START_ADDR, SCREEN_WIDTH, 50, 50, PAGE1_START_ADDR, SCREEN_WIDTH, (50 + 128 + 128), 228,
                                    PAGE1_START_ADDR, SCREEN_WIDTH, (50 + 128 + 128), 228, 128, 128, RA8876_BTE_ROP_CODE_3);
    /***bte memory copy with chromakey demo***/
    ra8876lite.putString(0, 356, "BTE memory copy with ChromaKey, copy page2 picture to page1 display");
    ra8876lite.bteMemoryCopyWithChromaKey(PAGE2_START_ADDR, SCREEN_WIDTH, 50, 50, PAGE1_START_ADDR, SCREEN_WIDTH, 50, 406, 128, 128, 0xf800);
    ra8876lite.bteMemoryCopyWithChromaKey(PAGE2_START_ADDR, SCREEN_WIDTH, 50, 50, PAGE1_START_ADDR, SCREEN_WIDTH, 50 + 128, 406, 128, 128, 0xf800);
    ra8876lite.bteMemoryCopyWithChromaKey(PAGE2_START_ADDR, SCREEN_WIDTH, 50, 50, PAGE1_START_ADDR, SCREEN_WIDTH, 50 + 128 + 128, 406, 128, 128, 0xf800);
    pause(3000);
    //while(1);
    //clear page1
    ra8876lite.canvasImageStartAddress(PAGE1_START_ADDR);
    ra8876lite.canvasImageWidth(SCREEN_WIDTH);
    ra8876lite.activeWindowXY(0, 0);
    ra8876lite.activeWindowWH(SCREEN_WIDTH, SCREEN_HEIGHT);
    ra8876lite.drawSquareFill(0, 0, 1023, 599, COLOR65K_BLUE);

    /***bte MPU write with ROP demo***/
    ra8876lite.textColor(COLOR65K_WHITE, COLOR65K_BLACK);
    ra8876lite.setTextParameter1(RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1); //cch
    ra8876lite.setTextParameter2(RA8876_TEXT_FULL_ALIGN_ENABLE, RA8876_TEXT_CHROMA_KEY_ENABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
    ra8876lite.putString(0, 0, "BTE MPU write with ROP, write picture to page1, format byte");
    ra8876lite.putString(0, 178, "BTE MPU write with ROP, write picture to page1, format word");

    ra8876lite.putString(0, 356, "BTE MPU write with Chroma Key, write picture to page1, format byte,word");
    pause(3000);
    //while(1);
    //clear page1
    ra8876lite.canvasImageStartAddress(PAGE1_START_ADDR);
    ra8876lite.canvasImageWidth(SCREEN_WIDTH);
    ra8876lite.activeWindowXY(0, 0);
    ra8876lite.activeWindowWH(SCREEN_WIDTH, SCREEN_HEIGHT);
    ra8876lite.drawSquareFill(0, 0, 1023, 599, COLOR65K_BLUE);
    /***bte MPU write color expansion demo***/
    ra8876lite.textColor(COLOR65K_WHITE, COLOR65K_BLACK);
    ra8876lite.setTextParameter1(RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1); //cch
    ra8876lite.setTextParameter2(RA8876_TEXT_FULL_ALIGN_ENABLE, RA8876_TEXT_CHROMA_KEY_ENABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
    ra8876lite.textColor(COLOR65K_WHITE, COLOR65K_BLACK);
    ra8876lite.putString(0, 178, "BTE MPU write with color expansion with chroma key, write black and white picture data to page1");
    pause(3000);
    //while(1);
    //clear page1
    ra8876lite.canvasImageStartAddress(PAGE1_START_ADDR);
    ra8876lite.canvasImageWidth(SCREEN_WIDTH);
    ra8876lite.activeWindowXY(0, 0);
    ra8876lite.activeWindowWH(SCREEN_WIDTH, SCREEN_HEIGHT);
    ra8876lite.drawSquareFill(0, 0, 1023, 599, COLOR65K_BLUE);

    //write picture to pattern1 ram
    ra8876lite.canvasImageStartAddress(PATTERN1_RAM_START_ADDR);
    ra8876lite.canvasImageWidth(16);
    ra8876lite.activeWindowXY(0, 0);
    ra8876lite.activeWindowWH(16, 16);
    ra8876lite.putPicture_16bpp(0, 0, 16, 16, pattern6);

    //set canvas and active window back
    ra8876lite.canvasImageStartAddress(PAGE1_START_ADDR);
    ra8876lite.canvasImageWidth(SCREEN_WIDTH);
    ra8876lite.activeWindowWH(SCREEN_WIDTH, SCREEN_HEIGHT);

    /***bte pattern fill demo***/
    ra8876lite.textColor(COLOR65K_WHITE, COLOR65K_BLACK);
    ra8876lite.setTextParameter1(RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1); //cch
    ra8876lite.setTextParameter2(RA8876_TEXT_FULL_ALIGN_ENABLE, RA8876_TEXT_CHROMA_KEY_ENABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
    ra8876lite.putString(0, 0, "BTE pattern fill, fill 16x16 pattern to page1");

    ra8876lite.btePatternFill(1, PATTERN1_RAM_START_ADDR, 16, 0, 0, PAGE1_START_ADDR, SCREEN_WIDTH, 50, 50, 700, 128);

    ra8876lite.btePatternFill(1, PATTERN2_RAM_START_ADDR, 16, 0, 0, PAGE1_START_ADDR, SCREEN_WIDTH, 50, 228, 700, 128);

    ra8876lite.putString(0, 356, "BTE pattern fill with chroma key, fill with chroma key 16x16 pattern to page1");
    ra8876lite.btePatternFillWithChromaKey(1, PATTERN3_RAM_START_ADDR, 16, 0, 0, PAGE1_START_ADDR, SCREEN_WIDTH, 50, 406, 700, 128, 0xe8e4);

    pause(3000);
    //while(1);
}