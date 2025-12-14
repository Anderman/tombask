#include <WiFi.h>
#include "secrets.h"
#include <WiFiManager.h>
void setupWifi()
{
  WiFiManager wifiManager;
  wifiManager.autoConnect("Tombask-Setup");
}