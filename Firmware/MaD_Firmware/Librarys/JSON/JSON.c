#include "JSON.h"
#include "MotionPlanning.h"

static char buffer[MAX_FILE_SIZE];
json_t mem[MAX_TOKENS];
static bool bufferSem = false;

// Private Function
static void double_to_json(FILE *file, const char *name, double value)
{
    fprintf(file, "\"%s\":%f", name, value);
}

static void int_to_json(FILE *file, const char *name, int value)
{
    fprintf(file, "\"%s\":%d", name, value);
}

static void string_to_json(FILE *file, const char *name, const char *value)
{
    fprintf(file, "\"%s\":\"%s\"", name, value);
}

static const char *json_property_to_string(const json_t *parser, const char *name)
{
    json_t const *property = json_getProperty(parser, name);
    if (property == NULL)
    {
        printf("Error, the %s property is not found.\n", name);
    }
    return json_getValue(property);
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

static double json_property_to_double(const json_t *json, const char *name)
{
    json_t const *property = json_getProperty(json, name);
    if (!property || JSON_REAL != json_getType(property))
    {
        printf("Error, the %s property is not found.", name);
    }
    return json_getReal(property);
}

static int json_property_to_double_array(double *array, const json_t *json, const char *name)
{
    const json_t *properties = json_getProperty(json, name);
    int index = 0;
    for (const json_t *property = json_getChild(properties); property != 0; property = json_getSibling(property))
    {
        array[index] = json_getReal(property);
        printf("param:%f \n", array[index]);
        index++;
    }

    return index;
}

/**Json to structure functions**/

/**
 * @brief Constructs a MachineConfiguration structure from a JSON string.
 *
 * @param json A JSON string containing a machine configuration.
 */

static void json_to_machine_configuration(const json_t *json, MachineConfiguration *configuration)
{
    strncpy(configuration->motorType, json_property_to_string(json, "Motor Type"), MAX_CONFIGURATION_MOTOR_TYPE);
    configuration->maxMotorRPM = json_property_to_double(json, "Max Motor RPM");
    configuration->maxMotorTorque = json_property_to_double(json, "Max Motor Torque");
    configuration->gearDiameter = json_property_to_double(json, "Gear Diameter");
    configuration->gearPitch = json_property_to_double(json, "Gear Pitch");
    configuration->systemIntertia = json_property_to_double(json, "System Intertia");
    configuration->staticTorque = json_property_to_double(json, "Static Torque");
    configuration->load = json_property_to_double(json, "Load");
    strncpy(configuration->positionEncoderType, json_property_to_string(json, "Position Encoder Type"), MAX_CONFIGURATION_ENCODER_TYPE);
    configuration->positionEncoderStepsPerRev = json_property_to_double(json, "Position Encoder Scale Factor");
    strncpy(configuration->forceGauge, json_property_to_string(json, "Force Gauge"), MAX_CONFIGURATION_FORCE_GAUGE);
    configuration->forceGaugeScaleFactor = json_property_to_double(json, "Force Gauge Scale Factor");
    configuration->forceGaugeZeroFactor = json_property_to_int(json, "Force Gauge Zero Factor");
}

/**
 * @brief Constructs a MachinePerformance structure from a JSON string.
 *
 * @param json A JSON string containing a machine performance.
 */

static void json_to_machine_performance(const json_t *json, MachinePerformance *performance)
{
    performance->minPosition = json_property_to_double(json, "Position Minimum");
    performance->maxPosition = json_property_to_double(json, "Position Maximum");
    performance->maxVelocity = json_property_to_double(json, "Velocity Maximum");
    performance->maxAcceleration = json_property_to_double(json, "Acceleration Maximum");
    performance->maxForceTensile = json_property_to_double(json, "Force Tensile Maximum");
    performance->maxForceCompression = json_property_to_double(json, "Force Compression Maximum");
    performance->forceGaugeNeutralOffset = json_property_to_double(json, "Force gauge Neutral Offset");
}

/**Structure to JSON Functions**/

static void machine_configuration_to_json(FILE *file, MachineConfiguration *configuration)
{
    fprintf(file, "\"Configuration\":{");

    string_to_json(file, "Motor Type", configuration->motorType);
    fprintf(file, ",");

    double_to_json(file, "Max Motor RPM", configuration->maxMotorRPM);
    fprintf(file, ",");

    double_to_json(file, "Max Motor Torque", configuration->maxMotorTorque);
    fprintf(file, ",");

    double_to_json(file, "Gear Diameter", configuration->gearDiameter);
    fprintf(file, ",");

    double_to_json(file, "Gear Pitch", configuration->gearPitch);
    fprintf(file, ",");

    double_to_json(file, "System Intertia", configuration->systemIntertia);
    fprintf(file, ",");

    double_to_json(file, "Static Torque", configuration->staticTorque);
    fprintf(file, ",");

    double_to_json(file, "Load", configuration->load);
    fprintf(file, ",");

    string_to_json(file, "Position Encoder Type", configuration->positionEncoderType);
    fprintf(file, ",");

    double_to_json(file, "Position Encoder Scale Factor", configuration->positionEncoderStepsPerRev);
    fprintf(file, ",");

    string_to_json(file, "Force Gauge", configuration->forceGauge);
    fprintf(file, ",");

    double_to_json(file, "Force Gauge Scale Factor", configuration->forceGaugeScaleFactor);
    fprintf(file, ",");

    int_to_json(file, "Force Gauge Zero Factor", configuration->forceGaugeZeroFactor);
    fprintf(file, "}");
}

static void machine_performance_to_json(FILE *file, MachinePerformance *performance)
{
    fprintf(file, "\"Performance\":{");

    double_to_json(file, "Position Minimum", performance->minPosition);
    fprintf(file, ",");

    double_to_json(file, "Position Maximum", performance->maxPosition);
    fprintf(file, ",");

    double_to_json(file, "Velocity Maximum", performance->maxVelocity);
    fprintf(file, ",");

    double_to_json(file, "Acceleration Maximum", performance->maxAcceleration);
    fprintf(file, ",");

    double_to_json(file, "Force Tensile Maximum", performance->maxForceTensile);
    fprintf(file, ",");

    double_to_json(file, "Force Compression Maximum", performance->maxForceCompression);
    fprintf(file, ",");

    double_to_json(file, "Force gauge Neutral Offset", performance->forceGaugeNeutralOffset);
    fprintf(file, "}");
}

/*Public Functions*/

/**Initialation Functions**/

void machine_configuration_init(MachineConfiguration *configuration)
{
    strcpy(configuration->motorType, "");
    configuration->maxMotorRPM = 0;
    configuration->maxMotorTorque = 0;
    configuration->gearDiameter = 0;
    configuration->gearPitch = 0;
    configuration->systemIntertia = 0;
    configuration->staticTorque = 0;
    configuration->load = 0;
    strcpy(configuration->positionEncoderType, "");
    configuration->positionEncoderStepsPerRev = 0;
    strcpy(configuration->forceGauge, "");
    configuration->forceGaugeScaleFactor = 0;
    configuration->forceGaugeZeroFactor = 0;
}

void machine_performance_init(MachinePerformance *performance)
{
    performance->minPosition = 0;
    performance->maxPosition = 0;
    performance->maxVelocity = 0;
    performance->maxAcceleration = 0;
    performance->maxForceTensile = 0;
    performance->maxForceCompression = 0;
    performance->forceGaugeNeutralOffset = 0;
}

void machine_profile_init(MachineProfile *profile)
{
    strcpy(profile->name, "");
    profile->number = 0;
    machine_configuration_init(&(profile->configuration));
    machine_performance_init(&(profile->performance));
}

void motion_quartet_init(MotionQuartet *quartet)
{
    strcpy(quartet->name, "");
    quartet->function = 0;
    for (int i = 0; i < MAX_MOTION_QUARTET_PARAMETERS; i++)
    {
        quartet->parameters[i] = 0;
    }
    quartet->dwell = 0;
}

void motion_set_init(MotionSet *set)
{
    strcpy(set->name, "");
    set->number = 0;
    strcpy(set->type, "");
    set->executions = 0;
    set->quartetCount = 0;
    for (int i = 0; i < MAX_MOTION_QUARTETS; i++)
    {
        motion_quartet_init(&(set->quartets[i]));
    }
}

void motion_profile_init(MotionProfile *profile)
{
    strcpy(profile->name, "");
    profile->number = 0;
    profile->setCount = 0;
    for (int i = 0; i < MAX_MOTION_PROFILE_SETS; i++)
    {
        motion_set_init(&(profile->sets[i]));
    }
}

void sample_profile_init(SampleProfile *sample)
{
    strcpy(sample->name, "");
    sample->number = 0;
    sample->length = 0.0;
    sample->stretchMax = 0.0;
    sample->maxVelocity = 0.0;
    sample->maxAcceleration = 0.0;
    sample->maxJerk = 0.0;
    sample->maxForceTensile = 0.0;
    sample->maxForceCompression = 0.0;
}

void test_profile_init(TestProfile *profile)
{
    strcpy(profile->name, "");
    profile->sampleSN = 0;
    machine_profile_init(&(profile->machineProfile));
    motion_profile_init(&(profile->motionProfile));
    sample_profile_init(&(profile->sampleProfile));
    strcpy(profile->comment, "");
}

Error machine_profile_to_json(MachineProfile *settings, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error opening file:%s\n", filename);
        return JSON_FILE_ERROR;
    }
    fprintf(file, "{");
    string_to_json(file, "Name", settings->name);
    printf("writing name: %s\n", settings->name);
    fprintf(file, ",");

    int_to_json(file, "Number", settings->number);
    fprintf(file, ",");

    machine_configuration_to_json(file, &(settings->configuration));
    fprintf(file, ",");

    machine_performance_to_json(file, &(settings->performance));
    fprintf(file, "}");
    fclose(file);
    return SUCCESS;
}

