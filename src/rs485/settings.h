#pragma once
#include <stdint.h>


struct ControlValues
{
    uint8_t SetpointTemp;
    uint8_t LegionellaTemp;
    uint8_t TopTemp;
    uint8_t BottomTemp;
    uint16_t Power ;
    bool Heating;
    bool Fan;
    bool Boost;
};
extern bool ControlValueChanged;
extern ControlValues controlValues;
