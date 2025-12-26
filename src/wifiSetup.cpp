#include <WiFi.h>
#include "secrets.h"
#include <WiFiManager.h>
#include "storage/storage.h"

static WiFiManager g_wifiManager;
  
void setupWifi()
{
char hostname[32];
snprintf(hostname, sizeof(hostname), "Tombask%d", deviceId);
WiFi.setHostname(hostname);  WiFi.setAutoReconnect(true);
  if (WiFi.status() != WL_CONNECTED)
  {
    // Blocking only during boot to get initial credentials.
    g_wifiManager.autoConnect("Tombask-Setup");
  }
}

void setupWifiConfigPage()
{
    // Start WiFiManager web portal on a secondary port so it won't conflict with your main web UI on :80.
    // NOTE: startWebPortal is non-blocking, but it still needs wifiManagerLoop() calling process().
    g_wifiManager.setConfigPortalBlocking(false);
    g_wifiManager.setHttpPort(8080);
    g_wifiManager.startWebPortal();
}

void wifiManagerLoop()
{
  g_wifiManager.process();
}