Error motion_profile_to_json(MotionProfile *motion, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error opening file: %s\n", filename);
        return JSON_FILE_ERROR;
    }

    fprintf(file, "{");

    string_to_json(file, "Name", motion->name);
    fprintf(file, ",");

    int_to_json(file, "Number", motion->number);
    fprintf(file, ",");

    fprintf(file, "\"Motion Sets\":[");
    for (int i = 0; i < motion->setCount; i++)
    {
        printf("writing motion set: %s\n", motion->sets[i].name);
        fprintf(file, "\"%s\"", motion->sets[i].name);
        if (i < motion->setCount - 1)
        {
            fprintf(file, ",");
        }
    }
    fprintf(file, "]}");
    fclose(file);
    return SUCCESS;
}

Error sample_profile_to_json(SampleProfile *sample, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error opening file: %s\n", filename);
        return JSON_FILE_ERROR;
    }

    fprintf(file, "{");

    string_to_json(file, "Name", sample->name);
    fprintf(file, ",");

    int_to_json(file, "Number", sample->number);
    fprintf(file, ",");

    double_to_json(file, "Length", sample->length);
    fprintf(file, ",");

    double_to_json(file, "Stretch Max", sample->stretchMax);
    fprintf(file, ",");

    double_to_json(file, "Max Velocity", sample->maxVelocity);
    fprintf(file, ",");

    double_to_json(file, "Max Acceleration", sample->maxAcceleration);
    fprintf(file, ",");

    double_to_json(file, "Max Jerk", sample->maxJerk);
    fprintf(file, ",");

    double_to_json(file, "Max Force Tensile", sample->maxForceTensile);
    fprintf(file, ",");

    double_to_json(file, "Max Force Compression", sample->maxForceCompression);
    fprintf(file, "}");

    fclose(file);
    return SUCCESS;
}

