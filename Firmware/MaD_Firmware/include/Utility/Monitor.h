#ifndef Motion_H
#define Motion_H
#include "DYN4.h"
#include "ForceGauge.h"
#include "StateMachine.h"
#include "JSON.h"

#define MONITOR_BUFFER_SIZE 10
typedef struct monitor_data_t
{
    int forceRaw;   // Raw force value
    int force;      // Force in milliNewtons
    int encoderRaw; // Raw encoder value
    int positionum; // Position in um
    int timems;     // time in us
} MonitorData;

typedef struct monitor_cog_t
{
    DYN4 *dyn4;
    ForceGauge *forceGauge;
    MachineProfile *profile;
    MonitorData data; // add list
    int sampleRate;   // sample rate in hz
    int cogid;
} Monitor;

typedef struct __using("jm_quadrature.spin2") Encoder;

Monitor *monitor_create();
void monitor_destroy(Monitor *monitor);

bool monitor_begin(Monitor *monitor, DYN4 *dyn4, ForceGauge *forceGauge, MachineProfile *profile, int sampleRate);
void monitor_set_position(int position);
#endif