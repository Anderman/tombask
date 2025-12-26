#pragma once
#include <stdint.h>


struct ControlValues
{
    uint8_t SetpointTemp;
    uint8_t LegionellaTemp;
    bool Fan;
    bool Boost;
};
extern bool ControlValueChanged;
extern ControlValues controlValues;
