#include "storage.h"

#include <Preferences.h>

static Preferences prefs;

uint8_t deviceId = 0;


static char mqttHost[64] = "";
static uint16_t mqttPort = 0;

void loadConfig()
{
    // Open RW so the namespace is created on first boot.
    // (Opening RO can fail with NOT_FOUND if it doesn't exist yet.)
    prefs.begin("_", false);
    deviceId = prefs.getUChar("deviceId", 0);
    prefs.getString("mqttHost", "").toCharArray(mqttHost, sizeof(mqttHost));
    mqttPort = static_cast<uint16_t>(prefs.getUInt("mqttPort", 0));
    prefs.end();

    // Provide a sane default so Home Assistant entities remain stable unless user changes it.
    if (deviceId == 0)
    {
        deviceId = 1;
    }
}

void saveConfig()
{
    prefs.begin("_", false);
    if (deviceId == 0)
    {
        deviceId = 1;
    }
    prefs.putUChar("deviceId", deviceId);
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
