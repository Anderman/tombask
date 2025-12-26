#include <Arduino.h>
#include "rs485_2d.h"
#include <cstdint>
#include "rs485.h"
#include "Settings.h"
#include "sensors.h"

struct __attribute__((packed)) Frame2D
{
    uint8_t start1;     // 7E 7E F0 F0 04 2D 90 31 02 15 21 99 40 4A 02 C0 02 05 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t start2;     // 7E F0 F0 04 2D 90 31 02 15 21 99 40 4A 02 C0 02 05 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t dst;        // F0 F0 04 2D 90 31 02 15 21 99 40 4A 02 C0 02 05 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t src;        // F0 04 2D 90 31 02 15 21 99 40 4A 02 C0 02 05 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t type;       // 04 2D 90 31 02 15 21 99 40 4A 02 C0 02 05 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t lengte;     // 2D 90 31 02 15 21 99 40 4A 02 C0 02 05 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint16_t protocol;  // 90 31 02 15 21 99 40 4A 02 C0 02 05 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t func;       // 02 15 21 99 40 4A 02 C0 02 05 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a9;         // 15 21 99 40 4A 02 C0 02 05 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a10;        // 21 99 40 4A 02 C0 02 05 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t Tsetpoint;  // 99 40 4A 02 C0 02 05 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a12;        // 40 4A 02 C0 02 05 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a13;        // 4A 02 C0 02 05 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a14;        // 02 C0 02 05 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a15;        // C0 02 05 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a16;        // 02 05 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a17;        // 05 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a18;        // 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a19;        // C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a20;        // 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a21;        // 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a22;        // 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a23;        // 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a24;        // 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a25;        // C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a26;        // 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a27;        // 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a28;        // 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a29;        // 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a30;        // B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t mmyyyyyy;   // 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t hdddddmm;   // 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t mmmmhhhh;   // 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t ssssssmm;   // 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a35;        // 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a36;        // 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a37;        // 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a38;        // A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a39;        // 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a40;        // B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a41;        // 8E 00 AA FF FF FF FF FF 73
    uint8_t a42;        // 00 AA FF FF FF FF FF 73
    uint8_t TSetpoint2; // AA FF FF FF FF FF 73
    uint8_t a44;        // FF FF FF FF FF 73
    uint8_t a45;        // FF FF FF FF 73
    uint8_t a46;        // FF FF FF 73
    uint8_t a47;        // FF FF 73
    uint8_t a48;        // FF 73
    uint8_t a49;        // 73
    uint8_t crc1;
    uint8_t crc2;
    uint8_t stop;
};
static Frame2D previousFrame;

static float extract_T(const uint8_t *b) { return b[11] - 100; }
static float extract_T2(const uint8_t *b) { return b[43] - 100; }
static float extract_boost(const uint8_t *b) { return b[10] & 0x04 ? 1 : 0; }

#define EXTRACT_BYTE(idx) [](const uint8_t *b) -> float { return b[idx]; }

