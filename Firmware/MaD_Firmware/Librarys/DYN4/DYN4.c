#include "DYN4.h"
void dyn4_begin(DYN4 *dyn4, int rx, int tx, int new_device_id)
{
    dyn4->device_id = new_device_id & 0x7F;
    // driver = serial_open(rx, tx, 3, 38400);
    uart_start(&(dyn4->serial), 14, 12, 2, 38400);

    //start encoder
    dyn4->encoder.Totenc = 1; // Number of encoders
    dyn4->encoder.Pin = 19;   // set the first PIN of the first encoder

    dyn4->encoder.pasm_cntr = 0;                  // Counter to observe life in asm code
    dyn4->encoder.PosPointer = dyn4->encoder.POS; // Pointer to POS counters

    int EncCog = encoder_start(&(dyn4->encoder)); // Start asm cog with address to first variable in static block
}

int dyn4_getPosition(DYN4 *dyn4)
{
    return (dyn4->encoder).POS[0];
}

void dyn4_send_command(DYN4 *dyn4, uint8_t command, int32_t data)
{
    int n = 0;
    if (data > 0x7FFFFFF || data < -0x8000000)
    {
        n = 4;
    }
    else if (data > 0xFFFFF || data < -0x100000)
    {
        n = 3;
    }
    else if (data > 0x1FFF || data < -0x2000)
    {
        n = 2;
    }
    else if (data > 0x3F || data < -0x40)
    {
        n = 1;
    }
    else
    {
        n = 0;
    }

    int package_size = 4 + n;
    uint8_t bytes[package_size];
    bytes[package_size - 1] = dyn4->device_id;
    bytes[package_size - 2] = 0x80 + (n << 5) + command; //0x80 is because the first bit is always 1

    int totalData = 0;
    for (int i = 0; i <= n; i++)
    {
        int index = package_size - 3 - i;
        bytes[index] = 0x80 + ((data >> (7 * (n - i))) & 0x007F);
        totalData += bytes[index];
    }
    bytes[0] = 0x80 + ((bytes[package_size - 1] + bytes[package_size - 2] + totalData) % 128);
    uart_write(&(dyn4->serial), bytes[4]);
    uart_write(&(dyn4->serial), bytes[3]);
    uart_write(&(dyn4->serial), bytes[2]);
    uart_write(&(dyn4->serial), bytes[1]);
    uart_write(&(dyn4->serial), bytes[0]);
}