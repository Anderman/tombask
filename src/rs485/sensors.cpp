#include "sensors.h"
#include <Arduino.h>
#include <globals.h> // mqttClient
#include "settings.h"

Sensors sensors = {
    .T1 = {"t1", "Air inlet temperature", "°C", "temperature"},
    .T2 = {"t2", "Top tank water temperature", "°C", "temperature"},
    .T3 = {"t3", "Bottom tank water temperature", "°C", "temperature"},
    .T4 = {"t4", "Discharge gas temperature", "°C", "temperature"},
    .T5 = {"t5", "Air outlet temperature", "°C", "temperature"},
    .T6 = {"t6", "Suction gas temperature", "°C", "temperature"},
    .T7 = {"t7", "Target temperature", "°C", "temperature"},
    .counter1 = {"counter1", "Counter 1 raw", "step", ""},
    .Exv = {"Exv", "Electronic eXpansion Valve", "step", ""},
    .compHz = {"compHz", "Compressor frequency", "Hz", "frequency"},
    .p1 = {"p1", "P1", "Hz", "frequency"},
    .p2 = {"p2", "P2", "Hz", "frequency"},
    .SuctionPressure = {"SuctionPressure", "Electronic eXpansion Valve", "Bar", "pressure"},
    .CondensationPressure = {"CondensationPressure", "Condensation pressure", "Bar", "pressure"},
    .FanSpeed = {"FanSpeed", "Fan speed", "rpm", "speed"}
};


static char uniqueId[64];
static char discoveryTopic[128];
static char stateTopic[128];
static char payload[512];

static void publishSensorDiscovery(const SensorField &field);
static void publishSensorState(const SensorField &field, const char *valueStr);
static void setStateTopic(const SensorField &field);
void ensureDeviceInfo();

void publishSensors()
{
    ensureDeviceInfo();

    publishSensorDiscovery(sensors.T1);
    publishSensorDiscovery(sensors.T2);
    publishSensorDiscovery(sensors.T3);
    publishSensorDiscovery(sensors.T4);
    publishSensorDiscovery(sensors.T5);
    publishSensorDiscovery(sensors.T6);
    publishSensorDiscovery(sensors.T7);
    publishSensorDiscovery(sensors.counter1);
    publishSensorDiscovery(sensors.p1);
    publishSensorDiscovery(sensors.p2);
    publishSensorDiscovery(sensors.SuctionPressure);
    publishSensorDiscovery(sensors.compHz);
    publishSensorDiscovery(sensors.Exv);
    publishSensorDiscovery(sensors.CondensationPressure);
    publishSensorDiscovery(sensors.FanSpeed);
}

void publishSensorsState()
{
    ensureDeviceInfo();

    publishSensorState(sensors.T1, "50.0");
    publishSensorState(sensors.T2, "45.0");
    publishSensorState(sensors.T3, "1.2");
    publishSensorState(sensors.T4, "0.8");
    publishSensorState(sensors.T5, "0.5");
    publishSensorState(sensors.T6, "0.3");
    publishSensorState(sensors.T7, "0.0");
    publishSensorState(sensors.counter1, "12345");
    publishSensorState(sensors.p1, "25.0");
    publishSensorState(sensors.p2, "30.0");
    publishSensorState(sensors.SuctionPressure, "12.5");
    publishSensorState(sensors.compHz, "45.0"); 
    publishSensorState(sensors.Exv, "75.0");
    publishSensorState(sensors.CondensationPressure, "20.0");
    publishSensorState(sensors.FanSpeed, "1500");
}

static void publishSensorDiscovery(const SensorField &field)
{
    snprintf(uniqueId, sizeof(uniqueId), "%s_%s", deviceIdStr, field.name);
    snprintf(discoveryTopic, sizeof(discoveryTopic), "homeassistant/sensor/%s/config", uniqueId);
    setStateTopic(field);

    snprintf(payload, sizeof(payload),
             "{"
             "\"name\":\"%s\","
             "\"unique_id\":\"%s\","
             "\"state_topic\":\"%s\","
             "\"unit_of_measurement\":\"%s\","
             "\"device_class\":\"%s\","
             "\"state_class\":\"measurement\","
             "\"device\":%s"
             "}",
             field.description, uniqueId, stateTopic, field.unit, field.deviceClass, deviceJson);

    mqttClient.publish(discoveryTopic, payload, true);
}

static void publishSensorState(const SensorField &field, const char *valueStr)
{
    setStateTopic(field);
    mqttClient.publish(stateTopic, valueStr, true);
}

static void setStateTopic(const SensorField &field)
{
    snprintf(stateTopic, sizeof(stateTopic), "tombask/%s/%s/state", deviceIdStr, field.name);
}

