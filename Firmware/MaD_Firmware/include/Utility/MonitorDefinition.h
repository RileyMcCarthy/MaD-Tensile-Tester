#ifndef MONITORDEFINITION_H
#define MONITORDEFINITION_H

typedef struct monitor_data_t
{
    int forceRaw;    // Raw force value
    int encoderRaw;  // Raw encoder value
    double force;    // Force value in N
    double position; // Position value in mm
    int timems;      // time in ms
    int timeus;      // time in us
} MonitorData;

#endif
