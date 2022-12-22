#ifndef IOBoard_H
#define IOBoard_H

#include "MCP23017.h"

#define SERVO_SRDY 14 // Servo ready status (Input)
#define SERVO_DIR 12  // Step direction (Output)
#define SERVO_PUL 13  // Step pulse (Output)
#define SERVO_ENA 11  // Servo enable (Output)

#define SD_CD 4   // Data SD card CD
#define SD_CMD 5  // Data SD card CMD
#define SD_CLK 6  // Data SD card clock
#define SD_DAT0 7 // Data SD card data 0

#define I2C_SCL 28 // clock for general I2C (Output)
#define I2C_SDA 29 // data for general I2C (Input/Output)
#define I2C_ADDR 0b00100100

#define DST_A 19 // A for distance differential encoder (Input)
#define DST_B 20 // B for distance differential encoder (Input)
#define DST_Z 21 // Z for distance differential encoder (Input)
#define DST_ZI 8 // ZI for distance differential encoder (Input)

#define FORCE_OVLD 17 // overload status of force gauge (Input)
#define FORCE_RX 15   // force gauge data (Input)
#define FORCE_TX 16   // force gauge data (Output)

#define GPIO_SDA 26 // MCP23017 (GPIO Expander) SDA pin
#define GPIO_SCL 0  // MCP23017 (GPIO Expander) SCL pin
#define GPIO_ADDR 0 // MCP23017 (GPIO Expander) I2C Address

#define DYN4_ENCODER_A 20 // A for dynamic 4-encoder (Input)
#define DYN4_ENCODER_B 19 // B for dynamic 4-encoder (Input)

#define DYN4_RX 14  // DYN4 data (Input)
#define DYN4_TX 12  // DYN4 data (Output)
#define DYN4_ADDR 3 // DYN4 Address

/*IO Expansion*/
#define GPO1_PIN_NUMBER 4
#define GPO1_PIN_REGISTER DIRA
#define GPO2_PIN_NUMBER 5
#define GPO2_PIN_REGISTER DIRA
#define GPO3_PIN_NUMBER 6
#define GPO3_PIN_REGISTER DIRA
#define GPO4_PIN_NUMBER 7
#define GPO4_PIN_REGISTER DIRA

#define GPI1_PIN_NUMBER 0
#define GPI1_PIN_REGISTER DIRB
#define GPI2_PIN_NUMBER 1
#define GPI2_PIN_REGISTER DIRB
#define GPI3_PIN_NUMBER 2
#define GPI3_PIN_REGISTER DIRB
#define GPI4_PIN_NUMBER 3
#define GPI4_PIN_REGISTER DIRB
#define GPI5_PIN_NUMBER 4
#define GPI5_PIN_REGISTER DIRB
#define GPI6_PIN_NUMBER 5
#define GPI6_PIN_REGISTER DIRB
#define GPI7_PIN_NUMBER 6
#define GPI7_PIN_REGISTER DIRB
#define GPI8_PIN_NUMBER 7
#define GPI8_PIN_REGISTER DIRB
#define GPI9_PIN_NUMBER 0
#define GPI9_PIN_REGISTER DIRA
#define GPI10_PIN_NUMBER 1
#define GPI10_PIN_REGISTER DIRA
#define GPI11_PIN_NUMBER 2
#define GPI11_PIN_REGISTER DIRA
#define GPI12_PIN_NUMBER 3
#define GPI12_PIN_REGISTER DIRA

#define CHARGE_PUMP_PIN GPO1_PIN_NUMBER
#define CHARGE_PUMP_REGISTER GPO1_PIN_REGISTER

#define SWITCHED_POWER_PIN GPI12_PIN_NUMBER
#define SWITCHED_POWER_REGISTER GPI12_PIN_REGISTER

#define DISTANCE_LIMIT_MAX GPI5_PIN_NUMBER
#define DISTANCE_LIMIT_MAX_REGISTER GPI5_PIN_REGISTER

#define DISTANCE_LIMIT_MIN GPI6_PIN_NUMBER
#define DISTANCE_LIMIT_MIN_REGISTER GPI6_PIN_REGISTER

#define DOOR_SWITCH GPI7_PIN_NUMBER
#define DOOR_SWITCH_REGISTER GPI7_PIN_REGISTER

#define ESD_LIMIT_MIN_PIN_NUMBER GPI10_PIN_NUMBER
#define ESD_LIMIT_MIN_PIN_REGISTER GPI10_PIN_REGISTER

#define ESD_LIMIT_MAX_PIN_NUMBER GPI9_PIN_NUMBER
#define ESD_LIMIT_MAX_PIN_REGISTER GPI9_PIN_REGISTER

#define ESD_ACTIVE_PIN GPI4_PIN_NUMBER
#define ESD_ACTIVE_REGISTER GPI4_PIN_REGISTER

#define SRVORDY_PIN GPI11_PIN_NUMBER
#define SRVORDY_REGISTER GPI11_PIN_REGISTER

#define SERVO_ENABLE_PIN GPO3_PIN_NUMBER
#define SERVO_ENABLE_REGISTER GPO3_PIN_REGISTER

#endif
