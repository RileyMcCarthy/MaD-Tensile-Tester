#include "DS3231.h"

#define REG_SECONDS 0x00 //Second register
#define REG_MINUTES 0x01 //Minute register
#define REG_HOURS 0x02   //Hour register
#define REG_DAY 0x04     //Day register
#define REG_MONTHS 0x05  //Month register
#define REG_YEARS 0x06   //Year register
#define REG_CONTROL 0x0E //Control register
#define REG_STATUS 0x0F  //Status register

#define STATUS_RST 0x80 //Clock reset status bit

#define SET_PM 0x20     //Control AM/PM set bit
#define SET_12HOUR 0x40 //Control 12/24 set bit

#define ADDR 0x68

//Private functions

/**
 * @brief Private helper function to read RTC register
 * 
 * @param addr 
 * @return uint8_t 
 */
static uint8_t read_register(DS3231 *ds3231, uint8_t addr)
{
    uint8_t rdata = 0xFF;
    i2c_start(&(ds3231->bus));
    i2c_writeByte(&(ds3231->bus), ds3231->writeAddr); //sends i2c address w/ write bit set
    i2c_writeByte(&(ds3231->bus), addr);
    i2c_start(&(ds3231->bus));
    i2c_writeByte(&(ds3231->bus), ds3231->readAddr);
    rdata = i2c_readByte(&(ds3231->bus), 1);
    i2c_stop(&(ds3231->bus));
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
static bool write_register(DS3231 *ds3231, uint8_t addr, uint8_t value)
{
    i2c_start(&(ds3231->bus));
    uint8_t ack = i2c_writeByte(&(ds3231->bus), ds3231->writeAddr);
    i2c_writeByte(&(ds3231->bus), addr);
    i2c_writeByte(&(ds3231->bus), value);
    i2c_stop(&(ds3231->bus));
    return !ack;
}

/**
 * @brief Decimal number to binary coded decimal format
 * 
 * @param val the decimal number to convert
 * @return uint8_t the equivalent binary coded decimal value
 */
static uint8_t dectobcd(const uint8_t val)
{
    return ((val / 10 * 16) + (val % 10));
}

/**
 * @brief Binary coded decimal to decimal format
 * 
 * @param val The binary coded decimal number
 * @return uint8_t the equivalent decimal value
 */
static uint8_t bcdtodec(const uint8_t val)
{
    return ((val / 16 * 10) + (val % 16));
}

/*Public functions*/

DS3231 *ds3231_create()
{
    DS3231 *ds3231 = malloc(sizeof(DS3231));
    return ds3231;
}

void ds3231_destroy(DS3231 *ds3231)
{
    free(ds3231);
}

/**
 * @brief Communicates with the DS3231 Real Time Clock IC
 * 
 * @param scl I2C Clock Pin
 * @param sda I2C Data Pin
 * @return Error: RTC_RESET if RTC lost power, RTC_NOT_FOUND if communication failed, SUCCESS otherwise.
 */
Error ds3231_begin(DS3231 *ds3231, int scl, int sda)
{
    Error error = SUCCESS;
    ds3231->writeAddr = (ADDR << 1) & 0b11111110; //@todo, statuc address can make definition instead of part of structure
    ds3231->readAddr = (ADDR << 1) | 0b00000001;
    i2c_open(&(ds3231->bus), scl, sda, 1); //No pullup on clk, mode = 1
    uint8_t status = read_register(ds3231, REG_STATUS);
    printf("Status: %d\n", status);
    if ((status & STATUS_RST) != 0)
    {
        //oscillator is stopped, time must be set again
        error = RTC_RESET;
    }
    if (!write_register(ds3231, REG_CONTROL, 0x00))
    {
        error = RTC_NOT_FOUND;
        return error;
    }
    status &= ~0x08; //sets bit 3 in status register to zero (disable 32khz output)
    status &= ~0x80;
    if (!write_register(ds3231, REG_STATUS, status))
    {
        error = RTC_NOT_FOUND;
        return error;
    }
    return error;
}

/**
 * @brief Sets the RTC Time
 * 
 * @param newTime The new time
 * @return Error: RTC_NOT_FOUND if communication error, SUCCESS otherwise
 * @TODO Error checking for bad Time struct values
 */
Error ds3231_set_time(DS3231 *ds3231, Time *newTime)
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
    write_register(ds3231, REG_SECONDS, dectobcd(newTime->second));
    write_register(ds3231, REG_MINUTES, dectobcd(newTime->minute));
    write_register(ds3231, REG_HOURS, hourFormat);
    write_register(ds3231, REG_DAY, dectobcd(newTime->day));
    write_register(ds3231, REG_MONTHS, dectobcd(newTime->month));
    write_register(ds3231, REG_YEARS, yearFormat);
    return SUCCESS;
}

/**
 * @brief Read time for RTC and update local copy
 * 
 * @return Error: RTC_NOT_FOUND if communication error, SUCCESS otherwise.
 */
Error ds3231_update_time(DS3231 *ds3231)
{
    ds3231->time.second = bcdtodec(read_register(ds3231, REG_SECONDS));
    ds3231->time.minute = bcdtodec(read_register(ds3231, REG_MINUTES));
    ds3231->time.hour = bcdtodec(read_register(ds3231, REG_HOURS) & 0x0F);
    ds3231->time.day = bcdtodec(read_register(ds3231, REG_DAY));
    ds3231->time.month = bcdtodec(read_register(ds3231, REG_MONTHS & 0x1F));
    ds3231->time.year = bcdtodec(read_register(ds3231, REG_YEARS)) + 2000;
    return SUCCESS;
}