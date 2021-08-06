#ifndef MaD_H
#define MaD_H

#include "simpletools.h"
#include "Clock.h"
#include "IOBoard.h"
#include "MCP23017.h"
#include "DS3231.h"
#include "State.h"
#include "Style.h"
#include "RA8876.h"
#include "i2cNavKey.h"
#include "ForceGauge.h"
#include "DYN4.h"

#define GT9271_INT 7     //SPI Interrupt pin
#define CLK 28           //SPI Clock Pin
#define DATA 29          //SPI Data Pin
#define RA8876_XNSCS 10  //SPI Chip Select Pin
#define RA8876_XNRESET 9 //Display reset
#define BACKLIGHT 8      //Display Backlight (replaced with pullup resistor)

#define MAD_SD_DO 22  //SD Data out
#define MAD_SD_CLK 23 //SD Clock
#define MAD_CD_DI 24  //SD Data In
#define MAD_CD_CS 25  //SD Chip Select

#define MAD_DS3231_SCL 38 //RTC clock pin (Output)
#define MAD_DS3231_SDA 37 //RTC data pin (bi-directional)
/**
 * @brief Main class responsible for initializing and starting components
 * 
 */
void mad_begin();
Error startDisplay();
#endif
