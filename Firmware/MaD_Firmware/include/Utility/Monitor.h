#ifndef Motion_H
#define Motion_H
#include "DYN4.h"
#include "ForceGauge.h"
#include "StateMachine.h"

#define MONITOR_BUFFER_SIZE 10
typedef struct monitor_data_t
{
    int forceRaw;
    int force;
    int position;
    int timems;
} MonitorData;

typedef struct monitor_cog_t
{
    DYN4 *dyn4;
    ForceGauge *forceGauge;
    MonitorData data; // add list
    int sampleRate;   // sample rate in hz
    int cogid;
} Monitor;

Monitor *monitor_create();
void monitor_destroy(Monitor *monitor);

bool monitor_begin(Monitor *monitor, DYN4 *dyn4, ForceGauge *forceGauge, int sampleRate);

#endif
