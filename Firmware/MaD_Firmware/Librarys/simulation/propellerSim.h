#ifdef SIMULATION
#ifndef PROPELLERSIM_H
#define PROPELLERSIM_H
#include <stdint.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <time.h>
#define _waitms(ms) sleep(ms / 1000.0)

double _gettimems(void);

#endif
#endif