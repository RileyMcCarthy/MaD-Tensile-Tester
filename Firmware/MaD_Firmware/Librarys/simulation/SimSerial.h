#ifdef SIMULATION
#ifndef SIMSERIAL_H
#define SIMSERIAL_H

#include <stdint.h>

class SimSerial
{
public:
    int32_t start(int32_t rxpin, int32_t txpin, int32_t mode, int32_t baud);
    void stop(void);
    int32_t rx(void);
    int32_t rxcheck(void);
    int32_t rxtime(int32_t ms);
    int32_t rxtix(int32_t tix);
    int32_t available(void);
    void rxflush(void);
    void tx(int32_t b);
};
#endif
#endif
