#include "storage.h"

#include <Preferences.h>

static Preferences prefs;

uint8_t deviceId = 0;
static char mqttHost[64] = "";
static uint16_t mqttPort = 0;
static char mqttUser[64] = "";
static char mqttPassword[64] = "";

void loadConfig()
{
    // Open RW so the namespace is created on first boot.
    // (Opening RO can fail with NOT_FOUND if it doesn't exist yet.)
    prefs.begin("_", false);
    deviceId = prefs.getUChar("deviceId", 0);
    prefs.getString("mqttHost", "").toCharArray(mqttHost, sizeof(mqttHost));
    mqttPort = static_cast<uint16_t>(prefs.getUInt("mqttPort", 0));
    prefs.getString("mqttUser", "").toCharArray(mqttUser, sizeof(mqttUser));
    prefs.getString("mqttPass", "").toCharArray(mqttPassword, sizeof(mqttPassword));
    prefs.end();
}

void saveConfig()
{
    prefs.begin("_", false);
    prefs.putUChar("deviceId", deviceId);
    prefs.putString("mqttHost", mqttHost);
    prefs.putUInt("mqttPort", mqttPort);
    prefs.putString("mqttUser", mqttUser);
    prefs.putString("mqttPass", mqttPassword);
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

const char* getMqttUser()
{
    return mqttUser;
}

const char* getMqttPassword()
{
    return mqttPassword;
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

void setMqttUser(const char* user)
{
    if (!user) user = "";
    strlcpy(mqttUser, user, sizeof(mqttUser));
}

void setMqttPassword(const char* password)
{
    if (!password) password = "";
    strlcpy(mqttPassword, password, sizeof(mqttPassword));
}
