#pragma once
#include <cstdint>

// Device identifier used for MQTT topics + Home Assistant unique_id/device identifiers.
// Stored in NVS and editable via Web UI.
extern uint8_t deviceId;

// Load/save all configuration from NVS (Preferences)
void loadConfig();
void saveConfig();

// Backwards-compatible helpers (used by older code)
void loadDeviceId();
void saveDeviceId();

// Accessors for config values
const char* getMqttHost();
uint16_t getMqttPort();
const char* getWifiSsid();
const char* getWifiPassword();

void setMqttHost(const char* host);
void setMqttPort(uint16_t port);