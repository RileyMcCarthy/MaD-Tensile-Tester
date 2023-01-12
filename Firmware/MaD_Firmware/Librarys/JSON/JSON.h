#define MAX_MACHINE_PROFILE_NAME 20

#define MAX_PATH_LENGTH 50

#define MAX_CONFIGURATION_MOTOR_TYPE 20
#define MAX_CONFIGURATION_ENCODER_TYPE 20
#define MAX_CONFIGURATION_FORCE_GAUGE 20

#define MAX_SAMPLE_PROFILE_NAME MAX_PATH_LENGTH

#define MAX_TEST_PROFILE_NAME MAX_PATH_LENGTH
#define MAX_TEST_PROFILE_MP_FILENAME 20
#define MAX_TEST_PROFILE_COMMENT 256

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

typedef struct MachineProfile
{
    char name[MAX_MACHINE_PROFILE_NAME]; // Name of the machine profile
    MachineConfiguration configuration;
    MachinePerformance performance;
} MachineProfile;

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

