#include "JSON.h"
#include "MotionPlanning.h"
// Private Functions

static char *get_string_from_index(FILE *file, int start, int end)
{

    int startPosition = ftell(file);

    fseek(file, start, SEEK_SET);
    char *string = (char *)malloc(sizeof(char) * (end - start + 1));
    fread(string, sizeof(char), end - start, file);

    fseek(file, startPosition, SEEK_SET);
    return string;
}

static int increment_file_to_string(FILE *json, const char *string)
{
    int temp;
    int position = 0;
    while ((temp = fgetc(json)) != EOF)
    {
        char c = (char)temp;
        if (c == string[position])
        {
            if (position == strlen(string) - 1)
            {
                break;
            }
            position++;
        }
        else
        {
            position = 0;
        }
    }
    return ftell(json);
}

static char *float_to_json(const char *name, float value)
{
    char *json;
    int size = 6 + strlen(name) + 32; //  "":\0 are 4 chars plus strlen(name) and strlen(value)
    json = (char *)malloc(sizeof(char) * size);
    if (json == NULL)
    {
        printf("Error: malloc failed\n");
        return NULL;
    }
    sprintf(json, "\"%s\":%f", name, value);
    return json;
}

static char *int_to_json(const char *name, int value)
{
    char *json;
    int size = 6 + strlen(name) + 32; //  "":\0 are 4 chars plus strlen(name) and 10 for integer
    json = (char *)malloc(sizeof(char) * size);
    sprintf(json, "\"%s\":%d", name, value);
    return json;
}
static char *string_to_json(const char *name, const char *value)
{
    char *json;
    int size = 8 + strlen(name) + strlen(value); //"":""\0 are 6 chars plus strlen(name,value)
    json = (char *)malloc(sizeof(char) * size);
    if (json == NULL)
    {
        printf("Error: malloc failed\n");
        return NULL;
    }
    sprintf(json, "\"%s\":\"%s\"", name, value);
    return json;
}

static char *json_property_to_string(const json_t *parser, const char *name)
{
    json_t const *property = json_getProperty(parser, name);
    printf("the type is: %d\n", json_getType(property));
    if (property == NULL)
    {
        printf("Error, the %s property is not found.\n", name);
    }
    char const *value = json_getValue(property);
    char *valueMem = (char *)malloc(sizeof(char) * (strlen(value) + 1));
    strcpy(valueMem, value);
    return valueMem;
}

static int json_property_to_int(const json_t *parser, const char *name)
{
    json_t const *property = json_getProperty(parser, name);
    if (!property || JSON_INTEGER != json_getType(property))
    {
        printf("Error, the %s property is not found.\n", name);
    }
    return (int)json_getInteger(property);
}

static float json_property_to_float(const json_t *json, const char *name)
{
    json_t const *property = json_getProperty(json, name);
    if (!property || JSON_REAL != json_getType(property))
    {
        printf("Error, the %s property is not found.", name);
    }
    return (float)json_getReal(property);
}

static float *json_property_to_float_array(const json_t *json, const char *name, int *size)
{
    const json_t *properties = json_getProperty(json, name);
    int index = 0;
    float *array = NULL;
    for (const json_t *property = json_getChild(properties); property != 0; property = json_getSibling(property))
    {
        array = realloc(array, sizeof(float) * (index + 1));
        array[index] = json_getReal(property);
        index++;
    }
    if (size != NULL)
    {
        *size = index;
    }
    return array;
}

static char **json_property_to_string_array(const json_t *json, const char *name, int *size)
{
    const json_t *properties = json_getProperty(json, name);
    int index = 0;
    char **array = NULL;
    for (const json_t *property = json_getChild(properties); property != 0; property = json_getSibling(property))
    {
        array = realloc(array, sizeof(float) * (index + 1));
        const char *value = json_getValue(property);
        array[index] = (char *)malloc(sizeof(char) * (strlen(value) + 1));
        strcpy(array[index], value);
        index++;
    }
    if (size != NULL)
    {
        *size = index;
    }
    return array;
}