static const SensorField2 sensorMap[] = {
    {"2d_9", "2d[9]", "", "", EXTRACT_BYTE(9)},
    {"2d_10", "2d[10] bit 3=1 boost", "", "", EXTRACT_BYTE(10)},
    {"boost", "boost", "", "", extract_boost, 0, 0, 0, 0, true, true},
    {"Tsetpoint", "Setpoint temperature", "°C", "temperature", extract_T, 0, 40, 70, 1, true},
    {"2d_12", "2d[12]", "", "", EXTRACT_BYTE(12)},
    {"2d_13", "2d[13]", "", "", EXTRACT_BYTE(13)},
    {"2d_14", "2d[14]", "", "", EXTRACT_BYTE(14)},
    {"2d_15", "2d[15] Timer system(c0) sunflower vacation", "", "", EXTRACT_BYTE(15)},
    {"2d_16", "2d[16]", "", "", EXTRACT_BYTE(16)},
    {"2d_17", "2d[17]", "", "", EXTRACT_BYTE(17)},
    {"2d_18", "2d[18]", "", "", EXTRACT_BYTE(18)},
    {"2d_19", "2d[19]", "", "", EXTRACT_BYTE(19)},
    {"2d_20", "2d[20]", "", "", EXTRACT_BYTE(20)},
    {"2d_21", "2d[21]", "", "", EXTRACT_BYTE(21)},
    {"2d_22", "2d[22]", "", "", EXTRACT_BYTE(22)},
    {"2d_23", "2d[23]", "", "", EXTRACT_BYTE(23)},
    {"2d_24", "2d[24]", "", "", EXTRACT_BYTE(24)},
    {"2d_25", "2d[25]", "", "", EXTRACT_BYTE(25)},
    {"2d_26", "2d[26]", "", "", EXTRACT_BYTE(26)},
    {"2d_27", "2d[27]", "", "", EXTRACT_BYTE(27)},
    {"2d_28", "2d[28]", "", "", EXTRACT_BYTE(28)},
    {"2d_29", "2d[29]", "", "", EXTRACT_BYTE(29)},
    {"2d_30", "2d[30]", "", "", EXTRACT_BYTE(30)},
    {"2d_31", "2d[31] mmyyy yyyy", "", "", EXTRACT_BYTE(31)},
    {"2d_32", "2d[32] hddd ddmm", "", "", EXTRACT_BYTE(32)},
    {"2d_33", "2d[33] mmmm hhhh", "", "", EXTRACT_BYTE(33)},
    {"2d_34", "2d[34] ssss ssmm", "", "", EXTRACT_BYTE(34)},
    {"2d_35", "2d[35]", "", "", EXTRACT_BYTE(35)},
    {"2d_36", "2d[36]", "", "", EXTRACT_BYTE(36)},
    {"2d_37", "2d[37]", "", "", EXTRACT_BYTE(37)},
    {"2d_38", "2d[38]", "", "", EXTRACT_BYTE(38)},
    {"2d_39", "2d[39]", "", "", EXTRACT_BYTE(39)},
    {"2d_40", "2d[40]", "", "", EXTRACT_BYTE(40)},
    {"2d_41", "2d[41]", "", "", EXTRACT_BYTE(41)},
    {"2d_42", "2d[42]", "", "", EXTRACT_BYTE(42)},
    {"2d_43", "TSetpoint Legionella", "°C", "temperature", extract_T2, 0, 40, 70, 1, true},
    {"2d_44", "2d[44]", "", "", EXTRACT_BYTE(44)},
    {"2d_45", "2d[45]", "", "", EXTRACT_BYTE(45)},
    {"2d_46", "2d[46]", "", "", EXTRACT_BYTE(46)},
    {"2d_47", "2d[47]", "", "", EXTRACT_BYTE(47)},
    {"2d_48", "2d[48]", "", "", EXTRACT_BYTE(48)},
    {"2d_49", "2d[49]", "", "", EXTRACT_BYTE(49)},
};

void read_2d(const uint8_t *buffer)
{
    memcpy(&previousFrame, buffer, sizeof(Frame2D));

    static const int length = sizeof(sensorMap) / sizeof(sensorMap[0]);
    static float lastValues[length] = {0};
    static bool discoveryPublished[length] = {0};
    publishSensors(sensorMap, buffer, lastValues, discoveryPublished, length);
    if (!ControlValueChanged)
    {
        controlValues.SetpointTemp = previousFrame.Tsetpoint - 100;
        controlValues.LegionellaTemp = previousFrame.TSetpoint2 - 100;
        controlValues.Boost = (previousFrame.a10 & 0x04) != 0;
    }
}

uint8_t get_2dFrame(uint8_t *buffer)
{
    previousFrame.src = SRC;
    previousFrame.dst = 0xF0;
    previousFrame.type = 0x02;
    previousFrame.func = 0x02;
    previousFrame.Tsetpoint = controlValues.SetpointTemp + 100;
    previousFrame.TSetpoint2 = controlValues.LegionellaTemp + 100;
    previousFrame.a10 = controlValues.Boost ? previousFrame.a10 | 0x04 : previousFrame.a10 & ~0x04;

    memcpy(buffer, &previousFrame, sizeof(Frame2D));
    return sizeof(Frame2D);
}
