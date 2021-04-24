#ifndef MCP23017_H
#define MCP23017_H

#include "propeller.h"
#include "simplei2c.h"

// registers
#define REG_IODIRA 0x00 // Direction Register A
#define REG_IPOLA 0x02  // Polarity Register A
#define REG_GPPUA 0x0C  // Pull-up Register A
#define REG_GPIOA 0x12  // I/O Register A
#define REG_OLATA 0x14  // Output latch register A

#define REG_IODIRB 0x01 // Direction Register B
#define REG_IPOLB 0x03  // Polarity Register B
#define REG_GPPUB 0x0D  // Pull-up Register B
#define REG_GPIOB 0x13  // I/O Register B
#define REG_OLATB 0x15  // Output latch register B

class MCP23017
{
private:
    int sda, scl;
    i2c bus;
    uint8_t writeAddr, readAddr;

public:
    uint8_t readRegister(uint8_t addr);
    void writeRegister(uint8_t addr, uint8_t value);
    void begin(uint8_t addr, int sda, int scl);

    void dira(uint8_t mask);
    uint8_t dira();
    void dirb(uint8_t mask);
    uint8_t dirb();

    void outa(uint8_t mask);
    uint8_t outa();
    void outb(uint8_t mask);
    uint8_t outb();

    void pula(uint8_t mask);
    uint8_t pula();
    void pulb(uint8_t mask);
    uint8_t pulb();
};
#endif