static int json_property_to_object_json_array(FILE *json, char *objectName, char ***objectArrayPtr)
{
    // Get current file position
    int startPosition = ftell(json);

    char **objectArray = NULL;

    // Determine size of pattern string and create it
    int size = strlen(objectName) + 6; // "":{\0, 6 chars plus interior
    char *pattern = (char *)malloc(sizeof(char) * size);
    sprintf(pattern, "\"%s\":", objectName);
    /// Find first index of pattern string in json
    int strStart = increment_file_to_string(json, pattern);
    free(pattern);

    // Find end of object string
    bool squareBegin = false;
    bool squigleBegin = false;
    int squareCount = 0;
    int squigleCount = 0;
    int lastPosition = 0;
    int objectCount = 0;
    int c;
    printf("\n\n");
    while ((c = fgetc(json)) != EOF)
    {
        printf("%c", (char)c);
        if (c == '[')
        {
            squareBegin = true;
            squareCount++;
        }
        else if (c == ']')
        {
            squareCount--;
        }
        if (squareCount == 0 && squareBegin)
        {
            break;
        }

        if (c == '{')
        {
            squigleBegin = true;
            squigleCount++;
            lastPosition = ftell(json);
        }
        else if (c == '}')
        {
            squigleCount--;
        }
        if (squigleCount == 0 && squigleBegin)
        {
            objectCount++;
            if (objectArray == NULL)
            {
                objectArray = (char **)malloc(sizeof(char *) * objectCount);
            }
            else
            {
                objectArray = (char **)realloc(objectArray, sizeof(char *) * objectCount);
            }
            int currentPosition = ftell(json);
            fseek(json, lastPosition, SEEK_SET);
            objectArray[objectCount - 1] = (char *)malloc(sizeof(char) * (currentPosition - lastPosition + 1));
            fread(objectArray[objectCount - 1], sizeof(char), currentPosition - lastPosition, json);
            fgetc(json);
        }
    }
    printf("\n\n");
    // Return file pointer to previous position
    fseek(json, startPosition, SEEK_SET);

    *objectArrayPtr = objectArray;

    return objectCount;
}

static char *json_property_to_object_json(FILE *json, char *objectName)
{
    // Get current file position
    int startPosition = ftell(json);

    // Determine size of pattern string and create it
    int size = strlen(objectName) + 6; // "":{\0, 6 chars plus interior
    char *pattern = (char *)malloc(sizeof(char) * size);
    sprintf(pattern, "\"%s\":", objectName);

    /// Find first index of pattern string in json
    int strStart = increment_file_to_string(json, pattern);
    free(pattern);

    // Find end of object string
    int count = 0;
    int strEnd = 0;
    bool begin = false;
    int c;
    while ((c = fgetc(json)) != EOF)
    {
        if (c == '{')
        {
            begin = true;
            count++;
        }
        else if (c == '}' && begin)
        {
            count--;
        }
        if (count == 0 && begin)
        {
            strEnd = ftell(json);
            break;
        }
    }

    if (strEnd == 0)
    {
        printf("Error: Object not found in json string\n");
        return NULL;
    }

    // Make copy of string to return
    char *objectString = (char *)malloc(sizeof(char) * (strEnd - strStart + 1));
    fseek(json, strStart, SEEK_SET);
    fread(objectString, sizeof(char), strEnd - strStart, json);

    // Return file pointer to previous position
    fseek(json, startPosition, SEEK_SET);

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
    configuration->positionEncoderStepsPerRev = 0.0;
    configuration->forceGauge = NULL;
    configuration->forceGaugeScaleFactor = 0.0;
    configuration->forceGaugeZeroFactor = 0;
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

/**Json to structure functions**/

/**
 * @brief Constructs a MachineConfiguration structure from a JSON string.
 *
 * @param json A JSON string containing a machine configuration.
 */

static void json_to_machine_configuration(const json_t *json, MachineConfiguration *configuration)
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
    configuration->positionEncoderStepsPerRev = json_property_to_float(json, "Position Encoder Scale Factor");
    configuration->forceGauge = json_property_to_string(json, "Force Gauge");
    configuration->forceGaugeScaleFactor = json_property_to_float(json, "Force Gauge Scale Factor");
    configuration->forceGaugeZeroFactor = json_property_to_int(json, "Force Gauge Zero Factor");
}

