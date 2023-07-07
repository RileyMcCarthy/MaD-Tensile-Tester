#include "Utility/JsonDecoder.h"
#include "JSON.h"
#include "Utility/StateMachine.h"
#include "Utility/ControlSystem.h"
#include "Utility/Debug.h"

/** Private JSON to Structure **/

/**
 * @brief Constructs a MachineConfiguration structure from a JSON string.
 *
 * @param json A JSON string containing a machine configuration.
 */

static bool json_to_self_check_parameters(const json_t *json, SelfCheckParameters *parameters)
{
    bool success = true;
    success &= json_property_to_bool(json, "Charge Pump", &(parameters->chargePump));
    return success;
}

static bool json_to_machine_check_parameters(const json_t *json, MachineCheckParameters *parameters)
{
    bool success = true;
    success &= json_property_to_bool(json, "Switched Power", &(parameters->switchedPower));
    success &= json_property_to_int(json, "ESD Travel Limit",&(parameters->esdTravelLimit));
    success &= json_property_to_bool(json, "ESD Switch",&(parameters->esdSwitch));
    success &= json_property_to_bool(json, "Servo OK",&(parameters->servoOK));
    success &= json_property_to_bool(json, "Force Gauge Com",&(parameters->forceGaugeCom));
    success &= json_property_to_bool(json, "Servo Com",&(parameters->servoCom));
    return success;
}

static bool json_to_motion_parameters(const json_t *json, MotionParameters *parameters)
{
    bool success = true;
    success &= json_property_to_int(json, "Status", &(parameters->status));
    success &= json_property_to_int(json, "Condition", &(parameters->condition));
    success &= json_property_to_int(json, "Mode", &(parameters->mode));
    return success;
}

/**
 * @brief Constructs a MachineConfiguration structure from a JSON string.
 *
 * @param json A JSON string containing a machine configuration.
 */

static bool inner_json_to_machine_configuration(const json_t *json, MachineConfiguration *configuration)
{
    bool success = true;
    success &= json_property_to_string(json, "Motor Type", configuration->motorType, MAX_CONFIGURATION_MOTOR_TYPE);
    DEBUG_INFO("SUCCESS: %d\n", success);
    DEBUG_INFO("Motor Type: %s\n", configuration->motorType);
    success &= json_property_to_int(json, "Max Motor RPM", &(configuration->maxMotorRPM));
    DEBUG_INFO("SUCCESS: %d\n", success);   
    DEBUG_INFO("Max Motor RPM: %d\n", configuration->maxMotorRPM);
    success &= json_property_to_int(json, "Max Motor Torque", &(configuration->maxMotorTorque));
    success &= json_property_to_int(json, "Gear Diameter (mm)", &(configuration->gearDiameter));
    DEBUG_INFO("SUCCESS: %d\n", success);
    DEBUG_INFO("Diameter: %d\n", configuration->gearDiameter);
    success &= json_property_to_int(json, "Gear Pitch (mm)", &(configuration->gearPitch));
    DEBUG_INFO("SUCCESS: %d\n", success);
    DEBUG_INFO("Intertia: %d\n", configuration->gearPitch);
    success &= json_property_to_int(json, "System Inertia", &(configuration->systemIntertia));
    DEBUG_INFO("SUCCESS: %d\n", success);
    DEBUG_INFO("Intertia: %d\n", configuration->systemIntertia);
    success &= json_property_to_int(json, "Static Torque", &(configuration->staticTorque));
    success &= json_property_to_int(json, "Load", &(configuration->load));
    DEBUG_INFO("SUCCESS: %d\n", success);
    DEBUG_INFO("load: %d\n", configuration->load);
    success &= json_property_to_string(json, "Position Encoder Type",configuration->positionEncoderType, MAX_CONFIGURATION_ENCODER_TYPE);
    DEBUG_INFO("SUCCESS: %d\n", success);
    DEBUG_INFO("enc type: %s\n", configuration->positionEncoderType);
    success &= json_property_to_int(json, "Position Encoder (steps/um)", &(configuration->encoderStepsPerUM));
    success &= json_property_to_string(json, "Force Gauge Name", configuration->forceGauge, MAX_CONFIGURATION_FORCE_GAUGE);
    success &= json_property_to_int(json, "Force Gauge Gain", &(configuration->forceGaugeGain));
    success &= json_property_to_int(json, "Force Gauge Offset", &(configuration->forceGaugeOffset));
    return success;
}

/**
 * @brief Constructs a MachinePerformance structure from a JSON string.
 *
 * @param json A JSON string containing a machine performance.
 */

static bool inner_json_to_machine_performance(const json_t *json, MachinePerformance *performance)
{
    bool success = true;
    success &= json_property_to_int(json, "Position Minimum", &(performance->minPosition));
    success &= json_property_to_int(json, "Position Maximum", &(performance->maxPosition));
    success &= json_property_to_int(json, "Velocity Maximum", &(performance->maxVelocity));
    success &= json_property_to_int(json, "Acceleration Maximum", &(performance->maxAcceleration));
    success &= json_property_to_int(json, "Force Tensile Maximum", &(performance->maxForceTensile));
    success &= json_property_to_int(json, "Force Compression Maximum", &(performance->maxForceCompression));
    success &= json_property_to_int(json, "Force gauge Neutral Offset", &(performance->forceGaugeNeutralOffset));
    return success;
}

