#ifndef ForceGauge_H
#define ForceGauge_H

#include "simpletools.h"
#include "Error.h"
#include "BufferlessSerial.h"
#define CONFIG_0 0x00 //Configuration register 0
#define CONFIG_1 0x01 //Configuration register 1
#define CONFIG_2 0x02 //Configuration register 2
#define CONFIG_3 0x03 //Configuration register 3
#define CONFIG_4 0x04 //Configuration register 4
#ifdef __cplusplus
extern "C"
{
#endif
    union Data_v
    {
        float fval;
        int32_t val;
        uint8_t bval[4];
    };

    typedef struct ForceGauge_s
    {
        int counter;
        unsigned int force;
        uint8_t buffer[4];
        BLSerial serial;

    } ForceGauge;

    Error ForceGauge_begin(ForceGauge *forceGauge, int rx, int tx);
#ifdef __cplusplus
}
#endif
#endif