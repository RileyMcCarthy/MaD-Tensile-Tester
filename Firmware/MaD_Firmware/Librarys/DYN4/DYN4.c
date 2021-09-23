#include "DYN4.h"

#define STATUS_ONRANGE 0x00
#define STATUS_MOTORFREE 0x01
#define STATUS_ALARM 0x02
#define STATUS_MOTORBUSY 0x05
#define STATUS_JP3STATUS 0x06

#define READ_DRIVE_STATUS 0x09
#define IS_STATUS 0x19

/**
 * @brief Begin communication with the servo controller
 * 
 * @param dyn4 dyn4 struct that will be used to hold servo information
 * @param rx serial rx pin
 * @param tx serial tx pin
 * @param new_device_id servo device id
 * @return Error: DYN4_NOT_RESPONDING if communications fail, DYN4_COG_FAIL if starting communications cog failed, SUCCESS otherwise.
 */
Error dyn4_begin(DYN4 *dyn4, int rx, int tx, int new_device_id)
{
    dyn4->device_id = new_device_id & 0x7F;
    //uart_start(&(dyn4->serial), 14, 12, 2, 38400);
    dyn4->serial.startx(14, 12, 2, 38400);

    DYN4_Status status;
    Error ret = dyn4_get_status(dyn4, &status);

    //start encoder
    dyn4->encoder.Totenc = 1; // Number of encoders
    dyn4->encoder.Pin = 19;   // set the first PIN of the first encoder

    dyn4->encoder.pasm_cntr = 0;                  // Counter to observe life in asm code
    dyn4->encoder.PosPointer = dyn4->encoder.POS; // Pointer to POS counters

    int EncCog = encoder_start(&(dyn4->encoder)); // Start asm cog with address to first variable in static block
    if (EncCog <= 0)
    {
        ret = DYN4_COG_FAIL;
    }
    return ret;
}

/**
 * @brief Get the Status of the servo
 * 
 * @param dyn4 the servo to get the status from
 * @param status the address status struct that will contain the updated status
 * @return Error: DYN4_NOT_RESPONDING if communications fail, SUCCESS otherwise.
 */
Error dyn4_get_status(DYN4 *dyn4, DYN4_Status *status)
{
    dyn4_send_command(dyn4, READ_DRIVE_STATUS, 0xFF);
    uint8_t *statusReg;
    if (dyn4_read_command(dyn4, IS_STATUS, &statusReg) == -1)
    {
        return DYN4_NOT_RESPONDING;
    }
    status->onRange = (statusReg[0] & (STATUS_ONRANGE + 1)) >> STATUS_ONRANGE;
    status->motorFree = (statusReg[0] & (STATUS_MOTORFREE + 1)) >> STATUS_MOTORFREE;
    status->alarm = (statusReg[0] & (STATUS_ALARM + 1)) >> STATUS_ALARM;
    status->motorBusy = (statusReg[0] & (STATUS_MOTORBUSY + 1)) >> STATUS_MOTORBUSY;
    status->jp3P2Status = (statusReg[0] & (STATUS_JP3STATUS + 1)) >> STATUS_JP3STATUS;
    free(statusReg);
    return SUCCESS;
}

/**
 * @brief Read incomming command from servo.
 * @todo if sending command then reading command in seperate functions is too slow, combine them.
 * @param dyn4 the servo to read command from
 * @param dyn4_read_command the command the statement read should contain
 * @param returnData dynamically allocated array of uint8 variables containing the read data (REMEMBER TO FREE) 
 * @return int number of bytes recieved
 */
int dyn4_read_command(DYN4 *dyn4, int dyn4_read_command, uint8_t **returnData)
{
    uint8_t byte;
    uint8_t *data;

    //uart_read(&(dyn4->serial), 1, &byte);
    byte = (uint8_t)dyn4->serial.rx;
    if ((byte & dyn4->device_id) != dyn4->device_id)
    {
        return -1;
    }
    //uart_read(&(dyn4->serial), 1, &byte);
    byte = (uint8_t)dyn4->serial.rx;
    if ((byte & IS_STATUS) != IS_STATUS)
    {
        return -1;
    }
    int n = ((byte >> 5) & 0x03) + 1;
    data = (uint8_t *)malloc(sizeof(uint8_t) * n);
    //uart_read(&(dyn4->serial), n, returnData);
    for (int i = 0; i < n; i++)
    {
        data[i] = (uint8_t)dyn4->serial.rx;
    }
    *returnData = data;
    return n;
}

/**
 * @brief Send command to servo controller
 * 
 * @param dyn4 The servo to send the command
 * @param command The command to send
 * @param data The data to send with the command
 */
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
    uint8_t *bytes = malloc(sizeof(uint8_t) * package_size);
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
    //uart_write(&(dyn4->serial), bytes[4]);
    //uart_write(&(dyn4->serial), bytes[3]);
    //uart_write(&(dyn4->serial), bytes[2]);
    //uart_write(&(dyn4->serial), bytes[1]);
    //uart_write(&(dyn4->serial), bytes[0]);

    for (int i = package_size - 1; i >= 0; i--)
    {
        dyn4->serial.tx(bytes[i]);
    }
    free(bytes);
}