#include "AutomaticPage.h"
#include "Motion.h"

enum button_names
{
    BUTTON_MOTION_MODE,
    BUTTON_CONDITION,
    BUTTON_NAVIGATION
};

/* Private Function Declarations */
void checkButtons(Button *buttons);
static void motionCOG(void *par);
void load_graph(AutomaticPage *page);
void draw_graph(AutomaticPage *page);

AutomaticPage *AutomaticPage_create(Display *display, Motion_Cog *cog)
{
    AutomaticPage *page = (AutomaticPage *)malloc(sizeof(AutomaticPage));
    page->display = display;
    page->complete = false;
    return page;
}

void AutomaticPage_destroy(AutomaticPage *page)
{
    free(page);
}

void AutomaticPage_run(AutomaticPage *page)
{

    // dyn4 = p_dyn4;
    // forceGauge = p_forceGauge;
    // sd = p_sd;

    //Init display with blue background
    canvasImageStartAddress(page->display, PAGE1_START_ADDR);
    canvasImageWidth(page->display, SCREEN_WIDTH);
    drawSquareFill(page->display, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BACKCOLOR);

    // clock.begin(&display);

    activeWindowXY(page->display, 0, 0);
    activeWindowWH(page->display, SCREEN_WIDTH, SCREEN_HEIGHT);
    drawSquareFill(page->display, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, COLOR65K_BLUE);
    setTextParameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_32, RA8876_SELECT_8859_1);
    setTextParameter2(page->display, RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
    textColor(page->display, COLOR65K_WHITE, COLOR65K_BLUE);
    drawString(page->display, SCREEN_WIDTH / 2 - 11 * 8, 300 - 16, "LOADING...");
    // display->loadImage("keyboard.bin", 3, 0, SCREEN_HEIGHT - 284, 1026, 284); //loading images adds another 10k bytes (maybe make own sd card system? or fopen)
    loadGraph(page->display, display);
    drawSquareFill(page->display, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BACKCOLOR);
    //setup real time clock

    //Clock
    textColor(page->display, COLOR65K_WHITE, COLOR65K_BLUE);
    setTextParameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_32, RA8876_SELECT_8859_1);
    setTextParameter2(page->display, RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
    char forceVal[100] = {10};
    drawSquareFill(page->display, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BACKCOLOR);
    //display->bteMemoryCopy(PAGE2_START_ADDR, SCREEN_WIDTH, (SCREEN_WIDTH - 500), 0, PAGE1_START_ADDR, SCREEN_WIDTH, (SCREEN_WIDTH - 500), 0, 500, 300);
    char temp[20];
    int samples = 0;
    int lastForce = 0;
    int lastPosition;

    drawSquareFill(page->display, 32, 32, 32 + 428, SCREEN_HEIGHT - 32, MAINCOLOR);
    textColor(page->display, COLOR65K_WHITE, MAINCOLOR);
    int titlex = 246 - strlen(temp) * 8;
    int titley = 47;
    drawString(page->display, titlex, titley, temp);
    drawLine(page->display, titlex, titley + 35, titlex + strlen(temp) * 16, titley + 35, COLOR65K_WHITE);

    titlex = 246 - strlen("Controls") * 8;
    titley = 47 + 240;
    drawString(page->display, titlex, titley, "Controls");
    drawLine(page->display, titlex, titley + 35, titlex + strlen("Controls") * 16, titley + 35, COLOR65K_WHITE);

    drawSquareFill(page->display, 492, 364, SCREEN_WIDTH - 32, SCREEN_HEIGHT - 32, MAINCOLOR);
    titlex = 742 - strlen("Stats") * 8;
    titley = 375;
    textColor(page->display, COLOR65K_WHITE, MAINCOLOR);
    drawString(page->display, titlex, titley, "Stats");
    drawLine(page->display, titlex, titley + 35, titlex + strlen("Stats") * 16, titley + 35, COLOR65K_WHITE);

    setTextParameter1(RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_16, RA8876_SELECT_8859_1);
    drawString(page->display, 55, 47 + 50, "Mode: Sinusoidal");
    drawString(page->display, 55, 47 + 70, "Cycles: 20");
    drawString(page->display, 55, 47 + 90, "Sample Rate: 800sps");

    int width = 50;
    int padding = 10;
    int color1 = COLOR65K_GRAYSCALE20;
    int color2 = COLOR65K_WHITE;
    Button keys[5];
    keys[0].xmin = 100;
    keys[0].xmax = keys[0].xmin + width;
    keys[0].ymin = 350;
    keys[0].ymax = keys[0].ymin + width;
    drawSquareFill(page->display, keys[0].xmin, keys[0].ymin, keys[0].xmax, keys[0].ymax, color1);
    drawTriangleFill(page->display, keys[0].xmin + padding, keys[0].ymax - padding, keys[0].xmax - padding, keys[0].ymax - padding, keys[0].xmin + width / 2, keys[0].ymin + padding, color2);

    keys[1].xmin = keys[0].xmin;
    keys[1].xmax = keys[1].xmin + width;
    keys[1].ymin = keys[0].ymin + width * 2 + padding * 2;
    keys[1].ymax = keys[1].ymin + width;
    drawSquareFill(page->display, keys[1].xmin, keys[1].ymin, keys[1].xmax, keys[1].ymax, color1);
    drawTriangleFill(page->display, keys[1].xmin + padding, keys[1].ymin + padding, keys[1].xmax - padding, keys[1].ymin + padding, keys[1].xmin + width / 2, keys[1].ymax - padding, color2);

    keys[2].xmin = keys[0].xmin;
    keys[2].xmax = keys[2].xmin + width;
    keys[2].ymin = keys[0].ymin + width + padding;
    keys[2].ymax = keys[2].ymin + width;
    drawSquareFill(page->display, keys[2].xmin, keys[2].ymin, keys[2].xmax, keys[2].ymax, color1);
    textColor(page->display, color2, color1);
    setTextParameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_32, RA8876_SELECT_8859_1);
    drawString(page->display, keys[2].xmin + 17, keys[2].ymin + 9, "H");

    drawGraph(page->display, display);
    float total = 0;
    int max = 0;
    int min = 0;
    //motionMailbox->dyn4->send_command(0x0c, 1000);
    while (1)
    {
        //navkey.updateStatus();
        //clock.render();
        textColor(page->display, COLOR65K_WHITE, MAINCOLOR);
        char pos[15];
        //sprintf(pos, "Position: %dmm   ", navkey.readCounterInt());

        drawString(page->display, 160, 350, pos);
        //int tempForce = force.getForce();
        //int tempForce = motionMailbox->force;
        int tempForce = 4;
        int tempPosition = 23;
        if (tempForce != -1)
        {
            samples++;
            total += tempForce;
            if (tempForce > max)
            {
                max = tempForce;
            }
            else if (tempForce < min)
            {
                min = tempForce;
            }
            drawSquareFill(page->display, SCREEN_WIDTH - strlen(forceVal) * 8 - 8, 0, SCREEN_WIDTH, 35, COLOR65K_BLUE);
            sprintf(forceVal, "Force: %d.%d ", tempForce / 1000, tempForce - (tempForce / 1000));
            drawString(page->display, 575, 420, forceVal);
            int avg = total / samples;
            sprintf(forceVal, "Average: %d.%d ", avg / 1000, avg - (avg / 1000));
            drawString(page->display, 575, 450, forceVal);
            sprintf(forceVal, "Min: %d.%d ", min / 1000, min - (min / 1000));
            drawString(page->display, 575, 480, forceVal);
            sprintf(forceVal, "Max: %d.%d ", max / 1000, max - (max / 1000));
            drawString(page->display, 575, 510, forceVal);
            drawLine(page->display, 498 + samples - 1, 182 + lastForce / 34, 498 + samples, 182 + tempForce / 34, COLOR65K_GREEN);    //force is -5000 to 5000 with 300px resolution
            drawLine(page->display, 498 + samples - 1, 182 + lastPosition / 5, 498 + samples, 182 + tempPosition / 5, COLOR65K_BLUE); //force is -5000 to 5000 with 300px resolution
            if (samples >= 445)
            {
                drawGraph(display);
                samples = 0;
            }
            lastForce = tempForce;
            lastPosition = tempPosition;
        }
    }
}

static void load_graph(AutomaticPage *page)
{
    textColor(page->display, COLOR65K_WHITE, MAINCOLOR);
    canvasImageStartAddress(page->display, PAGE4_START_ADDR);
    setTextParameter1(page->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_16, RA8876_SELECT_8859_1);
    drawSquareFill(page->display, 492, 32, SCREEN_WIDTH - 32, 32 + 300, MAINCOLOR);
    drawLine(page->display, 492 + 5, 32 + 5, 492 + 5, 332 - 5, COLOR65K_WHITE);
    drawLine(page->display, 492 + 5, 182, SCREEN_WIDTH - 32 - 5, 182, COLOR65K_WHITE);
    drawString(page->display, 492 + 10, 32 + 5, "Force(N)/Distance(Steps)");
    drawString(page->display, SCREEN_WIDTH - 32 - strlen("Time(s)") * 8, 182 - 20, "Time(s)");
    canvasImageStartAddress(page->display, PAGE1_START_ADDR);
}
static void draw_graph(AutomaticPage *page)
{
    bteMemoryCopy(page->display, PAGE4_START_ADDR, SCREEN_WIDTH, 492, 32, PAGE1_START_ADDR, SCREEN_WIDTH, 492, 32, 500, 300);
}