#pragma once
#include <stdint.h>

struct SensorField2
{
    const char *name;
    const char *description;
    const char *unit;
    const char *deviceClass;
    float (*extract)(const uint8_t *b);
    const uint8_t decimals;
    const float min;
    const float max;
    const float step;
    bool hasCommand;
    bool isSwitch;
};
extern void publishSensors(const SensorField2 sensorMap[], const uint8_t *buffer, float lastValues[], bool discoveryPublished[], const int length);