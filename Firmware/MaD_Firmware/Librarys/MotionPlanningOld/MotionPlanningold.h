#include <stdbool.h>
#include <math.h>
#include <stdarg.h>

enum
{
    PROFILE_SIGMOIDAL,
    PROFILE_LINEAR
};

float sigmoid(float t, va_list args);
void simulate_profile(float distance, float max_jerk, float max_acceleration, float max_velocity, float stepSize, float (*f)(float t, va_list args), ...);