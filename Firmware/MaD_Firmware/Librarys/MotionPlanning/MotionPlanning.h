#ifndef MotionPlanning_H
#define MotionPlanning_H
#include <simpletools.h>
#include <stddef.h>
#include <JSON.h>
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

#define FUNCTION_COUNT 2
typedef enum quartetFunc_s
{
    QUARTET_FUNC_LINE,
    QUARTET_FUNC_SIGMOIDAL,
} QuartetFunctions;

typedef struct functioninfo_s
{
    int id;
    char *name;
    double (*func)(double, void *args);
    int args_count;
    char **args;
} FunctionInfo;

typedef struct RunMotionProfile_s
{
    int currentSet;
    int currentExecution;
    int currentQuartet;

    bool profileComplete;
    bool setComplete;
    bool quartetComplete;

    double lastQuartetTime;

    double lastSetDistance;
    double lastExecutionDistance;
    double lastQuartetDistance;

    double dwellTime;
} RunMotionProfile;

RunMotionProfile *get_run_motion_profile();
void destroy_run_motion_profile(RunMotionProfile *run);

FunctionInfo *get_function_info(QuartetFunctions id);
void free_function_info(FunctionInfo *info);

double position_profile(double t, RunMotionProfile *run, MotionProfile *profile);
double position_set(double t, RunMotionProfile *run, MotionSet *set);
double position_quartet(double t, RunMotionProfile *run, MotionQuartet *quartet);

double sigmoid(float t, double *args);
void simulate_profile(SetPoint *setpoint, double t, double v_max, double a_max, float (*f)(float t, va_list args), ...);
SetPoint *create_empty_setpoint();

double steps_to_mm(int steps, MachineConfiguration *config);
int mm_to_steps(double mm, MachineConfiguration *config);
#endif
