#ifndef JSON_H
#define JSON_H

#include "simpletools.h"
#include <stddef.h>
#include <stdbool.h>
#include "string.h"
#include "Error.h"
#include <stdio.h>
#include <stdlib.h>
#include "tiny-json.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_TOKENS 200
#define MAX_MACHINE_PROFILE_NAME 20

#define MAX_PATH_LENGTH 50

#define MAX_CONFIGURATION_MOTOR_TYPE 20
#define MAX_CONFIGURATION_ENCODER_TYPE 20
#define MAX_CONFIGURATION_FORCE_GAUGE 20

#define MAX_MOTION_PROFILE_NAME MAX_PATH_LENGTH
#define MAX_MOTION_PROFILE_SETS 15

#define MAX_SAMPLE_PROFILE_NAME MAX_PATH_LENGTH

#define MAX_TEST_PROFILE_NAME MAX_PATH_LENGTH
#define MAX_TEST_PROFILE_MP_FILENAME 20
#define MAX_TEST_PROFILE_COMMENT 256

#define MAX_MOTION_PROFILE_SET_NAME MAX_PATH_LENGTH
#define MAX_MOTION_PROFILE_SET_TYPE 10

#define MAX_MOTION_QUARTET_NAME MAX_PATH_LENGTH
#define MAX_MOTION_QUARTET_PARAMETERS 10
#define MAX_MOTION_QUARTETS 10

#define MACHINE_CONFIGURATION_FIELD_COUNT 1

#define MAX_FILE_SIZE 2000

typedef struct MachineConfiguration_s
{
    char motorType[MAX_CONFIGURATION_MOTOR_TYPE];             // Motor used for the machine (model)
    double maxMotorTorque;                                     // Maximum torque of the motor (Nm)
    double maxMotorRPM;                                        // Maximum RPM of the motor (RPM)
    double gearDiameter;                                       // Diameter of the gear (mm)
    double gearPitch;                                          // Pitch of the gear (mm)
    double systemIntertia;                                     // Total inertia of the system (kgm2)
    double staticTorque;                                       // Static torque of the motor (Nm)
    double load;                                               // Estimated load of the system (N)
    char positionEncoderType[MAX_CONFIGURATION_ENCODER_TYPE]; // Encoder used for the position measurement (encoder or distance scale)
    double positionEncoderStepsPerRev;                         // steps per mm of position encoder (step/mm)
    char forceGauge[MAX_CONFIGURATION_FORCE_GAUGE];           // Force gauge used (model)
    double forceGaugeScaleFactor;                              // Force scale factor (mN/step)
    int forceGaugeZeroFactor;                                 // Force offset (steps)
} MachineConfiguration;

#define MACHINE_PERFORMANCE_FIELD_COUNT 7
typedef struct MachinePerformance_s
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
typedef struct MachineProfile_s
{
    char name[MAX_MACHINE_PROFILE_NAME]; // Name of the machine profile
    int number;                          // Number of the machine profile
    MachineConfiguration configuration;
    MachinePerformance performance;
} MachineProfile;

// make function enum to string method

#define MOTION_QUARTET_FIELD_COUNT 4 + 10
typedef struct MotionQuartet_s
{
    char name[MAX_MOTION_QUARTET_NAME];              // Filename of the motion quartet
    int function;                                    // Motion function type
    double parameters[MAX_MOTION_QUARTET_PARAMETERS]; // Parameters of the motion quartet (max 10)
    double dwell;                                     // Dwell of the motion quartet (us)
} MotionQuartet;

#define MOTION_SET_FIELD_COUNT 5 + 10
typedef struct MotionSet_s
{
    char name[MAX_MOTION_PROFILE_SET_NAME];      // Motion set Filename, user defined
    int number;                                  // Motion set number, autogenerated as incrment in file system
    char type[MAX_MOTION_PROFILE_SET_TYPE];      // descriptive purpose of the set (ie. precondition, test, etc)
    int executions;                              // Number of times to execute the motion set
    int quartetCount;                            // Number of motion quartets in the set
    MotionQuartet quartets[MAX_MOTION_QUARTETS]; // List of quartets to execute (max 10)
} MotionSet;

