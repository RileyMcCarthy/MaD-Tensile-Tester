#ifndef Motion_H
#define Motion_H
#include "DYN4.h"
#include "ForceGauge.h"
#include "StateMachine.h"
#include "JSON.h"
#include "Encoder.h"

#define MONITOR_BUFFER_SIZE 10
typedef struct monitor_data_t
{
    int forceRaw;   // Raw force value
    int encoderRaw; // Raw encoder value
    int timems;     // time in ms
} MonitorData;

typedef struct monitor_cog_t
{
    MonitorData data; // add list
    MachineState *machineState;
    int sampleRate;   // sample rate in hz
    int cogid;
} Monitor;

bool monitor_begin(Monitor *monitor, MachineState *machineState, int sampleRate)__fromfile("src/Utility/Monitor.c");
#endif
