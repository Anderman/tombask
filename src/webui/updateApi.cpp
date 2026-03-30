#include "webui/updateApi.h"

#include <ArduinoJson.h>
#include <WebServer.h>

#include "ota/auto_update.h"
#include "webui.h"

extern WebServer server;

void handleGetVersion()
{
    JsonDocument doc;
    doc["firmwareBuildId"] = getCurrentFirmwareBuildId();
    doc["spiffsBuildId"] = getCurrentSpiffsBuildId();

    String out;
    serializeJson(doc, out);
    server.send(200, "application/json", out);
}

void handleGetUpdateStatus()
{
    const AutoUpdateStatus &status = getAutoUpdateStatus();

    JsonDocument doc;
    doc["state"] = status.state;
    doc["lastError"] = status.lastError;
    doc["lastCheckMillis"] = status.lastCheckMillis;
    doc["checkQueued"] = status.checkQueued;
    doc["firmwareBuildId"] = status.currentFirmwareBuildId;
    doc["spiffsBuildId"] = status.currentSpiffsBuildId;
    doc["latestFirmwareBuildId"] = status.latestFirmwareBuildId;
    doc["latestSpiffsBuildId"] = status.latestSpiffsBuildId;
    doc["firmwareUpdateAvailable"] = status.firmwareUpdateAvailable;
    doc["spiffsUpdateAvailable"] = status.spiffsUpdateAvailable;

    String out;
    serializeJson(doc, out);
    server.send(200, "application/json", out);
}

void handlePostUpdateCheck()
{
    queueAutoUpdateCheck();
    server.send(202, "application/json", "{\"queued\":true}");
}
