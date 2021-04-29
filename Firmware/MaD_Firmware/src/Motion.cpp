#include "Motion.h"
#include <propeller.h>
#include "libpropeller/sd/sd.h"
extern Motion_Cog motionCog;

void runMotion(void *par)
{
    // ForceGauge forceGauge;
    //ForceGauge_begin(&forceGauge, 15, 16);
    ForceGauge forceGauge;
    ForceGauge_begin(&forceGauge, 15, 16);
    DYN4 dyn4;
    dyn4_begin(&dyn4, 14, 12, 0x03);
    //simpleterm_open();
    dyn4_send_command(&dyn4, 0x0c, 1000);
    // info.time = 0;
    //int lastCNT = CNT;
    //char temp[] = "TIME(sec),POSITION(STEPS),FORCE(mN)\n";
    //int count = 0;
    int startCNT = CNT;
    for (;;)
    {
        motionCog.time = (CNT - startCNT) / CLKFREQ;
        motionCog.position = dyn4_getPosition(&dyn4);
        motionCog.force = forceGauge.force;
        //char data[50];
        // sprintf(data, "%d,%d,%d\n", motionCog.time, motionCog.position, motionCog.force);
        // count++;
        waitcnt(CNT + CLKFREQ / 1000);
    }
    // sd.Close();
}