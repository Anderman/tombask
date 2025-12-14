#include "globals.h"
#include "secrets.h"
unsigned long lastMqttAttempt = 0;
const unsigned long mqttRetryInterval = 10000; // 10s
boolean tryConnect()
{
    char clientId[32];
    snprintf(clientId, sizeof(clientId), "tombask-esp-%08X", (uint32_t)ESP.getEfuseMac());
    return (mqttClient.connect(clientId, MQTT_USER, MQTT_PASS));
}

void ensureConnect()
{
    if (!mqttClient.connected() && (millis() - lastMqttAttempt > mqttRetryInterval))
    {
        if (tryConnect())
        {
            sendDiscovery();
        }
    }
}

void mqttLoop(){
    ensureConnect();
    mqttClient.loop();
}