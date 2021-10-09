#ifndef MCP23017_H
#define MCP23017_H

#include <simpletools.h>
#include "simpleI2CSlow.h"
#include <stdbool.h>

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

typedef struct MCP23017_t
{
    i2c_slow bus;
    uint8_t writeAddr, readAddr;
} MCP23017;

MCP23017 *mcp23017_create();
void mcp32017_destroy();

void mcp23017_begin(MCP23017 *mcp23017, uint8_t addr, int sda, int scl);

/**
 * @brief sets the direction of the pins
 * 
 * @param mcp23017 
 * @param pin the pin to set
 * @param direction 0: output, 1: input
 */
void mcp_set_direction(MCP23017 *mcp23017, uint16_t pin, uint8_t direction);
uint8_t mcp_get_direction(MCP23017 *mcp23017, uint16_t pin);

void mcp_set_pin(MCP23017 *mcp23017, uint16_t pin, uint8_t output);
uint8_t mcp_get_pin(MCP23017 *mcp23017, uint16_t pin);

void mcp_set_pullup(MCP23017 *mcp23017, uint16_t pin, uint8_t output);
uint8_t mcp_get_pullup(MCP23017 *mcp23017, uint16_t pin);

#endif
