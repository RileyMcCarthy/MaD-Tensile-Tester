#include "Graph.h"

// make graph an object so it has memory!
void draw_graph(Display *display, int GRAPH_X, int GRAPH_Y, int GRAPH_W, int GRAPH_H, float *signal)
{
    // Signal properties (to be determined and used)
    float maxValue;
    float minValue;
    float overallHeight;
    float relativeHeight1;
    float relativeHeight2;
    int i;
    maxValue = signal[0];
    minValue = signal[0];
    bool redraw = true;
    for (i = 1; i < GRAPH_W; i++)
    {
        if (signal[i] > maxValue)
        {
            maxValue = signal[i];
            redraw = false;
        }
        if (signal[i] < minValue)
        {
            minValue = signal[i];
            redraw = false;
        }
    }
    if (redraw && false) // redraw
    {
        printf("redraw: max:%d,min%d\n", maxValue, minValue);
        maxValue = signal[2];
        minValue = signal[2];
        for (i = 2; i < GRAPH_W; i++)
        {
            if (signal[i] > maxValue)
            {
                maxValue = signal[i];
            }
            if (signal[i] < minValue)
            {
                minValue = signal[i];
            }
        }
        // printf("new values: max:%d,min%d\n", maxValue, minValue);
        int lastX = GRAPH_X;
        int lastY = GRAPH_Y;
        display_draw_square_fill(display, GRAPH_X, GRAPH_Y, GRAPH_X + GRAPH_W, GRAPH_Y + GRAPH_H, MAINCOLOR);
        for (int i = 1; i < GRAPH_W; i++)
        {
            // In case the smallest signal value is being plotted, plot is dirrectly,
            // As relative height will be undefined
            int relativeHeight;
            if (overallHeight != 0)
            {
                relativeHeight = (signal[i] - minValue) / (overallHeight); // Relative height of a given signal value (decimal between 0 and 1)
            }
            else
            {
                relativeHeight = 0;
            }
            int x = GRAPH_X + i;
            int y = (int)(GRAPH_Y + relativeHeight * GRAPH_H);
            display_draw_line(display, x, y, lastX, lastY);
            lastX = x;
            lastY = y;
        }
        return;
    }

    overallHeight = maxValue - minValue;

    int lastX = GRAPH_X;
    int lastY = GRAPH_Y;
    /*for (int i = 0; i < GRAPH_W; i++) use this for single draw graphs, not live graphs
    {
        // In case the smallest signal value is being plotted, plot is dirrectly,
        // As relative height will be undefined
        if (overallHeight != 0)
        {
            relativeHeight = (signal[i] - minValue) / (overallHeight); // Relative height of a given signal value (decimal between 0 and 1)
        }
        else
        {
            relativeHeight = 0;
        }
        int x = GRAPH_X + i;
        int y = (int)(GRAPH_Y + relativeHeight * GRAPH_H);
        display_draw_line(display, x, y, lastX, lastY);
        lastX = x;
        lastY = y;
    }*/
    display_bte_memory_copy(display, PAGE1_START_ADDR, SCREEN_WIDTH, GRAPH_X + 2, GRAPH_Y, PAGE1_START_ADDR, SCREEN_WIDTH, GRAPH_X, GRAPH_Y, GRAPH_W, GRAPH_H);
    if (overallHeight != 0)
    {
        relativeHeight1 = (signal[GRAPH_W - 1] - minValue) / (overallHeight); // Relative height of a given signal value (decimal between 0 and 1)
        relativeHeight2 = (signal[GRAPH_W - 2] - minValue) / (overallHeight); // Relative height of a given signal value (decimal between 0 and 1)
    }
    else
    {
        relativeHeight1 = 0;
        relativeHeight2 = 0;
    }
    int x1 = GRAPH_X + GRAPH_W;
    int y1 = (int)(GRAPH_Y + GRAPH_H - relativeHeight1 * GRAPH_H);
    int x2 = GRAPH_X + GRAPH_W - 1;
    int y2 = (int)(GRAPH_Y + GRAPH_H - relativeHeight2 * GRAPH_H);
    // printf("%d %d\n", y1, y2);
    // display_draw_square_fill(display, x2, GRAPH_Y, x1, GRAPH_Y + GRAPH_H, MAINCOLOR);
    display_draw_line(display, x2, GRAPH_Y, x2, GRAPH_Y + GRAPH_H, MAINCOLOR);
    display_draw_line(display, x1, GRAPH_Y, x1, GRAPH_Y + GRAPH_H, MAINCOLOR);
    display_draw_line(display, x2, y2, x1, y1, COLOR65K_GREEN);
    char buf[20];
    sprintf(buf, "%0.3f", maxValue);
    display_draw_string(display, GRAPH_X + 20, GRAPH_Y - 20, buf);
    sprintf(buf, "%0.3f", minValue);
    display_draw_string(display, GRAPH_X + 20, GRAPH_Y + 20 + GRAPH_H, buf);
}