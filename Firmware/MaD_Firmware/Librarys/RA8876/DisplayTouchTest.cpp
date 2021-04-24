/***************************************************
//Web: http://www.buydisplay.com
#include "Ra8876_Lite.h"
EastRising Technology Co.,LTD
Examples for ER-TFTM101-1 with Capacitive Touch Panel 
Display is Hardward SPI 4-Wire SPI Interface and 5V Power Supply
Capacitive Touch Panel is I2C Interface
Tested and worked with:
Works with Arduino 1.6.0 IDE  
Works with Arduino Due Board
Note: If you use our company's adapter board, you must remove the R6 resistor on the adapter board, otherwise the capacitive touch screen will not work.
****************************************************/
#include <PropWare/memory/sd.h>
#include <PropWare/filesystem/fat/fatfs.h>
#include <PropWare/filesystem/fat/fatfilereader.h>

#include <stdint.h>
//#include "simpletools.h"
#include "Ra8876_Lite.h"
#define GT9271_INT 7
#define CLK 28
#define DATA 29
#define RA8876_XNSCS 10
#define RA8876_XNRESET 9

uint8_t buf[30];
Ra8876_Lite ra8876liteTouch(RA8876_XNSCS, RA8876_XNRESET, CLK, DATA, GT9271_INT, 8); //int xnscs, int xnreset, int clk, int data, int GT9271_INT
TouchLocation touchLocations[10];
void runDisplayTouch()
{
    if (!ra8876liteTouch.begin())
    {
        while (1)
            ;
    }
    ra8876liteTouch.displayOn(true);
    while (1)
    {
        static uint16_t w = SCREEN_WIDTH;
        static uint16_t h = SCREEN_HEIGHT;

        uint8_t flag = 1;
        uint8_t im;
        double float_data;

        ra8876liteTouch.canvasImageStartAddress(PAGE1_START_ADDR);
        ra8876liteTouch.canvasImageWidth(SCREEN_WIDTH);
        ra8876liteTouch.activeWindowXY(0, 0);
        ra8876liteTouch.activeWindowWH(SCREEN_WIDTH, SCREEN_HEIGHT);
        ra8876liteTouch.drawSquareFill(0, 0, 1023, 599, COLOR65K_BLACK);
        ra8876liteTouch.setTextParameter1(RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_16, RA8876_SELECT_8859_1); //cch
        ra8876liteTouch.setTextParameter2(RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
        ra8876liteTouch.textColor(COLOR65K_WHITE, COLOR65K_RED);

        ra8876liteTouch.putString(10, 20, " www.buydisplay.com  Capacitive touch screen test.Please touch the screen!");
        ra8876liteTouch.textColor(COLOR65K_BLACK, COLOR65K_WHITE);
        ra8876liteTouch.putString(960, 580, " Clear");
        ra8876liteTouch.putString(0, 580, "  Exit");
        print("flag status: %d\n", flag);
        while (flag)
        {

            uint32_t st = ra8876liteTouch.readGT9271TouchLocation(touchLocations, 10);
            if (1)
            {
                //uint8_t count = ra8876liteTouch.readGT9271TouchLocation(touchLocations, 10);
                for (im = 0; im < st; im++)
                {
                    if ((touchLocations[im].x > 960) && (touchLocations[im].y < 20))
                        flag = 0;

                    if ((touchLocations[im].x < 64) && (touchLocations[im].y < 20))
                    {
                        ra8876liteTouch.canvasImageStartAddress(PAGE1_START_ADDR);
                        ra8876liteTouch.canvasImageWidth(SCREEN_WIDTH);
                        ra8876liteTouch.activeWindowXY(0, 0);
                        ra8876liteTouch.activeWindowWH(SCREEN_WIDTH, SCREEN_HEIGHT);
                        ra8876liteTouch.drawSquareFill(0, 0, 1023, 599, COLOR65K_BLACK);
                        ra8876liteTouch.setTextParameter1(RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_16, RA8876_SELECT_8859_1); //cch
                        ra8876liteTouch.setTextParameter2(RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
                        ra8876liteTouch.textColor(COLOR65K_WHITE, COLOR65K_BLACK);
                        ra8876liteTouch.putString(10, 0, " www.buydisplay.com  Capacitive touch screen test.Please touch the screen!");
                        ra8876liteTouch.textColor(COLOR65K_BLACK, COLOR65K_WHITE);
                        ra8876liteTouch.putString(960, 580, " clear");
                        ra8876liteTouch.putString(0, 580, "  Exit");
                    }

                    ra8876liteTouch.setTextParameter1(RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_16, RA8876_SELECT_8859_1); //cch
                    ra8876liteTouch.setTextParameter2(RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
                    ra8876liteTouch.textColor(COLOR65K_WHITE, COLOR65K_BLACK);
                    snprintf((char *)buf, sizeof(buf), "(%3d,%3d)", touchLocations[im].x, touchLocations[im].y);

                    char *str = (char *)buf;
                    print("buf: '%s'\n", str);
                    ra8876liteTouch.textMode(true);
                    ra8876liteTouch.setTextCursor(380, 380 + 16 * im);
                    ra8876liteTouch.ramAccessPrepare();
                    while (*str != '\0')
                    {
                        ra8876liteTouch.checkWriteFifoNotFull();
                        ra8876liteTouch.lcdDataWrite(*str);
                        ++str;
                    }
                    ra8876liteTouch.check2dBusy();
                    ra8876liteTouch.textMode(false);

                    ra8876liteTouch.graphicMode(true);
                    if (im == 0)
                        ra8876liteTouch.drawCircleFill(SCREEN_WIDTH - touchLocations[im].x, SCREEN_HEIGHT - touchLocations[im].y, 3, COLOR65K_RED);
                    else if (im == 1)
                        ra8876liteTouch.drawCircleFill(SCREEN_WIDTH - touchLocations[im].x, SCREEN_HEIGHT - touchLocations[im].y, 3, COLOR65K_GREEN);
                    else if (im == 2)
                        ra8876liteTouch.drawCircleFill(SCREEN_WIDTH - touchLocations[im].x, SCREEN_HEIGHT - touchLocations[im].y, 3, COLOR65K_BLUE);
                    else if (im == 3)
                        ra8876liteTouch.drawCircleFill(SCREEN_WIDTH - touchLocations[im].x, SCREEN_HEIGHT - touchLocations[im].y, 3, COLOR65K_CYAN);
                    else if (im == 4)
                        ra8876liteTouch.drawCircleFill(SCREEN_WIDTH - touchLocations[im].x, SCREEN_HEIGHT - touchLocations[im].y, 3, COLOR65K_MAGENTA);
                    else if (im == 5)
                        ra8876liteTouch.drawCircleFill(SCREEN_WIDTH - touchLocations[im].x, SCREEN_HEIGHT - touchLocations[im].y, 3, COLOR65K_YELLOW);
                    else if (im == 6)
                        ra8876liteTouch.drawCircleFill(SCREEN_WIDTH - touchLocations[im].x, SCREEN_HEIGHT - touchLocations[im].y, 3, COLOR65K_LIGHTGREEN);
                    else if (im == 7)
                        ra8876liteTouch.drawCircleFill(SCREEN_WIDTH - touchLocations[im].x, SCREEN_HEIGHT - touchLocations[im].y, 3, COLOR65K_LIGHTBLUE);
                    else if (im == 8)
                        ra8876liteTouch.drawCircleFill(SCREEN_WIDTH - touchLocations[im].x, SCREEN_HEIGHT - touchLocations[im].y, 3, COLOR65K_LIGHTRED);
                    else if (im == 9)
                        ra8876liteTouch.drawCircleFill(SCREEN_WIDTH - touchLocations[im].x, SCREEN_HEIGHT - touchLocations[im].y, 3, COLOR65K_LIGHTCYAN);

                    ra8876liteTouch.graphicMode(false);
                }
            }
        }
    }
}

void DrawKeyboard()
{
    print("starting\n");
    sd_mount(22, 23, 24, 25);
    if (!ra8876liteTouch.begin())
    {
        while (1)
        {
        }
    }
    ra8876liteTouch.displayOn(true);

    ra8876liteTouch.canvasImageStartAddress(PAGE1_START_ADDR);
    ra8876liteTouch.canvasImageWidth(SCREEN_WIDTH);
    ra8876liteTouch.activeWindowXY(0, 0);
    ra8876liteTouch.activeWindowWH(SCREEN_WIDTH, SCREEN_HEIGHT);
    ra8876liteTouch.drawSquareFill(0, 0, 1023, 599, COLOR65K_BLUE);

    ra8876liteTouch.putPicture_16bpp((SCREEN_WIDTH - 850) / 2, SCREEN_HEIGHT - 351, 851, 351);

    FILE *fp = fopen("keyboard.bin", "r"); // Open a file for writing
    int end = -1;
    while (end != 0)
    {
        char data;
        end = fread(&data, sizeof(char), 1, fp);
        ra8876liteTouch.lcdDataWrite(data);
    }
}