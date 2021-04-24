#ifndef DS3231_H
#define DS3231_H
#include "simpletools.h"
#include "Error.h"
#define REG_SECONDS 0x00
#define REG_MINUTES 0x01
#define REG_HOURS 0x02
#define REG_DAY 0x04
#define REG_MONTHS 0x05
#define REG_YEARS 0x06
#define REG_CONTROL 0x0E
#define REG_STATUS 0x0F

#define STATUS_RST 0x80

#define SET_PM 0x20
#define SET_12HOUR 0x40

#define ADDR 0x68
typedef struct time_st
{
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint16_t year;
    bool am;
} Time;

class DS3231
{
public:
    Error begin(int scl, int sda);
    Error updateTime();
    Error setTime(Time *time);
    Time time;

private:
    int sda, scl;
    i2c *rtc_bus;
    uint8_t writeAddr, readAddr;

    uint8_t readRegister(uint8_t addr);
    bool writeRegister(uint8_t addr, uint8_t value);
    uint8_t dectobcd(const uint8_t val);
    uint8_t bcdtodec(const uint8_t val);
};
#endif