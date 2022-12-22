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

// only important info to send to rpi
typedef struct MonitorDataPacket
{
    int forcemN;         // Force in mN
    int encoderum;       // Encoder in um
    int setpointum;      // Setpoint in um
    unsigned int timeus; // time in us
    int log;
} MonitorDataPacket;
