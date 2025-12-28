#include "storage/sensor_log.h"
#include <functional>
#include <ArduinoJson.h>
#include "webui.h"
#include "webui/controlApi.h"
#include "webui/mqtt_config.h"
#include "webui/static_files.h"
#include <Arduino.h>
#include <WebServer.h>
#include "storage/storage.h"
#include <WiFiManager.h>
#include "webui/webLogger.h"
#include "../timesync/timesync_api.h"
#include "webui/temp_logs_api.h"
#include "webui/dayahead_api.h"
#include <SPIFFS.h>

WebServer server(80);
static void addPostEndpoint(const char *path, std::function<void()> handler);
static void addGetEndpoint(const char *path, std::function<void()> handler);

bool parseJsonBody(JsonDocument &doc)
{
    const String body = server.arg("plain");
    const DeserializationError err = deserializeJson(doc, body);
    if (err)
    {
        server.send(400, "text/plain", "Invalid JSON");
        return false;
    }
    return true;
}

void setupWebUi()
{
    extern bool spiffsOk;
    spiffsOk = SPIFFS.begin(false);

    addPostEndpoint("/api/settimezone", handleTimeZonePost);


    // mqtt config API
    addGetEndpoint("/api/config", handleGetConfig);
    addPostEndpoint("/api/config", handlePostConfig);

    // temperatuur log API
    addGetEndpoint("/api/templogs", handleGetTempLogs);

    // day-ahead prices API
    addGetEndpoint("/api/dayahead", handleGetDayAheadPrices);

    // weblogger SSE endpoint
    addGetEndpoint("/api/logs", handleSseLogs);

    // status endpoint
    addGetEndpoint("/api/status", handleGetStatus);

    addPostEndpoint("/api/setpoint", handlePostSetpoint);
    addPostEndpoint("/api/fan", handlePostFan);
    addPostEndpoint("/api/boost", handlePostBoost);

    // static file serving
    server.onNotFound(handleNotFound);

    server.begin();
}

void webUiLoop()
{
    server.handleClient();
}

std::function<void()> withCors(std::function<void()> handler)
{
    return [handler]()
    {
        SEND_CORS();
        handler();
    };
}
static void addPostEndpoint(const char *path, std::function<void()> handler)
{
    server.on(path, HTTP_POST, withCors(handler));
    server.on(path, HTTP_OPTIONS, []()
              {
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.sendHeader("Access-Control-Allow-Methods", "POST,OPTIONS");
        server.sendHeader("Access-Control-Allow-Headers", "Content-Type,Accept");
        server.send(204); });
}

static void addGetEndpoint(const char *path, std::function<void()> handler)
{
    server.on(path, HTTP_GET, withCors(handler));
}
