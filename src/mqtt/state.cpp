#include "../rs485/sensors.h"
#include <Arduino.h>
#include <globals.h> // mqttClient
#include "../rs485/settings.h"
#include "mqtt_properties.h"
#include "state.h"

static char stateTopic2[128];
void publishSensorState(const SensorField2 &field, const float value)
{
    snprintf(stateTopic2, sizeof(stateTopic2), "tombask/%s/%s/state", deviceIdStr, field.name);
    char valueStr[32];

    if (strcmp(field.name, "climate/action") == 0)
        snprintf(valueStr, sizeof(valueStr), "%s", value == 1 ? "heating" : "off");
    else if (strcmp(field.name, "climate/mode") == 0)
        snprintf(valueStr, sizeof(valueStr), "auto");
    else if (field.isSwitch)
        snprintf(valueStr, sizeof(valueStr), "%s", value ? "ON" : "OFF");
    else
        snprintf(valueStr, sizeof(valueStr), "%.*f", field.decimals, value);

    Publish(valueStr);
}

void Publish(const char *valueStr)
{
    const bool ok = mqttClient.publish(stateTopic2, valueStr, true);
    if (!ok)
    {
        Serial.printf("[mqtt] publish state failed topic=%s\n", stateTopic2);
    }
}
