#include "MCP23017.h"

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
    i2c_slow_start(&(mcp23017->bus));
    i2c_slow_writeByte(&(mcp23017->bus), mcp23017->writeAddr); //sends i2c address w/ write bit set
    i2c_slow_writeByte(&(mcp23017->bus), addr);
    i2c_slow_start(&(mcp23017->bus));
    i2c_slow_writeByte(&(mcp23017->bus), mcp23017->readAddr);
    rdata = i2c_slow_readByte(&(mcp23017->bus), 1);
    i2c_slow_stop(&(mcp23017->bus));
    return rdata;
}

static void write_register(MCP23017 *mcp23017, uint8_t addr, uint8_t value)
{
    i2c_slow_start(&(mcp23017->bus));
    i2c_slow_writeByte(&(mcp23017->bus), mcp23017->writeAddr);
    i2c_slow_writeByte(&(mcp23017->bus), addr);
    i2c_slow_writeByte(&(mcp23017->bus), value);
    i2c_slow_stop(&(mcp23017->bus));
}

MCP23017 *mcp23017_create()
{
    return (MCP23017 *)malloc(sizeof(MCP23017));
}

void mcp23017_destroy(MCP23017 *mcp23017)
{
    i2c_slow_stop(&(mcp23017->bus));
    free(mcp23017);
}

/**
 * @brief 
 * 
 * @param addr 
 * @param theSDA 
 * @param theSCL 
 */
void mcp_begin(MCP23017 *mcp23017, uint8_t addr, int theSDA, int theSCL)
{
    i2c_slow_open(&(mcp23017->bus), theSCL, theSDA, 1, 100);
    mcp23017->writeAddr = ((0x20 | addr) << 1) & 0b11111110;
    mcp23017->readAddr = ((0x20 | addr) << 1) | 0b00000001;
}

void mcp_set_direction(MCP23017 *mcp23017, uint16_t pin, uint8_t direction)
{
    int reg;
    int mask;
    if (pin > 8)
    { //register B
        reg = REG_IODIRB;
    }
    else
    { //register A
        reg = REG_IODIRA;
    }
    if (direction == 1)
    {
        mask = 1 << (pin - 1);
    }
    else
    {
        mask = ~(1 << (pin - 1));
    }
    int currentValue = read_register(mcp23017, reg);
    int newValue = currentValue & mask;
    read_register(mcp23017, reg, newValue);
}

uint8_t mcp_get_direction(MCP23017 *mcp23017, uint16_t pin)
{
    int reg;
    if (pin > 8)
    { //register B
        reg = REG_IODIRB;
    }
    else
    { //register A
        reg = REG_IODIRA;
    }
    return read_register(mcp23017, reg);
}

void mcp_set_pin(MCP23017 *mcp23017, uint16_t pin, uint8_t output)
{
    int reg;
    int mask;
    if (pin > 8)
    { //register B
        reg = REG_GPIOB;
    }
    else
    { //register A
        reg = REG_GPIOA;
    }
    if (output == 1)
    {
        mask = 1 << (pin - 1);
    }
    else
    {
        mask = ~(1 << (pin - 1));
    }
    int currentValue = read_register(mcp23017, reg);
    int newValue = currentValue & mask;
    read_register(mcp23017, reg, newValue);
}
uint8_t mcp_get_pin(MCP23017 *mcp23017, uint16_t pin)
{
    int reg;
    if (pin > 8)
    { //register B
        reg = REG_GPIOB;
    }
    else
    { //register A
        reg = REG_GPIOA;
    }
    return read_register(mcp23017, reg);
}

void mcp_set_pullup(MCP23017 *mcp23017, uint16_t pin, uint8_t output)
{
    int reg;
    int mask;
    if (pin > 8)
    { //register B
        reg = REG_IPOLB;
    }
    else
    { //register A
        reg = REG_IPOLA;
    }
    if (output == 1)
    {
        mask = 1 << (pin - 1);
    }
    else
    {
        mask = ~(1 << (pin - 1));
    }
    int currentValue = read_register(mcp23017, reg);
    int newValue = currentValue & mask;
    read_register(mcp23017, reg, newValue);
}
uint8_t mcp_get_pullup(MCP23017 *mcp23017, uint16_t pin)
{
    int reg;
    if (pin > 8)
    { //register B
        reg = REG_IPOLB;
    }
    else
    { //register A
        reg = REG_IPOLA;
    }
    return read_register(mcp23017, reg);
}