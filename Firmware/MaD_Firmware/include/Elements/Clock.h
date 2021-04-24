#ifndef Clock_H
#define Clock_H
#include "simpletools.h"
#include "Ra8876_Lite.h"
#include "DS3231.h"
#include "IOBoard.h"
#include "Error.h"
#include "Style.h"

/**
 * @brief The Clock class uses the DS3231 library to manage and display a clock in the upper left corner.
 * 
 */
class Clock
{
public:
    Error begin(Ra8876_Lite *theDisplay);
    Error render();
    Time getDateTime();

private:
    DS3231 rtc;
    Ra8876_Lite *display;
    bool active;
};

#endif