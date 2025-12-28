// Simple mapping from browser timeZone to POSIX TZ string for ESP32
// Only most common world zones, easily extendable
// Usage: const char* posix = lookupPosixTz(browserTzName);

#include <cstring>
struct TzMapEntry {
    const char* browserTz;
    const char* posixTz;
};

static const TzMapEntry tzMap[] = {
    // Europe
    {"Europe/Amsterdam", "CET-1CEST,M3.5.0,M10.5.0/3"},
    {"Europe/Berlin",    "CET-1CEST,M3.5.0,M10.5.0/3"},
    {"Europe/Brussels",  "CET-1CEST,M3.5.0,M10.5.0/3"},
    {"Europe/Paris",     "CET-1CEST,M3.5.0,M10.5.0/3"},
    {"Europe/London",    "GMT0BST,M3.5.0/1,M10.5.0"},
    {"Europe/Moscow",    "MSK-3"},
    // US
    {"America/New_York",    "EST5EDT,M3.2.0/2,M11.1.0/2"},
    {"America/Chicago",     "CST6CDT,M3.2.0/2,M11.1.0/2"},
    {"America/Denver",      "MST7MDT,M3.2.0/2,M11.1.0/2"},
    {"America/Los_Angeles", "PST8PDT,M3.2.0/2,M11.1.0/2"},
    // Asia
    {"Asia/Tokyo",      "JST-9"},
    {"Asia/Shanghai",   "CST-8"},
    {"Asia/Kolkata",    "IST-5:30"},
    {"Asia/Dubai",      "GST-4"},
    // Australia
    {"Australia/Sydney", "AEST-10AEDT,M10.1.0,M4.1.0/3"},
    {"Australia/Perth",  "AWST-8"},
    // UTC fallback
    {"UTC",             "UTC0"},
    {"Etc/UTC",         "UTC0"},
};

inline const char* lookupPosixTz(const char* browserTz) {
    for (unsigned i = 0; i < sizeof(tzMap)/sizeof(tzMap[0]); ++i) {
        if (strcmp(browserTz, tzMap[i].browserTz) == 0) {
            return tzMap[i].posixTz;
        }
    }
    return "UTC0"; // fallback
}
