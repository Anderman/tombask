#include "../rs485/sensors.h"

extern char discoveryTopic[128];
extern char deviceIdStr[64];
extern char deviceJson[128];
extern char uniqueId[64];
extern char deviceClass[64];
extern char unitOfMeasurement[64];
extern char cmdTopic[128];
extern char stateTopic[128];

extern void SetProperties(const SensorField2 &field);
extern void SetDiscoveryTopic(const SensorField2 &field);
extern void ensureDeviceInfo();