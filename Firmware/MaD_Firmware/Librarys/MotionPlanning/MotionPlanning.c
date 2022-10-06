#include "MotionPlanning.h"
#include <JSON.h>
// Needs to know current set, current quartet, quartet execution.

void run_motion_profile_init(RunMotionProfile *run)
{
    run->currentSet = 0;
    run->currentExecution = 0;
    run->currentQuartet = 0;

    run->profileComplete = false;
    run->setComplete = false;
    run->quartetComplete = false;
    run->lastQuartetTime = 0;
    run->lastQuartetDistance = 0;
    run->dwellTime = 0;
    run->lastExecutionDistance = 0;
    run->lastSetDistance = 0;
}

double position_profile(double t, double position, RunMotionProfile *run, MotionProfile *profile)
{
    if (run == NULL || profile == NULL)
    {
        // printf("Null pointer passed to position_profile\n");
        return 0;
    }
    if (run->currentSet > MAX_MOTION_PROFILE_SETS)
    {
        // printf("Error: currentSet > MAX_MOTION_PROFILE_SETS\n");
        run->currentSet = 0;
    }
    double position = position_set(t, position, run, &(profile->sets[run->currentSet]));
    if (run->setComplete)
    {
        run->currentSet++;
        run->setComplete = false;
    }
    if (run->currentSet >= profile->setCount)
    {
        run->currentSet = 0;
        run->profileComplete = true;
    }
    return position;
}

double position_set(double t, double position, RunMotionProfile *run, MotionSet *set)
{
    if (run == NULL || set == NULL)
    {
        //("Null pointer passed to position_set\n");
        return 0;
    }
    if (run->currentQuartet > MAX_MOTION_QUARTETS)
    {
        // printf("Error: currentQuartet > MAX_MOTION_QUARTETS\n");
        run->currentQuartet = 0;
    }
    double position = position_quartet(t, position, run, &(set->quartets[run->currentQuartet]));
    if (run->quartetComplete)
    {
        // printf("quartet complete:%d\n", run->currentQuartet);
        run->currentQuartet++;
        run->quartetComplete = false;
    }
    if (run->currentQuartet >= set->quartetCount)
    {
        // printf("Set %d execution %d done\n", run->currentSet, run->currentExecution);
        run->currentExecution++;
        run->currentQuartet = 0;
    }
    if (run->currentExecution >= set->executions)
    {
        // printf("Set %d done\n", run->currentSet);
        run->setComplete = true;
        run->currentExecution = 0;
    }
    return position;
}

double position_quartet(double t, double position, RunMotionProfile *run, MotionQuartet *quartet)
{
    if (run == NULL || quartet == NULL)
    {
        //("Null pointer passed to position_set\n");
        return 0;
    }

    FunctionInfo info;
    get_function_info(&info, quartet->function);

    if (info.func == NULL)
    {
        // printf("no function for %s\n", info.name);
        run->lastQuartetTime = t;
        run->lastQuartetDistance += quartet->parameters[0];
        run->quartetComplete = true;
        return 0;
    }
    double positionSetpoint = info.func(t - run->lastQuartetTime, quartet->parameters); // Get setpoint
    if (abs(positionSetpoint) > abs(quartet->parameters[0]))
    {
        positionSetpoint = quartet->parameters[0];
    }

    if (abs(position - quartet->parameters[0]) < 0.1) // Actual position has reached final position
    {
        if (run->dwellTime == 0) // Dwell has not started
        {
            run->dwellTime = t;
        }
        if (t - run->dwellTime > quartet->dwell) // Dwell period is done, quartet has completed
        {
            run->dwellTime = 0;
            run->lastQuartetTime = t;
            run->lastQuartetDistance += quartet->parameters[0];
            run->quartetComplete = true;
        }
    }
    return run->lastQuartetDistance + positionSetpoint;
}

// args = [distance, strain rate, error]
double sigmoid(double t, double *args)
{
    double distance = args[0];
    double strain_rate = args[1];
    double error = args[2];

    int dir = 1;
    if (distance < 0)
    {
        dir = -1;
        distance = -distance;
    }

    double E = error;
    double A = distance;
    double C = strain_rate * 4 / A;
    double D = log(A / E - 1) / C;
    double position = distance / (1 + expf(-1 * C * (t - D)));
    if (abs(abs(position) - abs(distance)) < abs(error))
    { // If position is less than error, then we are at the end of the function.
        return dir * distance;
    }
    return dir * position;
}

// args = [distance, strain rate]
double line(double t, double *args)
{
    double position = t * args[1];
    if (abs(position) > abs(args[0]))
    {
        return args[0];
    }
    return position;
}

void get_function_info(FunctionInfo *info, int id)
{
    switch (id)
    {
    case QUARTET_FUNC_LINE:
    {
        info->id = QUARTET_FUNC_LINE;

        strncpy(info->name, "Line", FUNCTION_MAX_NAME_LENGTH);

        info->func = line;

        info->args_count = 2;
        strncpy(info->args[0], "distance", FUNCTION_MAX_ARG_LENGTH);
        strncpy(info->args[1], "strain rate", FUNCTION_MAX_ARG_LENGTH);
        break;
    }
    case QUARTET_FUNC_SIGMOIDAL:
    {
        info->id = QUARTET_FUNC_SIGMOIDAL;

        strncpy(info->name, "Sigmoid", FUNCTION_MAX_NAME_LENGTH);

        info->func = sigmoid;

        info->args_count = 3;
        strncpy(info->args[0], "distance", FUNCTION_MAX_ARG_LENGTH);
        strncpy(info->args[1], "strain rate", FUNCTION_MAX_ARG_LENGTH);
        strncpy(info->args[2], "error", FUNCTION_MAX_ARG_LENGTH);
        break;
    }
    default:
        info->id = QUARTET_FUNC_SIGMOIDAL;

        strcpy(info->name, "");

        info->func = NULL;

        info->args_count = 0;
        break;
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

int mm_to_steps(double mm, MachineConfiguration *config)
{
    if (config == NULL)
    {
        // printf("mm_to_steps: config is NULL\n");
        return 0;
    }
    return (int)round(mm * (double)config->positionEncoderStepsPerRev / (double)(config->gearDiameter * 3.14159));
}
