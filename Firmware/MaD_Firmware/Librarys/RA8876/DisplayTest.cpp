#include "Ra8876_Lite.h"
//#include "simpletools.h"
const int RA8876_XNSCS = 10;
const int RA8876_XNRESET = 9;

char stringEnd = '\0';
char string1[] = {0xa6, 0xb0, 0xa4, 0xe9, 0xaa, 0x46, 0xa4, 0xe8, stringEnd}; //BIG5
char string2[] = {0xbb, 0xb6, 0xd3, 0xad, 0xca, 0xb9, 0xd3, 0xc3, stringEnd}; //BG2312

Ra8876_Lite ra8876lite(RA8876_XNSCS, RA8876_XNRESET, 0, 0, 0, 8);

void runDisplay()
{
    high(8); //backlight
    pause(1000);
    if (!ra8876lite.begin())
    {
        print("RA8876 or RA8877 Fail\n");
        while (1)
            ;
    }
    print("RA8876 or RA8877 Pass!");

    ra8876lite.displayOn(true);

    while (1)
    {
        unsigned int i;
        double float_data;
        ra8876lite.canvasImageStartAddress(PAGE1_START_ADDR);
        ra8876lite.canvasImageWidth(SCREEN_WIDTH);
        ra8876lite.activeWindowXY(0, 0);
        ra8876lite.activeWindowWH(SCREEN_WIDTH, SCREEN_HEIGHT);
        ra8876lite.drawSquareFill(0, 0, 1023, 599, COLOR65K_BLUE);

        ra8876lite.setTextParameter1(RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_16, RA8876_SELECT_8859_1); //cch
        ra8876lite.setTextParameter2(RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
        ra8876lite.textColor(COLOR65K_WHITE, COLOR65K_BLACK);
        ra8876lite.putString(10, 0, "Show internal font 8x16   www.buydisplay.com");

        ra8876lite.setTextParameter1(RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1); //cch
        ra8876lite.setTextParameter2(RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
        ra8876lite.textColor(COLOR65K_BLUE, COLOR65K_MAGENTA);
        ra8876lite.putString(10, 26, "Show internal font 12x24  www.buydisplay.com");

        ra8876lite.setTextParameter1(RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_32, RA8876_SELECT_8859_1); //cch
        ra8876lite.setTextParameter2(RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
        ra8876lite.textColor(COLOR65K_RED, COLOR65K_YELLOW);
        ra8876lite.putString(10, 60, "Show internal font 16x32 www.buydisplay.com");

        ra8876lite.setTextParameter1(RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_16, RA8876_SELECT_8859_1); //cch
        ra8876lite.setTextParameter2(RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_ENABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X2, RA8876_TEXT_HEIGHT_ENLARGEMENT_X2);
        ra8876lite.textColor(COLOR65K_WHITE, COLOR65K_RED);
        ra8876lite.putString(10, 102, "font enlarge x2");

        ra8876lite.setTextParameter1(RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_16, RA8876_SELECT_8859_1); //cch
        ra8876lite.setTextParameter2(RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X3, RA8876_TEXT_HEIGHT_ENLARGEMENT_X3);
        ra8876lite.textColor(COLOR65K_WHITE, COLOR65K_RED);
        ra8876lite.putString(10, 144, "font enlarge x3");

        ra8876lite.setTextParameter1(RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_16, RA8876_SELECT_8859_1); //cch
        ra8876lite.setTextParameter2(RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X4, RA8876_TEXT_HEIGHT_ENLARGEMENT_X4);
        ra8876lite.textColor(COLOR65K_WHITE, COLOR65K_LIGHTCYAN);
        ra8876lite.putString(10, 202, "font enlarge x4");

        //used genitop rom

        ra8876lite.setTextParameter1(RA8876_SELECT_EXTERNAL_CGROM, RA8876_CHAR_HEIGHT_16, RA8876_SELECT_8859_1); //cch
        ra8876lite.setTextParameter2(RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_ENABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
        ra8876lite.genitopCharacterRomParameter(RA8876_SERIAL_FLASH_SELECT0, RA8876_SPI_DIV4, RA8876_GT30L24T3Y, RA8876_BIG5, RA8876_GT_FIXED_WIDTH);
        ra8876lite.textColor(COLOR65K_BLACK, COLOR65K_RED);
        ra8876lite.putString(10, 276, "show external GT font 16x16");

        ra8876lite.setTextParameter1(RA8876_SELECT_EXTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1); //cch
        ra8876lite.setTextParameter2(RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_ENABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
        ra8876lite.genitopCharacterRomParameter(RA8876_SERIAL_FLASH_SELECT0, RA8876_SPI_DIV4, RA8876_GT30L24T3Y, RA8876_BIG5, RA8876_GT_VARIABLE_WIDTH_ARIAL);
        ra8876lite.putString(10, 302, "show external GT font 24x24 with Arial font");

        ra8876lite.putString(10, 336, string1);
        ra8876lite.setTextParameter1(RA8876_SELECT_EXTERNAL_CGROM, RA8876_CHAR_HEIGHT_24, RA8876_SELECT_8859_1); //cch
        ra8876lite.genitopCharacterRomParameter(RA8876_SERIAL_FLASH_SELECT0, RA8876_SPI_DIV4, RA8876_GT30L24T3Y, RA8876_GB2312, RA8876_GT_FIXED_WIDTH);
        ra8876lite.putString(10, 370, string2);

        //while(1);
    }
}
