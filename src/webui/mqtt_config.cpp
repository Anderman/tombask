#include <WebServer.h>
#include <ArduinoJson.h>
#include "mqtt_config.h"
#include "../storage/storage.h"
#include "webui.h"

extern WebServer server;
extern uint8_t deviceId;

void handleGetConfig()
{
    JsonDocument doc;
    doc["deviceId"] = deviceId;
    doc["mqttHost"] = getMqttHost();
    doc["mqttPort"] = getMqttPort();
    doc["mqttUser"] = getMqttUser();
    doc["mqttPass"] = "";
    String out;
    serializeJson(doc, out);
    server.send(200, "application/json", out);
}

void handlePostConfig()
{
    JsonDocument doc;
    if (!parseJsonBody(doc)) return;
    const int newDeviceId = doc["deviceId"] | 1;
    deviceId = static_cast<uint8_t>(newDeviceId);
    setMqttHost(doc["mqttHost"].as<const char *>());
    setMqttPort(doc["mqttPort"].as<uint16_t>());
    setMqttUser(doc["mqttUser"].as<const char *>());
    if (doc["mqttPass"].as<const char *>()[0] != '\0')
    {
        setMqttPassword(doc["mqttPass"].as<const char *>());
    }
    saveConfig();
    server.send(204);
}
