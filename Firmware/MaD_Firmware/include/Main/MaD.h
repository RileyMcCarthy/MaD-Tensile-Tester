#ifndef MaD_H
#define MaD_H

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
