#include "secrets.h"
#include "globals.h"
#include "storage/storage.h"
#include "mqtt/receiver.h"

void setupMqtt()
{
    const char *host = getMqttHost();
    uint16_t port = getMqttPort();
    if (!host || host[0] == '\0')
        host = MQTT_HOST;
    if (port == 0)
        port = MQTT_PORT;
    mqttClient.setServer(host, port);

    // Home Assistant discovery payloads are often >256 bytes.
    // PubSubClient defaults to 256 which makes publish() fail silently.
    mqttClient.setBufferSize(1024);
    mqttClient.setKeepAlive(30);
    mqttClient.setSocketTimeout(5);

    mqttClient.setCallback(onMqttMessage);
}
