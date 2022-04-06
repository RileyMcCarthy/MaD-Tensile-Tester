#include "motionPlanning.h"
#include "bbpr.h"

static double cubic(double t, double a, double b, double c, double d)
{
    return a * t * t * t + b * t * t + c * t + d;
}

static double vcubic(double t, va_list args)
{
    double a = va_arg(args, double);
    double b = va_arg(args, double);
    double c = va_arg(args, double);
    double d = va_arg(args, double);
    return cubic(t, a, b, c, d);
}

static double vsolve(double point, double (*f)(double t, va_list args), ...)
{
    va_list args;

    va_start(args, f);
    double t = 0;
    while (abs(t - point) > 0.001)
    {
        t = t + (f(t, args) - point) / central_difference(f(t + 0.1, args), f(t - 0.1, args), 0.1);
    }
    return t;
}

// Find positive root of quadratic function of form a*x^2 + b*x + c = 0
double quadratic_solve(double a, double b, double c)
{
    return abs((-b + sqrt(powf(b, 2) - 4 * a * c)) / (2 * a));
}

double secant_method(double point, double t0, double t1, double (*f)(double t, va_list args), ...)
{
    va_list args;
    va_start(args, f);
    while (abs(f(t1, args) - point) > 0.0001)
    {
        double num = (f(t1, args) - point) * (t1 - t0);
        double den = (f(t1, args) - f(t0, args));
        t0 = t1;
        t1 = t1 - num / den;
    }
    return t1;
}

void computePeriods(double *Xpeak, double *T)
{
    // T[0] is time constant of distance peak (final motion time)
    // T[1] is period of velocity peak stage
    // T[2] is period of acceleration peak stage
    // T[3] is period of jerk peak stage
    // Xpeak[0-3] = d,v,a,j
    int n = 0;
    double coeffs[4] = {0, 0, 0, 0};
    double Xmax = 0;

    n = 3;
    coeffs[0] = 2 * Xpeak[3];
    coeffs[1] = T[1] * Xpeak[3] + 3 * T[2] * Xpeak[3];
    coeffs[2] = T[1] * T[2] * Xpeak[3] + powf(T[2], 2) * Xpeak[3];
    coeffs[3] = -Xpeak[0];
    // coeffs[0]*t^3+coeffs[1]*t^2+coeffs[2]*t+coeffs[3] = 2*Jmax&t^3+ (T1*Jmax+3*T2*Jmax)*t^2+ (T1*T2*Jmax+T2^2*Jmax)*t + -Xmax = 2*Jmax&t^3 - Xmax = 0
    T[3] = secant_method(0, 1, 2, vcubic, coeffs[0], coeffs[1], coeffs[2], coeffs[3]);
    Xmax = (2 * T[2] + 2 * T[3]) * T[3] * Xpeak[3] / 2; // (3)

    if (Xmax > Xpeak[1])
    {
        n = 2;
        coeffs[0] = Xpeak[3];
        coeffs[1] = T[2] * Xpeak[3];
        coeffs[2] = -Xpeak[1];
        T[3] = quadratic_solve(coeffs[0], coeffs[1], coeffs[2]);
    }
    else
    {
        Xpeak[1] = Xmax;
    }

    Xmax = T[3] * Xpeak[3];

    if (Xmax > Xpeak[2])
    {
        n = 1;
        coeffs[0] = Xpeak[3];
        coeffs[1] = -Xpeak[2];
        T[3] = quadratic_solve(coeffs[0], coeffs[1], 0);
    }
    else
    {
        Xpeak[2] = Xmax;
    }

    n = 2;
    coeffs[0] = T[3] * Xpeak[3];
    coeffs[1] = T[1] * T[3] * Xpeak[3] + 3 * powf(T[3], 2) * Xpeak[3];
    coeffs[2] = T[1] * powf(T[3], 2) * Xpeak[3] + 2 * powf(T[3], 3) * Xpeak[3] - Xpeak[0];
    T[2] = quadratic_solve(coeffs[0], coeffs[1], coeffs[2]);

    Xmax = (2 * T[2] + 2 * T[3]) * T[3] * Xpeak[3] / 2;

    if (Xmax > Xpeak[1])
    {
        n = 1;
        coeffs[0] = T[3] * Xpeak[3];
        coeffs[1] = powf(T[3], 2) * Xpeak[3] - Xpeak[1];
        T[2] = quadratic_solve(coeffs[0], coeffs[1], 0);
    }
    else
    {
        Xpeak[1] = Xmax;
    }

    n = 1;
    coeffs[0] = T[2] * T[3] * Xpeak[3] + powf(T[3], 2) * Xpeak[3];
    coeffs[1] = powf(T[2], 2) * T[3] * Xpeak[3] + 3 * T[2] * powf(T[3], 2) * Xpeak[3] + 2 * powf(T[3], 3) * Xpeak[3] - Xpeak[0];
    T[1] = quadratic_solve(coeffs[0], coeffs[1], 0);
}

