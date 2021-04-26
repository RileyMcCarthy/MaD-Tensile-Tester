#include "DS3231.h"


/**
 * @brief Communicates with the DS3231 Real Time Clock IC
 * 
 * @param scl I2C Clock Pin
 * @param sda I2C Data Pin
 * @return Error: RTC_RESET if RTC lost power, RTC_NOT_FOUND if communication failed, SUCCESS otherwise.
 */
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

/**
 * @brief Sets the RTC Time
 * 
 * @param newTime The new time
 * @return Error: RTC_NOT_FOUND if communication error, SUCCESS otherwise
 * @TODO Error checking for bad Time struct values
 */
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

/**
 * @brief Read time for RTC and update local copy
 * 
 * @return Error: RTC_NOT_FOUND if communication error, SUCCESS otherwise.
 */
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

/**
 * @brief Private helper function to read RTC register
 * 
 * @param addr 
 * @return uint8_t 
 */
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
/**
 * @brief Private helper function to write RTC register
 * 
 * @param addr Register address
 * @param value Value to write to the register
 * @return true if write was acknoledged
 * @return false if write was not acknoledged
 */
bool DS3231::writeRegister(uint8_t addr, uint8_t value)
{
    i2c_start(rtc_bus);
    uint8_t ack = i2c_writeByte(rtc_bus, writeAddr);
    i2c_writeByte(rtc_bus, addr);
    i2c_writeByte(rtc_bus, value);
    i2c_stop(rtc_bus);
    return !ack;
}

/**
 * @brief Decimal number to binary coded decimal format
 * 
 * @param val the decimal number to convert
 * @return uint8_t the equivalent binary coded decimal value
 */
uint8_t DS3231::dectobcd(const uint8_t val)
{
    return ((val / 10 * 16) + (val % 10));
}

/**
 * @brief Binary coded decimal to decimal format
 * 
 * @param val The binary coded decimal number
 * @return uint8_t the equivalent decimal value
 */
uint8_t DS3231::bcdtodec(const uint8_t val)
{
    return ((val / 16 * 10) + (val % 16));
}