/**
 * @brief Constructs a MachinePerformance structure from a JSON string.
 *
 * @param json A JSON string containing a machine performance.
 */

static void json_to_machine_performance(const json_t *json, MachinePerformance *performance)
{
    performance->minPosition = json_property_to_float(json, "Position Minimum");
    performance->maxPosition = json_property_to_float(json, "Position Maximum");
    performance->maxVelocity = json_property_to_float(json, "Velocity Maximum");
    performance->maxAcceleration = json_property_to_float(json, "Acceleration Maximum");
    performance->maxForceTensile = json_property_to_float(json, "Force Tensile Maximum");
    performance->maxForceCompression = json_property_to_float(json, "Force Compression Maximum");
    performance->forceGaugeNeutralOffset = json_property_to_float(json, "Force gauge Neutral Offset");
}

/**Structure to JSON Functions**/

static char *machine_configuration_to_json(MachineConfiguration *configuration)
{
    char *json;
    int size = 24; //"Configuration":{}\0, 19 chars plus interior
    json = (char *)malloc(sizeof(char) * size);
    strcpy(json, "\"Configuration\":{");

    char *motorTypeJSON = string_to_json("Motor Type", configuration->motorType);
    size += strlen(motorTypeJSON) + 2;
    json = (char *)realloc(json, sizeof(char) * size);
    strcat(json, motorTypeJSON);
    free(motorTypeJSON);
    strcat(json, ",");

    char *maxMotorRPMJSON = float_to_json("Max Motor RPM", configuration->maxMotorRPM);
    size += strlen(maxMotorRPMJSON) + 2;
    json = (char *)realloc(json, sizeof(char) * size);
    strcat(json, maxMotorRPMJSON);
    free(maxMotorRPMJSON);
    strcat(json, ",");

    char *maxMotorTorqueJSON = float_to_json("Max Motor Torque", configuration->maxMotorTorque);
    size += strlen(maxMotorTorqueJSON) + 2;
    json = (char *)realloc(json, sizeof(char) * size);
    strcat(json, maxMotorTorqueJSON);
    free(maxMotorTorqueJSON);
    strcat(json, ",");

    char *gearDiameterJSON = float_to_json("Gear Diameter", configuration->gearDiameter);
    size += strlen(gearDiameterJSON) + 2;
    json = (char *)realloc(json, sizeof(char) * size);
    strcat(json, gearDiameterJSON);
    free(gearDiameterJSON);
    strcat(json, ",");

    char *gearPitchJSON = float_to_json("Gear Pitch", configuration->gearPitch);
    size += strlen(gearPitchJSON) + 2;
    json = (char *)realloc(json, sizeof(char) * size);
    strcat(json, gearPitchJSON);
    free(gearPitchJSON);
    strcat(json, ",");

    char *systemInertiaJSON = float_to_json("System Intertia", configuration->systemIntertia);
    size += strlen(systemInertiaJSON) + 2;
    json = (char *)realloc(json, sizeof(char) * size);
    strcat(json, systemInertiaJSON);
    free(systemInertiaJSON);
    strcat(json, ",");

    char *staticTorqueJSON = float_to_json("Static Torque", configuration->staticTorque);
    size += strlen(staticTorqueJSON) + 2;
    json = (char *)realloc(json, sizeof(char) * size);
    strcat(json, staticTorqueJSON);
    free(staticTorqueJSON);
    strcat(json, ",");

    char *loadJSON = float_to_json("Load", configuration->load);
    size += strlen(loadJSON) + 2;
    json = (char *)realloc(json, sizeof(char) * size);
    strcat(json, loadJSON);
    free(loadJSON);
    strcat(json, ",");

    char *encoderTypeJSON = string_to_json("Position Encoder Type", configuration->positionEncoderType);
    size += strlen(encoderTypeJSON) + 2;
    json = (char *)realloc(json, sizeof(char) * size);
    strcat(json, encoderTypeJSON);
    free(encoderTypeJSON);
    strcat(json, ",");

    char *encoderScaleFactorJSON = float_to_json("Position Encoder Scale Factor", configuration->positionEncoderStepsPerRev);
    size += strlen(encoderScaleFactorJSON) + 2;
    json = (char *)realloc(json, sizeof(char) * size);
    strcat(json, encoderScaleFactorJSON);
    free(encoderScaleFactorJSON);
    strcat(json, ",");

    char *forceGaugeJSON = string_to_json("Force Gauge", configuration->forceGauge);
    size += strlen(forceGaugeJSON) + 2;
    json = (char *)realloc(json, sizeof(char) * size);
    strcat(json, forceGaugeJSON);
    free(forceGaugeJSON);
    strcat(json, ",");

    char *forceGaugeScaleFactorJSON = float_to_json("Force Gauge Scale Factor", configuration->forceGaugeScaleFactor);
    printf("Force Scalle: %s\n", forceGaugeScaleFactorJSON);
    size += strlen(forceGaugeScaleFactorJSON) + 2;
    json = (char *)realloc(json, sizeof(char) * size);
    strcat(json, forceGaugeScaleFactorJSON);
    free(forceGaugeScaleFactorJSON);
    strcat(json, ",");

    char *forceGaugeZeroFactorJSON = int_to_json("Force Gauge Zero Factor", configuration->forceGaugeZeroFactor);
    printf("Force Scalle: %s\n", forceGaugeZeroFactorJSON);
    size += strlen(forceGaugeZeroFactorJSON) + 2;
    json = (char *)realloc(json, sizeof(char) * size);
    strcat(json, forceGaugeZeroFactorJSON);
    free(forceGaugeZeroFactorJSON);
    strcat(json, "}");

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
    // printf("Performance:%s\n", json);
    free(minPositionStr);
    free(maxPositionStr);
    free(maxVelocityStr);
    free(maxAccelerationStr);
    free(maxForceTensileStr);
    free(maxForceCompressionStr);
    free(forceGaugeNeutralOffsetStr);
    return json;
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
    test->sampleSN = -1;
    test->machineProfile = NULL;
    test->sampleProfile = NULL;
    test->motionProfile = 0;
    test->comment = NULL;
    return test;
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

// First parameter must be final distance
MotionQuartet *get_motion_quartet()
{
    MotionQuartet *quartet = (MotionQuartet *)malloc(sizeof(MotionQuartet));
    quartet->name = NULL;
    quartet->function = -1;
    quartet->parameters = NULL; // Distance, ...
    quartet->dwell = 0.0;
    return quartet;
}

Error machine_profile_to_json(MachineProfile *settings, char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return JSON_FILE_ERROR;
    }

    char *nameJSON = string_to_json("Name", settings->name);
    fprintf(file, "{%s,", nameJSON);
    free(nameJSON);

    char *numberJSON = int_to_json("Number", settings->number);
    fprintf(file, "%s,", numberJSON);
    free(numberJSON);

    char *configurationJSON = machine_configuration_to_json(settings->configuration);
    fprintf(file, "%s,", configurationJSON);
    free(configurationJSON);

    char *performanceJSON = machine_performance_to_json(settings->performance);
    fprintf(file, "%s}", performanceJSON);
    free(performanceJSON);
    fclose(file);
    return SUCCESS;
}

