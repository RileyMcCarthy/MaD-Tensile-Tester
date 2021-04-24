#ifndef JSON_H
#define JSON_H

#include "simpletools.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct MachineConfiguration_s
    {
        char *motorType;
        int maxMotorTorque;
        int maxMotorRPM;
        int gearDiameter;
        int gearPitch;
        int systemIntertia;
        int staticTorque;
        int load;
        char *positionEncoderType;
        int positionEncoderScaleFactor;
        char *forceGauge;
    } MachineConfiguration;

    typedef struct MachinePerformance_s
    {
        int minPosition;
        int maxPosition;
        int maxVelocity;
        int maxAcceleration;
        int maxForceTensile;
        int maxForceCompression;
        int forceGaugeNeutralOffset;
    } MachinePerformance;

    typedef struct MachineSettings_s
    {
        char *name;
        int number;
        MachineConfiguration *configuration;
        MachinePerformance *performance;

    } MachineSettings;

    MachineSettings *getMachineSettings();
    char *machineSettingsToJSON(MachineSettings *settings);
    void freeMachineSettings(MachineSettings *settings);

    void JSONtoMachineSettings(MachineSettings *settings, char *json);

#ifdef __cplusplus
}
#endif
#endif