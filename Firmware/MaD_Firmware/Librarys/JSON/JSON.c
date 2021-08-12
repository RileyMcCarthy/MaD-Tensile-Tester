#include "JSON.h"
#ifdef __MEMORY_CHECK__
#include "leak_detector_c.h"
#endif
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
    char *strEnd1 = strstr(strStart, "\",");
    char *strEnd2 = strstr(strStart, "\"}");
    char *strEnd = strlen(strEnd1) > strlen(strEnd2) ? strEnd1 : strEnd2;

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
    char *strEnd1 = strchr(strStart, ',');
    char *strEnd2 = strchr(strStart, '}');
    char *strEnd = strlen(strEnd1) > strlen(strEnd2) ? strEnd1 : strEnd2;

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
    char *strEnd1 = strchr(strStart, ',');
    char *strEnd2 = strchr(strStart, '}');
    char *strEnd = strlen(strEnd1) > strlen(strEnd2) ? strEnd1 : strEnd2;

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

static int json_property_to_string_array(char *json, char *name, char ***stringArrayPtr)
{
    char **stringArray;
    //Determine size of pattern string and create it
    int size = strlen(name) + 6; // "":{\0, 6 chars plus interior
    char *pattern = (char *)malloc(sizeof(char) * size);
    sprintf(pattern, "\"%s\":[", name);
    //Find first index of pattern string in json, add strlen of pattern to get first character of object string
    char *strStart = strstr(json, pattern) + strlen(pattern);

    //Find end of object string
    char *strEnd = strchr(strStart, ']') + 1;
    int length = strlen(strStart) - strlen(strEnd);
    int stringCount = 0;
    int cursor = 0;
    stringArray = NULL;
    for (int i = 0; i < length; i++)
    {
        if (strStart[i] == ',' || strStart[i] == ']')
        {
            stringCount++;
            if (stringArray == NULL)
            {
                stringArray = (char **)malloc(sizeof(char *) * stringCount);
            }
            else
            {
                stringArray = (char **)realloc(stringArray, sizeof(char *) * stringCount);
            }
            stringArray[stringCount - 1] = (char *)malloc(sizeof(char) * (i - cursor + 1));
            strncpy(stringArray[stringCount - 1], strStart + cursor + 1, i - cursor - 2);
            cursor = i + 1;
        }
    }
    free(pattern);
    *stringArrayPtr = stringArray;
    return stringCount;
}

static int json_property_to_object_json_array(char *json, char *objectName, char ***objectArrayPtr)
{
    //Determine size of pattern string and create it
    int size = strlen(objectName) + 6; // "":{\0, 6 chars plus interior
    char *pattern = (char *)malloc(sizeof(char) * size);
    sprintf(pattern, "\"%s\":", objectName);
    //Find first index of pattern string in json, add strlen of pattern to get first character of object string
    char *strStart = strstr(json, pattern) + strlen(pattern);
    //Find end of object string
    int length = strlen(strStart);
    int count = 0;
    int position = 0;
    for (int i = 0; i < length; i++)
    {
        if (strStart[i] == '[')
        {
            count++;
        }
        else if (strStart[i] == ']')
        {
            count--;
        }
        if (count == 0)
        {
            position = i;
            break;
        }
    }

    if (position == 0)
    {
        printf("Error: Object not found in json string\n");
        return NULL;
    }

    int count = 0;
    int cursor = 1;
    int objectCount = 0;
    char **objectArray = NULL;
    for (int i = 1; i < position; i++)
    {
        if (strStart[i] == '{')
        {
            count++;
        }
        else if (strStart[i] == '}')
        {
            count--;
        }
        if (count == 0)
        {
            i++;
            objectCount++;
            if (objectArray == NULL)
            {
                objectArray = (char **)malloc(sizeof(char *) * objectCount);
            }
            else
            {
                objectArray = (char **)realloc(objectArray, sizeof(char *) * objectCount);
            }
            objectArray[objectCount - 1] = (char *)malloc(sizeof(char) * (i - cursor + 1));
            strncpy(objectArray[objectCount - 1], strStart + cursor, i - cursor);
            cursor = i + 1;
        }
    }
    *objectArrayPtr = objectArray;
    free(pattern);
    return objectCount;
}

