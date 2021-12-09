#ifndef MaD_H
#define MaD_H

#include <simpletools.h>
#include <stdio.h>
#include <stdlib.h>

#include "IOBoard.h"
#include "MCP23017.h"
#include "DS3231.h"
#include "StateMachine.h"
#include "Style.h"
#include "RA8876.h"
#include "ForceGauge.h"
#include "DYN4.h"
#include "Keyboard.h"
#include "NavigationPage.h"
#include "LoadingOverlay.h"
#include "IOBoard.h"
#include "Monitor.h"
#include "ManualPage.h"
#include "StatusPage.h"
#include "NavigationPage.h"
#include "AutomaticPage.h"
#include "CalibrateForcePage.h"
#include "JSON.h"

#define DISPLAY_SCK 51     // SPI Clock Pin
#define DISPLAY_MISO 53    // SPI Data Pin
#define DISPLAY_MOSI 52    // SPI Clock Pin
#define DISPLAY_XNSCS 55   // SPI Chip Select Pin
#define DISPLAY_XNRESET 54 // Display reset pin
#define DISPLAY_CLK 56     // Display I2C clock pin
#define DISPLAY_DATA 57    // Display I2C data pin

#define MAD_SD_DO 39  // SD Data out
#define MAD_SD_CLK 40 // SD Clock
#define MAD_SD_DI 41  // SD Data In
#define MAD_SD_CS 42  // SD Chip Select

#define MAD_DS3231_SCL 38 // RTC clock pin (Output)
#define MAD_DS3231_SDA 37 // RTC data pin (bi-directional)
/**
 * @brief Main class responsible for initializing and starting components
 *
 */
void mad_begin();

#endif
