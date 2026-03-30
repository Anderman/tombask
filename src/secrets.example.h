#pragma once

// Example secrets file. Copy to secrets.h and fill in your real values.

#include <cstdint>
#define WIFI_SSID     "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
#define HOSTNAME "Tombask"
#define OTA_PASSWORD     "password" 
static const char* MQTT_HOST     = "192.168.2.4";   // Home Assistant / Mosquitto
static const uint16_t MQTT_PORT  = 1883;
static const char* MQTT_USER     = "mqtt_user";
static const char* MQTT_PASS     = "mqtt_pass";

static const char* MQTT_BASE_TOPIC = "tombask/esp";  // e.g. tombask/esp/t1
static const char* ENSTSO_API_KEY = "key"; // Get your own API key from https://transparency.entsoe.eu/content/static_content/Static%20content/web%20api/Guide.html