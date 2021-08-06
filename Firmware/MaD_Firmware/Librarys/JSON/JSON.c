#include "JSON.h"

//Private Functions

static char *float_to_json(char *name, float value)
{
    char *json;
    char floatStr[32]; //Buffer to hold the float as a string
    sprintf(floatStr, "%f", value);
    int size = 4 + strlen(name) + strlen(floatStr); //  "":\0 are 4 chars plus strlen(name) and strlen(value)
    json = (char *)malloc(sizeof(char) * size);
    sprintf(json, "\"%s\":%s", name, floatStr);
    return json;
}

static char *int_to_json(char *name, int value)
{
    char *json;
    char intStr[32]; //Buffer to hold the int as a string
    sprintf(intStr, "%d", value);
    int size = 4 + strlen(name) + strlen(intStr); //  "":\0 are 4 chars plus strlen(name) and 10 for integer
    json = (char *)malloc(sizeof(char) * size);
    sprintf(json, "\"%s\":%s", name, intStr);
    return json;
}
static char *string_to_json(char *name, char *value)
{
    char *json;
    int size = 6 + strlen(name) + strlen(value); //"":""\0 are 6 chars plus strlen(name,value)
    json = (char *)malloc(sizeof(char) * size);
    sprintf(json, "\"%s\":\"%s\"", name, value);
    return json;
}

static char *json_property_to_string(char *json, char *name)
{
    //Determine size of pattern string and create it
    int size = strlen(name) + 5; // "":" \0, 4 chars plus interior
    char *pattern = (char *)malloc(sizeof(char) * size);
    sprintf(pattern, "\"%s\":\"", name);

    //Find first index of pattern string in json, add strlen of pattern to get first character of value string
    char *strStart = strstr(json, pattern) + strlen(pattern);

    //Find end of value string
    char *strEnd = strstr(strStart, "\",");

    //Make copy of string to return
    int valueLength = strlen(strStart) - strlen(strEnd);
    char *propertyValue = (char *)malloc(sizeof(char) * (valueLength + 1));
    strncpy(propertyValue, strStart, valueLength);

    //Free pattern
    free(pattern);

    return propertyValue;
}

static int json_property_to_int(char *json, char *name)
{
    //Determine size of pattern string and create it
    int size = strlen(name) + 4; // "": \0, 3 chars plus interior
    char *pattern = (char *)malloc(sizeof(char) * size);
    sprintf(pattern, "\"%s\":", name);

    //Find first index of pattern string in json, add strlen of pattern to get first character of value string
    char *strStart = strstr(json, pattern) + strlen(pattern);

    //Find end of value string
    char *strEnd = strstr(strStart, ",");

    //Make copy of string to convert
    int valueLength = strlen(strStart) - strlen(strEnd);
    char *propertyValueString = (char *)malloc(sizeof(char) * (valueLength + 1));
    strncpy(propertyValueString, strStart, valueLength);

    //Convert string to int
    int propertyValue = atoi(propertyValueString);

    //Free pattern and value string
    free(pattern);
    free(propertyValueString);

    return propertyValue;
}

static float json_property_to_float(char *json, char *name)
{
    //Determine size of pattern string and create it
    int size = strlen(name) + 4; // "": \0, 4 chars plus interior
    char *pattern = (char *)malloc(sizeof(char) * size);
    sprintf(pattern, "\"%s\":", name);

    //Find first index of pattern string in json, add strlen of pattern to get first character of value string
    char *strStart = strstr(json, pattern) + strlen(pattern);

    //Find end of value string
    char *strEnd = strstr(strStart, ",");

    //Make copy of string to convert
    int valueLength = strlen(strStart) - strlen(strEnd);
    char *propertyValueString = (char *)malloc(sizeof(char) * (valueLength + 1));
    strncpy(propertyValueString, strStart, valueLength);

    //Convert string to float
    float propertyValue = atof(propertyValueString);

    //Free pattern and value string
    free(pattern);
    free(propertyValueString);

    return propertyValue;
}

static char *json_to_object_json(char *json, char *objectName)
{
    //Determine size of pattern string and create it
    int size = strlen(objectName) + 6; // "":{\0, 6 chars plus interior
    char *pattern = (char *)malloc(sizeof(char) * size);
    sprintf(pattern, "\"%s\":{", objectName);

    //Find first index of pattern string in json, add strlen of pattern to get first character of object string
    char *strStart = strstr(json, pattern) + strlen(pattern);

    //Find end of object string
    char *strEnd = strstr(strStart, "}");

    //Make copy of string to return
    int objectStringLength = strlen(strStart) - strlen(strEnd);
    char *objectString = (char *)malloc(sizeof(char) * (objectStringLength + 1));
    strncpy(objectString, strStart, objectStringLength);

    //free
    free(pattern);

    return objectString;
}

/**
 * @brief Constructs a MachineConfiguration structure from a JSON string.
 * 
 * @param json A JSON string containing a machine configuration.
 */

