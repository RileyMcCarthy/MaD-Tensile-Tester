#include "MotionPlanning.h"

typedef struct Data_s
{
    float x, y;
} Data;

static float interpolate(Data *f, float xi, int n)
{
    float result = 0; // Initialize result
                      // printf("interpolate: xi: %f\n", xi);
    for (int i = 0; i < n; i++)
    {
        // Compute individual terms of above formula
        float term = f[i].y;
        for (int j = 0; j < n; j++)
        {
            if (j != i)
                term = ((int)((term * (xi - f[j].x) / (f[i].x - f[j].x)) * 10000)) / 10000.0;
            //  printf("bottom:%f top: %f\n", term * (xi - f[j].x), xi - f[j].x);
        }
        //  printf("term: %f, result:%f\n", term, result);
        // Add current term to result
        result += term;
    }

    return result;
}

static float vsolve(float point, float (*f)(float t, va_list args), va_list args)
{
    float t = 0;
    while (abs(t - point) > 0.001)
    {
        t = t + f(t, args) / central_difference(f(t + 0.1, args), f(t - 0.1, args), 0.1);
    }
    return t;
}

// args = [distance, strain rate, error]
float sigmoid(float t, va_list args)
{
    float distance = va_arg(args, float);
    float strain_rate = va_arg(args, float);
    float error = va_arg(args, float);
    float E = error;
    float A = distance;
    float C = strain_rate * 4 / A;
    float D = log(A / E - 1) / C;
    return distance / (1 + expf(-1 * C * (t - D)));
}

float central_difference(float d1, float dn1, float h)
{
    return (-0.5 * dn1 + 0.5 * d1) / h;
}

float vcentral_difference(float point, float step, float (*f)(float t, va_list args), va_list args)
{
    return (-0.5 * f(point - step, args) + 0.5 * f(point + step, args)) / step;
}

float vcentral_difference_d2x(float point, float step, float (*f)(float t, va_list args), va_list args)
{
    return (f(point - step, args) - 2 * f(point, args) + f(point + step, args)) / powf(step, 2);
}

float central_difference_order_two(float d1, float d0, float dn1, float h)
{
    return (dn1 - 2 * d0 + d1) / powf(h, 2);
}
float central_difference_order_three(float d2, float d1, float dn1, float dn2, float h)
{
    return (-0.5 * dn2 + dn1 - d1 + 0.5 * d2) / powf(h, 3);
}

float secant_method(float point, float t0, float t1, float (*f)(float t, va_list args), va_list args)
{
    float t2 = t1;
    while (abs(f(t2, args) - point) < 0.01)
    {
        float t2 = t1 - (f(t1, args) - point) * (t1 - t0) / (f(t1, args) - f(t0, args));
        t0 = t1;
        t1 = t2;
    }
    return t2;
}

float secant_method_d2x(float point, float t0, float step, float (*f)(float t, va_list args), va_list args)
{
    float fx0 = 0;
    float t1 = t0 + step;
    printf("starting secant: %f %f\n", t0, t1);

    while (abs(fx0 = (vcentral_difference(t1, step, f, args) - point)) > 0.01)
    {
        t1 = t1 - (vcentral_difference(t1, step, f, args) - point) * (t1 - t0) / ((vcentral_difference(t1, step, f, args) - point) - (vcentral_difference(t0, step, f, args) - point));
        t0 = t1;
        printf("secant: %f,%f,%f\n", t0, t1, fx0);
    }
    printf("secant: %f,%f,%f\n", t0, t1, fx0);

    return t1;
}

