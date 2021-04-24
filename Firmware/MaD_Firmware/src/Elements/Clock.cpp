#include "Clock.h"

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
Error Clock::begin(Ra8876_Lite *theDisplay)
{
    Error error;
    active = true;
    display = theDisplay;
    error = rtc.begin(RTC_SCL, RTC_SDA);
    if (error == RTC_RESET) //set time
    {
        Time newTime;
        newTime = getDateTime();
        //add time check here
        rtc.setTime(&newTime);
    }
    else if (error == RTC_NOT_FOUND)
    {
        active = false;
    }
    return error;
}

/**
 * @brief Renders date and time in the top left on the display
 * 
 * @return Enum Error, RTC_NOT_FOUND if no communication with clock,
 * SUCCESS otherwise.
 */
Error Clock::render()
{
    if (!active)
    {
        return RTC_NOT_FOUND;
    }
    display->setTextParameter1(RA8876_SELECT_INTERNAL_CGROM, RA8876_CHAR_HEIGHT_16, RA8876_SELECT_8859_1);
    display->setTextParameter2(RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE, RA8876_TEXT_WIDTH_ENLARGEMENT_X1, RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
    display->textColor(MAINTEXTCOLOR, BACKCOLOR);
    rtc.updateTime();
    char buf[25];
    sprintf(buf, "%04d-%02d-%02d  %02d:%02d:%02d", rtc.time.year, rtc.time.month, rtc.time.day, rtc.time.hour, rtc.time.minute, rtc.time.second);
    display->putString(2, 0, buf);
    display->textColor(COLOR65K_WHITE, MAINCOLOR);
    return SUCCESS;
}