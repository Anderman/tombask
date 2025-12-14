#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h>
#include <cstdio>
#include "secrets.h"
#include "mtqq/discovery.h"
#include "mtqq/setup.h"
#include "mtqq/mqttLoop.h"
#include "Ota.h"
#include "wifiSetup.h"
#include "rs485/rs485.h"
#include "storage/storage.h"

// ==== GLOBALS ====
WiFiClient espClient;
PubSubClient mqttClient(espClient);
unsigned long lastPublish = 0;

void setup()
{
  loadDeviceId();
  Serial.begin(115200);
  setupWifi();
  setupOTA();
  setupMtqq();
  setupRs485();
}

void loop()
{
  ArduinoOTA.handle();
  mqttLoop();
  rs485Loop();
  
  // unsigned long now = millis();
  // if (now - lastPublish > 10000)
  // { // every 10s
  //   lastPublish = now;

  //   float t1 = readTemperature1();
  //   float t2 = readTemperature2();

  //   char topic[64];
  //   char payload[32];

  //   snprintf(topic, sizeof(topic), "%s/t1", MQTT_BASE_TOPIC);
  //   dtostrf(t1, 0, 1, payload);
  //   mqttClient.publish(topic, payload, true);

  //   snprintf(topic, sizeof(topic), "%s/t2", MQTT_BASE_TOPIC);
  //   dtostrf(t2, 0, 1, payload);
  //   mqttClient.publish(topic, payload, true);
  // }
}
