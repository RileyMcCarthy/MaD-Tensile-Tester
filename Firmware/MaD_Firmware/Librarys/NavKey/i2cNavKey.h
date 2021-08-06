
//
//    FILE: i2cNavKey.h
// VERSION: 0.1..
// PURPOSE: Libreary for the i2c NavKey board with arduinp
// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
//
// DATASHEET:
//
//     URL:
//
// AUTHOR:
// Simone Caron
//

#ifndef i2cNavKey_H
#define i2cNavKey_H

#include "simpletools.h"
#include "stdbool.h"
#include "inttypes.h"

union Data_v
{
	float fval;
	uint16_t wval;
	int32_t val;
	uint8_t bval[4];
};

typedef struct navkey_t
{
	i2c bus;
	int scl;
	int sda;

	uint8_t _add;
	uint16_t _stat;
	uint8_t _stat2;
	uint8_t _statfade;
	uint8_t _gconf;
	union Data_v _tem_data;
} NavKey;

void navkey_begin(NavKey *navkey, uint8_t conf);
void navkey_reset(NavKey *navkey);
void autoconfigInterrupt(NavKey *navkey);

/**    Read functions   **/
/** Configuration function **/
uint8_t readGP1conf(NavKey *navkey);
uint8_t readGP2conf(NavKey *navkey);
uint8_t readGP3conf(NavKey *navkey);
uint16_t readInterruptConfig(NavKey *navkey);

/** Status function **/
uint16_t readStatus(NavKey *navkey);

uint8_t readInt2(NavKey *navkey);

uint8_t readFadeStatus(NavKey *navkey);

/** NavKey functions **/
float readCounterFloat(NavKey *navkey);
int32_t readCounterLong(NavKey *navkey);
int16_t readCounterInt(NavKey *navkey);
int8_t readCounterByte(NavKey *navkey);

int32_t readMax(NavKey *navkey);
float readMaxFloat(NavKey *navkey);

int32_t readMin(NavKey *navkey);
float readMinFloat(NavKey *navkey);

int32_t readStep(NavKey *navkey);
float readStepFloat(NavKey *navkey);

/** GP LED Functions **/
uint8_t readGP1(NavKey *navkey);
uint8_t readGP2(NavKey *navkey);
uint8_t readGP3(NavKey *navkey);

/** Timing registers **/
uint8_t readDoublePushPeriod(NavKey *navkey);
uint8_t readFadeGP(NavKey *navkey);

/** EEPROM register **/
uint8_t readEEPROM(NavKey *navkey, uint8_t add);

/** **/
uint8_t readIDCode(NavKey *navkey);
uint8_t readVersion(NavKey *navkey);

/******    Write functions   ********/
void writeGP1conf(NavKey *navkey, uint8_t gp1);
void writeGP2conf(NavKey *navkey, uint8_t gp2);
void writeGP3conf(NavKey *navkey, uint8_t gp3);
void writeInterruptConfig(NavKey *navkey, uint16_t interrupt);

/** NavKey functions **/
void writeCounter(NavKey *navkey, float counter);

void writeMax(NavKey *navkey, float max);

void writeMin(NavKey *navkey, float min);

void writeStep(NavKey *navkey, float step);

/** GP LED Functions **/
void writeGP1(NavKey *navkey, uint8_t gp1);
void writeGP2(NavKey *navkey, uint8_t gp2);
void writeGP3(NavKey *navkey, uint8_t gp3);

/** Timing registers **/
void writeDoublePushPeriod(NavKey *navkey, uint8_t dperiod);
void writeFadeGP(NavKey *navkey, uint8_t fade);

void writeGammaGP1(NavKey *navkey, uint8_t gamma);
void writeGammaGP2(NavKey *navkey, uint8_t gamma);
void writeGammaGP3(NavKey *navkey, uint8_t gamma);

/** EEPROM register **/
void writeEEPROM(NavKey *navkey, uint8_t add, uint8_t data);

void writeNavKey8(NavKey *navkey, uint8_t reg, uint8_t data);
void writeNavKey16(NavKey *navkey, uint8_t reg, uint16_t data);
void writeNavKey32(NavKey *navkey, uint8_t reg, int32_t data);
void writeNavKeyFloat(NavKey *navkey, uint8_t reg, float data);
void writeNavKey24(NavKey *navkey, uint8_t reg, uint32_t data);

#endif