Error motion_set_to_json(MotionSet *set, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error opening file: %s\n", filename);
        return JSON_FILE_ERROR;
    }

    fprintf(file, "{");

    string_to_json(file, "Name", set->name);
    fprintf(file, ",");

    int_to_json(file, "Number", set->number);
    fprintf(file, ",");

    string_to_json(file, "Type", set->type);
    fprintf(file, ",");

    int_to_json(file, "Executions", set->executions);
    fprintf(file, ",");

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

Error motion_quartet_to_json(MotionQuartet *quartet, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error opening file: %s\n", filename);
        return JSON_FILE_ERROR;
    }
    fprintf(file, "{");
    string_to_json(file, "Name", quartet->name);
    fprintf(file, ",");

    int_to_json(file, "Function", quartet->function);
    fprintf(file, ",");

    FunctionInfo info;
    get_function_info(&info, quartet->function);
    fprintf(file, "\"Parameters\":[");
    for (int i = 0; i < info.args_count; i++)
    {
        fprintf(file, "%f", (quartet->parameters)[i]);
        if (i < info.args_count - 1)
        {
            fprintf(file, ",");
        }
    }
    fprintf(file, "],");
    double_to_json(file, "Dwell", quartet->dwell);
    fprintf(file, "}");
    fclose(file);
    return SUCCESS;
}

