#ifndef ForceGauge_H
#define ForceGauge_H

#include <stdlib.h>
#include "JSON.h"
#include <stdbool.h>
#include <stdint.h>

// Driver for ADC122u04

#define FORCE_GAUGE_RX 15
#define FORCE_GAUGE_TX 16

typedef struct __using("lib/Protocol/jm_fullduplexserial.spin2") FDS;

typedef struct ForceGauge_s
{
    int counter;
    unsigned int force;
    uint8_t buffer[4];
    FDS serial;
    int rx, tx;
    int forceRaw;
    bool responding;
    int cogid;
} ForceGauge;

int force_gauge_raw_to_force(int zero, double slope, int raw);
int raw_to_force(int raw, MachineConfiguration *configuration);

bool force_gauge_begin(ForceGauge *forceGauge, int rx, int tx);
void force_gauge_stop(ForceGauge *forceGauge);
#endif
