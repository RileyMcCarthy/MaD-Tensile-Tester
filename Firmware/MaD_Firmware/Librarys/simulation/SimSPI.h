#ifdef SIMULATION
#ifndef SIMSPI_H
#define SIMSPI_H

class SimSPI
{
public:
    int32_t start(int32_t sdipin, int32_t sdopin, int32_t sckpin, int32_t khz);
    void stop(void);
    int32_t shiftin(int32_t mode, int32_t bits);
    void shiftout(int32_t mode, int32_t value, int32_t bits);
    int32_t shiftio(int32_t mode, int32_t outval, int32_t bits);
};

#endif
#endif