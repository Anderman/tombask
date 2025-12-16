#include "secrets.h"
#include "globals.h"
#include "storage/storage.h"
void setupMtqq()
{
    const char* host = getMqttHost();
    uint16_t port = getMqttPort();
    if (!host || host[0] == '\0')
        host = MQTT_HOST;
    if (port == 0)
        port = MQTT_PORT;
    mqttClient.setServer(host, port);
}

