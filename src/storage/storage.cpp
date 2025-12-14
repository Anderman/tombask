#include "storage.h"

#include <Preferences.h>

#include <rs485/settings.h>

static Preferences prefs;


static char mqttHost[64] = "";
static uint16_t mqttPort = 0;

static void setDefaultIfEmpty(char* target, size_t targetSize, const char* value)
{
    if (!target || targetSize == 0) return;
    if (target[0] != '\0') return;
    if (!value) value = "";
    strlcpy(target, value, targetSize);
}

void loadConfig()
{
    // Open RW so the namespace is created on first boot.
    // (Opening RO can fail with NOT_FOUND if it doesn't exist yet.)
    prefs.begin("_", false);
    prefs.getString("deviceId", "").toCharArray(deviceId, sizeof(deviceId));
    prefs.getString("mqttHost", "").toCharArray(mqttHost, sizeof(mqttHost));
    mqttPort = static_cast<uint16_t>(prefs.getUInt("mqttPort", 0));
    prefs.end();

    // Provide a sane default so Home Assistant entities remain stable unless user changes it.
    setDefaultIfEmpty(deviceId, sizeof(deviceId), "tombask");
}

void saveConfig()
{
    prefs.begin("_", false);
    prefs.putString("deviceId", deviceId);
    prefs.putString("mqttHost", mqttHost);
    prefs.putUInt("mqttPort", mqttPort);
    prefs.end();
}

void loadDeviceId()
{
    loadConfig();
}

void saveDeviceId()
{
    saveConfig();
}

const char* getMqttHost()
{
    return mqttHost;
}

uint16_t getMqttPort()
{
    return mqttPort;
}

void setMqttHost(const char* host)
{
    if (!host) host = "";
    strlcpy(mqttHost, host, sizeof(mqttHost));
}

void setMqttPort(uint16_t port)
{
    mqttPort = port;
}
