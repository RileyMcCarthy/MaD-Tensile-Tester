/** Decoding API **/
#include <stdbool.h>
#include "StateMachine.h"
#include "Motion.h"
#include "Memory/MachineProfile.h"

typedef struct TestDataRequest
{
    uint32_t index; // Index of the requested data
    uint8_t count; // Number of data points requested
}TestDataRequest;

bool json_to_machine_profile(MachineProfile *profile, const char *json);
bool json_to_machine_configuration(MachineConfiguration *configuration, char *json);
bool json_to_machine_performance(MachinePerformance *performance, char *json);
bool json_to_sample_profile(SampleProfile *sample, char *json);
bool json_to_test_data_request(TestDataRequest *request, char *json);
bool json_to_motion_mode(MotionMode *mode, char *json);
bool json_to_motion_status(MotionStatus *status, char *json);
bool json_to_move(Move *move, char *json);