/**
 * @brief Converts JSON string to a MachineProfile structure.
 *
 * @param json A JSON string containing a machine profile.
 * @return A MachineProfile structure containing the machine profile from JSON.
 */
bool json_to_machine_profile(MachineProfile *profile, char *json)
{
    const json_t *parser = json_create_static(json);

    if (!parser || JSON_OBJ != json_getType(parser))
    {
        DEBUG_ERROR("%s","Error, the  JSON cannot be parsed.\n");
        return false;
    }

    if (!json_property_to_string(parser, "Name", profile->name, MAX_MACHINE_PROFILE_NAME))
    {
        DEBUG_ERROR("%s","Error, the  Machine Profile Name  property is not found.\n");
        return false;
    }

    DEBUG_INFO("Machine Profile Name: %s\n", profile->name);

    const json_t *mcParser = json_getProperty(parser, "Configuration");
    DEBUG_INFO("Machine Configuration: %p\n", mcParser);
    if (!mcParser || JSON_OBJ != json_getType(mcParser))
    {
        DEBUG_ERROR("%s","Error, the  Machine Configuration  property is not found.\n");
        return false;
    }

    if (!inner_json_to_machine_configuration(mcParser, &(profile->configuration)))
    {
        DEBUG_ERROR("%s","Error, the  Machine Configuration  property is not found.\n");
        return false;
    }

    DEBUG_INFO("Machine Configuration: %d\n", profile->configuration.forceGaugeGain);
    DEBUG_INFO("Machine Configuration: %d\n", profile->configuration.maxMotorRPM);

    const json_t *mpParser = json_getProperty(parser, "Performance");
    if (!mpParser || JSON_OBJ != json_getType(mpParser))
    {
        DEBUG_ERROR("%s","Machine Performance  property is not found.\n");
    }
    
    if (!inner_json_to_machine_performance(mpParser, &(profile->performance)))
    {
        DEBUG_ERROR("%s","Error, the  Machine Performance  property is not found.\n");
        return false;
    }

    return true;
}

bool json_to_machine_configuration(MachineConfiguration *configuration, char *json)
{
    // Use tiny-json to parse the string
    const json_t *parser = json_create_static(json);

    return inner_json_to_machine_configuration(parser, configuration);
}

bool json_to_machine_performance(MachinePerformance *performance, char *json)
{
    // Use tiny-json to parse the string
    const json_t *parser = json_create_static(json);

    return inner_json_to_machine_performance(parser, performance);
}

bool json_to_sample_profile(SampleProfile *sample, char *json)
{
    // Use tiny-json to parse the string
    const json_t *parser = json_create_static(json);

    bool success = true;
    success &= json_property_to_string(parser, "Name", sample->name, MAX_SAMPLE_PROFILE_NAME);
    success &= json_property_to_int(parser, "Length", &(sample->length));
    success &= json_property_to_int(parser, "Stretch Max", &(sample->stretchMax));
    success &= json_property_to_int(parser, "Max Velocity", &(sample->maxVelocity));
    success &= json_property_to_int(parser, "Max Acceleration", &(sample->maxAcceleration));
    success &= json_property_to_int(parser, "Max Jerk", &(sample->maxJerk));
    success &= json_property_to_int(parser, "Max Force Tensile", &(sample->maxForceTensile));
    success &= json_property_to_int(parser, "Max Force Compression", &(sample->maxForceCompression));
    return success;
}

/**
 * @brief Converts JSON string to a MachineProfile structure.
 *
 * @param json A JSON string containing a machine profile.
 * @return A MachineProfile structure containing the machine profile from JSON.
 */
bool json_to_test_data_request(TestDataRequest *request, char *json)
{
    // Use tiny-json to parse the string
    const json_t *parser = json_create_static(json);

    bool success = true;
    success &= json_property_to_int(parser, "Index", &(request->index));
    success &= json_property_to_int(parser, "Count", (int *)&(request->count));
    return success;
}

bool json_to_motion_mode(MotionMode *mode, char *json)
{
    // Use tiny-json to parse the string
    const json_t *parser = json_create_static(json);

    bool success = true;
    success &= json_property_to_int(parser, "Mode", mode);
    return success;
}

bool json_to_motion_status(MotionStatus *status, char *json)
{
    // Use tiny-json to parse the string
    const json_t *parser = json_create_static(json);

    bool success = true;
    success &= json_property_to_int(parser, "Status", status);
    return success;
}

// {G:0,X:254,F:1000}
bool json_to_move(Move *move, char *json)
{
    // Use tiny-json to parse the string
    const json_t *parser = json_create_static(json);

    bool success = true;
    success &= json_property_to_int(parser, "G", (int*)&(move->g));
    success &= json_property_to_int(parser, "X", (int*)&(move->x));
    success &= json_property_to_int(parser, "F", (int*)&(move->f));
    return success;
}
