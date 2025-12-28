#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include "sensor_log.h"
#include <algorithm>
#include <time.h>

// Forward declarations for logs in each frame file
SensorLog topTempLog("TTop");
SensorLog bottomTempLog("TBottom");

// Add more externs as needed for other frame logs (e.g., 2d, 1c, etc.)

void registerAllSensorLogs()
{
    registerSensorLog(&topTempLog);
    registerSensorLog(&bottomTempLog);
    // Add more registerSensorLog calls for other logs here
}

static std::vector<SensorLog *> _allLogs;

SensorLog::SensorLog(const char *name, size_t maxEntries)
    : _maxEntries(maxEntries), _name(name) {}

void SensorLog::log(int16_t value)
{
    u_int32_t timestamp = time(nullptr); // epoch seconds
    if (_entries.size() >= _maxEntries)
    {
        _entries.erase(_entries.begin()); // ringbuffer: verwijder oudste
    }
    _entries.push_back({timestamp, value});
}

void SensorLog::logIfChanged(int16_t value)
{
    if (_entries.empty() || _entries.back().value != value)
    {
        log(value);
    }
}

const std::vector<SensorLogEntry> &SensorLog::entries() const
{
    return _entries;
}

void SensorLog::clear()
{
    _entries.clear();
}

const char *SensorLog::getName() const
{
    return _name;
}

void registerSensorLog(SensorLog *log)
{
    if (std::find(_allLogs.begin(), _allLogs.end(), log) == _allLogs.end())
        _allLogs.push_back(log);
}

SensorLog *getSensorLog(const char *name)
{
    for (auto *log : _allLogs)
    {
        if (strcmp(log->getName(), name) == 0)
            return log;
    }
    return nullptr;
}

const std::vector<SensorLog *> &allSensorLogs()
{
    return _allLogs;
}

// Helper: log opslaan in NVS (Preferences) per log
void saveAllSensorLogs()
{
    Preferences prefs;
    for (auto *log : allSensorLogs())
    {
        String nvsKey = log->getName();
        if (!prefs.begin(nvsKey.c_str(), false))
        {
            Serial.print("[sensor_log] ERROR: NVS open failed for namespace: ");
            Serial.println(nvsKey);
            continue;
        }
        size_t n = log->_entries.size();
        if (n > 0)
        {
            prefs.putBytes("data", log->_entries.data(), n * sizeof(SensorLogEntry));
            prefs.putUInt("count", n);
        }
        else
        {
            prefs.remove("data");
            prefs.putUInt("count", 0);
        }
        prefs.end();
    }
}

// Helper: log teruglezen uit NVS (Preferences) per log
void loadAllSensorLogs()
{
    Preferences prefs;
    for (auto *log : allSensorLogs())
    {
        String nvsKey = log->getName();
        if (!prefs.begin(nvsKey.c_str(), true))
        {
            Serial.print("[sensor_log] ERROR: NVS open failed for namespace: ");
            Serial.println(nvsKey);
            continue;
        }
        size_t n = prefs.getUInt("count", 0);
        log->_entries.clear();
        if (n > 0)
        {
            log->_entries.resize(n);
            prefs.getBytes("data", log->_entries.data(), n * sizeof(SensorLogEntry));
        }
        Serial.printf("[sensor_log] Loaded %u entries for log ", n);
        Serial.println(nvsKey);
        prefs.end();
    }
}
