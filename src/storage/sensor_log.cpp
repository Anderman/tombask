#include <FS.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include "sensor_log.h"
#include <time.h>
#include <set>

// Forward declarations for logs in each frame file
SensorLog topTempLog("TTop");
SensorLog bottomTempLog("TBottom");
static SensorLog *const allLogs[] = {&topTempLog, &bottomTempLog};

SensorLog::SensorLog(const char *name, size_t maxEntries)
    : _maxEntries(maxEntries), name(name) {}

void SensorLog::log(int16_t value)
{
    u_int32_t timestamp = time(nullptr); // epoch seconds
    while (entries.size() >= _maxEntries)
    {
        entries.erase(entries.begin()); // ringbuffer: verwijder oudste
    }
    entries.push_back({timestamp, value});
}

void SensorLog::logIfChanged(int16_t logValue)
{
    const size_t bufferSize = 5;
    _lastValues.push_back(logValue);
    if (_lastValues.size() > bufferSize)
    {
        _lastValues.erase(_lastValues.begin());
    }

    if (_lastValues.size() == 1)
    {
        return;
    }
    const int16_t prevLogValue = _lastValues[_lastValues.size() - 2];
    size_t n = entries.size();
    if (n == 0)
    {
        log(prevLogValue);
        return;
    }

    int16_t last = entries.back().value;
    if (prevLogValue == last)
    {
        return;
    }

    // Flip-detectie: check of buffer alleen uit 2 waarden bestaat en die afwisselt
    std::set<int16_t> uniqueVals(_lastValues.begin(), _lastValues.end());

    bool mustLog = false;
    if (n >= 2 && uniqueVals.size() != 1)
    {
        // Flip-detectie: als we flippen tussen 2 waarden, log niet
        int16_t prev = entries[n - 2].value;
        mustLog = (logValue != last && logValue != prev);
        if (prevLogValue == prev && !mustLog)
        {
            return;
        }
    }
    if (entries.empty() || last != prevLogValue || mustLog)
    {
        log(prevLogValue);
        return;
    }
}

// Helper: log opslaan in NVS (Preferences) per log
void saveAllSensorLogs()
{
    Preferences prefs;
    for (auto *log : allLogs)
    {
        String nvsKey = log->name;
        if (!prefs.begin(nvsKey.c_str(), false))
        {
            Serial.print("[sensor_log] ERROR: NVS open failed for namespace: ");
            Serial.println(nvsKey);
            continue;
        }
        size_t n = log->entries.size();
        {
            prefs.putBytes("data", log->entries.data(), n * sizeof(SensorLogEntry));
            prefs.putUInt("count", n);
        }
        prefs.end();
    }
}

// Helper: log teruglezen uit NVS (Preferences) per log
void loadAllSensorLogs()
{
    Preferences prefs;
    for (auto *log : allLogs)
    {
        String nvsKey = log->name;
        if (!prefs.begin(nvsKey.c_str(), true))
        {
            Serial.print("[sensor_log] ERROR: NVS open failed for namespace: ");
            Serial.println(nvsKey);
            continue;
        }
        size_t n = prefs.getUInt("count", 0);
        log->entries.clear();
        if (n > 0)
        {
            log->entries.resize(n);
            prefs.getBytes("data", log->entries.data(), n * sizeof(SensorLogEntry));
        }
        Serial.printf("[sensor_log] Loaded %u entries for log ", n);
        Serial.println(nvsKey);
        prefs.end();
    }
}