Error motion_profile_to_json(MotionProfile *motion, char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error opening file: %s\n", file);
        return JSON_FILE_ERROR;
    }

    char *nameJSON = string_to_json("Name", motion->name);
    fprintf(file, "{%s,", nameJSON);
    free(nameJSON);

    char *numberJSON = int_to_json("Number", motion->number);
    fprintf(file, "%s,", numberJSON);
    free(numberJSON);

    fprintf(file, "\"Motion Sets\":[");
    for (int i = 0; i < motion->setCount; i++)
    {
        // motion_set_to_json(&(motion->sets[i]), file);
        fprintf(file, "%s", motion->sets[i].name);
        if (i < motion->setCount - 1)
        {
            fprintf(file, ",");
        }
    }
    fprintf(file, "]}");
    fclose(file);
    return SUCCESS;
}

Error sample_profile_to_json(SampleProfile *sample, char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error opening file: %s\n", file);
        return JSON_FILE_ERROR;
    }

    char *nameJSON = string_to_json("Name", sample->name);
    fprintf(file, "{%s,", nameJSON);
    free(nameJSON);

    char *numberJSON = int_to_json("Number", sample->number);
    fprintf(file, "%s,", numberJSON);
    free(numberJSON);

    char *lengthJSON = float_to_json("Length", sample->length);
    fprintf(file, "%s,", lengthJSON);
    free(lengthJSON);

    char *stretchMaxJSON = float_to_json("Stretch Max", sample->stretchMax);
    fprintf(file, "%s,", stretchMaxJSON);
    free(stretchMaxJSON);

    char *maxVelocityJSON = float_to_json("Max Velocity", sample->maxVelocity);
    fprintf(file, "%s,", maxVelocityJSON);
    free(maxVelocityJSON);

    char *maxAccelerationJSON = float_to_json("Max Acceleration", sample->maxAcceleration);
    fprintf(file, "%s,", maxAccelerationJSON);
    free(maxAccelerationJSON);

    char *maxJerkJSON = float_to_json("Max Jerk", sample->maxJerk);
    fprintf(file, "%s,", maxJerkJSON);
    free(maxJerkJSON);

    char *maxForceTensileJSON = float_to_json("Max Force Tensile", sample->maxForceTensile);
    fprintf(file, "%s,", maxForceTensileJSON);
    free(maxForceTensileJSON);

    char *maxForceCompressionJSON = float_to_json("Max Force Compression", sample->maxForceCompression);
    fprintf(file, "%s}", maxForceCompressionJSON);
    free(maxForceCompressionJSON);
    fclose(file);
    return SUCCESS;
}

