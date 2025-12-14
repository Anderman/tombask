#pragma once
#include <cstdint>

// Example secrets file. Copy this to 'secrets.h' and fill in
// your real WiFi and MQTT credentials. DO NOT commit secrets.h.

static const char* WIFI_SSID     = "YOUR_WIFI_SSID";
static const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

static const char* MQTT_HOST     = "192.168.2.4";   // Home Assistant / Mosquitto
static const uint16_t MQTT_PORT  = 1883;
static const char* MQTT_USER     = "mqtt_user";
static const char* MQTT_PASS     = "mqtt_pass";

static const char* MQTT_BASE_TOPIC = "tombask/esp";  // e.g. tombask/esp/t1
