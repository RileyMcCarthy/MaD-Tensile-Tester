#ifndef Clock_H
#define Clock_H
#include "simpletools.h"
#include "RA8876.h"
#include "DS3231.h"
#include "MaD.h"
#include "Error.h"
#include "Style.h"
#include "stdint.h"
#include "stdio.h"

/**
 * @brief The Clock class uses the DS3231 library to manage and display a clock in the upper left corner.
 * 
 */
typedef struct clock_t
{
    DS3231 *rtc;
    Display *display;
    bool active;
} Clock;

Clock *clock_create(Display *display);
void clock_destroy(Clock *clock);

Error clock_begin(Clock *clock);
Error clock_render(Clock *clock);
Time clock_get_date_time(Clock *clock);

#endif