Error motion_set_to_json(MotionSet *set, char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error opening file: %s\n", file);
        return JSON_FILE_ERROR;
    }

    char *nameJSON = string_to_json("Name", set->name);
    fprintf(file, "{%s,", nameJSON);
    free(nameJSON);

    char *numberJSON = int_to_json("Number", set->number);
    fprintf(file, "%s,", numberJSON);
    free(numberJSON);

    char *typeJSON = string_to_json("Type", set->type);
    fprintf(file, "%s,", typeJSON);
    free(typeJSON);

    char *executionJSON = int_to_json("Executions", set->executions);
    fprintf(file, "%s,", executionJSON);
    free(executionJSON);

    // Create string_array_to_json
    fprintf(file, "\"Quartets\":[");
    for (int i = 0; i < set->quartetCount; i++)
    {
        fprintf(file, "\"%s\"", set->quartets[i].name);
        if (i < set->quartetCount - 1)
        {
            fprintf(file, ",");
        }
    }
    fprintf(file, "]}");
    fclose(file);
    return SUCCESS;
}

Error motion_quartet_to_json(MotionQuartet *quartet, char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error opening file: %s\n", file);
        return JSON_FILE_ERROR;
    }
    printf("file opened:%s\n", filename);
    char *nameJSON = string_to_json("Name", quartet->name);
    printf("%s\n", nameJSON);
    fprintf(file, "{%s,", nameJSON);
    free(nameJSON);

    char *functionJSON = int_to_json("Function", quartet->function);
    fprintf(file, "%s,", functionJSON);
    free(functionJSON);

    FunctionInfo *info = get_function_info(quartet->function);
    int max = info->args_count;
    free_function_info(info);
    fprintf(file, "\"Parameters\":[");
    for (int i = 0; i < max; i++)
    {
        fprintf(file, "\"%f\"", quartet->parameters[i]);
        if (i < max - 1)
        {
            fprintf(file, ",");
        }
    }
    fprintf(file, "],");
    char *dwellJSON = float_to_json("Dwell", quartet->dwell);
    fprintf(file, "%s}", dwellJSON);
    free(dwellJSON);
    fclose(file);
    return SUCCESS;
}

