#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdbool.h>
#include <stdint.h>
#include "JSON.h"
#include "Utility/Monitor.h"
#include "Utility/ControlSystem.h"
#include "CommunicationDefinition.h"

void beginCommunication(MachineProfile *machineProfile, MachineState *machineState, Monitor *monitor, ControlSystem *control);

#endif
