#include "settings.h"
#include <globals.h>
Settings settings = {
    .SetpointTemp = {53, "SetpointTemp", "Normal setpoint temperature for boiler operation", "°C", 40, 89, 1},
    .LegionellaTemp = {70, "LegionellaTemp", "Legionella prevention temperature", "°C", 40, 89, 1},
    .Fan = {0, "Fan", "Fan on/off", "bool", 0, 1, 1},
    // ...andere velden indien nodig
};
char deviceId[32] = "";
void publish()
{
    char device[128];
    snprintf(device, sizeof(device), "{\"identifiers\":[\"tombask_%s\"],\"name\":\"Heat pomp boiler\",\"model\":\"ESP32\",\"manufacturer\":\"tsi/1001\"}", deviceId);
}

void publishSwitchDiscovery(const SettingField &field)
{
    char topic[128];
    const char *name = field.name;
    char uniqueId[64];
    snprintf(uniqueId, sizeof(uniqueId), "tombask_%s", name);
    snprintf(topic, sizeof(topic), "homeassistant/switch/%s/config", uniqueId);

    char payload[512];
    snprintf(payload, sizeof(payload),
             "{"
             "\"name\":\"%s\","
             "\"unique_id\":\"%s\","
             "\"command_topic\":\"homeassistant/switch/%s/set\","
             "\"state_topic\":\"homeassistant/switch/%s/state\","
             "\"payload_on\":\"ON\","
             "\"payload_off\":\"OFF\","
             "\"device_class\":\"switch\","
             "\"device\":{\"identifiers\":[\"%s\"],\"name\":\"%s\",\"model\":\"ESP32\",\"manufacturer\":\"Jijzelf\"}"
             "}",
             name, uniqueId, uniqueId, uniqueId,
             name, name);

    mqttClient.publish(topic, payload, true);
}