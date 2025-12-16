struct SensorField {
    const char* name;
    const char* description;
    const char* unit;
    const char* deviceClass;
};


struct Sensors {
    SensorField T1;
    SensorField T2;
    SensorField T3;
    SensorField T4;
    SensorField T5;
    SensorField T6;
    SensorField T7;
    SensorField counter1;
    SensorField Exv;
    SensorField compHz;
    SensorField p1;
    SensorField p2;
    SensorField SuctionPressure;
    SensorField CondensationPressure;
    SensorField FanSpeed;
};

extern Sensors sensors; 
void publishSensors();
void publishSensorsState();