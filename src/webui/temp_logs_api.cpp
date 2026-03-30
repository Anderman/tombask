#include "temp_logs_api.h"
#include <ArduinoJson.h>
#include <WebServer.h>
#include "storage/sensor_log.h"
#include <esp_system.h>       // esp_restart()
#include <soc/soc.h>          // REG_SET_BIT(), etc.
#include <soc/rtc_cntl_reg.h> // RTC_CNTL_OPTION1_REG, RTC_CNTL_FORCE_DOWNLOAD_BOOT

extern WebServer server;
extern SensorLog topTempLog;
extern SensorLog bottomTempLog;

void handleGetTempLogs()
{
    JsonDocument doc;
    JsonArray topArr = doc["top"].to<JsonArray>();
    for (const auto &entry : topTempLog.entries)
    {
        JsonObject obj = topArr.add<JsonObject>();
        obj["t"] = entry.timestamp;
        obj["v"] = entry.value;
    }
    JsonArray bottomArr = doc["bottom"].to<JsonArray>();
    for (const auto &entry : bottomTempLog.entries)
    {
        JsonObject obj = bottomArr.add<JsonObject>();
        obj["t"] = entry.timestamp;
        obj["v"] = entry.value;
    }
    String out;
    serializeJson(doc, out);
    server.send(200, "application/json", out);
}

void handleGetReboot()
{
    server.send(200, "text/plain", "Rebooting...");
    REG_SET_BIT(RTC_CNTL_OPTION1_REG, RTC_CNTL_FORCE_DOWNLOAD_BOOT);
    delay(50);
    esp_restart();
}