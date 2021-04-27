#ifndef Motion_H
#define Motion_H
#include "DYN4.h"
#include "ForceGauge.h"
#include "State.h"
#define STACK_SIZE 600
#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct motion_t
    {
        int force;
        int position;
        int time;
    } Motion_Cog;
    void runMotion(void *par);
#ifdef __cplusplus
}
#endif
#endif