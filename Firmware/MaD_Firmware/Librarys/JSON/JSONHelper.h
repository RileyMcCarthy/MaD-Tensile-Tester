#ifndef JSONHelper_H
#define JSONHelper_H

#include "simpletools.h"
#include "JSON.h"
#ifdef __cplusplus
extern "C"
{
#endif

    MachineConfiguration *getMachineConfiguration();
    MachinePerformance *getMachinePerformance();

    char *machinePerformanceToJSON(MachinePerformance *performance);
    char *machineConfigurationToJSON(MachineConfiguration *configuration);

    void freeMachineConfiguration(MachineConfiguration *configuration);
    void freeMachinePerformance(MachinePerformance *performance);

    char *intToJSON(char *name, int value);
    char *stringToJSON(char *name, char *value);

#ifdef __cplusplus
}
#endif
#endif