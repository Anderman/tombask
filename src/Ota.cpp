#include <ArduinoOTA.h>
#include <globals.h>
#include <secrets.h>
void setupOTA()
{
    ArduinoOTA.setHostname(HOSTNAME);
    ArduinoOTA.setPasswordHash(OTA_HASH);
    ArduinoOTA.begin();
}
