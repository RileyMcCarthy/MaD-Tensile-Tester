#ifndef Motion_H
#define Motion_H

#include "DYN4.h"
#include "ForceGauge.h"
#include "libpropeller/fullduplexserial/full_duplex_serial.h"

struct mailbox_t
{
    int force;
    int distance;
    int time;
    DYN4 *dyn4;
    ForceGauge *forceGauge;
};
#endif