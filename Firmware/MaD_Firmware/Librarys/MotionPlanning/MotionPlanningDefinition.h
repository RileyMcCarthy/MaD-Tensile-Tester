#define FUNCTION_COUNT 2
#define QUARTET_FUNC_LINE 0
#define QUARTET_FUNC_SIGMOIDAL 1

#define FUNCTION_MAX_NAME_LENGTH 10
#define FUNCTION_MAX_ARGS 10
#define FUNCTION_MAX_ARG_LENGTH 20

double line(double t, double *args);
double sigmoid(double t, double *args);

typedef struct functioninfo_s
{
    char name[20];
    double (*func)(double, double *args);
    int args_count;
    char args[FUNCTION_MAX_ARGS][FUNCTION_MAX_ARG_LENGTH]; // max 10 args with 20 chars each
} FunctionInfo;

// Name, function*, # params, param names
const FunctionInfo functions[] = {
    {"Line", line, 2, {"distance", "strain rate"}},
    {"Sigmoid", sigmoid, 3, {"distance", "strain rate", "error"}},
};
