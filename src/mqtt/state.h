#include "../rs485/sensors.h"
extern void publishSensorState(const SensorField2 &field, const float value);

void Publish(const char *valueStr);
