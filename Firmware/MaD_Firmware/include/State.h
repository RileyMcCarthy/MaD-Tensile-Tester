#ifndef State_H
#define State_H
#include "ForceGauge.h"
#include "DS3231.h"
#include "MCP23017.h"
#include "propeller.h"

enum Motion_Status
{
    STATUS_DISABLED,
    STATUS_ENABLED,
    STATUS_RESTRICTED
};
enum Motion_Condition
{
    MOTION_STOPPED,
    MOTION_MOVING,
    MOTION_TENSION,
    MOTION_COMPRESSION,
    MOTION_UPPER,
    MOTION_LOWER,
    MOTION_DOOR
};
enum Motion_Mode
{
    MODE_MANUAL,
    MODE_AUTOMATIC,
    MODE_OVERRIDE
};
class State
{
public:
    virtual void enter() = 0;
    virtual State *nextState() = 0;
    virtual void exit() = 0;
    State *previousState;
    State *successState;
    bool activeState;
};

class SelfCheck : public State
{
public:
    bool chargePumpOK; //internal
    SelfCheck()
    {
        //default flags
        chargePumpOK = true;
        activeState = false;
    }

    void enter()
    {
        //default flags when entering state
        activeState = true;
    }

    State *nextState()
    {
        if (chargePumpOK)
        {
            return successState;
        }
        else
        {
            return this;
        }
    }

    void exit()
    {
        //default flags when exiting state
        activeState = false;
    }
};

class MachineCheck : public State
{
public:
    bool power;
    bool upperLimit;
    bool lowerLimit;
    bool esd;
    bool servoReady;
    bool forceGaugeResponding;
    bool machineReady;

    MachineCheck()
    {
        power = false;                //internal
        upperLimit = false;           //internal
        lowerLimit = false;           //internal
        esd = false;                  //internal
        servoReady = false;           //internal
        forceGaugeResponding = false; //external
        machineReady = false;         //internal

        activeState = false;
    }

    void enter()
    {
        //default values when entering state
        power = false;
        upperLimit = false;
        lowerLimit = false;
        esd = false;
        servoReady = false;
        forceGaugeResponding = false;
        machineReady = false;

        activeState = true;
    }
    State *nextState()
    {
        machineReady = power && upperLimit && lowerLimit && esd && servoReady && forceGaugeResponding;
        if (previousState->nextState() != this)
        {
            //previous state has error, revert back
            return previousState;
        }
        else if (machineReady)
        {
            //no errrors move onto next state
            return successState;
        }
        else
        {
            return this;
        }
    }
    void exit()
    {
        //default values when exiting state
        activeState = false;
    }
};

class Motion : public State
{
public:
    Motion_Status status;       //internal and external
    Motion_Condition condition; //internal and external
    Motion_Mode mode;           //internal and external
    Motion()
    {
        status = Motion_Status::STATUS_DISABLED;
        condition = Motion_Condition::MOTION_STOPPED;
        mode = Motion_Mode::MODE_MANUAL;

        activeState = false;
    }

    void enter()
    {
        //default values when entering state
        status = Motion_Status::STATUS_DISABLED;
        condition = Motion_Condition::MOTION_STOPPED;
        mode = Motion_Mode::MODE_MANUAL;

        activeState = true;
    }

    State *nextState()
    {
        if (previousState->nextState() != this)
        {
            return previousState;
        }
        else
        {
            return this;
        }
    }

    void exit()
    {
        //default values when exiting state
        status = Motion_Status::STATUS_DISABLED;
        condition = Motion_Condition::MOTION_STOPPED;
        mode = Motion_Mode::MODE_MANUAL;

        activeState = false;
    }
    void Status(Motion_Status newStatus)
    {
        if (!activeState)
        {
            return;
        }
        if (status != Motion_Status::STATUS_RESTRICTED)
        {
            status = newStatus;
        }
    }
    Motion_Status Status()
    {
        return status;
    }

    //will set condition flag only if valid substate of status
    void Condition(Motion_Condition newCondition)
    {
        if (!activeState)
        {
            return;
        }
        switch (newCondition)
        {
        case Motion_Condition::MOTION_STOPPED:
            //condition cannot be STOPPED with status RESTRICTED
            if (status != Motion_Status::STATUS_RESTRICTED)
            {
                condition = newCondition;
            }
            break;
        case Motion_Condition::MOTION_MOVING:
            //condition can only be MOVING exist when status is ENABLED
            if (status == Motion_Status::STATUS_ENABLED)
            {
                condition = newCondition;
            }
            break;
        default: //all other states are faults
                 //condition can only be FAULT when status is RESTRICTED
            if (status == Motion_Status::STATUS_RESTRICTED)
            {
                condition = newCondition;
            }
            break;
        }
    }
    Motion_Condition Condition()
    {
        return condition;
    }

    void Mode(Motion_Mode newMode)
    {
        if (condition == Motion_Condition::MOTION_STOPPED)
        {
            mode = newMode;
        }
    }
    Motion_Mode Mode()
    {
        return mode;
    }
};

class MachineState
{
public:
    //state varables
    State *currentState;

    //state objects
    SelfCheck selfCheck;
    MachineCheck machineCheck;
    Motion motion;

    //utility
    MCP23017 *gpio;

    MachineState()
    {
        currentState = &selfCheck;
        selfCheck.previousState = NULL;
        selfCheck.successState = &machineCheck;

        machineCheck.previousState = &selfCheck;
        machineCheck.successState = &motion;

        motion.previousState = &machineCheck;
        motion.successState = NULL;
    }
    void update()
    {
        State *newState;
        if ((newState = currentState->nextState()) != currentState)
        {
            currentState->exit();
            currentState = newState;
            currentState->enter();
        }
    }

private:
};
#endif
