#ifdef SIMULATION
#ifndef SIMI2C_H
#define SIMI2C_H

class SimI2C
{
public:
    void setup(int32_t scl, int32_t sda, int32_t khz, int32_t pullup);
    void start(void);
    int32_t write(int32_t i2cbyte);
    int32_t wr_block(int32_t p_block, int32_t count);
    int32_t read(int32_t ackbit);
    void rd_block(int32_t p_block, int32_t count, int32_t ackbit);
    void stop(void);
};

#endif
#endif