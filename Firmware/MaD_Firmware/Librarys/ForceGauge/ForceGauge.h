#ifndef ForceGauge_H
#define ForceGauge_H

#include "propeller.h"
#include "Error.h"
#include "libpropeller/fullduplexserial/full_duplex_serial.h"

#define CONFIG_0 0x00
#define CONFIG_1 0x01
#define CONFIG_2 0x02
#define CONFIG_3 0x03
#define CONFIG_4 0x04

union Data_v
{
    float fval;
    int32_t val;
    uint8_t bval[4];
};
class ForceGauge
{
private:
    libpropeller::FullDuplexSerial serial;
    void write(uint8_t reg, uint8_t data);
    uint8_t read(uint8_t reg);

public:
    ~ForceGauge()
    {
        serial.Stop();
    }
    Error begin(int rx, int tx);
    int getForce();
};
#endif