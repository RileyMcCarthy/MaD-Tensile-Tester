#include "ForceGauge.h"
#include "StateMachine.h"
#include "Encoder.h"
#include "stdint.h"
typedef struct MonitorData
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

typedef struct Monitor
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

// API for reading and writing machine profiles to SD card
bool read_sd_profile(MachineProfile * profile);
int read_sd_card_data(MonitorData *data, int index, int count);
int read_data_size();
bool write_sd_profile(MachineProfile * profile);
