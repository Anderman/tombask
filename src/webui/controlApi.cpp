#include "webui.h"
#include "controlApi.h"
#include <ArduinoJson.h>
#include <WebServer.h>
#include "webui.h"
#include "../rs485/settings.h"

extern WebServer server;
extern ControlValues controlValues;
template <typename T> bool getValue(const char *key, T &value);

void handlePostSetpoint()
{
    uint8_t setpointTemp;
    if (!getValue("SetpointTemp", setpointTemp))
        return;
    controlValues.SetpointTemp = setpointTemp;
    ControlValueChanged = true;
    server.send(204);
}

void handlePostFan()
{
    bool fan;
    if (!getValue("Fan", fan))
        return;
    controlValues.Fan = fan;
    ControlValueChanged = true;
    server.send(204);
}

void handlePostBoost()
{
    bool boost;
    if (!getValue("Boost", boost))
        return;
    controlValues.Boost = boost;
    ControlValueChanged = true;
    server.send(204);
}

void handlePostLegionellaOn()
{
    bool legionellaOn;
    if (!getValue("LegionellaOn", legionellaOn))
        return;
    controlValues.LegionellaOn = legionellaOn;
    ControlValueChanged = true;
    server.send(204);
}

void handlePostLegionellaHour()
{
    uint8_t legionellaHour;
    if (!getValue("LegionellaHour", legionellaHour))
        return;
    controlValues.LegionellaHour = legionellaHour;
    ControlValueChanged = true;
    server.send(204);
}

void handlePostLegionellaTemp()
{
    uint8_t legionellaTemp;
    if (!getValue("LegionellaTemp", legionellaTemp))
        return;
    controlValues.LegionellaTemp = legionellaTemp;
    ControlValueChanged = true;
    server.send(204);
}

// Generieke getValue template voor elk type
template<typename T>
bool getValue(const char* key, T &value)
{
    JsonDocument doc;
    if (!parseJsonBody(doc))
        return false;
    if (!doc[key].is<T>())
    {
        server.send(400, "text/plain", "Missing or invalid value");
        return false;
    }
    value = doc[key].as<T>();
    return true;
}

void handleGetStatus()
{
    JsonDocument doc;
    doc["SetpointTemp"] = controlValues.SetpointTemp;
    doc["LegionellaTemp"] = controlValues.LegionellaTemp;
    doc["TopTemp"] = controlValues.TopTemp;
    doc["BottomTemp"] = controlValues.BottomTemp;
    doc["Power"] = controlValues.Power;
    doc["Heating"] = controlValues.Heating;
    doc["Fan"] = controlValues.Fan;
    doc["Boost"] = controlValues.Boost;
    doc["LegionellaHour"] = controlValues.LegionellaHour;
    doc["LegionellaOn"] = controlValues.LegionellaOn;
    String out;
    serializeJson(doc, out);
    server.send(200, "application/json", out);
}
