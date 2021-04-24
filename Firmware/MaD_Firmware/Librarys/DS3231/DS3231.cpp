#include "DS3231.h"

Error DS3231::begin(int scl, int sda)
{
    Error error = SUCCESS;
    writeAddr = (ADDR << 1) & 0b11111110;
    readAddr = (ADDR << 1) | 0b00000001;
    i2c_open(rtc_bus, scl, sda, 1); //current no pullup on clk, mode = 1
    uint8_t status = readRegister(REG_STATUS);
    if ((status & STATUS_RST) != 0)
    {
        //oscillator is stopped, time must be set again
        error = RTC_RESET;
    }
    if (!writeRegister(REG_CONTROL, 0x00))
    {
        error = RTC_NOT_FOUND;
        return error;
    }
    status &= ~0x08; //sets bit 3 in status register to zero (disable 32khz output)
    status &= ~0x80;
    writeRegister(REG_STATUS, status);
    return error;
}

Error DS3231::setTime(Time *newTime)
{
    int hourFormat = dectobcd(newTime->hour);
    hourFormat |= SET_12HOUR;
    if (newTime->am)
    {
        hourFormat |= SET_PM;
    }
    else
    {
        hourFormat &= ~SET_PM;
    }
    int yearFormat = dectobcd(20);
    writeRegister(REG_SECONDS, dectobcd(newTime->second));
    writeRegister(REG_MINUTES, dectobcd(newTime->minute));
    writeRegister(REG_HOURS, hourFormat);
    writeRegister(REG_DAY, dectobcd(newTime->day));
    writeRegister(REG_MONTHS, dectobcd(newTime->month));
    writeRegister(REG_YEARS, yearFormat);
    return SUCCESS;
}

Error DS3231::updateTime()
{
    time.second = bcdtodec(readRegister(REG_SECONDS));
    time.minute = bcdtodec(readRegister(REG_MINUTES));
    time.hour = bcdtodec(readRegister(REG_HOURS) & 0x0F);
    time.day = bcdtodec(readRegister(REG_DAY));
    time.month = bcdtodec(readRegister(REG_MONTHS & 0x1F));
    time.year = bcdtodec(readRegister(REG_YEARS)) + 2000;
    return SUCCESS;
}

//*Helper function for reading registers*//
uint8_t DS3231::readRegister(uint8_t addr)
{
    uint8_t rdata = 0xFF;
    i2c_start(rtc_bus);
    i2c_writeByte(rtc_bus, writeAddr); //sends i2c address w/ write bit set
    i2c_writeByte(rtc_bus, addr);
    i2c_start(rtc_bus);
    i2c_writeByte(rtc_bus, readAddr);
    rdata = i2c_readByte(rtc_bus, 1);
    i2c_stop(rtc_bus);
    return rdata;
}

//*Helper function for writing registers*//
bool DS3231::writeRegister(uint8_t addr, uint8_t value)
{
    i2c_start(rtc_bus);
    uint8_t ack = i2c_writeByte(rtc_bus, writeAddr);
    i2c_writeByte(rtc_bus, addr);
    i2c_writeByte(rtc_bus, value);
    i2c_stop(rtc_bus);
    return !ack;
}

uint8_t DS3231::dectobcd(const uint8_t val)
{
    return ((val / 10 * 16) + (val % 10));
}

uint8_t DS3231::bcdtodec(const uint8_t val)
{
    return ((val / 16 * 10) + (val % 16));
}
