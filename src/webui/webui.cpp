#include "webui.h"

#include <Arduino.h>
#include <WebServer.h>

#include <ArduinoJson.h>

#include <SPIFFS.h>

#include "storage/storage.h"
#include <WiFiManager.h>
#include "webui/webLogger.h"

WebServer server(80);
static bool spiffsOk = false;

static const char *contentTypeForPath(const String &path)
{
    if (path.endsWith(".html"))
        return "text/html";
    if (path.endsWith(".css"))
        return "text/css";
    if (path.endsWith(".js"))
        return "application/javascript";
    if (path.endsWith(".json"))
        return "application/json";
    if (path.endsWith(".svg"))
        return "image/svg+xml";
    if (path.endsWith(".png"))
        return "image/png";
    if (path.endsWith(".ico"))
        return "image/x-icon";
    if (path.endsWith(".txt"))
        return "text/plain";
    if (path.endsWith(".map"))
        return "application/json";
    return "application/octet-stream";
}

static void sendConfigJson()
{
    // NOTE: we intentionally do not return wifiPassword.
    char json[384];
    snprintf(json, sizeof(json),
             "{\"deviceId\":%u,\"mqttHost\":\"%s\",\"mqttPort\":%u}",
             static_cast<unsigned>(deviceId),
             getMqttHost(),
             static_cast<unsigned>(getMqttPort()));

    server.send(200, "application/json", json);
}

static void handleGetConfig()
{
    sendConfigJson();
}

static void handlePostConfig()
{
    const String body = server.arg("plain");
    JsonDocument doc;
    const DeserializationError err = deserializeJson(doc, body);
    if (err)
    {
        server.send(400, "text/plain", "Invalid JSON");
        return;
    }

    const int newDeviceId = doc["deviceId"] | 1;
    const char *newMqttHost = doc["mqttHost"] | "";
    const uint16_t newMqttPort = static_cast<uint16_t>(doc["mqttPort"] | 0);

    deviceId = static_cast<uint8_t>(newDeviceId);

    if (newMqttHost && newMqttHost[0] != '\0')
    {
        setMqttHost(newMqttHost);
    }

    if (newMqttPort != 0)
    {
        setMqttPort(newMqttPort);
    }

    saveConfig();
    sendConfigJson();
}

static bool tryServeFromSpiffs(const String &uri)
{
    if (!spiffsOk)
        return false;

    String path = uri;
    if (path == "/")
        path = "/index.html";

    if (SPIFFS.exists(path))
    {
        File f = SPIFFS.open(path, "r");
        if (!f)
            return false;
        server.streamFile(f, contentTypeForPath(path));
        f.close();
        return true;
    }

    // SPA fallback: if no extension, serve index.html
    if (path.indexOf('.') < 0 && SPIFFS.exists("/index.html"))
    {
        File f = SPIFFS.open("/index.html", "r");
        if (!f)
            return false;
        server.streamFile(f, "text/html");
        f.close();
        return true;
    }

    return false;
}

void setupWebUi()
{
    spiffsOk = SPIFFS.begin(false);

    server.on("/api/config", HTTP_GET, handleGetConfig);
    server.on("/api/config", HTTP_POST, handlePostConfig);
    server.on("/api/logs", HTTP_GET, handleSseLogs);

    server.onNotFound([]()
                      {
        if (tryServeFromSpiffs(server.uri())) return;
        server.send(404, "text/plain", "Not found"); });

    server.begin();
}

void webUiLoop()
{
    server.handleClient();
}
