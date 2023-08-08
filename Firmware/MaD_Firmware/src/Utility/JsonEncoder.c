#include "Utility/JsonEncoder.h"
#include "JSON.h"

/** Private Structure to JSON **/
static int _json_encode_lock = -1;
static bool self_check_parameters_to_json(SelfCheckParameters *parameters)
{
    bool success = true;

    success &= header_to_json("Self Check Parameters");
    success &= open_json_block();

    success &= int_to_json("Charge Pump", parameters->chargePump);
    
    success &= delete_json_last_comma();
    success &= close_json_block();
    success &= json_append_comma();

    return success;
}

static bool machine_check_parameters_to_json(MachineCheckParameters *parameters)
{
    bool success = true;

    success &= header_to_json("Machine Check Parameters");
    success &= open_json_block();

    success &= string_to_json("ESD Chain", esd_chain_to_string(parameters->esdChain));
    success &= int_to_json("Servo OK", parameters->servoOK);
    success &= int_to_json("Force Gauge Com", parameters->forceGaugeCom);
    success &= int_to_json("Servo Com", parameters->servoCom);

    success &= delete_json_last_comma();
    success &= close_json_block();
    success &= json_append_comma();

    return success;
}

static bool motion_parameters_to_json(MotionParameters *parameters)
{
    bool success = true;

    success &= header_to_json("Motion Parameters");
    success &= open_json_block();

    success &= string_to_json("Status", motion_status_to_string(parameters->status));
    success &= string_to_json("Condition", motion_condition_to_string(parameters->condition));
    success &= string_to_json("Mode", motion_mode_to_string(parameters->mode));

    success &= delete_json_last_comma();
    success &= close_json_block();
    success &= json_append_comma();

    return success;
}

static bool machine_configuration_to_json(MachineConfiguration *configuration)
{
    bool success = true;

    success &= header_to_json("Configuration");
    success &= open_json_block();

    success &= string_to_json("Motor Type", configuration->motorType);
    success &= int_to_json("Max Motor RPM", configuration->maxMotorRPM);
    success &= int_to_json("Max Motor Torque", configuration->maxMotorTorque);
    success &= int_to_json("Gear Diameter (mm)", configuration->gearDiameter);
    success &= int_to_json("Gear Pitch (mm)", configuration->gearPitch);
    success &= int_to_json("System Inertia", configuration->systemIntertia);
    success &= int_to_json("Static Torque", configuration->staticTorque);
    success &= int_to_json("Load", configuration->load);
    success &= string_to_json("Position Encoder Type", configuration->positionEncoderType);
    success &= int_to_json("Position Encoder (steps/mm)", configuration->encoderStepsPermm);
    success &= int_to_json("Servo Step (steps/mm)", configuration->servoStepPermm);
    success &= string_to_json("Force Gauge Name", configuration->forceGauge);
    success &= int_to_json("Force Gauge Gain", configuration->forceGaugeGain);
    success &= int_to_json("Force Gauge Offset", configuration->forceGaugeOffset);

    success &= delete_json_last_comma();
    success &= close_json_block();
    success &= json_append_comma();

    return success;
}

static bool machine_performance_to_json(MachinePerformance *performance)
{
    bool success = true;

    success &= header_to_json("Performance");
    success &= open_json_block();

    success &= int_to_json("Position Minimum", performance->minPosition);
    success &= int_to_json("Position Maximum", performance->maxPosition);
    success &= int_to_json("Velocity Maximum", performance->maxVelocity);
    success &= int_to_json("Acceleration Maximum", performance->maxAcceleration);
    success &= int_to_json("Force Tensile Maximum", performance->maxForceTensile);
    success &= int_to_json("Force Compression Maximum", performance->maxForceCompression);
    success &= int_to_json("Force gauge Neutral Offset", performance->forceGaugeNeutralOffset);

    success &= delete_json_last_comma();
    success &= close_json_block();
    success &= json_append_comma();

    return success;
}


/** Public Structure to JSON **/