static char *json_property_to_object_json(char *json, char *objectName)
{
    //Determine size of pattern string and create it
    int size = strlen(objectName) + 6; // "":{\0, 6 chars plus interior
    char *pattern = (char *)malloc(sizeof(char) * size);
    sprintf(pattern, "\"%s\":", objectName);

    //Find first index of pattern string in json, add strlen of pattern to get first character of object string
    char *strStart = strstr(json, pattern) + strlen(pattern);

    //Find end of object string
    int length = strlen(strStart);
    int count = 0;
    int position = 0;
    for (int i = 0; i < length; i++)
    {
        if (strStart[i] == '{')
        {
            count++;
        }
        else if (strStart[i] == '}')
        {
            count--;
        }
        if (count == 0)
        {
            position = i;
            break;
        }
    }

    if (position == 0)
    {
        printf("Error: Object not found in json string\n");
        return NULL;
    }

    //Make copy of string to return
    char *objectString = (char *)malloc(sizeof(char) * (position + 1));
    strncpy(objectString, strStart, position);

    //free
    free(pattern);

    return objectString;
}

/**Structure Initialization Functions*/

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

MotionSet *get_motion_set()
{
    MotionSet *set = (MotionSet *)malloc(sizeof(MotionSet));
    set->name = NULL;
    set->number = 0;
    set->type = NULL;
    set->executions = 0;
    set->quartetCount = 0;
    set->quartets = NULL;
    return set;
}

/**Json to structure functions**/

/**
 * @brief Constructs a MachineConfiguration structure from a JSON string.
 * 
 * @param json A JSON string containing a machine configuration.
 */

static void json_to_machine_configuration(char *json, MachineConfiguration *configuration)
{
    configuration->motorType = json_property_to_string(json, "Motor Type");
    configuration->maxMotorRPM = json_property_to_float(json, "Max Motor RPM");
    configuration->maxMotorTorque = json_property_to_float(json, "Max Motor Torque");
    configuration->gearDiameter = json_property_to_float(json, "Gear Diameter");
    configuration->gearPitch = json_property_to_float(json, "Gear Pitch");
    configuration->systemIntertia = json_property_to_float(json, "System Intertia");
    configuration->staticTorque = json_property_to_float(json, "Static Torque");
    configuration->load = json_property_to_float(json, "Load");
    configuration->positionEncoderType = json_property_to_string(json, "Position Encoder Type");
    configuration->positionEncoderScaleFactor = json_property_to_int(json, "Position Encoder Scale Factor");
}

/**
 * @brief Constructs a MachinePerformance structure from a JSON string.
 * 
 * @param json A JSON string containing a machine performance.
 */

static void json_to_machine_performance(char *json, MachinePerformance *performance)
{
    performance->minPosition = json_property_to_float(json, "Position Minimum");
    performance->maxPosition = json_property_to_float(json, "Position Maximum");
    performance->maxVelocity = json_property_to_float(json, "Velocity Maximum");
    performance->maxAcceleration = json_property_to_float(json, "Acceleration Maximum");
    performance->maxForceTensile = json_property_to_float(json, "Force Tensile Maximum");
    performance->maxForceCompression = json_property_to_float(json, "Force Compression Maximum");
    performance->forceGaugeNeutralOffset = json_property_to_float(json, "Force gauge Neutral Offset");
}

static MotionSet *json_to_motion_set(char *json)
{

    MotionSet *set = get_motion_set();
    set->name = json_property_to_string(json, "Name");
    set->number = json_property_to_int(json, "Number");
    set->type = json_property_to_string(json, "Type");
    set->executions = json_property_to_int(json, "Executions");
    set->quartetCount = json_property_to_string_array(json, "Quartets", &(set->quartets));
    return set;
}

/**Structure to JSON Functions**/

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

static void free_motion_set(MotionSet *set)
{
    if (set != NULL)
    {
        if (set->name != NULL)
        {
            free(set->name);
        }
        if (set->type != NULL)
        {
            free(set->type);
        }
        if (set->quartets != NULL)
        {
            for (int i = 0; i < set->quartetCount; i++)
            {
                free_motion_quartet(set->quartets[i]);
            }
            free(set->quartets);
        }
        free(set);
    }
}

/*Public Functions*/

/**Initialation Functions**/

MachineProfile *get_machine_profile()
{
    MachineProfile *settings = (MachineProfile *)malloc(sizeof(MachineProfile));
    settings->name = NULL;
    settings->number = 0;
    settings->configuration = get_machine_configuration();
    settings->performance = get_machine_performance();
    return settings;
}

MotionProfile *get_motion_profile()
{
    MotionProfile *motion = (MotionProfile *)malloc(sizeof(MotionProfile));
    motion->name = NULL;
    motion->number = 0;
    motion->setCount = 0;
    motion->sets = NULL;
    return motion;
}

