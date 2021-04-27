#ifndef DYN4_H
#define DYN4_H

#include "BufferlessSerial.h"
#include "Error.h"
#include "EncoderAPI.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct DYN4_s
    {
        int device_id;
        encoder_t encoder;
        BLSerial serial;
    } DYN4;

    void dyn4_begin(DYN4 *dyn4, int rx, int tx, int new_device_id);

    int dyn4_getPosition(DYN4 *dyn4);

    void dyn4_send_command(DYN4 *dyn4, uint8_t command, int32_t data);
#ifdef __cplusplus
}
#endif

#endif