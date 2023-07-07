#ifndef ForceGauge_H
#define ForceGauge_H
#include <stdint.h>
#include <stdbool.h>

// Driver for ADC122u04

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

int raw_to_force(int raw, int zero, int slope);

bool force_gauge_begin(ForceGauge *forceGauge, int rx, int tx);
void force_gauge_stop(ForceGauge *forceGauge);
#endif
