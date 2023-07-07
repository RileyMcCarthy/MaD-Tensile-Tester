#include "MotionPlanning.h"

// Equation to determine position
static double position(double t, double xi, double vi, double a)
{
    return (xi + vi * t + 0.5 * a * powf(t, 2));
}

static double velocity(double t, double vi, double a)
{
    return (vi + a * t);
}

double steps_to_mm(int steps, double diameter, double stepmm)
{
    return steps * (diameter * 3.14159) / stepmm;
}

int steps_to_um(int steps, double diameter, double stepmm)
{
    return (int)round(steps * (diameter * 3.14159) / stepmm * 1000);
}

int mm_to_steps(double mm, double diameter, double stepmm)
{
    return (int)round(mm * stepmm / (double)(diameter * 3.14159));
}