Error test_profile_to_json(TestProfile *test, char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error opening file: %s\n", file);
        return JSON_FILE_ERROR;
    }

    char *nameJSON = string_to_json("Name", test->name);
    fprintf(file, "{%s,", nameJSON);
    free(nameJSON);

    char *sampleSerialNumberJSON = int_to_json("Sample Serial Number", test->sampleSN);
    fprintf(file, "%s,", sampleSerialNumberJSON);
    free(sampleSerialNumberJSON);

    char *machineProfileNameJSON = string_to_json("Machine Profile Name", test->machineProfile->name);
    fprintf(file, "%s,", machineProfileNameJSON);
    free(machineProfileNameJSON);

    char *sampleProfileFileNameJSON = string_to_json("Sample Profile Name", test->sampleProfile->name);
    fprintf(file, "%s,", sampleProfileFileNameJSON);
    free(sampleProfileFileNameJSON);

    char *motionProfileFileNameJSON = string_to_json("Motion Profile Name", test->sampleProfile->name);
    fprintf(file, "%s,", motionProfileFileNameJSON);
    free(motionProfileFileNameJSON);

    char *commentJSON = string_to_json("Comment", test->comment);
    fprintf(file, "%s}", commentJSON);
    free(commentJSON);
    fclose(file);
    return SUCCESS;
}

static void json_print_machine_configuration(MachineConfiguration *configuration)
{
    printf("Machine Configuration:\n");
    printf("    Motor Type: %s\n", configuration->motorType);
    printf("    maxMotorTorque: %f\n", configuration->maxMotorTorque);
    printf("    maxMotorRPM: %f\n", configuration->maxMotorRPM);
    printf("    gearPitch: %f\n", configuration->gearPitch);
    printf("    systemIntertia: %f\n", configuration->systemIntertia);
    printf("    staticTorque: %f\n", configuration->staticTorque);
    printf("    load: %f\n", configuration->load);
    printf("    positionEncoderType: %s\n", configuration->positionEncoderType);
    printf("    positionEncoderScaleFactor: %f\n", configuration->positionEncoderStepsPerRev);
    printf("    forceGauge: %s\n", configuration->forceGauge);
    printf("    forceGaugeScaleFactor: %f\n", configuration->forceGaugeScaleFactor);
    printf("    forceGaugeZeroFactor: %d\n", configuration->forceGaugeZeroFactor);
}

static void json_print_machine_performance(MachinePerformance *performance)
{
    printf("Machine performance:\n");
    printf("    minPosition: %f\n", performance->minPosition);
    printf("    maxPosition: %f\n", performance->maxPosition);
    printf("    maxVelocity: %f\n", performance->maxVelocity);
    printf("    maxAcceleration: %f\n", performance->maxAcceleration);
    printf("    maxForceTensile: %f\n", performance->maxForceTensile);
    printf("    maxForceCompression: %f\n", performance->maxForceCompression);
    printf("    forceGaugeNeutralOffset: %f\n", performance->forceGaugeNeutralOffset);
}

void json_print_machine_profile(MachineProfile *profile)
{
    printf("Name:%s\n", profile->name);
    printf("Number:%d\n", profile->number);
    json_print_machine_configuration(profile->configuration);
    json_print_machine_performance(profile->performance);
}

/**
 * @brief Converts JSON string to a MachineProfile structure.
 *
 * @param json A JSON string containing a machine profile.
 * @return A MachineProfile structure containing the machine profile from JSON.
 */
