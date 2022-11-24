#ifndef MONITORDEFINITION_H
#define MONITORDEFINITION_H
#include "stdint.h"
typedef struct monitor_data_t
{
    int forceRaw;        // Raw force value
    int encoderRaw;      // Raw encoder value
    int forcemN;         // Force in mN
    int encoderum;       // Encoder in um
    double force;        // Force value in N
    double position;     // Position value in mm
    int setpoint;        // Setpoint in um
    unsigned int timems; // time in ms
    unsigned int timeus; // time in us
    int log;
} MonitorData;

typedef struct DataPacket_s
{
    MonitorData data;
    uint8_t crc;
} DataPacket;

#endif