static void json_to_machine_configuration(char *json, MachineConfiguration *configuration)
{
    //Determine size of pattern string and create it
    char *name = "Configuration";
    char *configurationJSON = json_to_object_json(json, name);
    configuration->motorType = json_property_to_string(configurationJSON, "Motor Type");
    configuration->maxMotorRPM = json_property_to_float(configurationJSON, "Max Motor RPM");
    configuration->maxMotorTorque = json_property_to_float(configurationJSON, "Max Motor Torque");
    configuration->gearDiameter = json_property_to_float(configurationJSON, "Gear Diameter");
    configuration->gearPitch = json_property_to_float(configurationJSON, "Gear Pitch");
    configuration->systemIntertia = json_property_to_float(configurationJSON, "System Intertia");
    configuration->staticTorque = json_property_to_float(configurationJSON, "Static Torque");
    configuration->load = json_property_to_float(configurationJSON, "Load");
    configuration->positionEncoderType = json_property_to_string(configurationJSON, "Position Encoder Type");
    configuration->positionEncoderScaleFactor = json_property_to_int(configurationJSON, "Position Encoder Scale Factor");
}

/**
 * @brief Constructs a MachinePerformance structure from a JSON string.
 * 
 * @param json A JSON string containing a machine performance.
 */

static void json_to_machine_performance(char *json, MachinePerformance *performance)
{
    //Determine size of pattern string and create it
    char *name = "Performance";
    char *performanceJSON = json_to_object_json(json, name);
    performance->minPosition = json_property_to_float(performanceJSON, "Position Minimum");
    performance->maxPosition = json_property_to_float(performanceJSON, "Position Maximum");
    performance->maxVelocity = json_property_to_float(performanceJSON, "Velocity Maximum");
    performance->maxAcceleration = json_property_to_float(performanceJSON, "Acceleration Maximum");
    performance->maxForceTensile = json_property_to_float(performanceJSON, "Force Tensile Maximum");
    performance->maxForceCompression = json_property_to_float(performanceJSON, "Force Compression Maximum");
    performance->forceGaugeNeutralOffset = json_property_to_float(performanceJSON, "Force gauge Neutral Offset");
}

static MachineConfiguration *get_machine_configuration()
{
    MachineConfiguration *configuration = (MachineConfiguration *)malloc(sizeof(MachineConfiguration));
    configuration->motorType = NULL;
    configuration->maxMotorTorque = 0.0;
    configuration->maxMotorRPM = 0.0;
    configuration->gearDiameter = 0.0;
    configuration->gearPitch = 0.0;
    configuration->systemIntertia = 0.0;
    configuration->staticTorque = 0.0;
    configuration->load = 0.0;
    configuration->positionEncoderType = NULL;
    configuration->positionEncoderScaleFactor = 0;
    configuration->forceGauge = NULL;
    return configuration;
}

static MachinePerformance *get_machine_performance()
{
    MachinePerformance *performance = (MachinePerformance *)malloc(sizeof(MachinePerformance));
    performance->minPosition = 0.0;
    performance->maxPosition = 0.0;
    performance->maxVelocity = 0.0;
    performance->maxAcceleration = 0.0;
    performance->maxForceTensile = 0.0;
    performance->maxForceCompression = 0.0;
    performance->forceGaugeNeutralOffset = 0.0;
    return performance;
}

