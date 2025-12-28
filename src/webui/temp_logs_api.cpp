#include "temp_logs_api.h"
#include <ArduinoJson.h>
#include <WebServer.h>
#include "storage/sensor_log.h"

extern WebServer server;
extern SensorLog topTempLog;
extern SensorLog bottomTempLog;

void handleGetTempLogs()
{
    JsonDocument doc;
    JsonArray topArr = doc["top"].to<JsonArray>();
    for (const auto &entry : topTempLog.entries()) {
        JsonObject obj = topArr.add<JsonObject>();
        obj["t"] = entry.timestamp;
        obj["v"] = entry.value;
    }
    JsonArray bottomArr = doc["bottom"].to<JsonArray>();
    for (const auto &entry : bottomTempLog.entries()) {
        JsonObject obj = bottomArr.add<JsonObject>();
        obj["t"] = entry.timestamp;
        obj["v"] = entry.value;
    }
    String out;
    serializeJson(doc, out);
    server.send(200, "application/json", out);
}
