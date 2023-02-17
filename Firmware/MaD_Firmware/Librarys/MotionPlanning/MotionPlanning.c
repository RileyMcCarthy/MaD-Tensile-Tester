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

// computes the position of the setpoint at time t
static void compute_period(MotionPeriod *periods, double x_goal, double x0, double v0, double v_max, double a_max)
{
    double x_stop = abs((-1 * v0 + sqrt(powf(v0, 2) - 4 * (-0.5 * a_max) * x0)) / (2 * (-0.5 * a_max)));
    int d = (x_goal > x_stop) ? 1 : -1;
    double v = d * v_max;
    double a_acc = d * a_max;
    double a_dec = -1 * d * a_max;
    double T1 = abs((v - v0) / a_acc);
    double T3 = abs(v / a_dec);

    double X1 = position(T1, 0, v0, a_acc);
    double X3 = position(T3, 0, v, a_dec);

    double T2 = (x_goal - x0 - X1 - X3) / v;

    if (T2 < 0)
    {
        T2 = 0;
        v = d * sqrtf(d * a_max * (x_goal - x0) + 0.5 * powf(v0, 2));
        T1 = abs((v - v0) / a_acc);
        T3 = abs(v / a_dec);
    }
    periods->x_goal = x_goal;
    periods->x0 = x0;
    periods->v0 = v0;
    periods->v_max = v_max;
    periods->a_max = a_max;
    periods->T1 = T1;
    periods->T2 = T2;
    periods->T3 = T3;
    periods->a_acc = a_acc;
    periods->a_dec = a_dec;
    periods->v = v;
}

static void compute_setpoint(SetPoint *setpoint, double t, MotionPeriod *periods)
{
    double x1 = position(periods->T1, periods->x0, periods->v0, periods->a_acc);
    double v2 = periods->v;
    double x2 = position(periods->T2, x1, v2, 0);
    if (t <= 0)
    {
        setpoint->x = position(0, periods->x0, periods->v0, 0);
        setpoint->v = velocity(0, periods->v0, 0);
        setpoint->a = 0;
    }
    else if (t < periods->T1)
    {
        setpoint->x = position(t, periods->x0, periods->v0, periods->a_acc);
        setpoint->v = velocity(t, periods->v0, periods->a_acc);
        setpoint->a = periods->a_acc;
    }
    else if (t < periods->T1 + periods->T2)
    {
        setpoint->x = position(t - periods->T1, x1, v2, 0);
        setpoint->v = velocity(t - periods->T1, v2, 0);
        setpoint->a = 0;
    }
    else if (t < periods->T1 + periods->T2 + periods->T3)
    {
        setpoint->x = position(t - (periods->T1 + periods->T2), x2, v2, periods->a_dec);
        setpoint->v = velocity(t - (periods->T1 + periods->T2), v2, periods->a_dec);
        setpoint->a = periods->a_dec;
    }
    else
    {
        setpoint->x = periods->x_goal;
        setpoint->v = 0;
        setpoint->a = 0;
    }
}

/*void simulate_profile(SetPoint *setpoint, double t, double v_max, double a_max, double (*f)(double t, va_list args), void *args)
{

    // Computesetpoint
    MotionPeriod motionPeriod;
    compute_period(&motionPeriod, sigmoid(t, args), setpoint->x, setpoint->v, v_max, a_max);
    compute_setpoint(setpoint, t - setpoint->t, &motionPeriod);
    setpoint->t = t;
}*/

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