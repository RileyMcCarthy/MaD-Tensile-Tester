function [jerk, acceleration,velocity,distance,totalTime] = motionProfileConvert(j_max,a_max,v_max,d_max,t,d)
syms t positive
syms d positive

phaseTwo = true;
phaseFour = true;

%Phase 1 constant jerk, increasing acceleration
j1 = j_max; 
a1(t) = j_max*t;
v1(t) = (j_max*t^2)/2;
d1(t) = (j_max*t^3)/6;

t_vmax = v_max^(1/2)/j_max^(1/2);
t_amax = a_max/j_max;
t_dmax = ((d_max/(2*j_max))^(1/3));
t1 = min([t_vmax,t_amax,t_dmax]);

if t1==t_vmax
   phaseTwo=false;
   phaseFour=true;
elseif t1==t_amax
    phaseTwo=true;
    phaseFour=true;
elseif t1==t_dmax
    phaseTwo=false;
    phaseFour=false;
end
jerk(t)=j1*t^0;
acceleration(t)=a1(t);
velocity(t)=v1(t);
distance(t)=d1(t);

time1 = t1;
%Phase 2 constant acceleration (only if max acceleration is reached)
j2 = 0;
a2 = j_max*t1;
v2(t) = (j_max*t1^2)/2+j_max*t1*t;
d2(t) = (j_max*t1^3)/6+(j_max*t*t1^2)/2+(j_max*t1*t^2)/2;

if phaseTwo == true
    t_vmax = (- 2*j_max*t1^2 + v_max)/(j_max*t1);
    t_dmax = (2*((j_max*t1^3)/4 + d_max)^(1/2) - 3*j_max^(1/2)*t1^(3/2))/(2*j_max^(1/2)*t1^(1/2));
    t2=min(t_dmax,t_vmax);
    if t2==t_dmax
        phaseFour = false;
    else
        phaseFour = true;
    end
    if isempty(t2) 
        totalTime = time1;
        disp('increase jerk or lower acceleration');
        return;
    end
else 
    t2=0;
end

time2 = time1+t2;
jerk(t)=piecewise(t<=time1,jerk(t),t<=time2,j2);
acceleration(t)=piecewise(t<=time1,acceleration(t),t<=time2,a2);
velocity(t)=piecewise(t<=time1,velocity(t),t<=time2,v2(t-time1));
distance(t)=piecewise(t<=time1,distance(t),t<=time2,d2(t-time1));

%Phase 3 decreasing acceleration
j3 = -j_max;
a3(t) = j_max*t1-j_max*t;
v3(t) = (j_max*t1^2)/2+j_max*t1*t2+j_max*t1*t-(j_max*t^2)/2;
d3(t) = (j_max*t1^3)/6+(j_max*t2*t1^2)/2+(j_max*t1*t2^2)/2+(j_max*t*t1^2)/2+j_max*t1*t2*t +(j_max*t1*t^2)/2-(j_max*t^3)/6;

t3 = t1

time3 = time2+t3;
jerk(t)=piecewise(t<=time2,jerk(t),t<=time3,j3);
acceleration(t)=piecewise(t<=time2,acceleration(t),t<=time3,a3(t-time2));
velocity(t)=piecewise(t<=time2,velocity(t),t<=time3,v3(t-time2));
distance(t)=piecewise(t<=time2,distance(t),t<=time3,d3(t-time2));

%Phase 4 constant velocity
    j4 = 0;
    a4 = 0;
    v4 = (j_max*t1^2)/2 + (j_max*t3*(2*t1 - t3))/2 + j_max*t1*t2
    d4(t) = (j_max*t1^3)/6 + t*((j_max*t1^2)/2 + (j_max*t3*(2*t1 - t3))/2 + j_max*t1*t2) + (j_max*t3*(3*t1^2 + 3*t1*t3 + 6*t2*t1 - t3^2))/6 + (j_max*t1*t2*(t1 + t2))/2
if phaseFour == true
    
    t4 = -(2*(2*j_max*t1^3 + 3*j_max*t1^2*t2 + j_max*t1*t2^2 - d_max))/(2*j_max*t1^2 + 2*j_max*t2*t1);
    if isempty(t4) 
        disp('lower max speed or increase acceleration');
        totalTime = time3;
        return;
    end
else
    t4=0
