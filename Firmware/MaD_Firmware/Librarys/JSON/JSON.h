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
#define MACHINE_CONFIGURATION_FIELD_COUNT 13
typedef struct MachineConfiguration_s
{
    char *motorType;                  // Motor used for the machine (model)
    float maxMotorTorque;             // Maximum torque of the motor (Nm)
    float maxMotorRPM;                // Maximum RPM of the motor (RPM)
    float gearDiameter;               // Diameter of the gear (mm)
    float gearPitch;                  // Pitch of the gear (mm)
    float systemIntertia;             // Total inertia of the system (kgm2)
    float staticTorque;               // Static torque of the motor (Nm)
    float load;                       // Estimated load of the system (N)
    char *positionEncoderType;        // Encoder used for the position measurement (encoder or distance scale)
    float positionEncoderStepsPerRev; // steps per mm of position encoder (step/mm)
    char *forceGauge;                 // Force gauge used (model)
    float forceGaugeScaleFactor;      // Force scale factor (mN/step)
    int forceGaugeZeroFactor;         // Force offset (steps)
} MachineConfiguration;

#define MACHINE_PERFORMANCE_FIELD_COUNT 7
typedef struct MachinePerformance_s
{
    float minPosition;             // Minimum position of the motor (mm)
    float maxPosition;             // Maximum position of the motor (mm)
    float maxVelocity;             // Maximum velocity of the motor (mm/s)
    float maxAcceleration;         // Maximum acceleration of the motor (mm/s2)
    float maxForceTensile;         // Maximum force tensile of the motor (N)
    float maxForceCompression;     // Maximum force compression of the motor (N)
    float forceGaugeNeutralOffset; // Force gauge neutral offset (N)
} MachinePerformance;

#define MACHINE_PROFILE_FIELD_COUNT MACHINE_PERFORMANCE_FIELD_COUNT + MACHINE_CONFIGURATION_FIELD_COUNT + 2
typedef struct MachineProfile_s
{
    char *name; // Name of the machine profile
    int number; // Number of the machine profile
    MachineConfiguration *configuration;
    MachinePerformance *performance;

} MachineProfile;

// make function enum to string method

#define MOTION_QUARTET_FIELD_COUNT 4 + 10
typedef struct MotionQuartet_s
{
    char *name;        // Filename of the motion quartet
    int function;      // Motion function type
    float *parameters; // Parameters of the motion quartet (max 10)
    float distanceMax; // Maximum distance of the motion quartet (mm)
    float dwell;       // Dwell of the motion quartet (us)
} MotionQuartet;

#define MOTION_SET_FIELD_COUNT 5 + 10
typedef struct MotionSet_s
{
    char *name;              // Motion set Filename, user defined
    int number;              // Motion set number, autogenerated as incrment in file system
    char *type;              // descriptive purpose of the set (ie. precondition, test, etc)
    int executions;          // Number of times to execute the motion set
    int quartetCount;        // Number of motion quartets in the set
    MotionQuartet *quartets; // List of quartets to execute (max 10)
} MotionSet;

#define MOTION_PROFILE_FIELD_COUNT 4 + 20
typedef struct MotionProfile_s
{
    char *name;      // Filename of the motion profile
    int number;      // Number of the motion profile (autogenerated)
    int setCount;    // Number of motion sets in the profile
    MotionSet *sets; // Array of motion sets
} MotionProfile;

#define SAMPLE_PROFILE_FIELD_COUNT 9
typedef struct SampleProfile_s
{
    char *name;                // Name of the sample profile
    int number;                // Number of the sample profile
    float length;              // Length of the sample profile (mm)
    float stretchMax;          // Maximum stretch of the sample profile (% of length)
    float maxVelocity;         // Maximum velocity of the sample profile (mm/s)
    float maxAcceleration;     // Maximum acceleration of the sample profile (mm/s2)
    float maxJerk;             // Maximum jerk of the sample profile (mm/s3)
    float maxForceTensile;     // Maximum force tensile of the sample profile (N)
    float maxForceCompression; // Maximum force compression of the sample profile (N)

} SampleProfile;

#define TEST_PROFILE_FIELD_COUNT 6
typedef struct TestProfile_s
{
    char *name;                     // Descriptive test name
    int sampleSN;                   // Serial number of the sample
    MachineProfile *machineProfile; // Name of the machine profile
    SampleProfile *sampleProfile;   // Name of the sample profile
    MotionProfile *motionProfile;   // Name of the motion profile
    char *comment;                  // Comment for the test
} TestProfile;

// Helper Functions

MachineProfile *get_machine_profile(); // U
MotionProfile *get_motion_profile();   // U
SampleProfile *get_sample_profile();   // U
TestProfile *get_test_profile();       // U
MotionSet *get_motion_set();           // U
MotionQuartet *get_motion_quartet();   // U

Error machine_profile_to_json(MachineProfile *settings, char *filename); // U
Error sample_profile_to_json(SampleProfile *sample, char *filename);     // U
Error test_profile_to_json(TestProfile *test, char *filename);           // U
Error motion_profile_to_json(MotionProfile *motion, char *filename);     // U
Error motion_set_to_json(MotionSet *set, char *filename);                // U
Error motion_quartet_to_json(MotionQuartet *quartet, char *filename);    // U

MachineProfile *json_to_machine_profile(char *filename);
SampleProfile *json_to_sample_profile(char *filename);               // U
void json_to_test_profile(char *filename, TestProfile *test);        // U
void json_to_motion_profile(char *filename, MotionProfile *profile); // U
void json_to_motion_set(char *filename, MotionSet *set);             // U
void json_to_motion_quartet(char *filename, MotionQuartet *quartet); // U

void json_print_machine_profile(MachineProfile *profile);

void free_machine_profile(MachineProfile *settings);
void free_machine_performance(MachinePerformance *performance);
void free_machine_configuration(MachineConfiguration *configuration);
void free_sample_profile(SampleProfile *sample);
void free_test_profile(TestProfile *test);
void free_motion_profile(MotionProfile *profile);
void free_motion_set(MotionSet *set);
void free_motion_quartet(MotionQuartet *quartet);
#endif
