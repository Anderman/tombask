#include <Arduino.h>
#include "rs485/sensors.h"
#include "globals.h"
#include "storage/storage.h"

char discoveryTopic[128];
char deviceIdStr[64];
char deviceJson[128];
char uniqueId[64];
char deviceClass[64];
char unitOfMeasurement[64];
char cmdTopic[128];
char stateTopic[128];

void SetDiscoveryTopic(const SensorField2 &field)
{
    const char *kind = field.isSwitch ? "switch" :field.hasCommand ? "number" : "sensor";
    snprintf(discoveryTopic, sizeof(discoveryTopic), "homeassistant/%s/%s/config", kind, uniqueId);
}

void SetProperties(const SensorField2 &field)
{
    snprintf(uniqueId, sizeof(uniqueId), "%s_%s", deviceIdStr, field.name);

    if (field.hasCommand)
        snprintf(cmdTopic, sizeof(cmdTopic), "\"command_topic\":\"tombask/%s/%s/set\",", deviceIdStr, field.name);
    else
        cmdTopic[0] = '\0';

    if (field.deviceClass && field.deviceClass[0] != '\0')
        snprintf(deviceClass, sizeof(deviceClass), "\"device_class\":\"%s\",", field.deviceClass);
    else
        deviceClass[0] = '\0';

    if (field.unit && field.unit[0] != '\0')
        snprintf(unitOfMeasurement, sizeof(unitOfMeasurement), "\"unit_of_measurement\":\"%s\",", field.unit);
    else
        unitOfMeasurement[0] = '\0';

    snprintf(stateTopic, sizeof(stateTopic), "\"state_topic\":\"tombask/%s/%s/state\",", deviceIdStr, field.name);
}

void ensureDeviceInfo()
{
    if (deviceIdStr[0] != '\0')
        return;
    snprintf(deviceIdStr, sizeof(deviceIdStr), "tombask%u", (unsigned)deviceId);
    snprintf(deviceJson, sizeof(deviceJson), "{\"identifiers\":[\"%s\"],\"name\":\"Heat pomp boiler\",\"model\":\"ESP32\",\"manufacturer\":\"1001\"}", deviceIdStr);
}