#define MOTION_PROFILE_FIELD_COUNT 4 + 20
typedef struct MotionProfile_s
{
    char name[MAX_MOTION_PROFILE_NAME];      // Filename of the motion profile
    int number;                              // Number of the motion profile (autogenerated)
    int setCount;                            // Number of motion sets in the profile
    MotionSet sets[MAX_MOTION_PROFILE_SETS]; // Array of motion sets
} MotionProfile;

#define SAMPLE_PROFILE_FIELD_COUNT 9
typedef struct SampleProfile_s
{
    char name[MAX_SAMPLE_PROFILE_NAME];                // Name of the sample profile
    int number;                // Number of the sample profile
    double length;              // Length of the sample profile (mm)
    double stretchMax;          // Maximum stretch of the sample profile (% of length)
    double maxVelocity;         // Maximum velocity of the sample profile (mm/s)
    double maxAcceleration;     // Maximum acceleration of the sample profile (mm/s2)
    double maxJerk;             // Maximum jerk of the sample profile (mm/s3)
    double maxForceTensile;     // Maximum force tensile of the sample profile (N)
    double maxForceCompression; // Maximum force compression of the sample profile (N)

} SampleProfile;

#define TEST_PROFILE_FIELD_COUNT 6
typedef struct TestProfile_s
{
    char name[MAX_TEST_PROFILE_NAME];                     // Descriptive test name
    int sampleSN;                   // Serial number of the sample
    MachineProfile machineProfile; // Name of the machine profile
    SampleProfile sampleProfile;   // Name of the sample profile
    MotionProfile motionProfile;   // Name of the motion profile
    char comment[MAX_TEST_PROFILE_COMMENT];                  // Comment for the test
} TestProfile;

// Helper Functions

void machine_configuration_init(MachineConfiguration *configuration);
void machine_performance_init(MachinePerformance *performance);
void machine_profile_init(MachineProfile *profile);
void motion_quartet_init(MotionQuartet *quartet);
void motion_set_init(MotionSet *set);
void sample_profile_init(SampleProfile *sample);
void motion_profile_init(MotionProfile *profile);
void test_profile_init(TestProfile *profile);

Error machine_profile_to_json(MachineProfile *settings, const char *filename); // U
Error sample_profile_to_json(SampleProfile *sample, const char *filename);     // U
Error test_profile_to_json(TestProfile *test, const char *filename);           // U
Error motion_profile_to_json(MotionProfile *motion, const char *filename);     // U
Error motion_set_to_json(MotionSet *set, char *filename);                      // U
Error motion_quartet_to_json(MotionQuartet *quartet, const char *filename);    // U

void json_to_machine_profile(MachineProfile *profile, const char *filename);
void json_to_sample_profile(SampleProfile *sample, const char *filename);
void json_to_test_profile(const char *filename, TestProfile *test);        // U
void json_to_motion_profile(const char *filename, MotionProfile *profile); // U
void json_to_motion_set(const char *filename, MotionSet *set);             // U
void json_to_motion_quartet(const char *filename, MotionQuartet *quartet); // U

void json_print_machine_profile(MachineProfile *profile);
void json_print_motion_profile(MotionProfile *profile);

void free_machine_profile(MachineProfile *settings);
void free_machine_performance(MachinePerformance *performance);
void free_machine_configuration(MachineConfiguration *configuration);
void free_sample_profile(SampleProfile *sample);
void free_test_profile(TestProfile *test);
void free_motion_profile(MotionProfile *profile);
void free_motion_set(MotionSet *set);
void free_motion_quartet(MotionQuartet *quartet);
#endif
