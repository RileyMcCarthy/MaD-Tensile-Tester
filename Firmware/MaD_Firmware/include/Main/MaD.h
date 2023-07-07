#ifndef MaD_H
#define MaD_H

// NEW DEFINES
#define RPI_TX 55
#define RPI_RX 53

#define FORCE_GAUGE_RX 0
#define FORCE_GAUGE_TX 2

#define PIN_SERVO_ENA 6 // Servo enable pin
#define PIN_SERVO_PUL 8 // Servo pulse pin
#define PIN_SERVO_DIR 7 // Servo direction pin
#define PIN_SERVO_RDY 5 // Servo ready pin

/**
 * @brief Main class responsible for initializing and starting components
 *
 */
void mad_begin();

#endif
