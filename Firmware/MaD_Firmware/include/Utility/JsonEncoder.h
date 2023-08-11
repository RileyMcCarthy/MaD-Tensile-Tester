/** Encoding API **/
#include "StateMachine.h"
#include "Memory/MachineProfile.h"
#include "Monitor.h"
#include "Main/Communication/Communication.h"

char * notification_to_json(Notification * notification);
char *machine_state_to_json(MachineState *state);
char *machine_profile_to_json(MachineProfile *settings);
char *sample_profile_to_json(SampleProfile *sample);
char *test_data_to_json(MonitorData *data);
bool unlock_json_buffer();
