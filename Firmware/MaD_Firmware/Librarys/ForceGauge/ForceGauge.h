#ifndef ForceGauge_H
#define ForceGauge_H

#include <Error.h>
#include <stdlib.h>
#include "JSON.h"
#include <stdbool.h>

// Driver for ADC122u04

#define FORCE_GAUGE_RX 15
#define FORCE_GAUGE_TX 16

#ifndef SIMULATION
#include <propeller2.h>
typedef struct __using("jm_fullduplexserial.spin2") FDS;
#else
#include "SimSerial.h"
typedef SimSerial FDS;
#endif
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

Error force_gauge_begin(ForceGauge *forceGauge, int rx, int tx);
void force_gauge_stop(ForceGauge *forceGauge);
#endif
