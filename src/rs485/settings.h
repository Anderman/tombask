struct SettingField {
    float value;
    const char* name;
    const char* description;
    const char* unit;
    float min;
    float max;
    float step;
};

struct Settings {
    SettingField SetpointTemp;
    SettingField LegionellaTemp;
    SettingField Fan;
};
extern char deviceId[32];

extern Settings settings; 
