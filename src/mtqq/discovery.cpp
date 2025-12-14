#include "secrets.h" // defines WIFI_SSID and WIFI_PASSWORD
#include "globals.h"
#include "discovery.h"

void sendDiscovery()
{
  // Device object shared by all entities
  const char *deviceJson = R"({
    "identifiers":["tombask_esp_device"],
    "name":"Tombask ESP",
    "manufacturer":"Custom",
    "model":"ESP MQTT"
  })";
  struct SensorDesc
  {
    const char *key;
    const char *name;
    const char *unit;
  };

  const SensorDesc sensors[] = {
      {"t1", "Air inlet temperature", "°C"},
      {"t2", "Top tank water temperature", "°C"},
      {"t3", "Bottom tank water temperature", "°C"},
      {"t4", "Discharge gas temperature", "°C"},
      {"t5", "Air outlet temperature", "°C"},
      {"t6", "Suction gas temperature", "°C"},
      {"t7", "Target temperature", "°C"},
      {"counter1", "Counter 1 raw", "rpm"},
      {"p1", "P1", "Hz"},
      {"p2", "P2", "Hz"},
      {"Eev", "EEV position", "Hz"},
      {"compHz", "Compressor frequency", "Hz"},
      {"Zuigdruk", "Suction pressure", "rpm"},
      {"Condensatiedruk", "Condensation pressure", "Hz"},
      {"fanSpeed", "Fan speed", "rpm"},
  };

  char topic[128];
  char payload[256];

  for (auto &s : sensors)
  {
    // discovery topic: homeassistant/sensor/<unique_id>/config
    snprintf(topic, sizeof(topic), "homeassistant/sensor/tombask_esp_%s/config", s.key);

    // state topic: <base>/t1, <base>/t2, ...
    char stateTopic[96];
    snprintf(stateTopic, sizeof(stateTopic), "%s/%s", MQTT_BASE_TOPIC, s.key);

    // minimal JSON: name, state_topic, unit, unique_id, device
    snprintf(payload, sizeof(payload),
             "{\"name\":\"%s\",\"state_topic\":\"%s\",\"unit_of_measurement\":\"%s\",\"unique_id\":\"tombask_esp_%s\",\"device\":%s}",
             s.name, stateTopic, s.unit, s.key, deviceJson);

    mqttClient.publish(topic, payload, true); // retain config
  }
}