Error test_profile_to_json(TestProfile *test, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error opening file: %s\n", filename);
        return JSON_FILE_ERROR;
    }

    fprintf(file, "{");
    string_to_json(file, "Name", test->name);

    int_to_json(file, "Sample Serial Number", test->sampleSN);
    fprintf(file, ",");

    string_to_json(file, "Machine Profile Name", test->machineProfile.name);
    fprintf(file, ",");

    string_to_json(file, "Sample Profile Name", test->sampleProfile.name);
    fprintf(file, ",");

    string_to_json(file, "Motion Profile Name", test->sampleProfile.name);
    fprintf(file, ",");

    string_to_json(file, "Comment", test->comment);
    fprintf(file, "}");
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
    json_print_machine_configuration(&(profile->configuration));
    json_print_machine_performance(&(profile->performance));
}

void json_print_quartet(MotionQuartet *quartet)
{
    printf("        Name:%s\n", quartet->name);
    printf("        Function:%d\n", quartet->function);
    printf("        Dwell:%f\n", quartet->dwell);
    FunctionInfo info;
    get_function_info(&info, quartet->function);
    printf("        Parameters:[");
    for (int i = 0; i < info.args_count; i++)
    {
        printf("%f", quartet->parameters[i]);
        if (i < info.args_count - 1)
        {
            printf(",");
        }
    }
    printf("\n");
}

void json_print_motion_set(MotionSet *set)
{
    printf("    Name:%s\n", set->name);
    printf("    Number:%d\n", set->number);
    printf("    Type:%s\n", set->type);
    printf("    Executions:%d\n", set->executions);
    printf("    Quartest:\n");
    for (int i = 0; i < set->quartetCount; i++)
    {
        json_print_quartet(&(set->quartets[i]));
    }
}

void json_print_motion_profile(MotionProfile *profile)
{
    printf("Name:%s\n", profile->name);
    printf("Number:%d\n", profile->number);
    printf("Sets:\n");
    for (int i = 0; i < profile->setCount; i++)
    {
        json_print_motion_set(&(profile->sets[i]));
    }
}

static void read_file_to_buf(const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        printf("Error opening file:%s\n", filename);
        return;
    }

    // Read file into string
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    if (fileSize > MAX_FILE_SIZE)
    {
        fclose(file);
        printf("File too large:%s\n", filename);
        return;
    }

    fread(buffer, fileSize, 1, file);
    fclose(file);

    buffer[fileSize] = '\0';
    printf("%s\n", buffer);
}

/**
 * @brief Converts JSON string to a MachineProfile structure.
 *
 * @param json A JSON string containing a machine profile.
 * @return A MachineProfile structure containing the machine profile from JSON.
 */
void json_to_machine_profile(MachineProfile *profile, const char *filename)
{
    read_file_to_buf(filename);

    const json_t *parser = json_create(buffer, mem, MAX_TOKENS);

    if (!parser || JSON_OBJ != json_getType(parser))
    {
        printf("Error, the  JSON cannot be parsed.\n");
        return;
    }

    strncpy(profile->name, json_property_to_string(parser, "Name"), MAX_MACHINE_PROFILE_NAME);
    profile->number = json_property_to_int(parser, "Number");

    const json_t *mcParser = json_getProperty(parser, "Configuration");
    if (!mcParser || JSON_OBJ != json_getType(mcParser))
    {
        printf("Error, the  Machine Configuration  property is not found.\n");
    }
    json_to_machine_configuration(mcParser, &(profile->configuration));

    const json_t *mpParser = json_getProperty(parser, "Performance");
    if (!mpParser || JSON_OBJ != json_getType(mpParser))
    {
        printf("Error, the  Machine Profile  property is not found.\n");
    }
    json_to_machine_performance(mpParser, &(profile->performance));
    bufferSem = false; // Unlock buffer
}

void json_to_sample_profile(SampleProfile *sample, const char *filename)
{
    read_file_to_buf(filename);

    // Use tiny-json to parse the string
    const json_t *parser = json_create(buffer, mem, MAX_TOKENS);

    strncpy(sample->name, json_property_to_string(parser, "Name"), MAX_SAMPLE_PROFILE_NAME);
    sample->number = json_property_to_int(parser, "Number");
    sample->length = json_property_to_double(parser, "Length");
    sample->stretchMax = json_property_to_double(parser, "Stretch Max");
    sample->maxVelocity = json_property_to_double(parser, "Max Velocity");
    sample->maxAcceleration = json_property_to_double(parser, "Max Acceleration");
    sample->maxJerk = json_property_to_double(parser, "Max Jerk");
    sample->maxForceTensile = json_property_to_double(parser, "Max Force Tensile");
    sample->maxForceCompression = json_property_to_double(parser, "Max Force Compression");

    bufferSem = false; // Unlock buffer
}

