#define MAX_TOKENS 200
#define MAX_MACHINE_PROFILE_NAME 20

#define MAX_PATH_LENGTH 50

#define MAX_CONFIGURATION_MOTOR_TYPE 20
#define MAX_CONFIGURATION_ENCODER_TYPE 20
#define MAX_CONFIGURATION_FORCE_GAUGE 20

#define MAX_MOTION_PROFILE_NAME MAX_PATH_LENGTH
#define MAX_MOTION_PROFILE_SETS 6

#define MAX_SAMPLE_PROFILE_NAME MAX_PATH_LENGTH

#define MAX_TEST_PROFILE_NAME MAX_PATH_LENGTH
#define MAX_TEST_PROFILE_MP_FILENAME 20
#define MAX_TEST_PROFILE_COMMENT 256

#define MAX_MOTION_PROFILE_SET_NAME MAX_PATH_LENGTH
#define MAX_MOTION_PROFILE_SET_TYPE 10

#define MAX_MOTION_QUARTET_NAME MAX_PATH_LENGTH
#define MAX_MOTION_QUARTET_PARAMETERS 10
#define MAX_MOTION_QUARTETS 6

#define MACHINE_CONFIGURATION_FIELD_COUNT 1

#define MAX_FILE_SIZE 1000

typedef struct MachineConfiguration
{
    char motorType[MAX_CONFIGURATION_MOTOR_TYPE];             // Motor used for the machine (model)
    double maxMotorTorque;                                    // Maximum torque of the motor (Nm)
    double maxMotorRPM;                                       // Maximum RPM of the motor (RPM)
    double gearDiameter;                                      // Diameter of the gear (mm)
    double gearPitch;                                         // Pitch of the gear (mm)
    double systemIntertia;                                    // Total inertia of the system (kgm2)
    double staticTorque;                                      // Static torque of the motor (Nm)
    double load;                                              // Estimated load of the system (N)
    char positionEncoderType[MAX_CONFIGURATION_ENCODER_TYPE]; // Encoder used for the position measurement (encoder or distance scale)
    double positionEncoderStepsPerRev;                        // steps per mm of position encoder (step/mm)
    char forceGauge[MAX_CONFIGURATION_FORCE_GAUGE];           // Force gauge used (model)
    double forceGaugeScaleFactor;                             // Force scale factor (mN/step)
    int forceGaugeZeroFactor;                                 // Force offset (steps)
} MachineConfiguration;

#define MACHINE_PERFORMANCE_FIELD_COUNT 7
typedef struct MachinePerformance
{
    double minPosition;             // Minimum position of the motor (mm)
    double maxPosition;             // Maximum position of the motor (mm)
    double maxVelocity;             // Maximum velocity of the motor (mm/s)
    double maxAcceleration;         // Maximum acceleration of the motor (mm/s2)
    double maxForceTensile;         // Maximum force tensile of the motor (N)
    double maxForceCompression;     // Maximum force compression of the motor (N)
    double forceGaugeNeutralOffset; // Force gauge neutral offset (N)
} MachinePerformance;

#define MACHINE_PROFILE_FIELD_COUNT MACHINE_PERFORMANCE_FIELD_COUNT + MACHINE_CONFIGURATION_FIELD_COUNT + 2
typedef struct MachineProfile
{
    char name[MAX_MACHINE_PROFILE_NAME]; // Name of the machine profile
    MachineConfiguration configuration;
    MachinePerformance performance;
} MachineProfile;

typedef enum Function
{
    FUNCTION_LINE,
    FUNCTION_SIGMOID,
} Function;

// Modify this to have distance variable instead of param[0]=distance
typedef struct FunctionInfo
{
    Function function;
    double parameters[MAX_MOTION_QUARTET_PARAMETERS]; // Parameters of the motion quartet (max 10)
} FunctionInfo;

#define MOTION_QUARTET_FIELD_COUNT 4 + 10
typedef struct MotionQuartet
{
    char name[MAX_MOTION_QUARTET_NAME]; // Filename of the motion quartet
    FunctionInfo function;
    double dwell; // Dwell of the motion quartet (us)
} MotionQuartet;

#define MOTION_SET_FIELD_COUNT 5 + 10
typedef struct MotionSet
{
    char name[MAX_MOTION_PROFILE_SET_NAME];      // Motion set Filename, user defined
    char type[MAX_MOTION_PROFILE_SET_TYPE];      // descriptive purpose of the set (ie. precondition, test, etc)
    int executions;                              // Number of times to execute the motion set
    int quartetCount;                            // Number of motion quartets in the set
    MotionQuartet quartets[MAX_MOTION_QUARTETS]; // List of quartets to execute (max 10)
} MotionSet;

#define MOTION_PROFILE_FIELD_COUNT 4 + 20
typedef struct MotionProfile
{
    char name[MAX_MOTION_PROFILE_NAME];      // Filename of the motion profile
    int setCount;                            // Number of motion sets in the profile
    MotionSet sets[MAX_MOTION_PROFILE_SETS]; // Array of motion sets
} MotionProfile;

#define SAMPLE_PROFILE_FIELD_COUNT 9
typedef struct SampleProfile
{
    char name[MAX_SAMPLE_PROFILE_NAME]; // Name of the sample profile
    double length;                      // Length of the sample profile (mm)
    double stretchMax;                  // Maximum stretch of the sample profile (% of length)
    double maxVelocity;                 // Maximum velocity of the sample profile (mm/s)
    double maxAcceleration;             // Maximum acceleration of the sample profile (mm/s2)
    double maxJerk;                     // Maximum jerk of the sample profile (mm/s3)
    double maxForceTensile;             // Maximum force tensile of the sample profile (N)
    double maxForceCompression;         // Maximum force compression of the sample profile (N)

} SampleProfile;

#define TEST_PROFILE_FIELD_COUNT 6
typedef struct TestProfile
{
    char name[MAX_TEST_PROFILE_NAME];       // Descriptive test name
    int sampleSN;                           // Serial number of the sample
    MachineProfile machineProfile;          // Name of the machine profile
    SampleProfile sampleProfile;            // Name of the sample profile
    MotionProfile motionProfile;            // Name of the motion profile
    char comment[MAX_TEST_PROFILE_COMMENT]; // Comment for the test
} TestProfile;
