#include <ArduinoJson.h>
#include "timesync_api.h"
#include <webui/webui.h>
#include "../storage/storage.h"
#include "tzmap.h"

extern char timezone[64];
void handleTimeZonePost()
{
    JsonDocument doc;
    if (parseJsonBody(doc) && doc["timezone"].is<const char*>())
    {
        const char* browserTz = doc["timezone"];
        const char* posixTz = lookupPosixTz(browserTz);
        strlcpy(timezone, posixTz, sizeof(timezone));
        saveConfig();
        server.send(200, "application/json", "{\"ok\":true}");
    }
    else
    {
        server.send(400, "application/json", "{\"error\":\"timezone string required\"}");
    }
}