/*void json_to_test_profile(const char *filename, TestProfile *test)
{
    printf("before open\n");
    FILE *file = fopen(filename, "r");
    printf("after open\n");
    if (file == NULL)
    {
        printf("Error opening file:%s\n", filename);
        return;
    }

    // Read file into string
    bufferSem = true; // Lock buffer
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    printf("before read\n");
    fread(buffer, fileSize, 1, file);
    fclose(file);
    buffer[fileSize] = '\0';
    printf("%s\n", buffer);

    // Use tiny-json to parse the string
    const json_t *parser = json_create(buffer, mem, MAX_TOKENS);

    strncpy(test->name, json_property_to_string(parser, "Name"), MAX_TEST_PROFILE_NAME);
    test->sampleSN = json_property_to_int(parser, "Sample Serial Number");

    const char *machineProfileFilename = json_property_to_string(parser, "Machine Profile Name");
    json_to_machine_profile(test->machineProfile, machineProfileFilename);

    const char *sampleProfileFileName = json_property_to_string(parser, "Sample Profile Name");
    json_to_sample_profile(test->sampleProfile, sampleProfileFileName);

    const char *motionProfileFileName = json_property_to_string(parser, "Motion Profile Name");
    json_to_motion_profile(motionProfileFileName, test->motionProfile);

    bufferSem = false; // Unlock buffer
}*/

void json_to_motion_profile(const char *filename, MotionProfile *profile)
{
    read_file_to_buf(filename);

    // Use tiny-json to parse the string
    const json_t *parser = json_create(buffer, mem, MAX_TOKENS);

    strncpy(profile->name, json_property_to_string(parser, "Name"), MAX_MOTION_PROFILE_NAME);
    profile->number = json_property_to_int(parser, "Number");

    const json_t *properties = json_getProperty(parser, "Motion Sets");

    int index = 0;
    char filenames[MAX_MOTION_PROFILE_SETS][MAX_MOTION_PROFILE_SET_NAME];
    for (const json_t *property = json_getChild(properties); property != 0; property = json_getSibling(property))
    {
        strncpy(filenames[index], json_getValue(property), MAX_MOTION_PROFILE_SET_NAME);
        index++;
    }
    profile->setCount = index;

    for (int i = 0; i < profile->setCount; i++)
    {
        printf("reading motion profile: %s\n", filenames[i]);
        json_to_motion_set(filenames[i], &(profile->sets[i]));
    }

    bufferSem = false; // Unlock buffer
}

void json_to_motion_set(const char *filename, MotionSet *set)
{
    read_file_to_buf(filename);

    // Use tiny-json to parse the string
    const json_t *parser = json_create(buffer, mem, MAX_TOKENS);

    strncpy(set->name, json_property_to_string(parser, "Name"), MAX_MOTION_PROFILE_SET_NAME);
    set->number = json_property_to_int(parser, "Number");
    strncpy(set->type, json_property_to_string(parser, "Type"), MAX_MOTION_PROFILE_SET_TYPE);
    set->executions = json_property_to_int(parser, "Executions");

    char filenames[MAX_MOTION_QUARTETS][MAX_MOTION_QUARTET_NAME];
    const json_t *properties = json_getProperty(parser, "Quartets");

    int index = 0;
    for (const json_t *property = json_getChild(properties); property != 0; property = json_getSibling(property))
    {
        strncpy(filenames[index], json_getValue(property), MAX_MOTION_QUARTET_NAME);
        index++;
    }
    set->quartetCount = index;

    for (int i = 0; i < set->quartetCount; i++)
    {
        printf("reading motion set: %s\n", filenames[i]);
        json_to_motion_quartet(filenames[i], &(set->quartets[i]));
    }

    bufferSem = false; // Unlock buffer
}

void json_to_motion_quartet(const char *filename, MotionQuartet *quartet)
{
    read_file_to_buf(filename);

    // Use tiny-json to parse the string
    const json_t *parser = json_create(buffer, mem, MAX_TOKENS);
    printf("parsing name\n");
    strncpy(quartet->name, json_property_to_string(parser, "Name"), MAX_MOTION_PROFILE_SET_NAME);
    printf("parsing Function\n");
    quartet->function = json_property_to_int(parser, "Function");
    printf("parsing Parameters\n");
    json_property_to_double_array(quartet->parameters, parser, "Parameters");
    printf("paramouter:%f \n", quartet->parameters[0]);
    printf("parsing Dwell\n");
    quartet->dwell = json_property_to_double(parser, "Dwell");
    printf("done\n");

    bufferSem = false; // Unlock buffer
}
