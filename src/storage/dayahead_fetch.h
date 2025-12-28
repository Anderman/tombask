#pragma once
#include <cstdint>
#include <cstddef>
struct DayAheadPrices
{
    uint32_t startEpoch;
    float prices[192];
    size_t count;
    bool valid;
};
DayAheadPrices fetchDayAheadFromEntsoe();

// Haal de day-ahead prijzen op, eerst uit cache, anders fetch
DayAheadPrices getDayAheadPrices();
