#ifndef JSON_H
#define JSON_H

#include "simpletools.h"
#include <stddef.h>
#include <stdbool.h>
#include "string.h"

typedef struct MachineConfiguration_s
{
    char *motorType;                // Motor used for the machine (model)
    float maxMotorTorque;           // Maximum torque of the motor (Nm)
    float maxMotorRPM;              // Maximum RPM of the motor (RPM)
    float gearDiameter;             // Diameter of the gear (mm)
    float gearPitch;                // Pitch of the gear (mm)
    float systemIntertia;           // Total inertia of the system (kgm2)
    float staticTorque;             // Static torque of the motor (Nm)
    float load;                     //Estimated load of the system (N)
    char *positionEncoderType;      // Encoder used for the position measurement (encoder or distance scale)
    int positionEncoderScaleFactor; //steps per mm of position encoder (step/mm)
    char *forceGauge;               //Force gauge used (model)
} MachineConfiguration;

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

typedef struct MachineProfile_s
{
    char *name; //Name of the machine profile
    int number; //Number of the machine profile
    MachineConfiguration *configuration;
    MachinePerformance *performance;

} MachineProfile;

MachineProfile *get_machine_profile();
char *machine_profile_to_json(MachineProfile *settings);
void free_machine_settings(MachineProfile *settings);

void json_to_machine_profile(MachineProfile *settings, char *json);

#endif
