#ifndef Motion_H
#define Motion_H
#include "ForceGauge.h"
#include "StateMachine.h"
#include "Encoder.h"
#include "MonitorDefinition.h"

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
void monitor_sync_setpoint();
void monitor_set_address(int addr);
MonitorData *monitor_read_data();
bool monitor_begin(Monitor *monitor, MachineState *machineState, MachineConfiguration *configuration, int sampleRate);
#endif
