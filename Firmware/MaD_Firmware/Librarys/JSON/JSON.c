#include "JSON.h"
#include "JSONHelper.h"

MachineSettings *getMachineSettings()
{
    MachineSettings *settings = (MachineSettings *)malloc(sizeof(MachineSettings));
    settings->name = NULL;
    settings->number = 0;
    settings->configuration = getMachineConfiguration();
    settings->performance = getMachinePerformance();
    return settings;
}

void freeMachineSettings(MachineSettings *settings)
{
    if (settings != NULL)
    {
        if (settings->name != NULL)
        {
            free(settings->name);
        }
        if (settings->configuration != NULL)
        {
            freeMachineConfiguration(settings->configuration);
        }
        if (settings->performance != NULL)
        {
            freeMachinePerformance(settings->performance);
        }
        free(settings);
    }
}

char *machineSettingsToJSON(MachineSettings *settings)
{
    char *json;
    int size = 5; //{}\0, 3 chars plus interior
    char *nameJSON = stringToJSON("Name", settings->name);
    size += strlen(nameJSON) + 2;
    char *numberJSON = intToJSON("Number", settings->number);
    size += strlen(numberJSON) + 2;
    char *configurationJSON = machineConfigurationToJSON(settings->configuration);
    size += strlen(configurationJSON) + 2;
    char *performanceJSON = machinePerformanceToJSON(settings->performance);
    size += strlen(performanceJSON);
    json = (char *)malloc(sizeof(char) * size);
    sprintf(json, "{%s,%s,%s,%s}", nameJSON, numberJSON, configurationJSON, performanceJSON);
    printf("Settings:%s\n", json);
    free(nameJSON);
    free(numberJSON);
    free(configurationJSON);
    free(performanceJSON);
    return json;
}
void JSONtoMachineSettings(MachineSettings *settings, char *jsonFileName)
{
}