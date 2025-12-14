#pragma once
#include <cstdint>

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
void setWifiSsid(const char* ssid);
// Set to "" to clear. For web UI: empty payload can be treated as "keep unchanged" at API layer.
void setWifiPassword(const char* password);