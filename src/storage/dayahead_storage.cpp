#include "dayahead_storage.h"
#include <Preferences.h>
#include <cstring>
#include "dayahead_fetch.h"

static Preferences prefs;

struct DayAheadBinary {
    uint32_t startEpoch;
    float prices[192]; // maximaal 2 dagen kwartierprijzen
};

void saveDayAheadPrices(const DayAheadPrices& prices) {
    prefs.begin("_", false);
    prefs.putBytes("dayahead_bin", &prices, sizeof(prices));
    prefs.end();
}

bool loadDayAheadPrices(DayAheadPrices& prices) {
    prefs.begin("_", true);
    size_t n = prefs.getBytes("dayahead_bin", &prices, sizeof(prices));
    prefs.end();
    prices.valid = (n == sizeof(prices) && prices.startEpoch != 0);
    // Count nonzero prices
    prices.count = 0;
    for (size_t i = 0; i < 192; ++i) {
        if (prices.prices[i] == 0.0f) break;
        prices.count++;
    }
    return prices.valid;
}
