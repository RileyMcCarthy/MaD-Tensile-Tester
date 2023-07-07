#ifndef ControlSystem_H
#define ControlSystem_H
#include "StateMachine.h"
#include "IOBoard.h"

#define MAX_SAMPLE_PROFILE_NAME 50

#define MAX_TEST_PROFILE_NAME 50
#define MAX_TEST_PROFILE_MP_FILENAME 20
#define MAX_TEST_PROFILE_COMMENT 256

#define MAX_FILE_SIZE 1000

typedef enum homingstate_e
{
    HOMING_NONE,
    HOMING_COMPLETE,
    HOMING_SEEKING,
    HOMING_BACKING_OFF,
    HOMING_SEEKING_SLOW
} HomingState;

bool control_begin();

#endif
