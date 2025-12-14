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
  Serial.print("[tombask] deviceId=");
  Serial.println(deviceId);
  setupWifi();
  Serial.print("[tombask] wifi status=");
  Serial.println(WiFi.status());
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.print("[tombask] ip=");
    Serial.println(WiFi.localIP());
  }
  setupWebUi();
  Serial.println("[tombask] webui started (http://<ip>/)");
  setupOTA();
  setupMtqq();
  setupRs485();
  setupWifiConfig();
  Serial.println("[tombask] setup done");
}

void loop()
{
  ArduinoOTA.handle();
  webUiLoop();
  wifiManagerLoop();
  mqttLoop();
  rs485Loop();
}
