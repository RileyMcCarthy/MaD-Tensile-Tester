#ifndef State_H
#define State_H
#include "propeller.h"
#include "stdbool.h"

//FIX STATE MACHINE USING https://stackoverflow.com/questions/1371460/state-machines-tutorials#:~:text=State%20machines%20are%20very%20simple,and%20then%20just%20execute%20it.
#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum States_e
    {
        State_SelfCheck,
        State_MachineCheck,
        State_Motion
    } State;

    typedef enum MotionStatus_e
    {
        STATUS_DISABLED,
        STATUS_ENABLED,
        STATUS_RESTRICTED
    } MotionStatus;
    typedef enum MotionCondition_e
    {
        MOTION_STOPPED,
        MOTION_MOVING,
        MOTION_TENSION,
        MOTION_COMPRESSION,
        MOTION_UPPER,
        MOTION_LOWER,
        MOTION_DOOR
    } MotionCondition;
    typedef enum MotionMode_e
    {
        MODE_MANUAL,
        MODE_AUTOMATIC,
        MODE_OVERRIDE
    } MotionMode;

    typedef struct SelfCheck_t
    {
        bool chargePumpOK;
    } SelfCheckParameters;

    typedef struct MachineCheck_t
    {
        bool power;
        bool upperLimit;
        bool lowerLimit;
        bool esd;
        bool servoReady;
        bool forceGaugeResponding;
        bool machineReady;
    } MachineCheckParameters;

    typedef struct Motion_t
    {
        MotionStatus status;       //internal and external
        MotionCondition condition; //internal and external
        MotionMode mode;           //internal and external

    } MotionParameters;

    static struct MachineState_t
    {
        State currentState;
        SelfCheckParameters selfCheckParameters;
        MachineCheckParameters machineCheckParameters;
        MotionParameters motionParameters;

    } MachineState;

    /**
    * @brief Initial state on power up. Automatic transition into if any Self Check condition not satisfied. No motion shall occur in Self Check state. 
    * 
    */
    static State SelfCheckState()
    {
        State newState;
        //Entering Self Check State
        bool tempChargePumpOK = false;

        //Checking Self Check Conditions
        //@TODO Make this more in depth
        if (1 == 1)
        {
            tempChargePumpOK = true;
        }

        //Decide the next state
        if (MachineState.selfCheckParameters.chargePumpOK)
        {
            newState = State_MachineCheck;
        }
        else
        {
            newState = State_SelfCheck;
        }

        //Dump temporary parameters to machine state
        MachineState.selfCheckParameters.chargePumpOK = tempChargePumpOK;

        return newState;
    }

    /**
 * @brief Automatic transition into when all Self Check conditions satisfied or if any Machine Check condition not satisfied and Self Check conditions are satisfied. 
 * No motion shall occur in Machine Check.  
 * Motion Status is Disabled in Machine Check State. 
 * 
 */
    static State MachineCheckState()
    {
        State newState;
        //Entering Machine Check State, Set default state parameters
        bool tempPower = false;
        bool tempUpperLimit = false;
        bool tempLowerLimit = false;
        bool tempESD = false;
        bool tempServoReady = false;
        bool tempForceGaugeResponding = false;
        bool tempMachineReady = false;

        //Checking Machine Check Conditions
        //For example is the force gauge responding, are limits hit... need to figure out how to give State these objects
        tempMachineReady = true; //This flag says all conditions meet, placeholder for actual checks

        //Decide the next state
        if (MachineState.machineCheckParameters.machineReady)
        {
            newState = State_Motion;
        }
        else
        {
            newState = State_MachineCheck;
        }

        //Dump temporary parameters to machine state
        MachineState.machineCheckParameters.power = tempPower;
        MachineState.machineCheckParameters.upperLimit = tempUpperLimit;
        MachineState.machineCheckParameters.lowerLimit = tempLowerLimit;
        MachineState.machineCheckParameters.esd = tempESD;
        MachineState.machineCheckParameters.servoReady = tempServoReady;
        MachineState.machineCheckParameters.forceGaugeResponding = tempForceGaugeResponding;
        MachineState.machineCheckParameters.machineReady = tempMachineReady;

        return newState;
    }

    static State MotionState()
    {
        State newState;
        //Entering Motion State
        MotionStatus tempStatus = STATUS_DISABLED;
        MotionCondition tempCondition = MOTION_STOPPED;
        MotionMode tempMode = MODE_MANUAL;

        //Check Machine Conditions

        //Decide the next state
        if (MachineCheckState() != State_Motion) //Check if previous state conditions are still valid back
        {
            newState = State_MachineCheck;

            tempStatus = STATUS_DISABLED;
            tempCondition = MOTION_STOPPED;
            tempMode = MODE_MANUAL;
        }
        else
        {
            newState = State_Motion;
        }
        //Dump temporary parameters to machine state
        MachineState.motionParameters.status = tempStatus;
        MachineState.motionParameters.condition = tempCondition;
        MachineState.motionParameters.mode = tempMode;
        return newState;
    }

    static bool updateMachineState()
    {
        State previousState = MachineState.currentState;
        State newState;
        switch (previousState)
        {
        case State_SelfCheck:
            newState = SelfCheckState();
            break;
        case State_MachineCheck:
            newState = MachineCheckState();
            break;
        case State_Motion:
            newState = MotionState();
            break;
        default:
            break;
        }
        MachineState.currentState = newState;
        return newState != previousState;
    }

#ifdef __cplusplus
}
#endif
#endif