char * notification_to_json(Notification * notification)
{
    if (!lock_json_buffer())
    {
        return NULL; // Buffer is in use, do not attempt to write to it
    }

    // Initialize the JSON buffer
    clear_json_buffer();

    bool success = true;
    success &= open_json_block();

    success &= string_to_json("Type", notification->type);
    success &= string_to_json("Message", notification->message);

    success &= delete_json_last_comma();
    success &= close_json_block();

    if (success)
    {
        return get_json_buffer();
    }
    else
    {
        return NULL;
    }
}

char *machine_state_to_json(MachineState *state)
{
    if (!lock_json_buffer())
    {
        return NULL; // Buffer is in use, do not attempt to write to it
    }

    // Initialize the JSON buffer
    clear_json_buffer();

    bool success = true;
    success &= open_json_block();

    success &= string_to_json("State", machine_state_to_string(state->state));
    success &= self_check_parameters_to_json(&(state->selfCheckParameters));
    success &= machine_check_parameters_to_json(&(state->machineCheckParameters));
    success &= motion_parameters_to_json(&(state->motionParameters));

    success &= delete_json_last_comma();
    success &= close_json_block();

    if (success)
    {
        return get_json_buffer();
    }
    else
    {
        return NULL;
    }
}

char *machine_profile_to_json(MachineProfile *settings)
{
    if (!lock_json_buffer())
    {
        return NULL; // Buffer is in use, do not attempt to write to it
    }

    // Initialize the JSON buffer
    clear_json_buffer();

    bool success = true;

    success &= open_json_block();

    success &= string_to_json("Name", settings->name);
    success &= machine_configuration_to_json(&(settings->configuration));
    success &= machine_performance_to_json(&(settings->performance));

    success &= delete_json_last_comma();
    success &= close_json_block();

    if (success)
    {
        return get_json_buffer();
    }
    else
    {
        return NULL;
    }
}

char *sample_profile_to_json(SampleProfile *sample)
{
    if (!lock_json_buffer())
    {
        return NULL; // Buffer is in use, do not attempt to write to it
    }
    
    // Initialize the JSON buffer
    clear_json_buffer();

    bool success = true;

    success &= open_json_block();

    success &= string_to_json("Name", sample->name);
    success &= int_to_json("Length", sample->length);
    success &= int_to_json("Stretch Max", sample->stretchMax);
    success &= int_to_json("Max Velocity", sample->maxVelocity);
    success &= int_to_json("Max Acceleration", sample->maxAcceleration);
    success &= int_to_json("Max Jerk", sample->maxJerk);
    success &= int_to_json("Max Force Tensile", sample->maxForceTensile);
    success &= int_to_json("Max Force Compression", sample->maxForceCompression);

    success &= delete_json_last_comma();
    success &= close_json_block();

    if (success)
    {
        return get_json_buffer();
    }
    else
    {
        return NULL;
    }
}

char *test_data_to_json(MonitorData *data, int count, int index)
{
    if (!lock_json_buffer())
    {
        return NULL; // Buffer is in use, do not attempt to write to it
    }

    // Initialize the JSON buffer
    clear_json_buffer();

    bool success = true;

    success &= open_json_block();

    success &= header_to_json("Test_Data");

    success &= custom_string_to_json("[");

    for (int i = 0; i < count; i++)
    {
        // Add the index as a field
        success &= int_to_json("Index", index+i);

        // Add the "Time" field
        success &= int_to_json("Time", data[i].timeus);

        // Add the "Position" field
        success &= int_to_json("Position", data[i].encoderum);

        // Add the "Velocity" field
        success &= int_to_json("Force", data[i].forcemN);

        // Add the "Acceleration" field
        success &= int_to_json("Setpoint", data[i].setpoint);

        // Add the "Force" field
        success &= int_to_json("Log", data[i].log);

        // Remove the last comma
        success &= delete_json_last_comma();
        success &= custom_string_to_json("},");
    }

    success &= delete_json_last_comma();

    // Close the JSON string
    success &= custom_string_to_json("]}");

    if (success)
    {
        return get_json_buffer();
    }
    else
    {
        return NULL;
    }
}

bool unlock_json_buffer()
{
    _lockrel(_json_encode_lock);
}

bool lock_json_buffer()
{
    if (_json_encode_lock == -1)
    {
        _json_encode_lock = _locknew();
    }
    return _locktry(_json_encode_lock) != 0;
}
