#pragma once
#include <PubSubClient.h>
static const byte TX_PIN  = 8; // XIAO D8 = GPIO8 (ESP TX -> RS485 module RX/DI)
static const byte RX_PIN  = 9; // XIAO D9 = GPIO9 (ESP RX <- RS485 module TX/RO)

extern PubSubClient mqttClient;