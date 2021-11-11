#ifndef Motion_H
#define Motion_H
#include "DYN4.h"
#include "ForceGauge.h"
#include "StateMachine.h"
#ifdef __MEMORY_CHECK__
#include "leak_detector_c.h"
#endif

#define MONITOR_BUFFER_SIZE 10
typedef struct monitor_data_t
{
    int force;
    int position;
    int timems;
} MonitorData;

typedef struct monitor_cog_t
{
    DYN4 *dyn4;
    ForceGauge *forceGauge;
    MonitorData dataBuffer[MONITOR_BUFFER_SIZE];
    bool writingData; // Mutex flag
    int sampleRate;   // sample rate in hz
    int cogid;
} Monitor;

Monitor *monitor_create();
void monitor_destroy(Monitor *monitor);

bool monitor_begin(Monitor *monitor, DYN4 *dyn4, ForceGauge *forceGauge, int sampleRate);

#endif
