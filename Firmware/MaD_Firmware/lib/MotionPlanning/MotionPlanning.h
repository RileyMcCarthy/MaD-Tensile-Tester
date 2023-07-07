#ifndef MotionPlanning_H
#define MotionPlanning_H
#include <stddef.h>
#include <math.h>

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

double steps_to_mm(int steps, double diameter, double stepmm);
int steps_to_um(int steps, double diameter, double stepmm);
int mm_to_steps(double mm, double diameter, double stepmm);
#endif
