#include <Arduino.h>
#include "rs485_25_5.h"
#include <cstdint>
#include "rs485.h"
#include "sensors.h"
struct __attribute__((packed)) Frame25_5
{
    uint8_t start1;        // 7E 7E F0 F0 03 25 90 31 05 00 00 00 0A 0A 00 00 00 00 00 00 00 00 01 3F 00 00 00 00 00 02 00 00 00 00 00 00 6A 00 00 00 14 19
    uint8_t start2;        // 7E F0 F0 03 25 90 31 05 00 00 00 0A 0A 00 00 00 00 00 00 00 00 01 3F 00 00 00 00 00 02 00 00 00 00 00 00 6A 00 00 00 14 19
    uint8_t dst;           // F0 F0 03 25 90 31 05 00 00 00 0A 0A 00 00 00 00 00 00 00 00 01 3F 00 00 00 00 00 02 00 00 00 00 00 00 6A 00 00 00 14 19
    uint8_t src;           // F0 03 25 90 31 05 00 00 00 0A 0A 00 00 00 00 00 00 00 00 01 3F 00 00 00 00 00 02 00 00 00 00 00 00 6A 00 00 00 14 19
    uint8_t type;          // 03 25 90 31 05 00 00 00 0A 0A 00 00 00 00 00 00 00 00 01 3F 00 00 00 00 00 02 00 00 00 00 00 00 6A 00 00 00 14 19
    uint8_t lengte;        // 25 90 31 05 00 00 00 0A 0A 00 00 00 00 00 00 00 00 01 3F 00 00 00 00 00 02 00 00 00 00 00 00 6A 00 00 00 14 19
    uint16_t protocol;     // 90 31 05 00 00 00 0A 0A 00 00 00 00 00 00 00 00 01 3F 00 00 00 00 00 02 00 00 00 00 00 00 6A 00 00 00 14 19
    uint8_t func;          // 05 00 00 00 0A 0A 00 00 00 00 00 00 00 00 01 3F 00 00 00 00 00 02 00 00 00 00 00 00 6A 00 00 00 14 19
    uint8_t a9;            // 22 22 01 0a 0a 00 00 00 22 22 01 01 10 01 7d 00 00 00 00 aa 06 01 00 00 00 00 00 6a 6d 00 97 14 19
    uint8_t comp1;         // 22 01 0a 0a 00 00 00 22 22 01 01 10 01 7d 00 00 00 00 aa 06 01 00 00 00 00 00 6a 6d 00 97 14 19
    uint8_t a11;           // 01 0a 0a 00 00 00 22 22 01 01 10 01 7d 00 00 00 00 aa 06 01 00 00 00 00 00 6a 6d 00 97 14 19
    uint8_t a12;           // 0a 0a 00 00 00 22 22 01 01 10 01 7d 00 00 00 00 aa 06 01 00 00 00 00 00 6a 6d 00 97 14 19
    uint8_t a13;           // 0a 00 00 00 22 22 01 01 10 01 7d 00 00 00 00 aa 06 01 00 00 00 00 00 6a 6d 00 97 14 19
    uint8_t a14;           // 00 00 00 22 22 01 01 10 01 7d 00 00 00 00 aa 06 01 00 00 00 00 00 6a 6d 00 97 14 19
    uint8_t a15;           // 00 00 22 22 01 01 10 01 7d 00 00 00 00 aa 06 01 00 00 00 00 00 6a 6d 00 97 14 19
    uint8_t a16;           // 00 22 22 01 01 10 01 7d 00 00 00 00 aa 06 01 00 00 00 00 00 6a 6d 00 97 14 19
    uint8_t a17;           // 22 22 01 01 10 01 7d 00 00 00 00 aa 06 01 00 00 00 00 00 6a 6d 00 97 14 19
    uint8_t a18;           // 22 01 01 10 01 7d 00 00 00 00 aa 06 01 00 00 00 00 00 6a 6d 00 97 14 19
    uint8_t a19;           // 01 01 10 01 7d 00 00 00 00 aa 06 01 00 00 00 00 00 6a 6d 00 97 14 19
    uint16_t a20_;          // 01 10 01 7d 00 00 00 00 aa 06 01 00 00 00 00 00 6a 6d 00 97 14 19
    uint8_t a22;           // 01 7d 00 00 00 00 aa 06 01 00 00 00 00 00 6a 6d 00 97 14 19
    uint8_t condensationP; // 7d 00 00 00 00 aa 06 01 00 00 00 00 00 6a 6d 00 97 14 19
    uint8_t a24;           // 00 00 00 00 aa 06 01 00 00 00 00 00 6a 6d 00 97 14 19
    uint8_t a25;           // 00 00 00 aa 06 01 00 00 00 00 00 6a 6d 00 97 14 19
    uint8_t a26;           // 00 00 aa 06 01 00 00 00 00 00 6a 6d 00 97 14 19
    uint16_t fanSpeed;     // 00 aa 06 01 00 00 00 00 00 6a 6d 00 97 14 19
    uint8_t a29;           // 06 01 00 00 00 00 00 6a 6d 00 97 14 19
    uint8_t a30;           // 01 00 00 00 00 00 6a 6d 00 97 14 19
    uint8_t a31;           // 00 00 00 00 00 6a 6d 00 97 14 19
    uint8_t a32;           // 00 00 00 00 6a 6d 00 97 14 19
    uint8_t a33;           // 00 00 00 6a 6d 00 97 14 19
    uint8_t a34;           // 00 00 6a 6d 00 97 14 19
    uint8_t a35;           // 00 6a 6d 00 97 14 19
    uint8_t a36;           // 6a 6d 00 97 14 19
    uint8_t a37;           // 6d 00 97 14 19
    uint16_t Exv2;         // 00 97 14 19
    uint8_t a40;           // 14 19
    uint8_t a41;           // 19
};

