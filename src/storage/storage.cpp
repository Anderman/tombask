#include <Preferences.h>
#include <cstring>
#include <nvs.h>
#include <nvs_flash.h>

// List all NVS keys in the default namespace using Preferences API
void listNvsKeys()
{
    nvs_iterator_t it = nvs_entry_find("nvs", NULL, NVS_TYPE_ANY);
    while (it != NULL)
    {
        nvs_entry_info_t info;
        nvs_entry_info(it, &info);
        Serial.printf("Key: %s, Type: %d, Namespace: %s\n", info.key, info.type, info.namespace_name);
        it = nvs_entry_next(it);
    }
}

// Delete a single NVS key by name
void deleteNvsKey(const char *key)
{
    static Preferences prefs;
    prefs.begin("_", false);
    prefs.remove(key);
    prefs.end();
}

// Delete all NVS keys with a given prefix
void deleteNvsKeysByPrefix(const char *prefix)
{
    size_t prefixLen = std::strlen(prefix);
    nvs_iterator_t it = nvs_entry_find(NULL, "_", NVS_TYPE_ANY);
    while (it)
    {
        nvs_entry_info_t info;
        nvs_entry_info(it, &info);
        if (std::strncmp(info.key, prefix, prefixLen) == 0)
        {
            printf("Deleting NVS key: %s\n", info.key);
            deleteNvsKey(info.key);
        }
        it = nvs_entry_next(it);
    }
}
#include "storage.h"

#include <Preferences.h>

static Preferences prefs;

uint8_t deviceId = 0;
char timezone[64] = "EST5EDT,M3.2.0/2,M11.1.0/2";

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
    prefs.getString("timezone", "").toCharArray(timezone, sizeof(timezone));
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
    prefs.putString("timezone", timezone);
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

const char *getMqttHost()
{
    return mqttHost;
}

uint16_t getMqttPort()
{
    return mqttPort;
}

const char *getMqttUser()
{
    return mqttUser;
}

const char *getMqttPassword()
{
    return mqttPassword;
}

void setMqttHost(const char *host)
{
    if (!host)
        host = "";
    strlcpy(mqttHost, host, sizeof(mqttHost));
}

void setMqttPort(uint16_t port)
{
    mqttPort = port;
}

void setMqttUser(const char *user)
{
    if (!user)
        user = "";
    strlcpy(mqttUser, user, sizeof(mqttUser));
}

void setMqttPassword(const char *password)
{
    if (!password)
        password = "";
    strlcpy(mqttPassword, password, sizeof(mqttPassword));
}
