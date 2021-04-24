#ifndef DYN4_H
#define DYN4_H

#include "BufferlessSerial.h"
#include "Error.h"
#include "EncoderAPI.h"

class DYN4
{
private:
    int device_id;
    //  fdserial *driver;
    encoder_t encoder;
    BLSerial serial;

public:
    ~DYN4()
    {
        //driver.Stop();
    }
    Error begin(int rx, int tx, int new_device_id)
    {
        device_id = new_device_id & 0x7F;
        // driver = serial_open(rx, tx, 3, 38400);
        uart_start(&serial, 14, 12, 2, 38400);

        //start encoder
        encoder.Totenc = 1; // Number of encoders
        encoder.Pin = 19;   // set the first PIN of the first encoder

        encoder.pasm_cntr = 0;            // Counter to observe life in asm code
        encoder.PosPointer = encoder.POS; // Pointer to POS counters

        int EncCog = encoder_start(&encoder); // Start asm cog with address to first variable in static block
    }

    int getPosition()
    {
        return encoder.POS[0];
    }

    void send_command(uint8_t command, int32_t data)
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
        bytes[package_size - 1] = device_id;
        bytes[package_size - 2] = 0x80 + (n << 5) + command; //0x80 is because the first bit is always 1

        int totalData = 0;
        for (int i = 0; i <= n; i++)
        {
            int index = package_size - 3 - i;
            bytes[index] = 0x80 + ((data >> (7 * (n - i))) & 0x007F);
            totalData += bytes[index];
        }
        bytes[0] = 0x80 + ((bytes[package_size - 1] + bytes[package_size - 2] + totalData) % 128);
        uart_write(&serial, bytes[4]);
        uart_write(&serial, bytes[3]);
        uart_write(&serial, bytes[2]);
        uart_write(&serial, bytes[1]);
        uart_write(&serial, bytes[0]);
    }
};
#endif