void simulate_profile(float distance, float max_jerk, float max_acceleration, float max_velocity, float stepSize, float error, float (*f)(float t, va_list args), ...)
{
    va_list args;

    va_start(args, f);
    /* Position */
    float dn3 = f(-3 * stepSize, args);
    float dn2 = f(-2 * stepSize, args);
    float dn1 = f(-stepSize, args);
    float d0 = f(0, args);
    float d1 = f(stepSize, args);
    float d2 = f(2 * stepSize, args);
    float d3 = f(3 * stepSize, args);
    /* Velocity */
    float d1dn2 = central_difference(dn1, dn3, stepSize);
    float d1dn1 = central_difference(d0, dn2, stepSize);
    float d1d0 = central_difference(d1, dn1, stepSize);
    float d1d1 = central_difference(d2, d0, stepSize);
    float d1d2 = central_difference(d3, d1, stepSize);

    /* Acceleration */
    float d2dn1 = central_difference(d1d0, d1dn2, stepSize);
    float d2d0 = central_difference(d1d1, d1dn1, stepSize);
    float d2d1 = central_difference(d1d2, d1d0, stepSize);

    /* Jerk */
    float d3d0 = central_difference(d2d1, d2dn1, stepSize);

    int i = 1;
    float offset = 0;
    while (((distance - d0) > error) && ((distance - d0) > 0))
    {
        float t = i * stepSize;

        /* Position */
        dn3 = dn2;
        dn2 = dn1;
        dn1 = d0;
        d0 = d1;
        d1 = d2;
        d2 = d3;
        d3 = f(t + 3 * stepSize, args);

        /* Velocity */
        d1dn2 = central_difference(dn1, dn3, stepSize);
        d1dn1 = central_difference(d0, dn2, stepSize);
        d1d0 = central_difference(d1, dn1, stepSize);
        d1d1 = central_difference(d2, d0, stepSize);
        d1d2 = central_difference(d3, d1, stepSize);

        /* Acceleration */
        d2dn1 = central_difference(d1d0, d1dn2, stepSize);
        d2d0 = central_difference(d1d1, d1dn1, stepSize);
        d2d1 = central_difference(d1d2, d1d0, stepSize);

        /* Jerk */
        d3d0 = central_difference(d2d1, d2dn1, stepSize);

        if (abs(d1d2 = central_difference(d3, d1, stepSize)) > max_velocity)
        {
            float t_max_velocity = secant_method_d2x(max_velocity, t + stepSize * 3, stepSize, f, args);
            d3 = f(t_max_velocity, args);
            printf("max velocity: %f\n", d3);
        }
        d1d2 = central_difference(d3, d1, stepSize);
        d1d0 = central_difference(d1, dn1, stepSize);
        /*if (abs(d2d1 = central_difference(d1d2, d1d0, stepSize)) > max_acceleration)
        {
            float step = abs(d3 - d2);
            d2dn1 = central_difference(d1d1, d1dn1, stepSize);
            d2d1 = central_difference(d1d2, d1d0, stepSize);
            if (central_difference(d2d1, d2dn1, stepSize) > 0)
            {
                step *= -1;
            }
            offset = 0;
            while (abs(central_difference(d1d2, d1d0, stepSize)) - max_acceleration > 0.0001)
            {
                step /= 2;
                while (abs(d2d1 = central_difference(d1d2, d1d0, stepSize)) > max_acceleration)
                {
                    offset += step;
                    d3 = f(t + 3 * stepSize, args) - offset;
                    d1d2 = central_difference(d3, d1, stepSize);
                    d1d0 = central_difference(d1, dn1, stepSize);
                    printf("offsetacc: %f,d3:%f,d2:%f,%f\n", offset, d3, d2, d2d1);
                }
                offset -= step;
                d3 = f(t + 3 * stepSize, args) - offset;
                d1d2 = central_difference(d3, d1, stepSize);
                d1d0 = central_difference(d1, dn1, stepSize);
                step /= 2;
            }
        }*/
        /* Recalculate */
        /* Velocity */
        d1dn2 = central_difference(dn1, dn3, stepSize);
        d1dn1 = central_difference(d0, dn2, stepSize);
        d1d0 = central_difference(d1, dn1, stepSize);
        d1d1 = central_difference(d2, d0, stepSize);
        d1d2 = central_difference(d3, d1, stepSize);

        /* Acceleration */
        d2dn1 = central_difference(d1d0, d1dn2, stepSize);
        d2d0 = central_difference(d1d1, d1dn1, stepSize);
        d2d1 = central_difference(d1d2, d1d0, stepSize);

        /* Jerk */
        d3d0 = central_difference(d2d1, d2dn1, stepSize);

        printf("%f,%f,%f,%f,%f,\n", t, d0, d1d0, d2d0, d3d0);
        i++;
    }
}

