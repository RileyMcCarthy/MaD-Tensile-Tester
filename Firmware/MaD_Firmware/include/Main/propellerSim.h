//#ifdef SIMULATION
#ifndef PROPELLERSIM_H
#define PROPELLERSIM_H
#include <stdint.h>

//FlexCC Compiler macros
#define __fromfile(x) //gcc/g++ have a linker so fromfile is replaced with nothing

/*Propeller 1 Registers*/
static uint32_t _DIRA;
static uint32_t _DIRB;
static uint32_t _OUTA;
static uint32_t _OUTB;
static uint32_t _INA;
static uint32_t _INB;

#endif
//#endif