end
time4 = time3+t4;
jerk(t)=piecewise(t<=time3,jerk(t),t<=time4,j4);
acceleration(t)=piecewise(t<=time3,acceleration(t),t<=time4,a4);
velocity(t)=piecewise(t<=time3,velocity(t),t<=time4,v4);
distance(t)=piecewise(t<=time3,distance(t),t<=time4,d4(t-time3));

%Phase 5
j5 = -j_max;
a5(t) = -j_max*t;
v5(t) = (j_max*t1^2)/2 - (j_max*t^2)/2 + (j_max*t3*(2*t1 - t3))/2 + j_max*t1*t2;
d5(t) = (j_max*t1^3)/6 + t4*((j_max*t1^2)/2 + (j_max*t3*(2*t1 - t3))/2 + j_max*t1*t2) + (j_max*t3*(3*t1^2 + 3*t1*t3 + 6*t2*t1 - t3^2))/6 + (j_max*t*(- t^2 + 3*t1^2 + 6*t1*t3 + 6*t2*t1 - 3*t3^2))/6 + (j_max*t1*t2*(t1 + t2))/2;

t5 = t1;
time5 = time4 + t5;
jerk(t)=piecewise(t<=time4,jerk(t),t<=time5,j5);
acceleration(t)=piecewise(t<=time4,acceleration(t),t<=time5,a5(t-time4));
velocity(t)=piecewise(t<=time4,velocity(t),t<=time5,v5(t-time4));
distance(t)=piecewise(t<=time4,distance(t),t<=time5,d5(t-time4));

%Phase 6
j6 = 0;
a6 = -1*j_max*t5;
v6(t) = (j_max*t1^2)/2 - (j_max*t5^2)/2 + (j_max*t3*(2*t1 - t3))/2 + j_max*t1*t2 - j_max*t*t5;
d6(t) = (j_max*t1^3)/6 + t4*((j_max*t1^2)/2 + (j_max*t3*(2*t1 - t3))/2 + j_max*t1*t2) - (j_max*t^2*t5)/2 + j_max*t*(t1*t2 + t1^2/2 - t5^2/2 + (t3*(2*t1 - t3))/2) + (j_max*t3*(3*t1^2 + 3*t1*t3 + 6*t2*t1 - t3^2))/6 + (j_max*t5*(3*t1^2 + 6*t1*t3 + 6*t2*t1 - 3*t3^2 - t5^2))/6 + (j_max*t1*t2*(t1 + t2))/2;

t6 = t2;
time6 = time5+t6;
jerk(t)=piecewise(t<=time5,jerk(t),t<=time6,j6);
acceleration(t)=piecewise(t<=time5,acceleration(t),t<=time6,a6);
velocity(t)=piecewise(t<=time5,velocity(t),t<=time6,v6(t-time5));
distance(t)=piecewise(t<=time5,distance(t),t<=time6,d6(t-time5));

%Phase 7
j7 = j_max;
a7(t) = j_max*t;
v7(t) = (j_max*t^2)/2 + (j_max*t1^2)/2 - (j_max*t5^2)/2 + (j_max*t3*(2*t1 - t3))/2 + j_max*t1*t2 - j_max*t5*t6;
d7(t) = (j_max*(t^3 - 3*t^2*t5 + 3*t*t1^2 + 6*t*t1*t2 + 6*t*t1*t3 - 3*t*t3^2 - 3*t*t5^2 - 6*t*t5*t6 + t1^3 + 3*t1^2*t2 + 3*t1^2*t3 + 3*t1^2*t5 + 3*t1^2*t6 + 3*t4*t1^2 + 3*t1*t2^2 + 6*t1*t2*t3 + 6*t1*t2*t5 + 6*t1*t2*t6 + 6*t4*t1*t2 + 3*t1*t3^2 + 6*t1*t3*t5 + 6*t1*t3*t6 + 6*t4*t1*t3 - t3^3 - 3*t3^2*t5 - 3*t3^2*t6 - 3*t4*t3^2 - t5^3 - 3*t5^2*t6 - 3*t5*t6^2))/6;
t7 = t3;
time7 = time6+t7;
jerk(t)=piecewise(t<=time6,jerk(t),t>=time6,j7);
acceleration(t)=piecewise(t<=time6,acceleration(t),t>=time6,a7(t-time6));
velocity(t)=piecewise(t<=time6,velocity(t),t>=time6,v7(t-time6));
distance(t)=piecewise(t<=time6,distance(t),t>=time6,d7(t-time6));

t1
t2
t3

totalTime = time7;
end

