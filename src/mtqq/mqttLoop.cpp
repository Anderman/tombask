#include "globals.h"
#include "secrets.h"
#include "rs485/settings.h" // for publish()
#include "rs485/sensors.h"
unsigned long lastMqttAttempt = 0;
const unsigned long mqttRetryInterval = 10000; // 10s
boolean tryConnect()
{
    char clientId[32];
    snprintf(clientId, sizeof(clientId), "tombask-esp-%08X", (uint32_t)ESP.getEfuseMac());
    return (mqttClient.connect(clientId, MQTT_USER, MQTT_PASS));
}

static void onMqttConnected()
{
    publishSettings(); // settings (number/switch)
    publishSensors();  // sensors discovery
}

void ensureConnect()
{
    if (!mqttClient.connected() && (millis() - lastMqttAttempt > mqttRetryInterval))
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