SampleProfile *get_sample_profile()
{
    SampleProfile *sample = (SampleProfile *)malloc(sizeof(SampleProfile));
    sample->name = NULL;
    sample->number = 0;
    sample->length = 0.0;
    sample->stretchMax = 0.0;
    sample->maxVelocity = 0.0;
    sample->maxAcceleration = 0.0;
    sample->maxJerk = 0.0;
    sample->maxForceTensile = 0.0;
    sample->maxForceCompression = 0.0;
    return sample;
}

TestProfile *get_test_profile()
{
    TestProfile *test = (TestProfile *)malloc(sizeof(TestProfile));
    test->name = NULL;
    test->machineProfileFileName = NULL;
    test->sampleProfileFileName = NULL;
    test->sampleSerialNumber = 0;
    return test;
}

MotionQuartet *get_motion_quartet()
{
    MotionQuartet *quartet = (MotionQuartet *)malloc(sizeof(MotionQuartet));
    quartet->name = NULL;
    quartet->type = NULL;
    quartet->distance = 0.0;
    quartet->velocity = 0.0;
    quartet->acceleration = 0.0;
    quartet->jerk = 0.0;
    quartet->dwell = 0.0;
    return quartet;
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

char *motion_profile_to_json(MotionProfile *motion)
{
    char *json;
    int size = 5; //{}\0, 3 chars plus interior
    char *nameJSON = string_to_json("Name", motion->name);
    size += strlen(nameJSON) + 2;
    char *numberJSON = int_to_json("Number", motion->number);
    size += strlen(numberJSON) + 2;
    char **setJSONS = (char **)malloc(sizeof(char *) * motion->setCount);
    int setJSONSize = 20; //\"Motion Sets\":[, 3 chars plus interior
    for (int i = 0; i < motion->setCount; i++)
    {
        setJSONS[i] = motion_set_to_json(motion->sets[i]);
        setJSONSize += strlen(setJSONS[i]) + 2;
    }
    char *setJSON = malloc(sizeof(char) * setJSONSize);
    strcpy(setJSON, "\"Motion Sets\":[");
    for (int i = 0; i < motion->setCount; i++)
    {
        strcat(setJSON, setJSONS[i]);
        free(setJSONS[i]);
        if (i < motion->setCount - 1)
        {
            strcat(setJSON, ",");
        }
    }
    free(setJSONS);
    strcat(setJSON, "]");
    size += strlen(setJSON) + 2;
    json = (char *)malloc(sizeof(char) * size);
    sprintf(json, "{%s,%s,%s}", nameJSON, numberJSON, setJSON);
    free(nameJSON);
    free(numberJSON);
    free(setJSON);
    return json;
}

char *sample_profile_to_json(SampleProfile *sample)
{
    char *json;
    int size = 5; //{}\0, 3 chars plus interior
    char *nameJSON = string_to_json("Name", sample->name);
    size += strlen(nameJSON) + 2;
    char *numberJSON = int_to_json("Number", sample->number);
    size += strlen(numberJSON) + 2;
    char *lengthJSON = float_to_json("Length", sample->length);
    size += strlen(lengthJSON) + 2;
    char *stretchMaxJSON = float_to_json("Stretch Max", sample->stretchMax);
    size += strlen(stretchMaxJSON) + 2;
    char *maxVelocityJSON = float_to_json("Max Velocity", sample->maxVelocity);
    size += strlen(maxVelocityJSON) + 2;
    char *maxAccelerationJSON = float_to_json("Max Acceleration", sample->maxAcceleration);
    size += strlen(maxAccelerationJSON) + 2;
    char *maxJerkJSON = float_to_json("Max Jerk", sample->maxJerk);
    size += strlen(maxJerkJSON) + 2;
    char *maxForceTensileJSON = float_to_json("Max Force Tensile", sample->maxForceTensile);
    size += strlen(maxForceTensileJSON) + 2;
    char *maxForceCompressionJSON = float_to_json("Max Force Compression", sample->maxForceCompression);
    size += strlen(maxForceCompressionJSON) + 2;
    json = (char *)malloc(sizeof(char) * size);
    sprintf(json, "{%s,%s,%s,%s,%s,%s,%s,%s,%s}", nameJSON, numberJSON, lengthJSON, stretchMaxJSON, maxVelocityJSON, maxAccelerationJSON, maxJerkJSON, maxForceTensileJSON, maxForceCompressionJSON);
    free(nameJSON);
    free(numberJSON);
    free(lengthJSON);
    free(stretchMaxJSON);
    free(maxVelocityJSON);
    free(maxAccelerationJSON);
    free(maxJerkJSON);
    free(maxForceTensileJSON);
    free(maxForceCompressionJSON);
    return json;
}

char *motion_quartet_to_json(MotionQuartet *quartet)
{
    char *json;
    int size = 5; //{}\0, 3 chars plus interior
    char *nameJSON = string_to_json("Name", quartet->name);
    size += strlen(nameJSON) + 2;
    char *typeJSON = string_to_json("Type", quartet->type);
    size += strlen(typeJSON) + 2;
    char *distanceJSON = float_to_json("Distance", quartet->distance);
    size += strlen(distanceJSON) + 2;
    char *velocityJSON = float_to_json("Velocity", quartet->velocity);
    size += strlen(velocityJSON) + 2;
    char *accelerationJSON = float_to_json("Acceleration", quartet->acceleration);
    size += strlen(accelerationJSON) + 2;
    char *jerkJSON = float_to_json("Jerk", quartet->jerk);
    size += strlen(jerkJSON) + 2;
    char *dwellJSON = float_to_json("Dwell", quartet->dwell);
    size += strlen(dwellJSON) + 2;
    json = (char *)malloc(sizeof(char) * size);
    sprintf(json, "{%s,%s,%s,%s,%s,%s,%s}", nameJSON, typeJSON, distanceJSON, velocityJSON, accelerationJSON, jerkJSON, dwellJSON);
    free(nameJSON);
    free(typeJSON);
    free(distanceJSON);
    free(velocityJSON);
    free(accelerationJSON);
    free(jerkJSON);
    free(dwellJSON);
    return json;
}

char *test_profile_to_json(TestProfile *test)
{
    char *json;
    int size = 5; //{}\0, 3 chars plus interior
    char *nameJSON = string_to_json("Name", test->name);
    size += strlen(nameJSON) + 2;
    char *machineProfileFileNameJSON = string_to_json("Machine Profile File Name", test->machineProfileFileName);
    size += strlen(machineProfileFileNameJSON) + 2;
    char *sampleProfileFileNameJSON = string_to_json("Sample Profile File Name", test->sampleProfileFileName);
    size += strlen(sampleProfileFileNameJSON) + 2;
    char *sampleSerialNumberJSON = int_to_json("Sample Serial Number", test->sampleSerialNumber);
    size += strlen(sampleSerialNumberJSON) + 2;
    json = (char *)malloc(sizeof(char) * size);
    sprintf(json, "{%s,%s,%s,%s}", nameJSON, machineProfileFileNameJSON, sampleProfileFileNameJSON, sampleSerialNumberJSON);
    free(nameJSON);
    free(machineProfileFileNameJSON);
    free(sampleProfileFileNameJSON);
    free(sampleSerialNumberJSON);
    return json;
}

char *motion_set_to_json(MotionSet *set)
{
    char *json;
    int size = 20; //"Motion Set":{}\0, 19 chars plus interior
    char *nameJSON = string_to_json("Name", set->name);
    size += strlen(nameJSON) + 2;
    char *numberJSON = int_to_json("Number", set->number);
    size += strlen(numberJSON) + 2;
    char *typeJSON = string_to_json("Type", set->type);
    size += strlen(typeJSON) + 2;
    char *executionJSON = int_to_json("Executions", set->executions);
    size += strlen(executionJSON) + 2;
    char *quartetJSON = malloc(sizeof(char) * (strlen("Quartets:[") + 4));
    strcpy(quartetJSON, "\"Quartets\":["); //@todo: turn this into string_array_to_json
    for (int i = 0; i < set->quartetCount; i++)
    {
        quartetJSON = (char *)realloc(quartetJSON, sizeof(char) * (strlen(quartetJSON) + strlen(set->quartets[i]) + 5));
        strcat(quartetJSON, "\"");
        strcat(quartetJSON, set->quartets[i]);
        strcat(quartetJSON, "\"");
        if (i < set->quartetCount - 1)
        {
            strcat(quartetJSON, ",");
        }
    }
    strcat(quartetJSON, "]");
    size += strlen(quartetJSON) + 2;

    json = (char *)malloc(sizeof(char) * size);
    sprintf(json, "{%s,%s,%s,%s,%s}", nameJSON, numberJSON, typeJSON, executionJSON, quartetJSON);

    free(nameJSON);
    free(numberJSON);
    free(typeJSON);
    free(executionJSON);
    free(quartetJSON);
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
    settings->name = json_property_to_string(json, "Name");
    settings->number = json_property_to_int(json, "Number");
    //Determine size of pattern string and create it
    char *configurationJSON = json_property_to_object_json(json, "Configuration");
    if (configurationJSON == NULL)
    {
        printf("Error: Configuration not found in json string\n");
    }
    free(configurationJSON);
    json_to_machine_configuration(configurationJSON, settings->configuration);

    char *performanceJSON = json_property_to_object_json(json, "Performance");
    if (performanceJSON == NULL)
    {
        printf("Error: Performance not found in json string\n");
    }
    json_to_machine_performance(performanceJSON, settings->performance);
    free(performanceJSON);
    return settings;
}

MotionProfile *json_to_motion_profile(char *json)
{
    MotionProfile *profile = get_motion_profile();
    profile->name = json_property_to_string(json, "Name");
    profile->number = json_property_to_int(json, "Number");
    char **setJSONArray;
    profile->setCount = json_property_to_object_json_array(json, "Motion Sets", &setJSONArray);
    profile->sets = (MotionSet *)malloc(sizeof(MotionSet) * profile->setCount);
    for (int i = 0; i < profile->setCount; i++)
    {
        profile->sets[i] = json_to_motion_set(setJSONArray[i]);
    }
    return profile;
}

SampleProfile *json_to_sample_profile(char *json)
{
    SampleProfile *sample = get_sample_profile();
    sample->name = json_property_to_string(json, "Name");
    sample->number = json_property_to_int(json, "Number");
    sample->length = json_property_to_float(json, "Length");
    sample->stretchMax = json_property_to_float(json, "Stretch Max");
    sample->maxVelocity = json_property_to_float(json, "Max Velocity");
    sample->maxAcceleration = json_property_to_float(json, "Max Acceleration");
    sample->maxJerk = json_property_to_float(json, "Max Jerk");
    sample->maxForceTensile = json_property_to_float(json, "Max Force Tensile");
    sample->maxForceCompression = json_property_to_float(json, "Max Force Compression");
    return sample;
}

TestProfile *json_to_test_profile(char *json)
{
    TestProfile *test = get_test_profile();
    test->name = json_property_to_string(json, "Name");
    test->machineProfileFileName = json_property_to_string(json, "Machine Profile File Name");
    test->sampleProfileFileName = json_property_to_string(json, "Sample Profile File Name");
    test->sampleSerialNumber = json_property_to_int(json, "Sample Serial Number");
    return test;
}

MotionQuartet *json_to_motion_quartet(char *json)
{
    MotionQuartet *quartet = get_motion_quartet();
    quartet->name = json_property_to_string(json, "Name");
    quartet->type = json_property_to_string(json, "Type");
    quartet->distance = json_property_to_float(json, "Distance");
    quartet->velocity = json_property_to_float(json, "Velocity");
    quartet->acceleration = json_property_to_float(json, "Acceleration");
    quartet->jerk = json_property_to_float(json, "Jerk");
    quartet->dwell = json_property_to_float(json, "Dwell");
    return quartet;
}

void free_machine_profile(MachineProfile *settings)
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

void free_sample_profile(SampleProfile *sample)
{
    if (sample != NULL)
    {
        if (sample->name != NULL)
        {
            free(sample->name);
        }
    }
    free(sample);
}

void free_test_profile(TestProfile *test)
{
    if (test != NULL)
    {
        if (test->name != NULL)
        {
            free(test->name);
        }
        if (test->machineProfileFileName != NULL)
        {
            free(test->machineProfileFileName);
        }
        if (test->sampleProfileFileName != NULL)
        {
            free(test->sampleProfileFileName);
        }
    }
    free(test);
}

void free_motion_quartet(MotionQuartet *quartet)
{
    if (quartet != NULL)
    {
        if (quartet->name != NULL)
        {
            free(quartet->name);
        }

        if (quartet->type != NULL)
        {
            free(quartet->type);
        }
    }
    free(quartet);
}

void free_motion_profile(MotionProfile *profile)
{
    if (profile != NULL)
    {
        if (profile->name != NULL)
        {
            free(profile->name);
        }
        if (profile->sets != NULL)
        {
            for (int i = 0; i < profile->setCount; i++)
            {
                free_motion_set(profile->sets[i]);
            }
            free(profile->sets);
        }
        free(profile);
    }
}