#include "MCP23017.h"

//Private Functions
static bool write_register(MCP23017 *mcp23017, mcp23017_register reg, uint8_t value)
{
	uint8_t ack;
	i2c_slow_start(&(mcp23017->bus));
	ack = i2c_slow_writeByte(&(mcp23017->bus), mcp23017->writeAddr);
	i2c_slow_writeByte(&(mcp23017->bus), reg);
	i2c_slow_writeByte(&(mcp23017->bus), value);
	i2c_slow_stop(&(mcp23017->bus));
	return ack == 0;
}

static uint8_t read_register(MCP23017 *mcp23017, mcp23017_register reg)
{
	uint8_t rdata = 0;
	i2c_slow_start(&(mcp23017->bus));
	i2c_slow_writeByte(&(mcp23017->bus), mcp23017->writeAddr); //sends i2c address w/ write bit set
	i2c_slow_writeByte(&(mcp23017->bus), reg);
	i2c_slow_start(&(mcp23017->bus));
	i2c_slow_writeByte(&(mcp23017->bus), mcp23017->readAddr);
	rdata = i2c_slow_readByte(&(mcp23017->bus), 1);
	i2c_slow_stop(&(mcp23017->bus));
	printf("Read: %d\n", rdata);
	return rdata;
}

//Public Functions

MCP23017 *mcp23017_create(uint8_t address, int sda, int scl)
{
	MCP23017 *mcp23017 = (MCP23017 *)malloc(sizeof(MCP23017));
	i2c_slow_open(&(mcp23017->bus), scl, sda, 1, 50);
	mcp23017->writeAddr = ((0x20 | address) << 1) & 0b11111110;
	mcp23017->readAddr = ((0x20 | address) << 1) | 0b00000001;
	return mcp23017;
}

void mcp23017_destroy(MCP23017 *mcp23017)
{
	free(mcp23017);
}

Error mcp23017_init(MCP23017 *mcp23017)
{
	//BANK = 	0 : sequential register addresses
	//MIRROR = 	0 : use configureInterrupt
	//SEQOP = 	1 : sequential operation disabled, address pointer does not increment
	//DISSLW = 	0 : slew rate enabled
	//HAEN = 	0 : hardware address pin is always enabled on 23017
	//ODR = 	0 : open drain output
	//INTPOL = 	0 : interrupt active low
	if (!write_register(mcp23017, IOCON, 0b00100000))
	{
		return MCP23017_NOT_FOUND;
	}
	return SUCCESS;
}

void mcp23017_port_mode(MCP23017 *mcp23017, mcp23017_port port, uint8_t directions, uint8_t pullups, uint8_t inverted)
{
	write_register(mcp23017, IODIR_A + port, directions);
	write_register(mcp23017, GPPU_A + port, pullups);
	write_register(mcp23017, IPOL_A + port, inverted);
}

void mcp23017_pin_mode(MCP23017 *mcp23017, uint8_t pin, uint8_t mode, bool inverted)
{
	mcp23017_register iodirreg = IODIR_A;
	mcp23017_register pullupreg = GPPU_A;
	mcp23017_register polreg = IPOL_A;
	uint8_t iodir, pol, pull;

	if (pin > 7)
	{
		iodirreg = IODIR_B;
		pullupreg = GPPU_B;
		polreg = IPOL_B;
		pin -= 8;
	}

	iodir = read_register(mcp23017, iodirreg);
	if (mode == INPUT || mode == INPUT_PULLUP)
		bitSet(iodir, pin);
	else
		bitClear(iodir, pin);

	pull = read_register(mcp23017, pullupreg);
	if (mode == INPUT_PULLUP)
		bitSet(pull, pin);
	else
		bitClear(pull, pin);

	pol = read_register(mcp23017, polreg);
	if (inverted)
		bitSet(pol, pin);
	else
		bitClear(pol, pin);

	write_register(mcp23017, iodirreg, iodir);
	write_register(mcp23017, pullupreg, pull);
	write_register(mcp23017, polreg, pol);
}

void mcp23017_digital_write(MCP23017 *mcp23017, uint8_t pin, uint8_t state)
{
	mcp23017_register gpioreg = GPIO_A;
	uint8_t gpio;
	if (pin > 7)
	{
		gpioreg = GPIO_B;
		pin -= 8;
	}

	gpio = read_register(mcp23017, gpioreg);
	printf("GPIO:%d\n", gpio);
	if (state == 1)
		bitSet(gpio, pin);
	else
		bitClear(gpio, pin);

	write_register(mcp23017, gpioreg, gpio);
	printf("GPIOA:%d\n", read_register(mcp23017, GPIO_A));
}

uint8_t mcp23017_digital_read(MCP23017 *mcp23017, uint8_t pin)
{
	mcp23017_register gpioreg = GPIO_A;
	uint8_t gpio;
	if (pin > 7)
	{
		gpioreg = GPIO_B;
		pin -= 8;
	}

	gpio = read_register(mcp23017, gpioreg);
	if (bitRead(gpio, pin))
		return HIGH;
	return LOW;
}

void mcp23017_write_port(MCP23017 *mcp23017, mcp23017_port port, uint8_t value)
{
	write_register(mcp23017, GPIO_A + port, value);
}

uint8_t mcp23017_read_port(MCP23017 *mcp23017, mcp23017_port port)
{
	return read_register(mcp23017, GPIO_A + port);
}