MachineProfile *json_to_machine_profile(const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        printf("Error opening file\n");
        return NULL;
    }

    // Read file into string
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *profileStr = malloc(fileSize + 1);
    fread(profileStr, fileSize, 1, file);
    fclose(file);
    profileStr[fileSize] = '\0';
    printf("%s\n", profileStr);
    // Use tiny-json to parse the string
    json_t *mem = malloc(sizeof(json_t) * MAX_TOKENS);
    if (mem == NULL)
    {
        printf("Error allocating memory\n");
        return NULL;
    }

    json_t *parser = json_create(profileStr, mem, MAX_TOKENS);

    if (!parser || JSON_OBJ != json_getType(parser))
    {
        printf("Error, the  JSON cannot be parsed.\n");
        return NULL;
    }

    MachineProfile *settings = get_machine_profile();
    settings->name = json_property_to_string(parser, "Name");
    settings->number = json_property_to_int(parser, "Number");

    const json_t *mcParser = json_getProperty(parser, "Configuration");
    if (!mcParser || JSON_OBJ != json_getType(mcParser))
    {
        printf("Error, the  Machine Configuration  property is not found.\n");
    }
    json_to_machine_configuration(mcParser, settings->configuration);

    const json_t *mpParser = json_getProperty(parser, "Performance");
    if (!mpParser || JSON_OBJ != json_getType(mpParser))
    {
        printf("Error, the  Machine Profile  property is not found.\n");
    }
    json_to_machine_performance(mpParser, settings->performance);
    free(profileStr);
    return settings;
}

SampleProfile *json_to_sample_profile(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return NULL;
    }

    // Read file into string
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *testStr = malloc(fileSize + 1);
    fread(testStr, fileSize, 1, file);
    testStr[fileSize] = '\0';

    // Use tiny-json to parse the string
    json_t mem[TEST_PROFILE_FIELD_COUNT];
    const json_t *parser = json_create(testStr, mem, sizeof(mem) / sizeof(*mem));

    SampleProfile *sample = get_sample_profile();
    sample->name = json_property_to_string(parser, "Name");
    sample->number = json_property_to_int(parser, "Number");
    sample->length = json_property_to_float(parser, "Length");
    sample->stretchMax = json_property_to_float(parser, "Stretch Max");
    sample->maxVelocity = json_property_to_float(parser, "Max Velocity");
    sample->maxAcceleration = json_property_to_float(parser, "Max Acceleration");
    sample->maxJerk = json_property_to_float(parser, "Max Jerk");
    sample->maxForceTensile = json_property_to_float(parser, "Max Force Tensile");
    sample->maxForceCompression = json_property_to_float(parser, "Max Force Compression");
    return sample;
}

void json_to_test_profile(char *filename, TestProfile *test)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return NULL;
    }

    // Read file into string
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *testStr = malloc(fileSize + 1);
    fread(testStr, fileSize, 1, file);
    testStr[fileSize] = '\0';

    // Use tiny-json to parse the string
    json_t mem[TEST_PROFILE_FIELD_COUNT];
    const json_t *parser = json_create(testStr, mem, sizeof(mem) / sizeof(*mem));

    test->name = json_property_to_string(parser, "Name");
    test->sampleSN = json_property_to_int(parser, "Sample Serial Number");
    char *machineProfileFilename = json_property_to_string(parser, "Machine Profile Name");
    test->machineProfile = json_to_machine_profile(machineProfileFilename);
    free(machineProfileFilename);
    char *sampleProfileFileName = json_property_to_string(parser, "Sample Profile Name");
    test->sampleProfile = json_to_sample_profile(sampleProfileFileName);
    free(sampleProfileFileName);
    char *motionProfileFileName = json_property_to_string(parser, "Motion Profile Name");
    test->motionProfile = get_motion_profile();
    json_to_motion_profile(motionProfileFileName, test->motionProfile);
    free(motionProfileFileName);
    fclose(file);
}

