
#include "dayahead_api.h"
#include <ArduinoJson.h>
#include <WebServer.h>
#include "webui.h"
#include "storage/dayahead_storage.h"
#include "storage/dayahead_fetch.h"
#include <time.h>
#include "../secrets.h"
#include <WiFiClientSecure.h>

extern WebServer server;

void dayAheadToJson(const DayAheadPrices& prices, JsonDocument& doc);
// Dummy implementation: returns a static price array for demo
void handleGetDayAheadPrices() {
    // Probeer eerst uit cache
    DayAheadPrices prices = getDayAheadPrices();
    JsonDocument doc;
    dayAheadToJson(prices, doc);
    String out;
    serializeJson(doc, out);
    server.send(200, "application/json", out);
    return;
}

void dayAheadToJson(const DayAheadPrices& prices, JsonDocument& doc) {
    JsonArray arr = doc["prices"].to<JsonArray>();
    for (size_t i = 0; i < prices.count; ++i) {
        time_t ts = prices.startEpoch + i*15*60;
        struct tm *tm_ts = gmtime(&ts);
        char iso[25];
        strftime(iso, sizeof(iso), "%Y-%m-%dT%H:%M:%SZ", tm_ts);
        JsonArray entry = arr.add<JsonArray>();
        entry.add(String(iso));
        entry.add(prices.prices[i]);
    }
}

