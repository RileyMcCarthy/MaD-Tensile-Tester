#ifndef ForceGauge_H
#define ForceGauge_H

#include "simpletools.h"
#include "Error.h"

typedef struct ForceGauge_s
{
    int counter;
    unsigned int force;
    uint8_t buffer[4];
    //BLSerial serial;@todo implement serial communication

} ForceGauge;

Error force_gauge_begin(ForceGauge *forceGauge, int rx, int tx);
#endif
