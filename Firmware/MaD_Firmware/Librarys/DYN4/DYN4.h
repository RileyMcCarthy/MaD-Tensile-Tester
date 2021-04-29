#ifndef DYN4_H
#define DYN4_H

#include "simpletools.h"
#include "BufferlessSerial.h"
#include "Error.h"
#include "EncoderAPI.h"
#include "stdbool.h"
#include "Error.h"
#define READ_DRIVE_STATUS 0x09
#define IS_STATUS 0x19

#ifdef __cplusplus
extern "C"
{
#endif

    /**
 * @brief Enumeration for the alerm states in the status register
 * 
 */
    typedef enum Alarm_e
    {
        noAlarm,
        phaseError,
        overCurrentError,
        overHeatPwrError,
        crcError
    } Alarm;

    /**
 * @brief Structural format for the status register
 * 
 */
    typedef struct DYN4Status_s
    {
        bool onRange;
        bool motorFree;
        Alarm alarm;
        bool motorBusy;
        bool jp3P2Status;
    } DYN4Status;

    /**
 * @brief DYN4 servo controller structure
 * 
 */
    typedef struct DYN4_s
    {
        int device_id;
        encoder_t encoder;
        BLSerial serial;
    } DYN4;

    Error dyn4_begin(DYN4 *dyn4, int rx, int tx, int new_device_id);

    int dyn4_getPosition(DYN4 *dyn4);

    void dyn4_send_command(DYN4 *dyn4, uint8_t command, int32_t data);
    int readCommand(DYN4 *dyn4, int readCommand, uint8_t *returnData);
    Error getStatus(DYN4 *dyn4, DYN4Status *status);

#ifdef __cplusplus
}
#endif

#endif