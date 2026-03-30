#include "ota/auto_update.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <HTTPClient.h>
#include <SPIFFS.h>
#include <Update.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

#include "build_info.generated.h"

namespace
{
const char *const kGitHubOwner = "Anderman";
const char *const kGitHubRepo = "tombask";
const char *const kGitHubRef = "main";
const char *const kGitHubApiHost = "https://api.github.com/repos/";
const char *const kFirmwarePath = "release/firmware";
const char *const kSpiffsPath = "release/spiffs";
const char *const kVersionFilePath = "/version.txt";
const char *const kUserAgent = "tombask-updater";
const unsigned long kInitialCheckDelayMs = 60UL * 1000UL;
const unsigned long kCheckIntervalMs = 7UL * 24UL * 60UL * 60UL * 1000UL;

struct RemoteArtifact
{
    String buildId;
    String downloadUrl;
};

AutoUpdateStatus status = {
    String(),
    String(),
    String(),
    String(),
    String("idle"),
    String(),
    0,
    false,
    false,
    true,
};

unsigned long nextCheckMillis = 0;

void setState(const char *value)
{
    status.state = value;
}

void setError(const String &message)
{
    status.lastError = message;
    setState("error");
    Serial.print("[autoupdate] ");
    Serial.println(message);
}

bool isBuildId(String value)
{
    if (!value.endsWith(".bin"))
    {
        return false;
    }

    value.remove(value.length() - 4);
    if (value.length() != 16 || value.charAt(8) != 'T' || value.charAt(15) != 'Z')
    {
        return false;
    }

    for (int index = 0; index < value.length(); ++index)
    {
        if (index == 8 || index == 15)
        {
            continue;
        }

        if (!isDigit(value.charAt(index)))
        {
            return false;
        }
    }

    return true;
}

String extractBuildId(const String &fileName)
{
    if (!isBuildId(fileName))
    {
        return String();
    }

    return fileName.substring(0, fileName.length() - 4);
}

bool isNewerBuildId(const String &candidate, const String &current)
{
    return candidate.length() > 0 && (current.length() == 0 || candidate > current);
}

String getContentsUrl(const char *path)
{
    String url = kGitHubApiHost;
    url += kGitHubOwner;
    url += "/";
    url += kGitHubRepo;
    url += "/contents/";
    url += path;
    url += "?ref=";
    url += kGitHubRef;
    return url;
}

bool beginRequest(HTTPClient &http, WiFiClientSecure &client, const String &url, const char *acceptHeader)
{
    client.setInsecure();
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    http.setTimeout(15000);
    if (!http.begin(client, url))
    {
        setError(String("Failed to open URL: ") + url);
        return false;
    }
    http.addHeader("User-Agent", kUserAgent);
    http.addHeader("Accept", acceptHeader);
    return true;
}

bool fetchLatestArtifact(const char *path, RemoteArtifact &artifact)
{
    WiFiClientSecure client;
    HTTPClient http;
    if (!beginRequest(http, client, getContentsUrl(path), "application/vnd.github+json"))
    {
        return false;
    }

    const int httpCode = http.GET();
    if (httpCode != HTTP_CODE_OK)
    {
        setError(String("GitHub listing failed for ") + path + ": HTTP " + httpCode);
        http.end();
        return false;
    }

    JsonDocument doc;
    const DeserializationError jsonError = deserializeJson(doc, http.getString());
    http.end();
    if (jsonError)
    {
        setError(String("Failed to parse GitHub listing for ") + path + ": " + jsonError.c_str());
        return false;
    }

    if (!doc.is<JsonArray>())
    {
        setError(String("Unexpected GitHub listing for ") + path);
        return false;
    }

    for (JsonObject item : doc.as<JsonArray>())
    {
        const char *name = item["name"];
        const char *downloadUrl = item["download_url"];
        if (name == nullptr || downloadUrl == nullptr)
        {
            continue;
        }

        const String buildId = extractBuildId(String(name));
        if (!isNewerBuildId(buildId, artifact.buildId))
        {
            continue;
        }

        artifact.buildId = buildId;
        artifact.downloadUrl = downloadUrl;
    }

    if (artifact.buildId.isEmpty())
    {
        setError(String("No valid release files found in ") + path);
        return false;
    }

    return true;
}

String readSpiffsBuildId()
{
    if (!SPIFFS.begin(false))
    {
        return String();
    }

    if (!SPIFFS.exists(kVersionFilePath))
    {
        return String();
    }

    File file = SPIFFS.open(kVersionFilePath, "r");
    if (!file)
    {
        return String();
    }

    String buildId = file.readString();
    file.close();
    buildId.trim();
    return buildId;
}

bool applyRemoteArtifact(const RemoteArtifact &artifact, int updateCommand, const char *stateName)
{
    WiFiClientSecure client;
    HTTPClient http;
    if (!beginRequest(http, client, artifact.downloadUrl, "application/octet-stream"))
    {
        return false;
    }

    const int httpCode = http.GET();
    if (httpCode != HTTP_CODE_OK)
    {
        setError(String("Download failed for ") + artifact.buildId + ": HTTP " + httpCode);
        http.end();
        return false;
    }

    const int contentLength = http.getSize();
    if (contentLength <= 0)
    {
        setError(String("Missing content length for ") + artifact.buildId);
        http.end();
        return false;
    }

    setState(stateName);
    if (!Update.begin(contentLength, updateCommand))
    {
        setError(String("Update.begin failed: ") + Update.errorString());
        http.end();
        return false;
    }

    WiFiClient *stream = http.getStreamPtr();
    const size_t written = Update.writeStream(*stream);
    http.end();

    if (written != static_cast<size_t>(contentLength))
    {
        Update.abort();
        setError(String("Incomplete download for ") + artifact.buildId);
        return false;
    }

    if (!Update.end())
    {
        setError(String("Update.end failed: ") + Update.errorString());
        return false;
    }

    if (!Update.isFinished())
    {
        setError(String("Update did not finish for ") + artifact.buildId);
        return false;
    }

    return true;
}

void refreshCurrentBuildIds()
{
    status.currentFirmwareBuildId = TOMBASK_BUILD_ID;
    status.currentSpiffsBuildId = readSpiffsBuildId();
}

void scheduleNextCheck(unsigned long delayMs)
{
    nextCheckMillis = millis() + delayMs;
}

void performUpdateCheck()
{
    refreshCurrentBuildIds();
    status.latestFirmwareBuildId = String();
    status.latestSpiffsBuildId = String();
    status.firmwareUpdateAvailable = false;
    status.spiffsUpdateAvailable = false;
    status.lastError = String();
    status.lastCheckMillis = millis();

    setState("checking");
    RemoteArtifact firmwareArtifact;
    if (!fetchLatestArtifact(kFirmwarePath, firmwareArtifact))
    {
        return;
    }

    status.latestFirmwareBuildId = firmwareArtifact.buildId;
    status.firmwareUpdateAvailable = isNewerBuildId(firmwareArtifact.buildId, status.currentFirmwareBuildId);
    if (status.firmwareUpdateAvailable)
    {
        Serial.print("[autoupdate] Applying firmware ");
        Serial.println(firmwareArtifact.buildId);
        if (applyRemoteArtifact(firmwareArtifact, U_FLASH, "updating-firmware"))
        {
            delay(500);
            ESP.restart();
        }
        return;
    }

    RemoteArtifact spiffsArtifact;
    if (!fetchLatestArtifact(kSpiffsPath, spiffsArtifact))
    {
        return;
    }

    status.latestSpiffsBuildId = spiffsArtifact.buildId;
    status.spiffsUpdateAvailable = isNewerBuildId(spiffsArtifact.buildId, status.currentSpiffsBuildId);
    if (status.spiffsUpdateAvailable)
    {
        Serial.print("[autoupdate] Applying spiffs ");
        Serial.println(spiffsArtifact.buildId);
        if (applyRemoteArtifact(spiffsArtifact, U_SPIFFS, "updating-spiffs"))
        {
            delay(500);
            ESP.restart();
        }
        return;
    }

    setState("idle");
}
} // namespace

void setupAutoUpdate()
{
    refreshCurrentBuildIds();
    status.latestFirmwareBuildId = status.currentFirmwareBuildId;
    status.latestSpiffsBuildId = status.currentSpiffsBuildId;
    status.checkQueued = true;
    scheduleNextCheck(kInitialCheckDelayMs);
}

void autoUpdateLoop()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        return;
    }

    const unsigned long now = millis();
    if (!status.checkQueued && static_cast<long>(now - nextCheckMillis) < 0)
    {
        return;
    }

    if (status.checkQueued && static_cast<long>(now - nextCheckMillis) < 0)
    {
        return;
    }

    status.checkQueued = false;
    performUpdateCheck();
    if (status.state != "error")
    {
        setState("idle");
    }
    scheduleNextCheck(kCheckIntervalMs);
}

void queueAutoUpdateCheck()
{
    status.checkQueued = true;
    scheduleNextCheck(0);
}

const AutoUpdateStatus &getAutoUpdateStatus()
{
    return status;
}

String getCurrentFirmwareBuildId()
{
    return TOMBASK_BUILD_ID;
}

String getCurrentSpiffsBuildId()
{
    return readSpiffsBuildId();
}
