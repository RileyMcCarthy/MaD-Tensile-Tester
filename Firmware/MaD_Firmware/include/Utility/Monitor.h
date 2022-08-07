#ifndef Motion_H
#define Motion_H
#include "ForceGauge.h"
#include "StateMachine.h"
#include "JSON.h"
#include "Encoder.h"

typedef struct monitor_data_t
{
    int forceRaw;    // Raw force value
    int encoderRaw;  // Raw encoder value
    double force;    // Force value in N
    double position; // Position value in mm
    int timems;      // time in ms
} MonitorData;

typedef struct monitor_cog_t
{
    MonitorData data; // add list
    MachineState *machineState;
    MachineConfiguration *configuration;
    int sampleRate; // sample rate in hz
    int cogid;

    MonitorData cache; // Cached data
    int cacheLock;
} Monitor;

MonitorData *monitor_read_data(int addr);
bool monitor_begin(Monitor *monitor, MachineState *machineState, MachineConfiguration *configuration, int sampleRate);
#endif
