#pragma once
#include <stdint.h>

struct SettingField
{
    const char *name;
    const char *friendlyName;
    const char *unit;
    const char *type;
    uint16_t min;
    uint16_t max;
    uint16_t step;
};

struct Settings
{
    SettingField SetpointTemp;
    SettingField LegionellaTemp;
    SettingField Fan;
};

extern char deviceIdStr[64];
extern char deviceJson[128];
extern Settings settings;
extern void publishSettings();
extern void ensureDeviceInfo();