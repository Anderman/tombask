struct SensorField {
    float value;
    const char* name;
    const char* description;
    const char* unit;
};


struct Sensors {
    SensorField Tin;
    SensorField Tout;
    SensorField druk_hoog;
    SensorField druk_laag;
};

extern Sensors sensors; 
