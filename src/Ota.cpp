#include <ArduinoOTA.h>
#include <globals.h>
#include <secrets.h>
void setupOTA()
{
    ArduinoOTA.setHostname(HOSTNAME);
    ArduinoOTA.setPassword(OTA_PASSWORD);
    ArduinoOTA.begin();
}
