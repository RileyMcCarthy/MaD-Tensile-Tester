#include "MotionPlanning.h"

float *motion_profile(float j_max, float a_max, float v_max, float d_max, int points)
{
    printf("Planning motion...\n");
    bool phaseTwo = true;
    bool phaseFour = true;

    /**This section is to plan the timing need to complete each phase**/

    //Phase 1 constant jerk, increasing acceleration
    float t1_vmax = sqrtf(v_max / j_max);
    float t1_amax = a_max / j_max;
    float t1_dmax = powf(d_max / (2 * j_max), 1.0 / 3.0);
    printf("phase 1:vmax=%f,amax=%f,dmax=%f\n", t1_vmax, t1_amax, t1_dmax);
    //Find the lowest t1
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
    //float *acceleration = (float *)malloc(sizeof(float) * points);
    //float *velocity = (float *)malloc(sizeof(float) * points);
    //float *jerk = (float *)malloc(sizeof(float) * points);
    float increment = time7 / points; //@todo this should be total time instead of t4
    printf("increment:%f\n", increment);
    for (int i = 0; i < points; i++) //@todo this equations are too large!, lookup good interpolation methods. this is just visualization.
    {
        float t_global = increment * ((float)i);
        float t;
        int midPos;
        //printf("time(%d):%f,\n", i, t);
        if (t_global < time1) //Phase 1
        {
            t = t_global;
            distance[i] = (j_max * powf(t, 3)) / 6;
        }
        else if (t_global < time2) //Phase 2
        {
            t = t_global - time1;
            distance[i] = (j_max * powf(t1, 3)) / 6 + (j_max * t * powf(t1, 2)) / 2 + (j_max * t1 * powf(t, 2)) / 2;
        }
        else if (t_global < time3) //Phase 3
        {
            t = t_global - time2;
            distance[i] = (j_max * powf(t1, 3)) / 6 + (j_max * t2 * powf(t1, 2)) / 2 + (j_max * t1 * powf(t2, 2)) / 2;
            distance[i] += (j_max * t * powf(t1, 2)) / 2 + j_max * t1 * t2 * t + (j_max * t1 * powf(t, 2)) / 2 - (j_max * powf(t, 3)) / 6;
            midPos = i;
        }
        else if (t_global < time4) //Phase 4
        {
            t = t_global - time3;

            distance[i] = (j_max * powf(t1, 3)) / 6 + t * ((j_max * powf(t1, 2)) / 2 + (j_max * t3 * (2 * t1 - t3)) / 2 + j_max * t1 * t2);
            distance[i] += (j_max * t3 * (3 * powf(t1, 2) + 3 * t1 * t3 + 6 * t2 * t1 - powf(t3, 2))) / 6 + (j_max * t1 * t2 * (t1 + t2)) / 2;
            midPos = i;
        }
        else if (t_global < time5) //Phase 5
        {                          //this method is very flawed, will need to use equations
            t = t_global - time4;
            distance[i] = (j_max * powf(t1, 3)) / 6 + t4 * ((j_max * powf(t1, 2)) / 2 + (j_max * t3 * (2 * t1 - t3)) / 2 + j_max * t1 * t2);
            distance[i] += (j_max * t3 * (3 * powf(t1, 2) + 3 * t1 * t3 + 6 * t2 * t1 - powf(t3, 2))) / 6;
            distance[i] += (j_max * t * (-powf(t, 2) + 3 * powf(t1, 2) + 6 * t1 * t3 + 6 * t2 * t1 - 3 * powf(t3, 2))) / 6 + (j_max * t1 * t2 * (t1 + t2)) / 2;
            //distance[i] = -distance[i - midPos]; //use this idea for velocity,accel,jerk
        }
        else if (t_global < time6) //Phase 6
        {
            t = t_global - time5;
            distance[i] = (j_max * powf(t1, 3)) / 6 + t4 * ((j_max * powf(t1, 2)) / 2 + (j_max * t3 * (2 * t1 - t3)) / 2 + j_max * t1 * t2);
            distance[i] += -(j_max * powf(t, 2) * t5) / 2 + j_max * t * (t1 * t2 + powf(t1, 2) / 2 - powf(t5, 2) / 2 + (t3 * (2 * t1 - t3)) / 2);
            distance[i] += (j_max * t3 * (3 * powf(t1, 2) + 3 * t1 * t3 + 6 * t2 * t1 - powf(t3, 2))) / 6;
            distance[i] += (j_max * t5 * (3 * powf(t1, 2) + 6 * t1 * t3 + 6 * t2 * t1 - 3 * powf(t3, 2) - powf(t5, 2))) / 6 + (j_max * t1 * t2 * (t1 + t2)) / 2;
        }
        else if (t_global < time7) //Phase 7
        {
            t = t_global - time6;
            distance[i] = (j_max * powf(t1, 3)) / 6 + t4 * ((j_max * powf(t1, 2)) / 2 + (j_max * t3 * (2 * t1 - t3)) / 2 + j_max * t1 * t2);
            distance[i] += -(j_max * t5 * powf(t6, 2)) / 2;
            distance[i] += j_max * t6 * (t1 * t2 + powf(t1, 2) / 2 - powf(t5, 2) / 2 + (t3 * (2 * t1 - t3)) / 2);
            distance[i] += (j_max * t3 * (3 * powf(t1, 2) + 3 * t1 * t3 + 6 * t2 * t1 - powf(t3, 2))) / 6;
            distance[i] += (j_max * t5 * (3 * powf(t1, 2) + 6 * t1 * t3 + 6 * t2 * t1 - 3 * powf(t3, 2) - powf(t5, 2))) / 6;
            distance[i] += (j_max * t * (powf(t, 2) + 3 * powf(t1, 2) + 6 * t1 * t3 + 6 * t2 * t1 - 3 * powf(t3, 2) - 3 * powf(t5, 2) - 6 * t6 * t5)) / 6;
            distance[i] += (j_max * t1 * t2 * (t1 + t2)) / 2;
        }
        printf("%f,", distance[i]);
    }
    free(distance);
}
