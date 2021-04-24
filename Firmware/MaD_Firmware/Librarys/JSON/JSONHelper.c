#include "JSON.h"
#include "JSONHelper.h"

MachineConfiguration *getMachineConfiguration()
{
    MachineConfiguration *configuration = (MachineConfiguration *)malloc(sizeof(MachineConfiguration));
    configuration->motorType = NULL;
    configuration->maxMotorTorque = 0;
    configuration->maxMotorRPM = 0;
    configuration->gearDiameter = 0;
    configuration->gearPitch = 0;
    configuration->systemIntertia = 0;
    configuration->staticTorque = 0;
    configuration->load = 0;
    configuration->positionEncoderType = NULL;
    configuration->positionEncoderScaleFactor = 0;
    configuration->forceGauge = NULL;
    return configuration;
}

MachinePerformance *getMachinePerformance()
{
    MachinePerformance *performance = (MachinePerformance *)malloc(sizeof(MachinePerformance));
    performance->minPosition = 0;
    performance->maxPosition = 0;
    performance->maxVelocity = 0;
    performance->maxAcceleration = 0;
    performance->maxForceTensile = 0;
    performance->maxForceCompression = 0;
    performance->forceGaugeNeutralOffset = 0;
    return performance;
}

char *machineConfigurationToJSON(MachineConfiguration *configuration)
{
    char *json;
    int size = 20; //"Configuration":{}\0, 19 chars plus interior
    char *motorTypeJSON = stringToJSON("Motor Type", configuration->motorType);
    size += strlen(motorTypeJSON) + 2;
    char *maxMotorRPMJSON = intToJSON("Max Motor RPM", configuration->maxMotorRPM);
    size += strlen(maxMotorRPMJSON) + 2;
    char *maxMotorTorqueJSON = intToJSON("Max Motor Torque", configuration->maxMotorTorque);
    size += strlen(maxMotorTorqueJSON) + 2;
    char *gearDiameterJSON = intToJSON("Gear Diameter", configuration->gearDiameter);
    size += strlen(gearDiameterJSON) + 2;
    char *gearPitchJSON = intToJSON("Gear Pitch", configuration->gearPitch);
    size += strlen(gearPitchJSON) + 2;
    char *systemInertiaJSON = intToJSON("System Intertia", configuration->systemIntertia);
    size += strlen(systemInertiaJSON) + 2;
    char *staticTorqueJSON = intToJSON("Static Torque", configuration->staticTorque);
    size += strlen(staticTorqueJSON) + 2;
    char *loadJSON = intToJSON("Load", configuration->load);
    size += strlen(loadJSON) + 2;
    char *encoderTypeJSON = stringToJSON("Position Encoder Type", configuration->positionEncoderType);
    size += strlen(encoderTypeJSON) + 2;
    char *encoderScaleFactorJSON = intToJSON("Position Encoder Scale Factor", configuration->positionEncoderScaleFactor);
    size += strlen(encoderScaleFactorJSON) + 2;
    char *forceGaugeJSON = stringToJSON("Force Gauge", configuration->forceGauge);
    size += strlen(forceGaugeJSON) + 2;
    json = (char *)malloc(sizeof(char) * size);
    sprintf(json, "\"Configuration\":{%s,%s,%s,%s,%s,%s,%s,%s,%s,%s}", motorTypeJSON, maxMotorRPMJSON, maxMotorTorqueJSON,
            gearDiameterJSON, gearPitchJSON, systemInertiaJSON, staticTorqueJSON, loadJSON, encoderTypeJSON,
            encoderScaleFactorJSON, forceGaugeJSON);
    printf("Configuration:%s\n", json);
    free(motorTypeJSON);
    free(maxMotorRPMJSON);
    free(maxMotorTorqueJSON);
    free(gearDiameterJSON);
    free(gearPitchJSON);
    free(systemInertiaJSON);
    free(staticTorqueJSON);
    free(loadJSON);
    free(encoderTypeJSON);
    free(encoderScaleFactorJSON);
    free(forceGaugeJSON);
    return json;
}
char *machinePerformanceToJSON(MachinePerformance *performance)
{
    char *json;
    int size = 17; //"Performance":{}\0, 17 chars plus interior
    char *minPositionStr = intToJSON("Position Minimum", performance->minPosition);
    size += strlen(minPositionStr) + 2;
    char *maxPositionStr = intToJSON("Position Maximum", performance->maxPosition);
    size += strlen(maxPositionStr) + 2;
    char *maxVelocityStr = intToJSON("Velocity Maximum", performance->maxVelocity);
    size += strlen(maxVelocityStr) + 2;
    char *maxAccelerationStr = intToJSON("Acceleration Maximum", performance->maxAcceleration);
    size += strlen(maxAccelerationStr) + 2;
    char *maxForceTensileStr = intToJSON("Force Tensile Maximum", performance->maxForceTensile);
    size += strlen(maxForceTensileStr) + 2;
    char *maxForceCompressionStr = intToJSON("Force Compression Maximum", performance->maxForceCompression);
    size += strlen(maxForceCompressionStr) + 2;
    char *forceGaugeNeutralOffsetStr = intToJSON("Force gauge Neutral Offset", performance->forceGaugeNeutralOffset);
    size += strlen(forceGaugeNeutralOffsetStr) + 2;
    json = (char *)malloc(sizeof(char) * size);
    sprintf(json, "\"Performance\"{%s,%s,%s,%s,%s,%s,%s}", minPositionStr, maxPositionStr, maxVelocityStr, maxAccelerationStr,
            maxForceTensileStr, maxForceCompressionStr, forceGaugeNeutralOffsetStr);
    printf("Performance:%s\n", json);
    free(minPositionStr);
    free(maxPositionStr);
    free(maxVelocityStr);
    free(maxAccelerationStr);
    free(maxForceTensileStr);
    free(maxForceCompressionStr);
    free(forceGaugeNeutralOffsetStr);
    return json;
}

void freeMachineConfiguration(MachineConfiguration *configuration)
{
}
void freeMachinePerformance(MachinePerformance *performance)
{
}

typedef struct SampleProfile_s
{

} SampleProfile;

char *intToJSON(char *name, int value)
{
    char *json;
    int size = 4 + strlen(name) + 10; //  "":\0 are 4 chars plus strlen(name) and 10 for integer
    json = (char *)malloc(sizeof(char) * size);
    sprintf(json, "\"%s\":%d", name, value);
    return json;
}
char *stringToJSON(char *name, char *value)
{
    char *json;
    int size = 6 + strlen(name) + strlen(value); //"":""\0 are 6 chars plus strlen(name,value)
    json = (char *)malloc(sizeof(char) * size);
    sprintf(json, "\"%s\":\"%s\"", name, value);
    return json;
}