/*Generates motion profile for straight line motion*/
float *motion_profile(float j_max, float a_max, float v_max, float d_max, int points)
{
    printf("Planning motion...\n");
    bool phaseTwo = true;
    bool phaseFour = true;

    /**This section is to plan the timing need to complete each phase**/

    // Phase 1 constant jerk, increasing acceleration
    float t1_vmax = sqrtf(v_max / j_max);
    float t1_amax = a_max / j_max;
    float t1_dmax = powf(d_max / (2 * j_max), 1.0 / 3.0);
    printf("phase 1:vmax=%f,amax=%f,dmax=%f\n", t1_vmax, t1_amax, t1_dmax);
    // Find the lowest t1
    float t1;
    if (t1_vmax < t1_amax && t1_vmax < t1_dmax)
    {
        printf("%d is smallest", t1_vmax);
        phaseTwo = false;
        phaseFour = true;
        t1 = t1_vmax;
    }
    else if (t1_amax < t1_dmax)
    {
        printf("%d is smallest", t1_amax);
        phaseTwo = true;
        phaseFour = true;
        t1 = t1_amax;
    }
    else
    {
        printf("%d is smallest", t1_dmax);
        phaseTwo = false;
        phaseFour = false;
        t1 = t1_dmax;
    }

    /*Phase 2*/
    float t2;
    if (phaseTwo)
    {
        float t2_vmax = (-2 * j_max * powf(t1, 2) + v_max) / (j_max * t1);
        float t2_dmax = (2 * sqrtf((j_max * powf(t1, 3)) / 4 + d_max) - 3 * sqrtf(j_max) * powf(t1, 3.0 / 2.0)) / (2 * sqrtf(j_max) * sqrtf(t1));
        if (t2_vmax > t2_dmax)
        {
            phaseFour = false;
            t2 = t2_dmax;
        }
        else
        {
            t2 = t2_vmax;
        }
        //@todo add check here for negitive time
    }
    else
    {
        t2 = 0;
    }

    /*Phase 3*/
    float t3 = t1;

    /*Phase 4*/
    float t4;
    if (phaseFour)
    {
        t4 = -(2 * (2 * j_max * powf(t1, 3) + 3 * j_max * powf(t1, 2) * t2 + j_max * t1 * powf(t2, 2) - d_max)) / (2 * j_max * powf(t1, 2) + 2 * j_max * t2 * t1);
        //@todo add check here for negitive time
    }
    else
    {
        t4 = 0;
    }
    //@todo add remaining phases
    float t5 = t1;
    float t6 = t2;
    float t7 = t3;
    printf("t1:%f,t2:%f,t3:%f,t4:%f\n", t1, t2, t3, t4);

    /**This section is to use the timing calculated previously and calculate the motion**/
    float time1 = t1;
    float time2 = time1 + t2;
    float time3 = time2 + t3;
    float time4 = time3 + t4;
    float time5 = time4 + t5;
    float time6 = time5 + t6;
    float time7 = time6 + t7;
    printf("time1:%f,time2:%f,time3:%f,time4:%f\n", time1, time2, time3, time4);
    /*
     * Note due to large memory allocation, this section may want to be split into
     * multiple functions that takes t1,t2,t3,t4 as parameters then we can draw them
     * on the graph individually
     */
    float *distance = (float *)malloc(sizeof(float) * points);
    // float *acceleration = (float *)malloc(sizeof(float) * points);
    // float *velocity = (float *)malloc(sizeof(float) * points);
    // float *jerk = (float *)malloc(sizeof(float) * points);
    float increment = time7 / (points - 1); //@todo this should be total time instead of t4
    printf("increment:%f\n", increment);
    /*for (int i = 0; i < points; i++) //@todo this equations are too large!, lookup good interpolation methods. this is just visualization.
    {
        float t_global = increment * ((float)i);
        float t;
        int midPos;
        // printf("time(%d):%f,\n", i, t);
        if (t_global < time1) // Phase 1
        {
            t = t_global;
            distance[i] = (j_max * powf(t, 3)) / 6;
        }
        else if (t_global < time2) // Phase 2
        {
            t = t_global - time1;
            distance[i] = (j_max * powf(t1, 3)) / 6 + (j_max * t * powf(t1, 2)) / 2 + (j_max * t1 * powf(t, 2)) / 2;
        }
        else if (t_global < time3) // Phase 3
        {
            t = t_global - time2;
            distance[i] = (j_max * powf(t1, 3)) / 6 + (j_max * t2 * powf(t1, 2)) / 2 + (j_max * t1 * powf(t2, 2)) / 2;
            distance[i] += (j_max * t * powf(t1, 2)) / 2 + j_max * t1 * t2 * t + (j_max * t1 * powf(t, 2)) / 2 - (j_max * powf(t, 3)) / 6;
            midPos = i;
        }
        else if (t_global < time4) // Phase 4
        {
            t = t_global - time3;

            distance[i] = (j_max * powf(t1, 3)) / 6 + t * ((j_max * powf(t1, 2)) / 2 + (j_max * t3 * (2 * t1 - t3)) / 2 + j_max * t1 * t2);
            distance[i] += (j_max * t3 * (3 * powf(t1, 2) + 3 * t1 * t3 + 6 * t2 * t1 - powf(t3, 2))) / 6 + (j_max * t1 * t2 * (t1 + t2)) / 2;
            midPos = i;
        }
        else if (t_global < time5) // Phase 5
        {                          // this method is very flawed, will need to use equations
            t = t_global - time4;
            distance[i] = (j_max * powf(t1, 3)) / 6 + t4 * ((j_max * powf(t1, 2)) / 2 + (j_max * t3 * (2 * t1 - t3)) / 2 + j_max * t1 * t2);
            distance[i] += (j_max * t3 * (3 * powf(t1, 2) + 3 * t1 * t3 + 6 * t2 * t1 - powf(t3, 2))) / 6;
            distance[i] += (j_max * t * (-powf(t, 2) + 3 * powf(t1, 2) + 6 * t1 * t3 + 6 * t2 * t1 - 3 * powf(t3, 2))) / 6 + (j_max * t1 * t2 * (t1 + t2)) / 2;
        }
        else if (t_global < time6) // Phase 6
        {
            t = t_global - time5;
            distance[i] = (j_max * powf(t1, 3)) / 6 + t4 * ((j_max * powf(t1, 2)) / 2 + (j_max * t3 * (2 * t1 - t3)) / 2 + j_max * t1 * t2);
            distance[i] += -(j_max * powf(t, 2) * t5) / 2 + j_max * t * (t1 * t2 + powf(t1, 2) / 2 - powf(t5, 2) / 2 + (t3 * (2 * t1 - t3)) / 2);
            distance[i] += (j_max * t3 * (3 * powf(t1, 2) + 3 * t1 * t3 + 6 * t2 * t1 - powf(t3, 2))) / 6;
            distance[i] += (j_max * t5 * (3 * powf(t1, 2) + 6 * t1 * t3 + 6 * t2 * t1 - 3 * powf(t3, 2) - powf(t5, 2))) / 6 + (j_max * t1 * t2 * (t1 + t2)) / 2;
        }
        else if (t_global < time7) // Phase 7
        {
            t = t_global - time6;

            distance[i] = (j_max * powf(t1, 3)) / 6 + t4 * ((j_max * powf(t1, 2)) / 2 + (j_max * t3 * (2 * t1 - t3)) / 2 + j_max * t1 * t2);
            distance[i] += -(j_max * t5 * powf(t6, 2)) / 2;
            distance[i] += j_max * t6 * (t1 * t2 + powf(t1, 2) / 2 - powf(t5, 2) / 2 + (t3 * (2 * t1 - t3)) / 2);
            distance[i] += (j_max * t * (powf(t, 2) - 3 * t * t5 + 3 * powf(t1, 2) + 6 * t1 * t3 + 6 * t2 * t1 - 3 * powf(t3, 2) - 3 * powf(t5, 2) - 6 * t6 * t5)) / 6;
            distance[i] += (j_max * t3 * (3 * powf(t1, 2) + 3 * t1 * t3 + 6 * t2 * t1 - powf(t3, 2))) / 6;
            distance[i] += (j_max * t5 * (3 * powf(t1, 2) + 6 * t1 * t3 + 6 * t2 * t1 - 3 * powf(t3, 2) - powf(t5, 2))) / 6 + (j_max * t1 * t2 * (t1 + t2)) / 2;
        }
        printf("%f,%f\n", distance[i], increment * i);
    }*/
    Data data[8];
    int index = 0;
    /*start zero*/
    data[index].x = 0;
    data[index].y = 0;
    index++;
    /*end of phase 1*/
    data[index].y = (j_max * powf(t1, 3)) / 6;
    data[index].x = time1;
    index++;

    if (phaseTwo)
    {
        /*end of phase 2*/
        data[index].x = (j_max * powf(t1, 3)) / 6 + (j_max * t2 * powf(t1, 2)) / 2 + (j_max * t1 * powf(t2, 2)) / 2;
        data[index].y = time2;
        index++;
    }

    /*end of phase 3*/
    data[index].y = (j_max * powf(t1, 3)) / 6 + (j_max * t2 * powf(t1, 2)) / 2 + (j_max * t1 * powf(t2, 2)) / 2;
    data[index].y += (j_max * t3 * powf(t1, 2)) / 2 + j_max * t1 * t2 * t3 + (j_max * t1 * powf(t3, 2)) / 2 - (j_max * powf(t3, 3)) / 6;
    data[index].x = time3;
    index++;

    if (phaseFour)
    {
        /*end of phase 4*/
        data[index].y = (j_max * powf(t1, 3)) / 6 + t4 * ((j_max * powf(t1, 2)) / 2 + (j_max * t3 * (2 * t1 - t3)) / 2 + j_max * t1 * t2);
        data[index].y += (j_max * t3 * (3 * powf(t1, 2) + 3 * t1 * t3 + 6 * t2 * t1 - powf(t3, 2))) / 6 + (j_max * t1 * t2 * (t1 + t2)) / 2;
        data[index].x = time4;
        index++;
    }

    /*end of phase 5*/
    data[index].y = (j_max * powf(t1, 3)) / 6 + t4 * ((j_max * powf(t1, 2)) / 2 + (j_max * t3 * (2 * t1 - t3)) / 2 + j_max * t1 * t2);
    data[index].y += (j_max * t3 * (3 * powf(t1, 2) + 3 * t1 * t3 + 6 * t2 * t1 - powf(t3, 2))) / 6;
    data[index].y += (j_max * t5 * (-1 * powf(t5, 2) + 3 * powf(t1, 2) + 6 * t1 * t3 + 6 * t2 * t1 - 3 * powf(t3, 2))) / 6 + (j_max * t1 * t2 * (t1 + t2)) / 2;
    data[index].x = time5;
    index++;

    if (phaseTwo)
    {
        /*end of phase 6*/
        data[index].y = (j_max * powf(t1, 3)) / 6 + t4 * ((j_max * powf(t1, 2)) / 2 + (j_max * t3 * (2 * t1 - t3)) / 2 + j_max * t1 * t2);
        data[index].y += -(j_max * powf(t6, 2) * t5) / 2 + j_max * t6 * (t1 * t2 + powf(t1, 2) / 2 - powf(t5, 2) / 2 + (t3 * (2 * t1 - t3)) / 2);
        data[index].y += (j_max * t3 * (3 * powf(t1, 2) + 3 * t1 * t3 + 6 * t2 * t1 - powf(t3, 2))) / 6;
        data[index].y += (j_max * t5 * (3 * powf(t1, 2) + 6 * t1 * t3 + 6 * t2 * t1 - 3 * powf(t3, 2) - powf(t5, 2))) / 6 + (j_max * t1 * t2 * (t1 + t2)) / 2;
        data[index].x = time6;
        index++;
    }

    /*end of phase 7*/
    printf("dmax: %f\n", d_max);
    data[index].x = time7;
    data[index].y = d_max;
    index++;

    for (int i = 0; i < points; i++)
    {
        distance[i] = interpolate(data, increment * i, index);
        printf("%f,%f\n", distance[i], increment * i);
    }

    printf("time1:%f,time2:%f,time3:%f,time4:%f,time5:%f,time6:%f,time7:%f\n", time1, time2, time3, time4, time5, time6, time7);
    for (int i = 0; i < index; i++)
    {
        printf("%f,%f\n", data[i].x, data[i].y);
    }
    free(distance);
}
