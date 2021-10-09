#ifndef MCP23017_H
#define MCP23017_H

//Arduino library converted for use on the propeller
//https://www.arduino.cc/reference/en/libraries/mcp23017/

#include <simpletools.h>
#include "simpleI2CSlow.h"
#include <stdbool.h>
#include "Error.h"

#define MCP23017_INPUT 0x0
#define MCP23017_OUTPUT 0x1
#define MCP23017_INPUT_PULLUP 0x2

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

typedef enum mcp23017_port_t
{
	A = 0,
	B = 1
} mcp23017_port;

/**
 * Registers addresses.
 * The library use addresses for IOCON.BANK = 0.
 * See "3.2.1 Byte mode and Sequential mode".
 */
typedef enum mcp23107_register_t
{
	IODIR_A = 0x00,	  ///< Controls the direction of the data I/O for port A.
	IODIR_B = 0x01,	  ///< Controls the direction of the data I/O for port B.
	IPOL_A = 0x02,	  ///< Configures the polarity on the corresponding GPIO_ port bits for port A.
	IPOL_B = 0x03,	  ///< Configures the polarity on the corresponding GPIO_ port bits for port B.
	GPINTEN_A = 0x04, ///< Controls the interrupt-on-change for each pin of port A.
	GPINTEN_B = 0x05, ///< Controls the interrupt-on-change for each pin of port B.
	DEFVAL_A = 0x06,  ///< Controls the default comparaison value for interrupt-on-change for port A.
	DEFVAL_B = 0x07,  ///< Controls the default comparaison value for interrupt-on-change for port B.
	INTCON_A = 0x08,  ///< Controls how the associated pin value is compared for the interrupt-on-change for port A.
	INTCON_B = 0x09,  ///< Controls how the associated pin value is compared for the interrupt-on-change for port B.
	IOCON = 0x0A,	  ///< Controls the device.
	GPPU_A = 0x0C,	  ///< Controls the pull-up resistors for the port A pins.
	GPPU_B = 0x0D,	  ///< Controls the pull-up resistors for the port B pins.
	INTF_A = 0x0E,	  ///< Reflects the interrupt condition on the port A pins.
	INTF_B = 0x0F,	  ///< Reflects the interrupt condition on the port B pins.
	INTCAP_A = 0x10,  ///< Captures the port A value at the time the interrupt occured.
	INTCAP_B = 0x11,  ///< Captures the port B value at the time the interrupt occured.
	GPIO_A = 0x12,	  ///< Reflects the value on the port A.
	GPIO_B = 0x13,	  ///< Reflects the value on the port B.
	OLAT_A = 0x14,	  ///< Provides access to the port A output latches.
	OLAT_B = 0x15,	  ///< Provides access to the port B output latches.
} mcp23017_register;

typedef struct MCP23017_s
{
	i2c_slow bus;
	uint8_t writeAddr, readAddr;
} MCP23017;

MCP23017 *mcp23017_create(uint8_t address, int sda, int scl);
void mcp23017_destroy(MCP23017 *mcp23017);

/**
	 * Initializes the chip with the default configuration.
	 * Enables Byte mode (IOCON.BANK = 0 and IOCON.SEQOP = 1).
	 * Enables pull-up resistors for all pins. This will only be effective for input pins.
	 * 
	 * See "3.2.1 Byte mode and Sequential mode".
	 */
Error mcp23017_init(MCP23017 *mcp23017);
/**
	 * Controls the pins direction on a whole port at once.
	 * 
	 * 1 = Pin is configured as an input.
	 * 0 = Pin is configured as an output.
	 * 
	 * See "3.5.1 I/O Direction register".
	 */
void mcp23017_port_mode(MCP23017 *mcp23017, mcp23017_port port, uint8_t directions, uint8_t pullups = 0xFF, uint8_t inverted = 0x00);
/**
	 * Controls a single pin direction. 
	 * Pin 0-7 for port A, 8-15 fo port B.
	 * 
	 * 1 = Pin is configured as an input.
	 * 0 = Pin is configured as an output.
	 *
	 * See "3.5.1 I/O Direction register".
	 * 
	 * Beware!  
	 * On Arduino platform, INPUT = 0, OUTPUT = 1, which is the inverse
	 * of the MCP23017 definition where a pin is an input if its IODIR bit is set to 1.
	 * This library pinMode function behaves like Arduino's standard pinMode for consistency.
	 * [ OUTPUT | INPUT | INPUT_PULLUP ]
	 */
void mcp23017_pin_mode(MCP23017 *mcp23017, uint8_t pin, uint8_t mode, bool inverted);

/**
	 * Writes a single pin state.
	 * Pin 0-7 for port A, 8-15 for port B.
	 * 
	 * 1 = Logic-high
	 * 0 = Logic-low
	 * 
	 * See "3.5.10 Port register".
	 */
void mcp23017_digital_write(MCP23017 *mcp23017, uint8_t pin, uint8_t state);
/**
	 * Reads a single pin state.
	 * Pin 0-7 for port A, 8-15 for port B.
	 * 
	 * 1 = Logic-high
	 * 0 = Logic-low
	 * 
	 * See "3.5.10 Port register".
	 */
uint8_t mcp23017_digital_read(MCP23017 *mcp23017, uint8_t pin);

/**
	 * Writes pins state to a whole port.
	 * 
	 * 1 = Logic-high
	 * 0 = Logic-low
	 * 
	 * See "3.5.10 Port register".
	 */
void mcp23017_write_port(MCP23017 *mcp23017, mcp23017_port port, uint8_t value);

/**
	 * Reads pins state for a whole port.
	 * 
	 * 1 = Logic-high
	 * 0 = Logic-low
	 * 
	 * See "3.5.10 Port register".
	 */
uint8_t mcp23017_read_port(MCP23017 *mcp23017, mcp23017_port port);

#endif