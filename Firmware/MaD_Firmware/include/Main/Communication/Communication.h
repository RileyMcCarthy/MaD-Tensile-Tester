#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdbool.h>
#include <stdint.h>
#include "JSON.h"
#include "Monitor.h"
#include "ControlSystem.h"
#include "CommunicationDefinition.h"

void beginCommunication(MachineProfile *machineProfile, MachineState *machineState, Monitor *monitor, ControlSystem *control);

#endif