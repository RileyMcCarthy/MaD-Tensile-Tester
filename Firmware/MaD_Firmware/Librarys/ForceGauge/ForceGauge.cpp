#include "ForceGauge.h"
#include "simpletools.h"
//UART is LSB first
//Manual reading mode needs command sent to read data, automatic reading mode has ADC send data once its ready
//Add data integrity check...
Error ForceGauge::begin(int rx, int tx)
{
    serial.Start(rx, tx, 3, 38400);
    serial.Put(0x55); //Synchronization word
    serial.Put(0x06); //Reset
    pause(10);
    //config 0: MUX=0000, GAIN=000, PGA_BYPASS: 0... 00000000 (default)
    //config 1: Data Rate=000, Mode = 0 (normal), conversion mode 1 (continuous), V_ref = 00 (internal), TS=0... 00001000
    //config 2: leave default
    //config 3: data output mode to auto: bit0=1
    //config 4: default
    // write(CONFIG_0, 0b00001110);
    write(CONFIG_1, 0b11101000);
    //write(CONFIG_3, 0x01);
    printf("config0: %d\n", read(CONFIG_0));
    printf("config1: %d\n", read(CONFIG_1));

    serial.Put(0x55);
    serial.Put(0x08);
    pause(10);
}

void ForceGauge::write(uint8_t reg, uint8_t data)
{
    //Write has format(rrr = reg to write): 0x55, 0b0001 rrrx {data}
    serial.Put(0x55);
    serial.Put(0x40 + (reg << 1));
    serial.Put(data);
}

uint8_t ForceGauge::read(uint8_t reg)
{
    //read has format(rrr = reg to read): 0x55, 0b0010 rrrx, {returned data}
    serial.Put(0x55);
    serial.Put(0x20 + (reg << 1));
    return serial.Get(100);
}

int ForceGauge::getForce()
{
    int temp = read(CONFIG_2);
    if ((temp & 0x80) != 0x80)
    {
        printf("Not ready:%d\n", temp);
        return -1;
    }
    //reading force has format: 0x55, 0x1X, {returned force data}
    union Data_v _temp_data;
    serial.Put(0x55);
    serial.Put(0x10);
    _temp_data.bval[0] = serial.Get(100);
    _temp_data.bval[1] = serial.Get(100);
    _temp_data.bval[2] = serial.Get(100);
    int force = (_temp_data.val - 2029542) / -665.760;

    return (int32_t)force;
}