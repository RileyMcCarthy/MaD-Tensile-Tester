#include "Clock.h"

Clock *clock_create(Display *display)
{
    Clock *clock = (Clock *)malloc(sizeof(Clock));
    clock->rtc = ds3231_create();
    clock->display = display;
    return clock;
}

void clock_destroy(Clock *clock)
{
    ds3231_destroy(clock->rtc);
    free(clock);
}

/**
 * @brief Begin method attempts to communicate with the DS3231,
 * if successful the user will be prompt to set the time if not 
 * already set.
 * 
 * @param theDisplay pointer to RA8876 display object to render clock
 * @return Enum Error, RTC_NOT_FOUND if communication fails,
 * RTC_RESET if time is not set (will prompt user to set time),
 * SUCCESS otherwise 
 */
Error clock_begin(Clock *clock, Display *theDisplay)
{
    Error error;
    clock->active = true;
    clock->display = theDisplay;
    error = ds3231_begin(clock->rtc, MAD_DS3231_SCL, MAD_DS3231_SDA);
    printf("Error: %d\n", error);
    if (error == RTC_RESET) //set time
    {
        Time newTime;
        //newTime = clock_get_date_time(clock);
        printf("Setting new time...\n");
        //add time check here
        //ds3231_set_time(clock->rtc, &newTime); @todo uncomment this section for setting clock
    }
    else if (error == RTC_NOT_FOUND)
    {
        clock->active = false;
    }
    return error;
}

/**
 * @brief Renders date and time in the top left on the display
 * 
 * @return Enum Error, RTC_NOT_FOUND if no communication with clock,
 * SUCCESS otherwise.
 */
Error clock_render(Clock *clock)
{
    if (!clock->active)
    {
        return RTC_NOT_FOUND;
    }
    setTextParameter1(clock->display, RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_16, RA8876_SELECT_8859_1);
    setTextParameter2(clock->display, RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
    textColor(clock->display, MAINTEXTCOLOR, BACKCOLOR);
    ds3231_update_time(clock->rtc);
    char buf[25];
    sprintf(buf, "%04d-%02d-%02d  %02d:%02d:%02d", clock->rtc->time.year, clock->rtc->time.month, clock->rtc->time.day, clock->rtc->time.hour, clock->rtc->time.minute, clock->rtc->time.second);
    drawString(clock->display, 2, 0, buf);
    textColor(clock->display, COLOR65K_WHITE, MAINCOLOR);
    return SUCCESS;
}