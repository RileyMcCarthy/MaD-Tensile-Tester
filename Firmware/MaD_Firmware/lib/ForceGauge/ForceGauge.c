#include "ForceGauge.h"
#include <math.h>
#include <propeller.h>
#include <smartpins.h>

#define CONFIG_0 0x00 // Configuration register 0
#define CONFIG_1 0x01 // Configuration register 1
#define CONFIG_2 0x02 // Configuration register 2
#define CONFIG_3 0x03 // Configuration register 3
#define CONFIG_4 0x04 // Configuration register 4

// 0bxxx first three determines speed
#define CONFIG_DATA1 0b00001000
#define CONFIG_DATA2 0b00000000
#define CONFIG_DATA3 0b00000001
#define CONFIG_DATA4 0b01110111

#define BAUD 115200

union Data_v
{
    int32_t val;
    uint8_t bval[4];
};

#define FORCE_MEMORY_SIZE 1024
static long force_stack[FORCE_MEMORY_SIZE];

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
    temp = forceGauge->serial.rxtime(10);
    return temp;
}

static bool force_gauge_reset(ForceGauge *forceGauge)
{
    forceGauge->serial.start(forceGauge->rx, forceGauge->tx, 0, BAUD);//3
    forceGauge->serial.tx(0x55); // Synchronization word
    forceGauge->serial.tx(0x02); // POWERDDOWN
    _waitms(100);
    forceGauge->serial.tx(0x55); // Synchronization word
    forceGauge->serial.tx(0x06); // Reset
    _waitms(100);

    write_register(forceGauge, CONFIG_1, CONFIG_DATA1); // Setting data mode to continuous
    write_register(forceGauge, CONFIG_2, CONFIG_DATA2); // Setting data counter on
    write_register(forceGauge, CONFIG_3, CONFIG_DATA3); // Setting data counter on
    //write_register(forceGauge, CONFIG_4, CONFIG_DATA4); // Setting data counter on CHANGED
    forceGauge->serial.tx(0x55);
    forceGauge->serial.tx(0x08);
    int temp;
    if ((temp = read_register(forceGauge, CONFIG_1)) != CONFIG_DATA1)
    {
        _waitms(100);
        forceGauge->serial.stop();
        return false;
    }

    _waitms(100);
    forceGauge->serial.stop();
    return true;
}

// Can be updated to use simpleSerial.spin2? inside spin folder of flexprop
static void continuous_data(void *arg)
{
    ForceGauge *forceGauge = (ForceGauge *)arg;
    int rx = forceGauge->rx;
    int tx = forceGauge->tx;
    int data = 0;
    int index = 0;
    int delay = (_clockfreq() / BAUD) / 2;
    int spmode = P_ASYNC_RX;
    int baudcfg = 7 + ((_clockfreq() / BAUD) << 16);
    long transmittx = delay * 35 * 2;
    long disconnecttx = _clockfreq() / 10; // 100ms before considered disconnected
    _pinclear(rx);
    _pinstart(rx, spmode, baudcfg, 0);
    long lastData = _getcnt();
    while (1)
    {
        while (!_pinr(rx))
        {
            if ((_getcnt() - lastData) > disconnecttx)
            {
                forceGauge->responding = false;
                if (force_gauge_reset(forceGauge))
                {
                    _pinclear(rx);
                    _pinstart(rx, spmode, baudcfg, 0);
                    forceGauge->responding = true;
                    continue;
                }
            }
            else if ((_getcnt() - lastData) > transmittx && index > 0)
            {
                index = 0;
                data = 0;
            }
        }
        lastData = _getcnt();
        uint8_t byte = (_rdpin(rx) >> 24) & 0xFF;
        data |= (byte << index * 8); 
        index++;
        if (index >= 3)
        {
            forceGauge->responding = true;
            forceGauge->forceRaw = data;
            forceGauge->counter++;
            data = 0;
            index = 0;
        }
    }
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
bool force_gauge_begin(ForceGauge *forceGauge, int rx, int tx)
{
    force_gauge_stop(forceGauge);
    forceGauge->rx = rx;
    forceGauge->tx = tx;
    
    forceGauge->responding = force_gauge_reset(forceGauge);

    forceGauge->cogid = _cogstart_C(continuous_data, forceGauge, &force_stack[0], sizeof(long) * FORCE_MEMORY_SIZE);
    if (forceGauge->cogid <= 0)
    {
        return false;
    }
    return true;
}

void force_gauge_stop(ForceGauge *forceGauge)
{
    if (forceGauge->cogid > 0)
    {
        _cogstop(forceGauge->cogid);
    }
}

// returns force in milliNewtons
int raw_to_force(int raw, int zero, int slope)
{
    return round((raw - zero) / slope);
}