double getSetpoint(double Xpeak[], double T[], double t, int *phase)
{
    // T[0] is time constant of distance peak (final motion time)
    // T[1] is period of velocity peak stage
    // T[2] is period of acceleration peak stage
    // T[3] is period of jerk peak stage

    if (T[0] <= t && t <= T[3] && *phase < 2)
    {
        *phase = 1;
        return powf((t - (T[0])), 3) * Xpeak[3] / 6; //  J*t^3/6
    }
    if (T[3] <= t && t <= T[2] + T[3] && *phase < 3)
    {
        *phase = 2;
        //(t - T[3])^2 * amax / 2 + (t - T[3]) * T[3]^2 * jmax / 2 + T[3]^3 * jmax / 6
        return powf((t - (T[3])), 2) * Xpeak[2] / 2 + (t - (T[3])) * powf(T[3], 2) * Xpeak[3] / 2 + powf(T[3], 3) * Xpeak[3] / 6;
    }
    if (T[2] + T[3] <= t && t <= T[2] + 2 * T[3] && *phase < 4)
    {
        *phase = 3;
        return -powf((t - (T[2] + T[3])), 3) * Xpeak[3] / 6 + powf((t - (T[2] + T[3])), 2) * T[3] * Xpeak[3] / 2 + (t - (T[2] + T[3])) * (T[2] * Xpeak[2] + powf(T[3], 2) * Xpeak[3] / 2) + powf(T[2], 2) * Xpeak[2] / 2 + T[2] * powf(T[3], 2) * Xpeak[3] / 2 + powf(T[3], 3) * Xpeak[3] / 6;
    }
    if (T[2] + 2 * T[3] <= t && t <= T[1] + T[2] + 2 * T[3] && *phase < 5)
    {
        *phase = 4;
        return (t - (T[2] + 2 * T[3])) * Xpeak[1] + (T[2] * Xpeak[2] + powf(T[3], 2) * Xpeak[3] / 2) * T[3] + powf(T[2], 2) * Xpeak[2] / 2 + T[2] * powf(T[3], 2) * Xpeak[3] / 2 + powf(T[3], 3) * Xpeak[3] / 2;
    }
    if (T[1] + T[2] + 2 * T[3] <= t && t <= T[1] + T[2] + 3 * T[3] && *phase < 6)
    {
        *phase = 5;
        return -powf((t - (T[1] + T[2] + 2 * T[3])), 3) * Xpeak[3] / 6 + (t - (T[1] + T[2] + 2 * T[3])) * (T[2] * Xpeak[2] + powf(T[3], 2) * Xpeak[3]) + (T[2] * Xpeak[2] + powf(T[3], 2) * Xpeak[3] / 2) * T[3] + T[1] * Xpeak[1] + powf(T[2], 2) * Xpeak[2] / 2 + T[2] * powf(T[3], 2) * Xpeak[3] / 2 + powf(T[3], 3) * Xpeak[3] / 2;
    }
    if (T[1] + T[2] + 3 * T[3] <= t && t <= T[1] + 2 * T[2] + 3 * T[3] && *phase < 7)
    {
        *phase = 6;
        return -powf((t - (T[1] + T[2] + 3 * T[3])), 2) * Xpeak[2] / 2 + (t - (T[1] + T[2] + 3 * T[3])) * (T[2] * Xpeak[2] + powf(T[3], 2) * Xpeak[3] / 2) + (T[2] * Xpeak[2] + powf(T[3], 2) * Xpeak[3] / 2) * T[3] + (T[2] * Xpeak[2] + powf(T[3], 2) * Xpeak[3]) * T[3] + T[1] * Xpeak[1] + powf(T[2], 2) * Xpeak[2] / 2 + T[2] * powf(T[3], 2) * Xpeak[3] / 2 + powf(T[3], 3) * Xpeak[3] / 3;
    }
    if ((T[1] + 2 * T[2] + 3 * T[3] <= t && t <= T[1] + 2 * T[2] + 4 * T[3]) && *phase < 8)
    {
        *phase = 7;
        return powf((t - (T[1] + 2 * T[2] + 3 * T[3])), 3) * Xpeak[3] / 6 - powf((t - (T[1] + 2 * T[2] + 3 * T[3])), 2) * T[3] * Xpeak[3] / 2 + (t - (T[1] + 2 * T[2] + 3 * T[3])) * powf(T[3], 2) * Xpeak[3] / 2 + (T[2] * Xpeak[2] + powf(T[3], 2) * Xpeak[3] / 2) * T[2] + (T[2] * Xpeak[2] + powf(T[3], 2) * Xpeak[3] / 2) * T[3] + (T[2] * Xpeak[2] + powf(T[3], 2) * Xpeak[3]) * T[3] + T[1] * Xpeak[1] + T[2] * powf(T[3], 2) * Xpeak[3] / 2 + powf(T[3], 3) * Xpeak[3] / 3;
    }
    else
    {
        *phase = 8;
        return Xpeak[0];
    }
}
