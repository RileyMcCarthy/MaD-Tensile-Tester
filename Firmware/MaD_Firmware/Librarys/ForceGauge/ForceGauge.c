#include "ForceGauge.h"
#include "simpletools.h"

#define CONFIG_0 0x00 // Configuration register 0
#define CONFIG_1 0x01 // Configuration register 1
#define CONFIG_2 0x02 // Configuration register 2
#define CONFIG_3 0x03 // Configuration register 3
#define CONFIG_4 0x04 // Configuration register 4

union Data_v
{
    float fval;
    int32_t val;
    uint8_t bval[4];
};

// Private Functions

static void write_register(ForceGauge *forceGauge, uint8_t reg, uint8_t data)
{
    // Write has format(rrr = reg to write): 0x55, 0001 rrrx {data}
    // uart_write(&(forceGauge->serial), 0x55); @todo implement serial
    // uart_write(&(forceGauge->serial), 0x40 + (reg << 1));
    // uart_write(&(forceGauge->serial), data);
    forceGauge->serial.tx(0x55);
    forceGauge->serial.tx(0x40 + (reg << 1));
    forceGauge->serial.tx(data);
}

static uint8_t read_register(ForceGauge *forceGauge, uint8_t reg)
{
    uint8_t temp;
    // read has format(rrr = reg to read): 0x55, 0010 rrrx, {returned data}
    // uart_write(&(forceGauge->serial), 0x55);@todo implement serial
    // uart_write(&(forceGauge->serial), 0x20 + (reg << 1));
    // uart_read(&(forceGauge->serial), 1, &temp);
    forceGauge->serial.tx(0x55);
    forceGauge->serial.tx(0x20 + (reg << 1));
    temp = forceGauge->serial.rxtime(100);
    return temp;
}
ForceGauge *force_gauge_create()
{
    ForceGauge *forceGauge = (ForceGauge *)malloc(sizeof(ForceGauge));
    return forceGauge;
}

void force_gauge_destroy(ForceGauge *forceGauge)
{
    free(forceGauge);
}

/**
 * @brief Begin the force gauge communication.
 * @note UART is LSB first
 * @todo Add data integrity check...
 * @param forceGauge The force gauge structure to start
 * @param rx serial rx pin
 * @param tx serial tx pin
 * @return Error: FORCEGAUGE_NOT_RESPONDING if communications fails, FORCEEGAUGE_COG_FAIL if cog fails to start, SUCCESS otherwise.
 */
Error force_gauge_begin(ForceGauge *forceGauge, int rx, int tx, int slope, int zero)
{
    _waitms(100);
    int configData1 = 0b11001000;
    int configData2 = 0b01000000;
    int configData4 = 0b01110111;

    forceGauge->interpolationSlope = slope;
    forceGauge->interpolationZero = zero;

    forceGauge->serial.start(rx, tx, 3, 19200);
    forceGauge->serial.tx(0x55); // Synchronization word
    forceGauge->serial.tx(0x06); // Reset
    _waitms(100);

    write_register(forceGauge, CONFIG_1, configData1); // Setting data mode to continuous
    write_register(forceGauge, CONFIG_2, configData2); // Setting data counter on
    write_register(forceGauge, CONFIG_4, configData4); // Setting data counter on
    int temp;
    if ((temp = read_register(forceGauge, CONFIG_1)) != configData1)
    {
        return FORCEGAUGE_NOT_RESPONDING;
    }
    else if ((temp = read_register(forceGauge, CONFIG_2)) != configData2)
    {
        return FORCEGAUGE_NOT_RESPONDING;
    }
    else if ((temp = read_register(forceGauge, CONFIG_4)) != configData4)
    {
        return FORCEGAUGE_NOT_RESPONDING;
    }

    forceGauge->serial.tx(0x55);
    forceGauge->serial.tx(0x08);
    return SUCCESS;
}

int force_gauge_get_force(ForceGauge *forceGauge)
{
    int forceRaw = force_gauge_get_raw(forceGauge);
    return force_gauge_raw_to_force(forceGauge, forceRaw);
}

float force_gauge_raw_to_force(ForceGauge *forceGauge, int raw)
{
    return (float)(raw - forceGauge->interpolationZero) / ((float)forceGauge->interpolationSlope * (float)1000);
}

int force_gauge_get_raw(ForceGauge *forceGauge)
{
    forceGauge->serial.rxflush();
    int dready = read_register(forceGauge, CONFIG_2);

    if ((dready & 0b10000000) != 0b10000000)
    {
        return NULL;
    }

    forceGauge->serial.tx(0x55);
    forceGauge->serial.tx(0x10); // Send RData command
    int counter = forceGauge->serial.rxtime(100);
    int forceRaw = forceGauge->serial.rxtime(100);
    forceRaw |= forceGauge->serial.rxtime(100) << 8;
    forceRaw |= forceGauge->serial.rxtime(100) << 16;
    return forceRaw;
}
