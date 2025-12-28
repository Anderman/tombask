#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h>
#include <cstdio>
#include "secrets.h"
#include "mqtt/setup.h"
#include "mqtt/mqttLoop.h"
#include "Ota.h"
#include "wifiSetup.h"
#include "rs485/rs485.h"
#include "storage/storage.h"
#include "storage/sensor_log.h"
#include "webui/webui.h"
#include "rs485/settings.h"

// ==== GLOBALS ====
WiFiClient espClient;
PubSubClient mqttClient(espClient);
unsigned long lastPublish = 0;

void setup()
{
  Serial.begin(115200);
  delay(2000);
  Serial.println();
  Serial.println("[tombask] boot");
  loadConfig();
  listNvsKeys();
  registerAllSensorLogs(); // Central registration for all logs
  loadAllSensorLogs();
  Serial.print("[tombask] deviceId=");
  Serial.println(deviceId);
  setupWifi();
  Serial.print("setupWifi done, IP=");
  Serial.print(WiFi.localIP());
  Serial.println();
  setupWebUi();
  setupWifiConfigPage();
  Serial.print("[tombask] webui started: http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  setupOTA();
  setupMqtt();
  setupRs485();
  Serial.println("[tombask] setup done");
}

void loop()
{
  static unsigned long lastLogSave = 0;
  ArduinoOTA.handle();
  webUiLoop();
  wifiManagerLoop();
  mqttLoop();
  rs485Loop();
  if (millis() - lastLogSave > 1UL * 60 * 1000) { // elke 1 min
    saveAllSensorLogs();
    lastLogSave = millis();
  }
}
