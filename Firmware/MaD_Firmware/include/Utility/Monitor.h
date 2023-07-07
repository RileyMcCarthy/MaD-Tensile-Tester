#ifndef MONITOR_H
#define MONITOR_H
#include <stdbool.h>
#include <stdint.h>
#include "Memory/MachineProfile.h"
#include "Memory/MonitorState.h"
#include "ForceGauge.h"
#include "Encoder.h"


void monitor_begin(int sampleRate);

// API for reading and writing machine profiles to SD card
bool read_sd_profile(MachineProfile * profile);
int read_sd_card_data(MonitorData *data, int index, int count);
int read_data_size();
bool write_sd_profile(MachineProfile * profile);
bool get_monitor_data(MonitorData *data, int timeout_ms);
void set_gauge_length();
#endif
