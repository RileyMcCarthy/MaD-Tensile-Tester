#ifndef MotionPlanning_H
#define MotionPlanning_H
#include <stddef.h>
#include <JSON.h>
#include <math.h>
#include <stdbool.h>
#include "MotionPlanningDefinition.h"

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

void run_motion_profile_init(RunMotionProfile *profile);
void destroy_run_motion_profile(RunMotionProfile *run);

void get_function_info(FunctionInfo *info, int id);

double position_profile(double t, double position, RunMotionProfile *run, MotionProfile *profile);
double position_set(double t, double position, RunMotionProfile *run, MotionSet *set);
double position_quartet(double t, double position, RunMotionProfile *run, MotionQuartet *quartet);

double sigmoid(double t, double *args);
// void simulate_profile(SetPoint *setpoint, double t, double v_max, double a_max, double (*f)(double t, va_list args), ...);

double steps_to_mm(int steps, MachineConfiguration *config);
int mm_to_steps(double mm, MachineConfiguration *config);
#endif
