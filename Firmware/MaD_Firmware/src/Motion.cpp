#include "Motion.h"
#include <propeller.h>
extern Motion_Cog motionCog;

/**
 * @brief The motion cog function that handles reading the force gauge and controlling the servo withen state machine restriction
 * 
 * @param par ignore this parameter, used for cog_run()
 */
void runMotion(void *par)
{
    //Start state machine
    if (!RunStateMachine())
    {
        //state machine failed to run
    }

    MachineState.selfCheckParameters.chargePumpOK = true; //motion cog started up successfully, enable charge pump

    ForceGauge forceGauge;
    if (ForceGauge_begin(&forceGauge, 15, 16) == SUCCESS)
    {
        MachineState.machineCheckParameters.forceGaugeResponding = true;
    }

    DYN4 dyn4;
    if (dyn4_begin(&dyn4, 14, 12, 0x03) == SUCCESS)
    {
        MachineState.machineCheckParameters.dyn4Responding = true;
    }
    for (;;)
    {
        switch (MachineState.currentState)
        {
        case State_SelfCheck:
            /* motion code for self check */
            //do nothing, motion isnt enabled
            break;
        case State_MachineCheck:
            /* Machine Check motion code */
            break;
        case State_Motion:
            /* Motion state motion code */
            switch (MachineState.motionParameters.mode)
            {
            case MODE_MANUAL:
                /* code */
                break;
            case MODE_AUTOMATIC:
                /* code */
                break;
            case MODE_OVERRIDE:
                /* code */
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
    dyn4_send_command(&dyn4, 0x0c, 1000);
    int startCNT = CNT;
    for (;;)
    {
        motionCog.time = (CNT - startCNT) / CLKFREQ;
        motionCog.position = dyn4.encoder.POS[0];
        motionCog.force = forceGauge.force;
        //char data[50];
        // sprintf(data, "%d,%d,%d\n", motionCog.time, motionCog.position, motionCog.force);
        // count++;
        waitcnt(CNT + CLKFREQ / 1000);
    }
    // sd.Close();
}