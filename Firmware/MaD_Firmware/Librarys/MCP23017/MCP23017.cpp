#include "MCP23017.h"

/**
 * @brief 
 * 
 * @param addr 
 * @param theSDA 
 * @param theSCL 
 */
void MCP23017::begin(uint8_t addr, int theSDA, int theSCL)
{
    i2c_open(&bus, theSCL, theSDA, 1);
    writeAddr = ((0x20 | addr) << 1) & 0b11111110;
    readAddr = ((0x20 | addr) << 1) | 0b00000001;
}

void MCP23017::dira(uint8_t mask)
{
    writeRegister(REG_IODIRA, mask);
}
uint8_t MCP23017::dira()
{
    return readRegister(REG_IODIRA);
}
void MCP23017::dirb(uint8_t mask)
{
    writeRegister(REG_IODIRB, mask);
}
uint8_t MCP23017::dirb()
{
    return readRegister(REG_IODIRB);
}

void MCP23017::outa(uint8_t mask)
{
    writeRegister(REG_GPIOA, mask);
}
uint8_t MCP23017::outa()
{
    return readRegister(REG_GPIOA);
}
void MCP23017::outb(uint8_t mask)
{
    writeRegister(REG_GPIOB, mask);
}
uint8_t MCP23017::outb()
{
    return readRegister(REG_GPIOB);
}

void MCP23017::pula(uint8_t mask)
{
    writeRegister(REG_IPOLA, mask);
}
uint8_t MCP23017::pula()
{
    return readRegister(REG_IPOLA);
}
void MCP23017::pulb(uint8_t mask)
{
    writeRegister(REG_IPOLB, mask);
}
uint8_t MCP23017::pulb()
{
    return readRegister(REG_IPOLB);
}

//*Helper function for reading registers*//
uint8_t MCP23017::readRegister(uint8_t addr)
{
    uint8_t rdata = 0xFF;
    i2c_start(&bus);
    i2c_writeByte(&bus, writeAddr); //sends i2c address w/ write bit set
    i2c_writeByte(&bus, addr);
    i2c_start(&bus);
    i2c_writeByte(&bus, readAddr);
    rdata = i2c_readByte(&bus, 1);
    i2c_stop(&bus);
    return rdata;
}

void MCP23017::writeRegister(uint8_t addr, uint8_t value)
{
    i2c_start(&bus);
    i2c_writeByte(&bus, writeAddr);
    i2c_writeByte(&bus, addr);
    i2c_writeByte(&bus, value);
    i2c_stop(&bus);
}