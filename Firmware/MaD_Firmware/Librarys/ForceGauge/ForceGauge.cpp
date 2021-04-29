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
    printf("read:%u\n", temp);
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

//UART is LSB first
//Manual reading mode needs command sent to read data, automatic reading mode has ADC send data once its ready
//Add data integrity check...
void ForceGauge_begin(ForceGauge *forceGauge, int rx, int tx)
{
    uart_start(&(forceGauge->serial), rx, tx, 2, 57600);
    uart_write(&(forceGauge->serial), 0x55); //Synchronization word
    uart_write(&(forceGauge->serial), 0x06); //Reset
    cogForceGauge = forceGauge;
    printf("finished writing registers\n");
    pause(10);
    //config 0: MUX=0000, GAIN=000, PGA_BYPASS: 0... 00000000 (default)
    //config 1: Data Rate=000, Mode = 0 (normal), conversion mode 1 (continuous), V_ref = 00 (internal), TS=0... 00001000
    //config 2: leave default
    //config 3: data output mode to auto: bit0=1
    //config 4: default
    // write(CONFIG_0, 0b00001110);
    write(forceGauge, CONFIG_1, 0b00001000);
    write(forceGauge, CONFIG_3, 0b00000001);
    //write(CONFIG_3, 0x01);
    // printf("config0: %d\n", read(forceGauge, CONFIG_0));
    //printf("config1: %d\n", read(cogForceGauge, CONFIG_1));
    //printf("config1: %d\n", read(forceGauge, CONFIG_1));

    uart_write(&(forceGauge->serial), 0x55);
    uart_write(&(forceGauge->serial), 0x08);
    pause(10);
    cog_run(monitor, 200);
}

int ForceGauge_getForce(ForceGauge *forceGauge)
{
    /*int temp = read(forceGauge, CONFIG_2);
    if ((temp & 0x80) != 0x80)
    {
        printf("Not ready:%d\n", temp);
        return -1;
    }*/
    //reading force has format: 0x55, 0x1X, {returned force data}
    /* union Data_v _temp_data;
    uart_write(&(forceGauge->serial), 0x55);
    uart_write(&(forceGauge->serial), 0x10);
    int newCount = uart_read(&(forceGauge->serial), 8);
    if (newCount == forceGauge->counter)
    {
        printf("dup\n");
        return -1;
    }
    forceGauge->counter = newCount;
    _temp_data.bval[0] = uart_read(&(forceGauge->serial), 8);
    _temp_data.bval[1] = uart_read(&(forceGauge->serial), 8);
    _temp_data.bval[2] = uart_read(&(forceGauge->serial), 8);
    int force = ((_temp_data.val - 16556732));
    printf("raw adc value(%d): %d, %d\n", newCount, _temp_data.val, force);
    return (int32_t)force;*/
}