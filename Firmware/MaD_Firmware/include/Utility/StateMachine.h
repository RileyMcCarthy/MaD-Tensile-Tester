#ifndef StateMachine_H
#define StateMachine_H
#include <stdbool.h>

typedef enum State
{
    STATE_SELFCHECK,
    STATE_MACHINECHECK,
    STATE_MOTION
} State;

typedef enum MotionStatus
{
    MOTIONSTATUS_DISABLED,
    MOTIONSTATUS_ENABLED,       // Motion is enabled
    MOTIONSTATUS_SAMPLE_LIMIT,  // Motion is enabled but limited by condition (ie. length, force)
    MOTIONSTATUS_MACHINE_LIMIT, // Motion is enabled but limited by condition (ie. tension, compression, travel upper/lower, door)
    MOTIONSTATUS_FAULTED        // Motion is disabled due to fault (ie. ESD switch, ESD travel)
} MotionStatus;

typedef enum MotionOverTravel
{
    MOTION_LIMIT_OK,    // No limit
    MOTION_LIMIT_UPPER, // Upper limit
    MOTION_LIMIT_LOWER  // Lower limit
} MotionOverTravel;

/**
 * @brief Motion conditions ordered from most to least critical.
 *
 */
typedef enum MotionCondition
{
    CONDITION_LENGTH,      // Test sample maximum length exceeded
    CONDITION_FORCE,       // Test sample maximum force exceeded
    CONDITION_TENSION,     // Machine maximum tensile force exceeded
    CONDITION_COMPRESSION, // Machine maximum compression force exceeded
    CONDITION_UPPER,       // Machine upper travel limit exceeded
    CONDITION_LOWER,       // Machine lower travel limit exceeded
    CONDITION_DOOR,        // Door is open
    CONDITION_STOPPED,     // Motor is stationary
    CONDITION_MOVING,      // Motion is moving
} MotionCondition;

typedef enum MotionMode
{
    MODE_MANUAL, // Manual mode
    MODE_TEST,   // Test mode
    MODE_TEST_RUNNING
} MotionMode;

typedef struct SelfCheckParameters
{
    bool chargePump; // Charge pump signal is activated, self check is satisfied. StateMachine, working
} SelfCheckParameters;

typedef struct MachineCheckParameters
{
    bool switchedPower;              // Switched power is activated to IO board. RED/GREEN buttons. ( GPI_1). Control.c. implemented, changed to GPI12
    MotionOverTravel esdTravelLimit; // Over travel limit status of upper/lower ( GPI_2/3 ). Control.c. implemented, needs switched power on
    bool esdSwitch;                  // User ESD switch is activated ( GPI_4 ). Control.c. implemented (big red button)
    bool servoOK;                    // Servo ready signal recieved ( GPI_11 ). Control.c, implemented
    bool forceGaugeCom;              // Force gauge communicating. ControlSystem.c, implemented
    bool servoCom;                   // DYN4 is communicating. Control.c
} MachineCheckParameters;

typedef struct MotionParameters
{
    MotionStatus status;       // internal and external
    MotionCondition condition; // internal and external
    MotionMode mode;           // internal and external
} MotionParameters;

//@TODO use lock for this struct (built into flexprop)
typedef struct MachineState
{
    State state;
    SelfCheckParameters selfCheckParameters;
    MachineCheckParameters machineCheckParameters;
    MotionParameters motionParameters;
} MachineState;

typedef enum Parameter
{
    PARAM_SELF_CHARGE_PUMP,
    PARAM_MACHINE_SWITCHED_POWER,
    PARAM_MACHINE_ESD_TRAVEL_LIMIT,
    PARAM_MACHINE_ESD_SWITCH,
    PARAM_MACHINE_SERVO_OK,
    PARAM_MACHINE_FORCE_GAUGE_COM,
    PARAM_MACHINE_SERVO_COM,
    PARAM_MACHINE_RTC_COM,
    PARAM_MOTION_STATUS,
    PARAM_MOTION_CONDITION,
    PARAM_MOTION_MODE,
    PARAM_FUNCTION
} Parameter;

// External State Setters
void machine_state_init();
void state_machine_set(Parameter param, int state);
bool state_machine_self_check_equal(SelfCheckParameters *selfCheckParameters1, SelfCheckParameters *selfCheckParameters2);
bool state_machine_check_equal(MachineCheckParameters *motionParameters1, MachineCheckParameters *motionParameters2);
bool state_machine_motion_equal(MotionParameters *motionParameters1, MotionParameters *motionParameters2);
bool state_machine_equal(MachineState *machineState1, MachineState *machineState2);
#endif
