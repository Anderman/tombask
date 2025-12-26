#include "../rs485/sensors.h"

extern void publishSensors(const SensorField2 sensorMap[], const uint8_t *buffer, float lastValues[], bool discoveryPublished[], const int length);