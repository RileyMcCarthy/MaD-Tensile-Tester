#include <stdint.h>
#include <stdbool.h>

void set_motion_status(int statusms);
void set_control_status(int statusms);
void set_communication_status(int statusms);
void set_monitor_status(int statusms);
bool get_machine_status();
