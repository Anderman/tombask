#include "globals.h"
#include "secrets.h"
#include <WiFi.h>
#include "rs485/settings.h" // for publish()
#include "rs485/sensors.h"
#include "storage/storage.h"
unsigned long lastMqttAttempt = -10000;
const unsigned long mqttRetryInterval = 1000; // 10s
boolean tryConnect()
{
    char clientId[32];
    snprintf(clientId, sizeof(clientId), "tombask-esp-%08X", (uint32_t)ESP.getEfuseMac());

    const char *user = getMqttUser();
    const char *pass = getMqttPassword();

    if (user && user[0] != '\0')
        return mqttClient.connect(clientId, user, pass);
    return mqttClient.connect(clientId);
}

static void onMqttConnected()
{
    mqttClient.subscribe("tombask/tombask1/+/set");
    Serial.println("Subscribed to all topics (#) after reconnect");
}

void ensureConnect()
{
    if (WiFi.status() != WL_CONNECTED)
        return;

    if (!mqttClient.connected() && ((millis() - lastMqttAttempt) > mqttRetryInterval))
    {
        lastMqttAttempt = millis();
        if (tryConnect())
        {
            onMqttConnected();
        }
    }
}
void mqttLoop()
{
    ensureConnect();
    mqttClient.loop();
}