#include "secrets.h"
#include "globals.h"
void setupMtqq()
{
    mqttClient.setServer(MQTT_HOST, MQTT_PORT);
}