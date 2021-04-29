#ifndef BufferlessSerial_H
#define BufferlessSerial_H

#include "propeller.h"
#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct BLSerial_s
    {
        int rx_mask;
        int tx_mask;
        int mode;
        int baudCNT;
    } BLSerial;

    void uart_read(BLSerial *serial, unsigned int bytes, uint8_t *buffer);
    void uart_write(BLSerial *serial, uint8_t data);
    void uart_start(BLSerial *serial, int rx, int tx, int mode, int baud);

#ifdef __cplusplus
}
#endif
#endif