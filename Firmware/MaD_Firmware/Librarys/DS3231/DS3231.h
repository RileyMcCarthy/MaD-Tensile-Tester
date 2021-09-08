#ifndef DS3231_H
#define DS3231_H
#include "simpletools.h"
#include "simpleI2CSlow.h"
#include "Error.h"
#include "stdbool.h"

typedef struct time_st //@todo replace with time library
{
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint16_t year;
    bool am;
} Time;

/**
 * @brief Real Time Clock IC Interface
 * 
 */

typedef struct DS3231_t
{
    i2c_slow bus;
    uint8_t writeAddr, readAddr;
    Time time;
} DS3231;

DS3231 *ds3231_create();
void ds3231_destroy(DS3231 *ds3231);

Error ds3231_begin(DS3231 *ds3231, int scl, int sda);
Error ds3231_update_time(DS3231 *ds3231);
Error ds3231_set_time(DS3231 *ds3231, Time *time);
#endif