static float extract_P1(const uint8_t *b) { return b[20] * 256 + b[21]; }
static float extract_P3(const uint8_t *b) { return b[27] * 256 + b[28]; }
static float extract_P4(const uint8_t *b) { return b[38] * 256 + b[39]; }
#define EXTRACT_BYTE(idx) [](const uint8_t *b) -> float { return b[idx]; }

static const SensorField2 sensorMap[] = {
    {"25_5_9", "compressor1 (target)", "Hz", "frequency", EXTRACT_BYTE(9)},
    {"Comp1", "compressor1", "Hz", "frequency", EXTRACT_BYTE(10)},
    {"25_5_11", "25_5[11] state", "", "", EXTRACT_BYTE(11)},
    {"climate/action", "25_5[11] state", "", "", EXTRACT_BYTE(11)},
    {"climate/mode", "25_5[11] state", "", "", EXTRACT_BYTE(11)},
    {"25_5_12", "25_5[12]", "", "", EXTRACT_BYTE(12)},
    {"25_5_13", "25_5[13]", "", "", EXTRACT_BYTE(13)},
    {"25_5_14", "25_5[14]", "", "", EXTRACT_BYTE(14)},
    {"25_5_15", "25_5[15]", "", "", EXTRACT_BYTE(15)},
    {"25_5_16", "25_5[16]", "", "", EXTRACT_BYTE(16)},
    {"25_5_17", "Compressor2 (target)", "Hz", "frequency", EXTRACT_BYTE(17)},
    {"25_5_18", "Compressor2", "Hz", "frequency", EXTRACT_BYTE(18)},
    {"25_5_19", "25_5[19] state", "", "", EXTRACT_BYTE(19)},
    {"25_5_20", "Pressure 1", "", "pressure", extract_P1},
    {"25_5_22", "25_5[22]", "", "", EXTRACT_BYTE(22)},
    {"25_5_23", "comp related", "", "", EXTRACT_BYTE(23), 1}, 
    {"25_5_24", "25_5[24]", "", "", EXTRACT_BYTE(24)},
    {"25_5_25", "25_5[25]", "", "", EXTRACT_BYTE(25)},
    {"25_5_26", "25_5[26]", "", "", EXTRACT_BYTE(26)},
    {"25_5_27", "power in 1", "W", "power", extract_P3},
    {"25_5_29", "25_5[29] state", "", "", EXTRACT_BYTE(29)},
    {"25_5_30", "25_5[30] state", "", "", EXTRACT_BYTE(30)},
    {"25_5_31", "25_5[31]", "", "", EXTRACT_BYTE(31)},
    {"25_5_32", "25_5[32]", "", "", EXTRACT_BYTE(32)},
    {"25_5_33", "25_5[33]", "", "", EXTRACT_BYTE(33)},
    {"25_5_34", "25_5[34]", "", "", EXTRACT_BYTE(34)},
    {"25_5_35", "25_5[35]", "", "", EXTRACT_BYTE(35)},
    {"25_5_36", "25_5[36]", "", "", EXTRACT_BYTE(36)},
    {"25_5_37", "25_5[37] temp?", "", "", EXTRACT_BYTE(37)},
    {"25_5_38", "Power in 2", "W", "power", extract_P4},
    {"25_5_40", "25_5[40]", "", "", EXTRACT_BYTE(40)},
    {"25_5_41", "25_5[41]", "", "", EXTRACT_BYTE(41)},
};

void read_25_5(const uint8_t *buffer)
{
    static const int length = sizeof(sensorMap) / sizeof(sensorMap[0]);
    static float lastValues[length] = {0};
    static bool discoveryPublished[length] = {0};

    publishSensors(sensorMap, buffer, lastValues, discoveryPublished, length);
}