#include <stdbool.h>
#define MAX_MACHINE_PROFILE_NAME 20

#define MAX_CONFIGURATION_MOTOR_TYPE 20
#define MAX_CONFIGURATION_ENCODER_TYPE 20
#define MAX_CONFIGURATION_FORCE_GAUGE 20

typedef struct MachineConfiguration
{
    char motorType[MAX_CONFIGURATION_MOTOR_TYPE];             // Motor used for the machine (model)
    int maxMotorTorque;                                    // Maximum torque of the motor (milli-Nm)
    int maxMotorRPM;                                       // Maximum RPM of the motor (RPM)
    int gearDiameter;                                      // Diameter of the gear (um)
    int gearPitch;                                         // Pitch of the gear (um)
    int systemIntertia;                                    // Total inertia of the system (mg*mm^22)
    int staticTorque;                                      // Static torque of the motor (milli-Nm)
    int load;                                              // Estimated load of the system (mN)
    char positionEncoderType[MAX_CONFIGURATION_ENCODER_TYPE]; // Encoder used for the position measurement (encoder or distance scale)
    int encoderStepsPermm;                        // steps per um of position encoder (step/mm)
    int servoStepPermm; //Steps per mm of servo motor
    char forceGauge[MAX_CONFIGURATION_FORCE_GAUGE];           // Force gauge used (model)
    int forceGaugeGain;                             // Force scale factor 
    int forceGaugeOffset;                                 // Force offset (steps)
} MachineConfiguration;

typedef struct MachinePerformance
{
    int minPosition;             // Minimum position of the motor (mm)
    int maxPosition;             // Maximum position of the motor (mm)
    int maxVelocity;             // Maximum velocity of the motor (mm/s)
    int maxAcceleration;         // Maximum acceleration of the motor (mm/s2)
    int maxForceTensile;         // Maximum force tensile of the motor (mN)
    int maxForceCompression;     // Maximum force compression of the motor (mN)
    int forceGaugeNeutralOffset; // Force gauge neutral offset (mN)å
} MachinePerformance;

typedef struct MachineProfile
{
    char name[MAX_MACHINE_PROFILE_NAME]; // Name of the machine profile
    MachineConfiguration configuration;
    MachinePerformance performance;
} MachineProfile;

#define MAX_SAMPLE_PROFILE_NAME 50
typedef struct SampleProfile
{
    char name[MAX_SAMPLE_PROFILE_NAME]; // Name of the sample profile
    int length;                      // Length of the sample profile (mm)
    int stretchMax;                  // Maximum stretch of the sample profile (% of length)
    int maxVelocity;                 // Maximum velocity of the sample profile (mm/s)
    int maxAcceleration;             // Maximum acceleration of the sample profile (mm/s2)
    int maxJerk;                     // Maximum jerk of the sample profile (mm/s3)
    int maxForceTensile;             // Maximum force tensile of the sample profile (N)
    int maxForceCompression;         // Maximum force compression of the sample profile (N)

} SampleProfile;

int init_machine_profile();
bool lock_machine_profile(MachineProfile ** profile);
bool lock_machine_profile_ms(MachineProfile ** profile, int ms);
bool unlock_machine_profile();