static char *machine_configuration_to_json(MachineConfiguration *configuration)
{
    char *json;
    int size = 20; //"Configuration":{}\0, 19 chars plus interior
    char *motorTypeJSON = string_to_json("Motor Type", configuration->motorType);
    size += strlen(motorTypeJSON) + 2;
    char *maxMotorRPMJSON = float_to_json("Max Motor RPM", configuration->maxMotorRPM);
    size += strlen(maxMotorRPMJSON) + 2;
    char *maxMotorTorqueJSON = float_to_json("Max Motor Torque", configuration->maxMotorTorque);
    size += strlen(maxMotorTorqueJSON) + 2;
    char *gearDiameterJSON = float_to_json("Gear Diameter", configuration->gearDiameter);
    size += strlen(gearDiameterJSON) + 2;
    char *gearPitchJSON = float_to_json("Gear Pitch", configuration->gearPitch);
    size += strlen(gearPitchJSON) + 2;
    char *systemInertiaJSON = float_to_json("System Intertia", configuration->systemIntertia);
    size += strlen(systemInertiaJSON) + 2;
    char *staticTorqueJSON = float_to_json("Static Torque", configuration->staticTorque);
    size += strlen(staticTorqueJSON) + 2;
    char *loadJSON = float_to_json("Load", configuration->load);
    size += strlen(loadJSON) + 2;
    char *encoderTypeJSON = string_to_json("Position Encoder Type", configuration->positionEncoderType);
    size += strlen(encoderTypeJSON) + 2;
    char *encoderScaleFactorJSON = int_to_json("Position Encoder Scale Factor", configuration->positionEncoderScaleFactor);
    size += strlen(encoderScaleFactorJSON) + 2;
    char *forceGaugeJSON = string_to_json("Force Gauge", configuration->forceGauge);
    size += strlen(forceGaugeJSON) + 2;
    json = (char *)malloc(sizeof(char) * size);
    sprintf(json, "\"Configuration\":{%s,%s,%s,%s,%s,%s,%s,%s,%s,%s}", motorTypeJSON, maxMotorRPMJSON, maxMotorTorqueJSON,
            gearDiameterJSON, gearPitchJSON, systemInertiaJSON, staticTorqueJSON, loadJSON, encoderTypeJSON,
            encoderScaleFactorJSON, forceGaugeJSON);
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
static char *machine_performance_to_json(MachinePerformance *performance)
{
    char *json;
    int size = 17; //"Performance":{}\0, 17 chars plus interior
    char *minPositionStr = float_to_json("Position Minimum", performance->minPosition);
    size += strlen(minPositionStr) + 2;
    char *maxPositionStr = float_to_json("Position Maximum", performance->maxPosition);
    size += strlen(maxPositionStr) + 2;
    char *maxVelocityStr = float_to_json("Velocity Maximum", performance->maxVelocity);
    size += strlen(maxVelocityStr) + 2;
    char *maxAccelerationStr = float_to_json("Acceleration Maximum", performance->maxAcceleration);
    size += strlen(maxAccelerationStr) + 2;
    char *maxForceTensileStr = float_to_json("Force Tensile Maximum", performance->maxForceTensile);
    size += strlen(maxForceTensileStr) + 2;
    char *maxForceCompressionStr = float_to_json("Force Compression Maximum", performance->maxForceCompression);
    size += strlen(maxForceCompressionStr) + 2;
    char *forceGaugeNeutralOffsetStr = float_to_json("Force gauge Neutral Offset", performance->forceGaugeNeutralOffset);
    size += strlen(forceGaugeNeutralOffsetStr) + 2;
    json = (char *)malloc(sizeof(char) * size);
    sprintf(json, "\"Performance\":{%s,%s,%s,%s,%s,%s,%s}", minPositionStr, maxPositionStr, maxVelocityStr, maxAccelerationStr,
            maxForceTensileStr, maxForceCompressionStr, forceGaugeNeutralOffsetStr);
    //printf("Performance:%s\n", json);
    free(minPositionStr);
    free(maxPositionStr);
    free(maxVelocityStr);
    free(maxAccelerationStr);
    free(maxForceTensileStr);
    free(maxForceCompressionStr);
    free(forceGaugeNeutralOffsetStr);
    return json;
}

static void free_machine_configuration(MachineConfiguration *configuration)
{
    free(configuration->motorType);
    free(configuration->positionEncoderType);
    free(configuration->forceGauge);
    free(configuration);
}
static void free_machine_performance(MachinePerformance *performance)
{
    free(performance);
}
/*Public Functions*/
MachineProfile *get_machine_profile()
{
    MachineProfile *settings = (MachineProfile *)malloc(sizeof(MachineProfile));
    settings->name = NULL;
    settings->number = 0;
    settings->configuration = get_machine_configuration();
    settings->performance = get_machine_performance();
    return settings;
}

void free_machine_settings(MachineProfile *settings)
{
    if (settings != NULL)
    {
        if (settings->name != NULL)
        {
            free(settings->name);
        }
        if (settings->configuration != NULL)
        {
            free_machine_configuration(settings->configuration);
        }
        if (settings->performance != NULL)
        {
            free_machine_performance(settings->performance);
        }
        free(settings);
    }
}

char *machine_profile_to_json(MachineProfile *settings)
{
    char *json;
    int size = 5; //{}\0, 3 chars plus interior
    char *nameJSON = string_to_json("Name", settings->name);
    size += strlen(nameJSON) + 2;
    char *numberJSON = int_to_json("Number", settings->number);
    size += strlen(numberJSON) + 2;
    char *configurationJSON = machine_configuration_to_json(settings->configuration);
    size += strlen(configurationJSON) + 2;
    char *performanceJSON = machine_performance_to_json(settings->performance);
    size += strlen(performanceJSON);
    json = (char *)malloc(sizeof(char) * size);
    sprintf(json, "{%s,%s,%s,%s}", nameJSON, numberJSON, configurationJSON, performanceJSON);
    free(nameJSON);
    free(numberJSON);
    free(configurationJSON);
    free(performanceJSON);
    return json;
}

/**
 * @brief Converts JSON string to a MachineProfile structure.
 * 
 * @param json A JSON string containing a machine profile.
 * @return A MachineProfile structure containing the machine profile from JSON. 
 */
MachineProfile *json_to_machine_profile(char *json)
{
    MachineProfile *settings = get_machine_profile();
    int temp = json_property_to_int(json, "Number");
    settings->name = json_property_to_string(json, "Name");
    settings->number = json_property_to_int(json, "Number");
    json_to_machine_configuration(json, settings->configuration);
    json_to_machine_performance(json, settings->performance);
    return settings;
}