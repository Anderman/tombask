// NVS key management
void listNvsKeys();
void deleteNvsKey(const char* key);
void deleteNvsKeysByPrefix(const char* prefix);
#pragma once
#include <cstdint>

// Device identifier used for MQTT topics + Home Assistant unique_id/device identifiers.
// Stored in NVS and editable via Web UI.
extern uint8_t deviceId;
extern char timezone[64];

// Load/save all configuration from NVS (Preferences)
void loadConfig();
void saveConfig();

// Backwards-compatible helpers (used by older code)
void loadDeviceId();
void saveDeviceId();

// Accessors for config values
const char* getMqttHost();
uint16_t getMqttPort();
const char* getMqttUser();
const char* getMqttPassword();
const char* getWifiSsid();
const char* getWifiPassword();

void setMqttHost(const char* host);
void setMqttPort(uint16_t port);
void setMqttUser(const char* user);
void setMqttPassword(const char* password);