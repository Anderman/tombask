#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h>
#include <cstdio>
#include "secrets.h"
#include "mqtt/setup.h"
#include "mqtt/mqttLoop.h"
#include "Ota.h"
#include "ota/auto_update.h"
#include "wifiSetup.h"
#include "rs485/rs485.h"
#include "storage/storage.h"
#include "storage/sensor_log.h"
#include "webui/webui.h"
#include "rs485/settings.h"
#include "status_led.h"

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

  statusLedSetup();
  // Boot indication (dim blue)
  statusLedSetRgb(0, 0, 16);

  loadConfig();
  listNvsKeys();
  loadAllSensorLogs();
  Serial.print("[tombask] deviceId=");
  Serial.println(deviceId);
  setupWifi();
  Serial.print("setupWifi done, IP=");
  Serial.print(WiFi.localIP());
  Serial.println();
  setupOTA();
  setupWebUi();
  setupAutoUpdate();
  setupWifiConfigPage();
  Serial.print("[tombask] webui started: http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  setupMqtt();
  setupRs485();
  Serial.println("[tombask] setup done");

  // Ready indication (dim green)
  statusLedSetRgb(0, 16, 0);
}

void loop()
{
  static unsigned long lastLogSave = 0;
  ArduinoOTA.handle();
  autoUpdateLoop();
  webUiLoop();
  wifiManagerLoop();
  mqttLoop();
  rs485Loop();
  if (millis() - lastLogSave > 1UL * 60 * 1000) { // elke 1 min
    saveAllSensorLogs();
    lastLogSave = millis();
  }
}
