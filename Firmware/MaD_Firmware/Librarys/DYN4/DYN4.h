#ifndef DYN4_H
#define DYN4_H

#include "Error.h"
#include "Encoder.h"
#include "stdbool.h"
#include "stdint.h"
#include "Error.h"
#include "stdlib.h"

/**
 * @brief 
 * @todo fix dyn4 full duplex serial using new simpleserial object
 */
typedef struct __using("spin/SimpleSerial.spin2") FDS;

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
} DYN4_Alarm;

/**
 * @brief Structural format for the status register
 *
 */
typedef struct DYN4Status_s
{
    bool onRange;
    bool motorFree;
    DYN4_Alarm alarm;
    bool motorBusy;
    bool jp3P2Status;
} DYN4_Status;

/**
 * @brief DYN4 servo controller structure
 *
 */
typedef struct DYN4_s
{
    int device_id;
    encoder_t encoder;
    FDS serial;
} DYN4;

Error dyn4_begin(DYN4 *dyn4, int rx, int tx, int new_device_id);

int dyn4_getPosition(DYN4 *dyn4);

void dyn4_send_command(DYN4 *dyn4, uint8_t command, int32_t data);
int dyn4_read_command(DYN4 *dyn4, int readCommand, uint8_t *returnData);
Error dyn4_get_status(DYN4 *dyn4, DYN4_Status *status);

#endif