void json_to_motion_profile(char *filename, MotionProfile *profile)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return NULL;
    }

    // Read file into string
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *setStr = malloc(fileSize + 1);
    fread(setStr, fileSize, 1, file);
    setStr[fileSize] = '\0';

    // Use tiny-json to parse the string
    json_t mem[MOTION_PROFILE_FIELD_COUNT];
    const json_t *parser = json_create(setStr, mem, sizeof(mem) / sizeof(*mem));

    profile->name = json_property_to_string(parser, "Name");
    profile->number = json_property_to_int(parser, "Number");
    char **setFiles = json_property_to_string_array(parser, "Sets", &(profile->setCount));
    profile->sets = (MotionSet *)malloc(sizeof(MotionSet) * profile->setCount);
    for (int i = 0; i < profile->setCount; i++)
    {
        json_to_motion_set(setFiles[i], &(profile->sets[i]));
        free(setFiles[i]);
    }
    free(setFiles);
    fclose(file);
}

void json_to_motion_set(char *filename, MotionSet *set)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return NULL;
    }

    // Read file into string
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *setStr = malloc(fileSize + 1);
    fread(setStr, fileSize, 1, file);
    setStr[fileSize] = '\0';

    // Use tiny-json to parse the string
    json_t mem[MOTION_SET_FIELD_COUNT];
    const json_t *parser = json_create(setStr, mem, sizeof(mem) / sizeof(*mem));

    set->name = json_property_to_string(parser, "Name");
    set->number = json_property_to_int(parser, "Number");
    set->type = json_property_to_string(parser, "Type");
    set->executions = json_property_to_int(parser, "Executions");
    char **quartetFiles = json_property_to_string_array(parser, "Quartets", &(set->quartetCount));
    set->quartets = (MotionQuartet *)malloc(sizeof(MotionQuartet) * set->quartetCount);
    for (int i = 0; i < set->quartetCount; i++)
    {
        json_to_motion_quartet(quartetFiles[i], &(set->quartets[i]));
        free(quartetFiles[i]);
    }
    free(quartetFiles);
    fclose(file);
}

void json_to_motion_quartet(char *filename, MotionQuartet *quartet)
{
    printf("Opening file\n");
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return NULL;
    }
    printf("file opened\n");
    // Read file into string
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *quartetStr = malloc(fileSize + 1);
    fread(quartetStr, fileSize, 1, file);
    quartetStr[fileSize] = '\0';

    // Use tiny-json to parse the string
    json_t mem[MOTION_QUARTET_FIELD_COUNT];
    const json_t *parser = json_create(quartetStr, mem, sizeof(mem) / sizeof(*mem));
    printf("parsing name\n");
    quartet->name = json_property_to_string(parser, "Name");
    printf("parsing Function\n");
    quartet->function = json_property_to_int(parser, "Function");
    printf("parsing Parameters\n");
    quartet->parameters = json_property_to_float_array(parser, "Parameters", NULL);
    printf("parsing Dwell\n");
    quartet->dwell = json_property_to_float(parser, "Dwell");
    printf("done\n");
    fclose(file);
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
        if (test->machineProfile != NULL)
        {
            free_machine_profile(test->machineProfile);
        }
        if (test->sampleProfile != NULL)
        {
            free_sample_profile(test->sampleProfile);
        }
        if (test->motionProfile != NULL)
        {
            free_motion_profile(test->motionProfile);
        }
        if (test->comment != NULL)
        {
            free(test->comment);
        }
    }
    free(test);
}

void free_machine_configuration(MachineConfiguration *configuration)
{
    free(configuration->motorType);
    free(configuration->positionEncoderType);
    free(configuration->forceGauge);
    free(configuration);
}
void free_machine_performance(MachinePerformance *performance)
{
    free(performance);
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
                free_motion_set(&(profile->sets[i]));
            }
            free(profile->sets);
        }
        free(profile);
    }
}

void free_motion_set(MotionSet *set)
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
                free_motion_quartet(&(set->quartets[i]));
            }
            free(set->quartets);
        }
        free(set);
    }
}

void free_motion_quartet(MotionQuartet *quartet)
{
    if (quartet != NULL)
    {
        if (quartet->name != NULL)
        {
            free(quartet->name);
        }

        if (quartet->parameters != NULL)
        {
            free(quartet->parameters);
        }
    }
    free(quartet);
}

// Helper Functions
