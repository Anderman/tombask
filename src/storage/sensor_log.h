// Sla alle logs op naar NVS
#pragma once
#include <Arduino.h>
#include <vector>
#include <stdint.h>

struct SensorLogEntry
{
    uint32_t timestamp; // epoch tijd
    int16_t value;      // sensorwaarde
};

void saveAllSensorLogs();
void loadAllSensorLogs();
class SensorLog
{
    friend void saveAllSensorLogs();
    friend void loadAllSensorLogs();

public:
    SensorLog(const char *name, size_t maxEntries = 256);
    void logIfChanged(int16_t value);
    std::vector<SensorLogEntry> entries;
    const char *name;

protected:
    void log(int16_t value);
    size_t _maxEntries;
    std::vector<int16_t> _lastValues;
};
