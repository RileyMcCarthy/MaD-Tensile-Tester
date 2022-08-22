#ifndef StateMachine_H
#define StateMachine_H
#include <stdbool.h>
#include "StateMachineDefinition.h"

MachineState *machine_state_create();

// External State Setters
void machine_state_init(MachineState *machineState);
void state_machine_set(MachineState *machineState, Parameter param, int state);
bool state_machine_self_check_equal(SelfCheckParameters *selfCheckParameters1, SelfCheckParameters *selfCheckParameters2);
bool state_machine_check_equal(MachineCheckParameters *motionParameters1, MachineCheckParameters *motionParameters2);
bool state_machine_motion_equal(MotionParameters *motionParameters1, MotionParameters *motionParameters2);
bool state_machine_equal(MachineState *machineState1, MachineState *machineState2);
#endif
