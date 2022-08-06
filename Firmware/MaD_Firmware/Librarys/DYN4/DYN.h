#ifndef DYN_H
#define DYN_H

#define DYN1_ADDR 0
#define DYN1_RX 14
#define DYN1_TX 12

#include <stdbool.h>

void dyn_init();
void move_rel32(char ID, long pos);
void ReadMotorTorqueCurrent(char ID);
void ReadMotorPosition32(char ID);
bool GetMotorPosition32();
void move_abs32(char MotorID, long Pos32);
void Turn_const_speed(char ID, long spd);

#endif