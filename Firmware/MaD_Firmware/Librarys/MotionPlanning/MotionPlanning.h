#ifndef MotionPlanning_H
#define MotionPlanning_H
#include <math.h>
#include <stdarg.h>
#include <simpletools.h>
#include <stddef.h>
#include <JSON.h>

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

FunctionInfo *get_function_info(QuartetFunctions id);
void free_function_info(FunctionInfo *info);
void update_quartet_function(MotionQuartet *quartet);

double sigmoid(float t, va_list args);
void simulate_profile(SetPoint *setpoint, double t, double v_max, double a_max, float (*f)(float t, va_list args), ...);
SetPoint *create_empty_setpoint();

#endif
