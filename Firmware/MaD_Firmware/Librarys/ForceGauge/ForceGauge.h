#ifndef ForceGauge_H
#define ForceGauge_H

#include <simpletools.h>
#include <Error.h>

#define FORCE_GAUGE_RX 15
#define FORCE_GAUGE_TX 16

typedef struct __using("jm_fullduplexserial.spin2") FDS;

typedef struct ForceGauge_s
{
    int counter;
    unsigned int force;
    uint8_t buffer[4];
    int interpolationSlope;
    int interpolationZero;
    FDS serial;

} ForceGauge;

ForceGauge *force_gauge_create();

void force_gauge_destroy(ForceGauge *force_gauge);

int force_gauge_get_force(ForceGauge *forceGauge);

Error force_gauge_begin(ForceGauge *forceGauge, int rx, int tx, int slope, int zero);
#endif
