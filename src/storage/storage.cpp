#include <rs485/settings.h>
#include <Preferences.h>

Preferences prefs;

void saveDeviceId()
{
    prefs.begin("_", false);
    prefs.putString("deviceId", deviceId);
    prefs.end();
}

void loadDeviceIdNVS()
{
    prefs.begin("_", true);
    prefs.getString("deviceId", "").toCharArray(deviceId, sizeof(deviceId));
    prefs.end();
}
