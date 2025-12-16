#include "settings.h"
#include <globals.h>
#include "storage/storage.h"
Settings settings = {
    .SetpointTemp = {"SetpointTemp", "Normal setpoint temperature for boiler operation", "°C", "number", 40, 89, 1},
    .LegionellaTemp = {"LegionellaTemp", "Legionella prevention temperature", "°C", "number", 40, 89, 1},
    .Fan = {"Fan", "Fan on/off", "bool", "switch", 0, 1, 1},
};


char deviceIdStr[64];
char deviceJson[128];
static char topic[128];
static char payload[512];
static char uniqueId[64];
static char cmdTopic[128];
static char stateTopic[128];

void ensureDeviceInfo();
static void setupDiscoveryStrings(const char *name, const char *type);
static void publishSwitchDiscovery(const SettingField &field);
static void publishNumberDiscovery(const SettingField &field);

void publishSettings()
{
    ensureDeviceInfo();
    publishSwitchDiscovery(settings.Fan);
    publishNumberDiscovery(settings.SetpointTemp);
    publishNumberDiscovery(settings.LegionellaTemp);
}

void ensureDeviceInfo()
{
    if (deviceIdStr[0] != '\0')
        return;
    snprintf(deviceIdStr, sizeof(deviceIdStr), "tombask%u", (unsigned)deviceId);
    snprintf(deviceJson, sizeof(deviceJson), "{\"identifiers\":[\"%s\"],\"name\":\"Heat pomp boiler\",\"model\":\"ESP32\",\"manufacturer\":\"1001\"}", deviceIdStr);
}


static void publishNumberDiscovery(const SettingField &field)
{
    setupDiscoveryStrings(field.name, field.type);

    snprintf(payload, sizeof(payload),
             "{"
             "\"name\":\"%s\","
             "\"unique_id\":\"%s\","
             "\"command_topic\":\"%s\","
             "\"state_topic\":\"%s\","
             "\"unit_of_measurement\":\"%s\","
             "\"device\":%s,"
             "\"min\":%u,"
             "\"max\":%u,"
             "\"step\":%u"
             "}",
             field.friendlyName, uniqueId, cmdTopic, stateTopic, field.unit, deviceJson,
             (unsigned)field.min, (unsigned)field.max, (unsigned)field.step);

    mqttClient.publish(topic, payload, true);
}

static void publishSwitchDiscovery(const SettingField &field)
{
    setupDiscoveryStrings(field.name, field.type);

    snprintf(payload, sizeof(payload),
             "{"
             "\"name\":\"%s\","
             "\"unique_id\":\"%s\","
             "\"command_topic\":\"%s\","
             "\"state_topic\":\"%s\","
             "\"device\":%s,"
             "\"payload_on\":\"ON\","
             "\"payload_off\":\"OFF\""
             "}",
             field.friendlyName, uniqueId, cmdTopic, stateTopic, deviceJson);

    mqttClient.publish(topic, payload, true);
}

static void setupDiscoveryStrings(const char *name, const char *type)
{
    snprintf(uniqueId, sizeof(uniqueId), "%s_%s", deviceIdStr, name);
    snprintf(topic, sizeof(topic), "homeassistant/%s/%s/config", type, uniqueId);
    snprintf(cmdTopic, sizeof(cmdTopic), "tombask/%s/%s/set", deviceIdStr, name);
    snprintf(stateTopic, sizeof(stateTopic), "tombask/%s/%s/state", deviceIdStr, name);
}
