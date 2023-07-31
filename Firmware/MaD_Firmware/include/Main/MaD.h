#ifndef MaD_H
#define MaD_H

// NEW DEFINES

#define GPI_3 22
#define GPI_4 23

#define RPI_TX 55
#define RPI_RX 53

#define FORCE_GAUGE_RX 0
#define FORCE_GAUGE_TX 2

#define PIN_SERVO_ENA 6 // Servo enable pin
#define PIN_SERVO_PUL 8 // Servo pulse pin
#define PIN_SERVO_DIR 7 // Servo direction pin
#define PIN_SERVO_RDY 5 // Servo ready pin

#define SERVO_ENCODER_A 9
#define SERVO_ENCODER_B 10
#define SERVO_ENCODER_Z 11
#define SERVO_ENCODER_ZI 12

#define CHARGE_PUMP_PIN 28
#define ESD_POWER_PIN GPI_3
#define ESD_UPPER_PIN 16
#define ESD_LOWER_PIN 17
#define ESD_SWITCH_PIN 18
#define ENDSTOP_UPPER_PIN 19
#define ENDSTOP_LOWER_PIN 20
#define ENDSTOP_DOOR_PIN 21

#define NAVKEY_SDA 15
#define NAVKEY_SCL 14
#define NAVKEY_INT 13
#define NAVKEY_I2C_ADDR 0b00100100

// IO Pin states
#define CHARGE_PUMP_PIN_ENABLE 1
#define ESD_POWER_PIN_TRIGGERED 1
#define ESD_UPPER_PIN_TRIGGERED 0
#define ESD_LOWER_PIN_TRIGGERED 0
#define ESD_SWITCH_PIN_TRIGGERED 0
#define ENDSTOP_UPPER_PIN_TRIGGERED 1
#define ENDSTOP_LOWER_PIN_TRIGGERED 1
#define ENDSTOP_DOOR_PIN_TRIGGERED 1

/**
 * @brief Main class responsible for initializing and starting components
 *
 */
void mad_begin();

#endif
