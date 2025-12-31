#include "../rs485/sensors.h"
#include <Arduino.h>
#include <globals.h> // mqttClient
#include "../rs485/settings.h"
#include "mqtt_properties.h"

static char payload[1024];
static void publishClimateDiscovery();

void publishSensorDiscovery(SensorField2 field)
{
    ensureDeviceInfo();
    SetProperties(field);
    if (field.isSwitch)
    {
        const char *icon = field.name == "boost" ? "mdi:rocket" : field.name == "fan" ? "mdi:fan"
                                                                                      : "mdi:toggle-switch";
        char availabilityTopic[128];
        snprintf(availabilityTopic, sizeof(availabilityTopic), "tombask/%s/status", deviceIdStr);
        snprintf(payload, sizeof(payload),
                 "{"
                 "\"name\":\"%s\","
                 "\"unique_id\":\"%s\","
                 "\"entity_category\": \"config\","
                 "%s"
                 "%s"
                 "\"icon\":\"%s\","
                 "\"payload_on\":\"ON\","
                 "\"payload_off\":\"OFF\","
                 "\"device\":%s"
                 "}",
                 field.description, uniqueId, stateTopic, cmdTopic, icon, deviceJson);
    }
    else if (field.step > 0)
    {
        publishClimateDiscovery();
        snprintf(payload, sizeof(payload),
                 "{"
                 "\"name\":\"%s\","
                 "\"unique_id\":\"%s\","
                 "%s"
                 "%s"
                 "%s"
                 "%s"
                 "\"min\":%u,"
                 "\"max\":%u,"
                 "\"step\":%u,"
                 "\"device\":%s"
                 "}",
                 field.description, uniqueId, stateTopic, cmdTopic, unitOfMeasurement, deviceClass,
                 (unsigned)field.min, (unsigned)field.max, (unsigned)field.step, deviceJson);
    }
    else
    {
        snprintf(payload, sizeof(payload),
                 "{"
                 "\"name\":\"%s\","
                 "\"unique_id\":\"%s\","
                 "\"state_class\":\"measurement\","
                 "%s"
                 "%s"
                 "%s"
                 "%s"
                 "\"device\":%s"
                 "}",
                 field.description, uniqueId, stateTopic, cmdTopic, unitOfMeasurement, deviceClass, deviceJson);
    }
    SetDiscoveryTopic(field);
    const bool ok = mqttClient.publish(discoveryTopic, payload, true);
    if (!ok)
    {
        Serial.printf("[mqtt] publish discovery failed topic=%s len=%u\n", discoveryTopic, (unsigned)strlen(payload));
    }
}

static void publishClimateDiscovery()
{
    const char *uniqueId = "tombask1_climate";
    const char *discoveryTopic = "homeassistant/climate/tombask1_climate/config";
    snprintf(payload, sizeof(payload),
             "{"
             "\"name\":\"Heat Pump Boiler\","
             "\"unique_id\":\"%s\","
             "\"temperature_state_topic\":\"tombask/tombask1/Tsetpoint/state\","
             "\"temperature_command_topic\":\"tombask/tombask1/Tsetpoint/set\","
             "\"temperature_unit\":\"C\","
             "\"min_temp\":40,"
             "\"max_temp\":70,"
             "\"temp_step\":1,"
             "\"current_temperature_topic\":\"tombask/tombask1/T8/state\","
             "\"mode_command_topic\":\"tombask/tombask1/climate/mode/set\","
             "\"mode_state_topic\":\"tombask/tombask1/climate/mode/state\","
             "\"modes\":[\"auto\"],"
             "\"action_topic\":\"tombask/tombask1/climate/action/state\","
             "\"action_template\":\"{{ value }}\","
             "\"actions\":[\"off\",\"heating\",\"idle\"],"
             "\"device\":%s"
             "}",
             uniqueId, deviceJson);
    mqttClient.publish(discoveryTopic, payload, true);
}
