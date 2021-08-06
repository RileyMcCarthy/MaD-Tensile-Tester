#include "ForceGauge.h"
#include "simpletools.h"

#define CONFIG_0 0x00 //Configuration register 0
#define CONFIG_1 0x01 //Configuration register 1
#define CONFIG_2 0x02 //Configuration register 2
#define CONFIG_3 0x03 //Configuration register 3
#define CONFIG_4 0x04 //Configuration register 4

union Data_v
{
    float fval;
    int32_t val;
    uint8_t bval[4];
};

//Private Function Declarations
void write_register(ForceGauge *forceGauge, uint8_t reg, uint8_t data);
uint8_t read_register(ForceGauge *forceGauge, uint8_t reg);

/**
 * @brief Begin the force gauge communication.
 * @note UART is LSB first
 * @todo Add data integrity check...
 * @param forceGauge The force gauge structure to start
 * @param rx serial rx pin
 * @param tx serial tx pin
 * @return Error: FORCEGAUGE_NOT_RESPONDING if communications fails, FORCEEGAUGE_COG_FAIL if cog fails to start, SUCCESS otherwise.
 */
Error force_gauge_begin(ForceGauge *forceGauge, int rx, int tx)
{
    //uart_start(&(forceGauge->serial), rx, tx, 2, 57600); @todo implement serial
    //uart_write(&(forceGauge->serial), 0x55); //Synchronization word
    //uart_write(&(forceGauge->serial), 0x06); //Reset
    _waitms(10);

    write_register(forceGauge, CONFIG_1, 0b00001000); //Setting data mode to continuous
    write_register(forceGauge, CONFIG_3, 0b00000001); //Setting data aquisition to automatic

    //uart_write(&(forceGauge->serial), 0x55); @todo implement serial
    //uart_write(&(forceGauge->serial), 0x08);

    _waitms(10);

    /*  if (read(cogForceGauge, CONFIG_1) != 8) //sanity check to see if register was set successfully
    {
        return FORCEGAUGE_NOT_RESPONDING;
    }*/

    /*cogForceGauge = forceGauge; @todo implement serial
    int *cogAddr = cog_run(monitor, 200); //@todo trim the stack until failure
    if (cog_num(cogAddr) <= 0)
    {
        return FORCEGAUGE_COG_FAIL;
    }*/
    return SUCCESS;
}

//Private Functions

static void write_register(ForceGauge *forceGauge, uint8_t reg, uint8_t data)
{
    //Write has format(rrr = reg to write): 0x55, 0b0001 rrrx {data}
    //uart_write(&(forceGauge->serial), 0x55); @todo implement serial
    //uart_write(&(forceGauge->serial), 0x40 + (reg << 1));
    //uart_write(&(forceGauge->serial), data);
}

static uint8_t read_register(ForceGauge *forceGauge, uint8_t reg)
{
    //read has format(rrr = reg to read): 0x55, 0b0010 rrrx, {returned data}

    //uart_write(&(forceGauge->serial), 0x55);@todo implement serial
    //uart_write(&(forceGauge->serial), 0x20 + (reg << 1));
    uint8_t temp;
    //uart_read(&(forceGauge->serial), 1, &temp);
    return temp;
}