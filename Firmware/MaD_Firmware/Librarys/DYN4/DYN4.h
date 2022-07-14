#ifndef DYN4_H
#define DYN4_H

#include "Error.h"
#include <stdbool.h>
#include <stdint.h>
#include "Error.h"
#include <stdlib.h>
#include "IOBoard.h"
#include <stdint.h>

/**Commands**/
#define dyn4_set_origin 0x00
#define dyn4_go_abs_pos 0x01
#define dyn4_make_linear_line 0x02
#define dyn4_go_rel_pos 0x03
#define dyn4_make_circular_arc 0x04
#define dyn4_assign_drive_id 0x05
#define dyn4_read_drive_id 0x06
#define dyn4_set_drive_config 0x07
#define dyn4_read_drive_config 0x08
#define dyn4_read_drive_status 0x09
#define dyn4_rotate_const_speed 0x0A
#define dyn4_square_wave 0x0B
#define dyn4_sin_wave 0x0C
#define dyn4_ss_frequency 0x0D
#define dyn4_general_read 0x0E
// skip 0x0F
#define dyn4_set_main_gain 0x10
#define dyn4_set_speed_gain 0x11
#define dyn4_set_int_gain 0x12
#define dyn4_trq_conss 0x13
#define dyn4_high_speed 0x14
#define dyn4_high_accel 0x15
#define dyn4_set_pos_onrange 0x16
//#define dyn4_set_gear_number 0x17 gear number also sets line number in byte 2
#define dyn4_read_main_gain 0x18
#define dyn4_read_speed_gain 0x19
#define dyn4_read_int_gain 0x1A
#define dyn4_read_trq_cons 0x1B
#define dyn4_read_high_speed 0x1C
#define dyn4_read_high_accel 0x1D
#define dyn4_read_pos_onrange 0x1E
#define dyn4_read_gear_number 0x1F

#define dyn4_is_gear_number 0x18
#define dyn4_is_status 0x19
#define dyn4_is_config 0x1a
#define dyn4_is_abs_pos 0x1b

/**Functions**/

typedef struct __using("jm_fullduplexserial.spin2") FDS;

/**
 * @brief Enumeration for the alerm states in the status register
 *
 */
typedef enum Alarm_e
{
    noAlarm,
    phaseError,
    overCurrentError,
    overHeatPwrError,
    crcError
} DYN4_Alarm;

/**
 * @brief Structural format for the status register
 *
 */
typedef struct DYN4Status_s
{
    bool onRange;
    bool motorFree;
    DYN4_Alarm alarm;
    bool motorBusy;
    bool jp3P2Status;
} DYN4_Status;

/**
 * @brief DYN4 servo controller structure
 *
 */
typedef struct DYN4_s
{
    int device_id;
    FDS serial;
} DYN4;

typedef union Data_union
{
    uint16_t wval;
    int32_t val;
    uint8_t bval[4];
} DataUnion;

Error dyn4_begin(DYN4 *dyn4, int rx, int tx, int new_device_id)__fromfile("Librarys/DYN4/DYN4.c");

int dyn4_getPosition(DYN4 *dyn4)__fromfile("Librarys/DYN4/DYN4.c");

void dyn4_send_command(DYN4 *dyn4, uint8_t command, int32_t data)__fromfile("Librarys/DYN4/DYN4.c");
int dyn4_read_command(DYN4 *dyn4, int command)__fromfile("Librarys/DYN4/DYN4.c");
Error dyn4_get_status(DYN4 *dyn4, DYN4_Status *status)__fromfile("Librarys/DYN4/DYN4.c");

#endif
