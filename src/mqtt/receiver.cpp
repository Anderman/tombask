#include "rs485/settings.h"
#include "secrets.h"
#include "globals.h"
#include "storage/storage.h"
void onMqttMessage(char *topic, byte *payload, unsigned int length)
{
    payload[length] = '\0'; // Zorg dat de payload een geldige C-string is
    String topicStr(topic);
    String payloadStr((char *)payload);
    Serial.printf("MQTT ontvangen: topic=%s payload=%s\n", topic, payloadStr.c_str());
    if (topicStr == "tombask/tombask1/fan/set")
    {
        const bool newValue = (payloadStr == "ON");
        if (newValue != controlValues.Fan)
        {
            controlValues.Fan = newValue;
            ControlValueChanged = true;
        }
    }
    if (topicStr == "tombask/tombask1/legionellaOn/set")
    {
        const bool newValue = (payloadStr == "ON");
        if (newValue != controlValues.LegionellaOn)
        {
            controlValues.LegionellaOn = newValue;
            ControlValueChanged = true;
        }
    }
    if (topicStr == "tombask/tombask1/boost/set")
    {
        const bool newValue = (payloadStr == "ON");
        if (newValue != controlValues.Boost)
        {
            controlValues.Boost = newValue;
            ControlValueChanged = true;
        }
    }
    if (topicStr == "tombask/tombask1/Tsetpoint/set")
    {
        const uint8_t newValue = payloadStr.toInt();
        if (newValue != controlValues.SetpointTemp)
        {
            controlValues.SetpointTemp = newValue;
            ControlValueChanged = true;
        }
    }
    if (topicStr == "tombask/tombask1/2d_21/set")
    {
        const uint8_t newValue = payloadStr.toInt();
        if (newValue != controlValues.LegionellaHour)
        {
            controlValues.LegionellaHour = newValue;
            ControlValueChanged = true;
        }
    }
    if (topicStr == "tombask/tombask1/2d_43/set")
    {
        const uint8_t newValue = payloadStr.toInt();
        if (newValue != controlValues.LegionellaTemp)
        {
            controlValues.LegionellaTemp = newValue;
            ControlValueChanged = true;
        }
    }
}
