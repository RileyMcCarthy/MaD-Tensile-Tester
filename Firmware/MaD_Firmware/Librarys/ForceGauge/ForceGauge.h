#ifndef ForceGauge_H
#define ForceGauge_H

#include <simpletools.h>
#include <Error.h>
#include <stdlib.h>
#include "JSON.h"

#define FORCE_GAUGE_RX 15
#define FORCE_GAUGE_TX 16

typedef struct __using("jm_fullduplexserial.spin2") FDS;

typedef struct ForceGauge_s
{
    int counter;
    unsigned int force;
    uint8_t buffer[4];
    FDS serial;

} ForceGauge;

int force_gauge_raw_to_force(ForceGauge *forceGauge, int raw);
int raw_to_force(int raw, MachineConfiguration *configuration);
int force_gauge_get_raw(ForceGauge *forceGauge, Error *err);

Error force_gauge_begin(ForceGauge *forceGauge, int rx, int tx);
void force_gauge_stop(ForceGauge *forceGauge);
#endif
