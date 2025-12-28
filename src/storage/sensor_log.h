// Sla alle logs op naar SPIFFS/NVS
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
void registerAllSensorLogs();
class SensorLog
{
    friend void saveAllSensorLogs();
    friend void loadAllSensorLogs();

public:
    SensorLog(const char *name, size_t maxEntries = 256);
    void log(int16_t value);
    // Only logs if value changed since last log
    void logIfChanged(int16_t value);
    const std::vector<SensorLogEntry> &entries() const;
    void clear();
    const char *getName() const;

protected:
    std::vector<SensorLogEntry> _entries;
    size_t _maxEntries;
    const char *_name;
};

// Globale registry voor logs
void registerSensorLog(SensorLog *log);
SensorLog *getSensorLog(const char *name);
const std::vector<SensorLog *> &allSensorLogs();
