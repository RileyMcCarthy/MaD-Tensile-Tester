#ifndef MotionPlanning_H
#define MotionPlanning_H
#include <stddef.h>
#include <JSON.h>
#include <math.h>
#include <stdbool.h>

typedef struct motionPeriods_s
{
    double x_goal;
    double x0;
    double v0;
    double v_max;
    double a_max;
    double T1;
    double T2;
    double T3;
    double a_acc;
    double a_dec;
    double v;
} MotionPeriod;

typedef struct setPoint_s
{
    double t;
    double x;
    double v;
    double a;
} SetPoint;

double steps_to_mm(int steps, MachineConfiguration *config);
int steps_to_um(int steps, MachineConfiguration *config);
int mm_to_steps(double mm, MachineConfiguration *config);
#endif
