#pragma once
#include <PubSubClient.h>

static constexpr uint8_t TX_PIN = 12;    // ESP TX -> RS485 module RX/DI
static constexpr uint8_t RX_PIN = 21;    // ESP RX <- RS485 module TX/RO
static constexpr uint8_t DE_RE_PIN = 18; // ESP DE/RE -> RS485 transceiver direction control
static constexpr uint8_t STATUS_LED_PIN = 45;

extern PubSubClient mqttClient;