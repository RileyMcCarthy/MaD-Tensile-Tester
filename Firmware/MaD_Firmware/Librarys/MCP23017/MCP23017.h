#ifndef MCP23017_H
#define MCP23017_H

#include <stdbool.h>
#include <stdlib.h>

#define DIRA 0x00
#define DIRB 0x01

#define MCP23017_INPUT 0x1
#define MCP23017_OUTPUT 0x0
#define MCP23017_INPUT_PULLUP 0x2

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

#ifndef SIMULATION
#include <simpletools.h>
typedef struct __using("jm_i2c.spin2") I2CBus;
#else
#include "SimI2C.h"
typedef SimI2C I2CBus;
#endif
typedef struct MCP23017_t
{
    I2CBus i2cBus;
    uint8_t writeAddr, readAddr;
    uint8_t dira, dirb;
} MCP23017;

bool mcp23017_begin(MCP23017 *mcp23017, uint8_t addr, int sda, int scl);
void mcp_update_register(MCP23017 *mcp23017);
/**
 * @brief sets the direction of the pins
 *
 * @param mcp23017
 * @param pin the pin to set
 * @param direction 0: output, 1: input
 */
void mcp_set_direction(MCP23017 *mcp23017, uint16_t pin, uint8_t reg, uint8_t direction);
uint8_t mcp_get_direction(MCP23017 *mcp23017, uint16_t pin, uint8_t reg);

void mcp_set_pin(MCP23017 *mcp23017, uint16_t pin, uint8_t reg, uint8_t output);
uint8_t mcp_get_pin(MCP23017 *mcp23017, uint16_t pin, uint8_t reg);

void mcp_set_pullup(MCP23017 *mcp23017, uint16_t pin, uint8_t reg, uint8_t output);
uint8_t mcp_get_pullup(MCP23017 *mcp23017, uint16_t pin, uint8_t reg);

#endif
