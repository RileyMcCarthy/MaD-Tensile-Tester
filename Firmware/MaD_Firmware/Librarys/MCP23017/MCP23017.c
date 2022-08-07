#include "MCP23017.h"
#include <stdio.h>
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

/*Private functions*/
static uint8_t read_register(MCP23017 *mcp23017, uint8_t addr)
{
    uint8_t rdata = 0xFF;

    mcp23017->i2cBus.start();
    mcp23017->i2cBus.write(mcp23017->writeAddr);
    mcp23017->i2cBus.write(addr);
    mcp23017->i2cBus.start();
    mcp23017->i2cBus.write(mcp23017->readAddr);
    rdata = mcp23017->i2cBus.read(1);
    mcp23017->i2cBus.stop();
    return rdata;
}

static bool write_register(MCP23017 *mcp23017, uint8_t addr, uint8_t value)
{
    int ack = 0;

    mcp23017->i2cBus.start();
    ack = mcp23017->i2cBus.write(mcp23017->writeAddr);
    mcp23017->i2cBus.write(addr);
    mcp23017->i2cBus.write(value);
    mcp23017->i2cBus.stop();
    return ack == 0;
}

/**
 * @brief
 *
 * @param addr
 * @param theSDA
 * @param theSCL
 */
bool mcp23017_begin(MCP23017 *mcp23017, uint8_t addr, int sda, int scl)
{
    mcp23017->i2cBus.setup(scl, sda, 100, 1); // 1.5k pullup
    mcp23017->writeAddr = ((0x20 | addr) << 1) & 0b11111110;
    mcp23017->readAddr = ((0x20 | addr) << 1) | 0b00000001;
    int DIRAVALUE = 0b00001111;
    int DIRBVALUE = 0xFF;

    bool ack = false;
    ack = write_register(mcp23017, DIRA, DIRAVALUE);  // first 4 inputs and last 4 outputs
    ack &= write_register(mcp23017, DIRB, DIRBVALUE); // all inputs
    if (read_register(mcp23017, DIRA) != DIRAVALUE || read_register(mcp23017, DIRB) != DIRBVALUE)
    {
        printf("Error setting up MCP23017: ACK:%d\n", ack);
        return false;
    }
    return true;
}

void mcp_update_register(MCP23017 *mcp23017)
{
    mcp23017->dira = read_register(mcp23017, REG_GPIOA);
    mcp23017->dirb = read_register(mcp23017, REG_GPIOB);
}

void mcp_set_direction(MCP23017 *mcp23017, uint16_t pin, uint8_t reg, uint8_t direction)
{
    if (reg == DIRA)
    {
        reg = REG_IODIRA;
    }
    else
    {
        reg = REG_IODIRB;
    }
    int value = read_register(mcp23017, reg);
    bitWrite(value, pin, direction);
    write_register(mcp23017, reg, value);
}

uint8_t mcp_get_direction(MCP23017 *mcp23017, uint16_t pin, uint8_t reg)
{ // this function is wrong @todo fix
    if (reg == DIRA)
    {
        return bitRead(mcp23017->dira, pin);
    }
    else
    {
        return bitRead(mcp23017->dirb, pin);
    }
}

void mcp_set_pin(MCP23017 *mcp23017, uint16_t pin, uint8_t reg, uint8_t state)
{
    // mcp_set_direction(mcp23017, pin, reg, MCP23017_OUTPUT);
    if (reg == DIRA)
    {
        reg = REG_GPIOA;
    }
    else
    {
        reg = REG_GPIOB;
    }
    int value = read_register(mcp23017, reg);
    bitWrite(value, pin, state);
    write_register(mcp23017, reg, value);
}
uint8_t mcp_get_pin(MCP23017 *mcp23017, uint16_t pin, uint8_t reg)
{
    return bitRead(reg == DIRA ? mcp23017->dira : mcp23017->dirb, pin);
}

void mcp_set_pullup(MCP23017 *mcp23017, uint16_t pin, uint8_t reg, uint8_t state)
{
    if (reg == DIRA)
    {
        reg = REG_IPOLA;
    }
    else
    {
        reg = REG_IPOLB;
    }
    int value = read_register(mcp23017, reg);
    bitWrite(value, pin, state);
    write_register(mcp23017, reg, value);
}
uint8_t mcp_get_pullup(MCP23017 *mcp23017, uint16_t pin, uint8_t reg)
{
    if (reg == DIRA)
    {
        reg = REG_IPOLA;
    }
    else
    {
        reg = REG_IPOLB;
    }
    return bitRead(read_register(mcp23017, reg), pin);
}
