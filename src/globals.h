#pragma once
#include <PubSubClient.h>
static const byte RX_PIN  = 16; // GPIO16
static const byte TX_PIN  = 17; // GPIO17

extern PubSubClient mqttClient;
void sendDiscovery();