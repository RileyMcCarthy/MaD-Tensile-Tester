#include "MotionPlanning.h"
// Needs to know current set, current quartet, quartet execution.

double position_profile(double t, int *currentSet, int *currentQuartet, int *currentExecution, MotionProfile *profile)
{
    double position = position_set(t, currentQuartet, currentExecution, &(profile->sets[*currentSet]));
    if (*currentQuartet > profile->sets[*currentSet].quartetCount)
    {
        *currentSet = *currentSet + 1;
        *currentQuartet = 0;
        *currentExecution = 0;
    }
    return position;
}

double position_set(double t, int *currentQuartet, int *currentExecution, MotionSet *set)
{
    double position = position_quartet(t, currentExecution, &(set->quartets[*currentQuartet]));
    if (*currentExecution >= set->executions)
    {
        *currentQuartet = *currentQuartet + 1;
        *currentExecution = 0;
    }
    if (*currentQuartet > set->quartetCount)
    {
        *currentQuartet = 0;
        *currentExecution = 0;
    }
}

double position_quartet(double t, int *currentExecution, MotionQuartet *quartet)
{
    FunctionInfo *info = get_function_info(quartet->function);
    double position = info->func(t, quartet->parameters); // Quartet position
    if (position == quartet->distanceMax)
    {
        *currentExecution = *currentExecution + 1;
    }
    return position;
}

// args = [distance, strain rate, error]
double sigmoid(float t, double *args)
{
    double distance = args[0];
    double strain_rate = args[1];
    double error = args[2];
    double E = error;
    double A = distance;
    double C = strain_rate * 4 / A;
    double D = log(A / E - 1) / C;
    return distance / (1 + expf(-1 * C * (t - D)));
}

void update_quartet_function(MotionQuartet *quartet)
{
    FunctionInfo *info = get_function_info(quartet->function);
    quartet->parameters = (double *)realloc(quartet->parameters, sizeof(double) * info->args_count);
    for (int i = 0; i < info->args_count; i++)
    {
        quartet->parameters[i] = 0;
    }
    free_function_info(info);
}

FunctionInfo *get_function_info(QuartetFunctions id)
{
    FunctionInfo *info = (FunctionInfo *)malloc(sizeof(FunctionInfo));
    switch (id)
    {
    case QUARTET_FUNC_LINE:
    {
        info->id = QUARTET_FUNC_LINE;

        char *name = "Line";
        info->name = (char *)malloc(strlen(name) + 1);
        strcpy(info->name, name);

        info->func = NULL;

        info->args_count = 2;
        info->args = (char **)malloc(sizeof(char *) * info->args_count);

        info->args[0] = (char *)malloc(strlen("distance") + 1);
        strcpy(info->args[0], "distance");
        info->args[1] = (char *)malloc(strlen("strain rate") + 1);
        strcpy(info->args[1], "strain rate");
        break;
    }
    case QUARTET_FUNC_SIGMOIDAL:
    {
        info->id = QUARTET_FUNC_SIGMOIDAL;

        char *name = "Sigmoid";
        info->name = (char *)malloc(strlen(name) + 1);
        strcpy(info->name, name);

        info->func = sigmoid;

        info->args_count = 3;
        info->args = (char **)malloc(sizeof(char *) * info->args_count);

        info->args[0] = (char *)malloc(strlen("distance") + 1);
        strcpy(info->args[0], "distance");
        info->args[1] = (char *)malloc(strlen("strain rate") + 1);
        strcpy(info->args[1], "strain rate");
        info->args[2] = (char *)malloc(strlen("error") + 1);
        strcpy(info->args[2], "error");
        break;
    }
    default:
        free(info);
        return NULL;
        break;
    }
    return info;
}

void free_function_info(FunctionInfo *info)
{
    if (info != NULL)
    {
        if (info->name != NULL)
        {
            free(info->name);
        }
        if (info->args != NULL)
        {
            for (int i = 0; i < info->args_count; i++)
            {
                if (info->args[i] != NULL)
                {
                    free(info->args[i]);
                }
            }
            free(info->args);
        }
        free(info);
    }
}

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
static MotionPeriod *compute_period(double x_goal, double x0, double v0, double v_max, double a_max)
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
    MotionPeriod *periods = malloc(sizeof(MotionPeriod));
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
    return periods;
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

/*void simulate_profile(SetPoint *setpoint, double t, double v_max, double a_max, float (*f)(float t, va_list args), ...)
{
    va_list args;
    va_start(args, f);

    // Computesetpoint
    MotionPeriod *motionPeriods = compute_period(sigmoid(t, args), setpoint->x, setpoint->v, v_max, a_max);
    compute_setpoint(setpoint, t - setpoint->t, motionPeriods);
    setpoint->t = t;
    va_end(args);
}*/

SetPoint *create_empty_setpoint()
{
    SetPoint *setpoint = malloc(sizeof(SetPoint));
    setpoint->t = 0;
    setpoint->x = 0;
    setpoint->v = 0;
    setpoint->a = 0;
    return setpoint;
}
