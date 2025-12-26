#include "../rs485/sensors.h"
#include "discovery.h"
#include "state.h"

void publishSensors(const SensorField2 sensorMap[], const uint8_t *buffer, float lastValues[], bool discoveryPublished[], const int length)
{
    for (int i = 0; i < length; ++i)
    {
        const SensorField2 &s = sensorMap[i];
        float value = s.extract(buffer);
        if (!discoveryPublished[i])
        {
            publishSensorDiscovery(s);
        }
        if (value != lastValues[i] || !discoveryPublished[i])
        {
            publishSensorState(s, value);
            lastValues[i] = value;
            discoveryPublished[i] = true;
        }
    }
}
