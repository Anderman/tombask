#include <WiFi.h>
#include "secrets.h"
#include <WiFiManager.h>
#include "storage/storage.h"

static WiFiManager g_wifiManager;
static bool g_wifiPortalActive = false;
void setupWifi()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    WiFiManager wifiManager;
    wifiManager.autoConnect("Tombask-Setup");
  }
}

void setupWifiConfig()
{
    // Start WiFiManager web portal on a secondary port so it won't conflict with your main web UI on :80.
    // NOTE: startWebPortal is non-blocking, but it still needs wifiManagerLoop() calling process().
    if (WiFi.status() != WL_CONNECTED)
    {
      g_wifiPortalActive = false;
      return;
    }

    g_wifiManager.setConfigPortalBlocking(false);
    g_wifiManager.setHttpPort(8080);
    g_wifiManager.startWebPortal();
    g_wifiPortalActive = true;
}

void wifiManagerLoop()
{
  if (!g_wifiPortalActive) return;

  g_wifiManager.process();

  // If the portal closed, stop processing.
  if (!g_wifiManager.getWebPortalActive() && !g_wifiManager.getConfigPortalActive())
  {
    g_wifiPortalActive = false;
  }
}