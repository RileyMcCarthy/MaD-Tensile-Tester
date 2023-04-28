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

double steps_to_mm(int steps, MachineConfiguration *config)
{
    if (config == NULL)
    {
        // printf("steps_to_mm: config is NULL\n");
        return 0;
    }
    return steps * (config->gearDiameter * 3.14159) / config->positionEncoderStepsPerRev;
}

int steps_to_um(int steps, MachineConfiguration *config)
{
    if (config == NULL)
    {
        // printf("steps_to_mm: config is NULL\n");
        return 0;
    }
    return (int)round(steps * (config->gearDiameter * 3.14159) / config->positionEncoderStepsPerRev * 1000);
}

int mm_to_steps(double mm, MachineConfiguration *config)
{
    if (config == NULL)
    {
        // printf("mm_to_steps: config is NULL\n");
        return 0;
    }
    return (int)round(mm * (double)config->positionEncoderStepsPerRev / (double)(config->gearDiameter * 3.14159));
}
