#include "ForceGauge.h"
#include "simpletools.h"

void write(ForceGauge *forceGauge, uint8_t reg, uint8_t data)
{
    //Write has format(rrr = reg to write): 0x55, 0b0001 rrrx {data}
    uart_write(&(forceGauge->serial), 0x55);
    uart_write(&(forceGauge->serial), 0x40 + (reg << 1));
    uart_write(&(forceGauge->serial), data);
}

uint8_t read(ForceGauge *forceGauge, uint8_t reg)
{
    //read has format(rrr = reg to read): 0x55, 0b0010 rrrx, {returned data}
    input(15);
    uart_write(&(forceGauge->serial), 0x55);
    uart_write(&(forceGauge->serial), 0x20 + (reg << 1));
    uint8_t temp;
    uart_read(&(forceGauge->serial), 1, &temp);
    return temp;
}
ForceGauge *cogForceGauge;
void monitor(void *par)
{
    cogForceGauge->counter = 0;
    while (1)
    {
        union Data_v _temp_data;
        uart_read(&(cogForceGauge->serial), 3, _temp_data.bval);
        int force = (_temp_data.val - 2048402) / -666;
        //printf("Force:%d\n", force);
        cogForceGauge->counter++;
        cogForceGauge->force = force;
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
Error ForceGauge_begin(ForceGauge *forceGauge, int rx, int tx)
{
    uart_start(&(forceGauge->serial), rx, tx, 2, 57600);
    uart_write(&(forceGauge->serial), 0x55); //Synchronization word
    uart_write(&(forceGauge->serial), 0x06); //Reset
    pause(10);

    write(forceGauge, CONFIG_1, 0b00001000); //Setting data mode to continuous
    write(forceGauge, CONFIG_3, 0b00000001); //Setting data aquisition to automatic

    uart_write(&(forceGauge->serial), 0x55);
    uart_write(&(forceGauge->serial), 0x08);

    pause(10);

    if (read(cogForceGauge, CONFIG_1) != 8) //sanity check to see if register was set successfully
    {
        return FORCEGAUGE_NOT_RESPONDING;
    }

    cogForceGauge = forceGauge;
    int *cogAddr = cog_run(monitor, 200); //@todo trim the stack until failure
    if (cog_num(cogAddr) <= 0)
    {
        return FORCEGAUGE_COG_FAIL;
    }
    return SUCCESS;
}