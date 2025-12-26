#include <Arduino.h>
#include "rs485_3c.h"
#include <cstdint>
#include "sensors.h"

struct __attribute__((packed)) Frame3c
{
    uint8_t start1;    // 7E 7E F0 F0 02 3C 90 31 01 75 97 00 95 00 00 00 00 77 00 72 77 00 00 10 02 01 90 00 00 00 00 10 00 00 00 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t start2;    // 7E F0 F0 02 3C 90 31 01 75 97 00 95 00 00 00 00 77 00 72 77 00 00 10 02 01 90 00 00 00 00 10 00 00 00 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t dst;       // F0 F0 02 3C 90 31 01 75 97 00 95 00 00 00 00 77 00 72 77 00 00 10 02 01 90 00 00 00 00 10 00 00 00 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t src;       // F0 02 3C 90 31 01 75 97 00 95 00 00 00 00 77 00 72 77 00 00 10 02 01 90 00 00 00 00 10 00 00 00 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t type;      // 02 3C 90 31 01 75 97 00 95 00 00 00 00 77 00 72 77 00 00 10 02 01 90 00 00 00 00 10 00 00 00 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t lengte;    // 3C 90 31 01 75 97 00 95 00 00 00 00 77 00 72 77 00 00 10 02 01 90 00 00 00 00 10 00 00 00 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t protocol1; // 90 31 01 75 97 00 95 00 00 00 00 77 00 72 77 00 00 10 02 01 90 00 00 00 00 10 00 00 00 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t protocol2; // 90 31 01 75 97 00 95 00 00 00 00 77 00 72 77 00 00 10 02 01 90 00 00 00 00 10 00 00 00 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t func;      // 01 75 97 00 95 00 00 00 00 77 00 72 77 00 00 10 02 01 90 00 00 00 00 10 00 00 00 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t T1;        // 75 97 00 95 00 00 00 00 77 00 72 77 00 00 10 02 01 90 00 00 00 00 10 00 00 00 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t T2;        // 97 00 95 00 00 00 00 77 00 72 77 00 00 10 02 01 90 00 00 00 00 10 00 00 00 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a11;       // 00 95 00 00 00 00 77 00 72 77 00 00 10 02 01 90 00 00 00 00 10 00 00 00 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t T3;        // 95 00 00 00 00 77 00 72 77 00 00 10 02 01 90 00 00 00 00 10 00 00 00 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a13;       // 00 00 00 00 77 00 72 77 00 00 10 02 01 90 00 00 00 00 10 00 00 00 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a14;       // 00 00 00 77 00 72 77 00 00 10 02 01 90 00 00 00 00 10 00 00 00 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a15;       // 00 00 77 00 72 77 00 00 10 02 01 90 00 00 00 00 10 00 00 00 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a16;       // 00 77 00 72 77 00 00 10 02 01 90 00 00 00 00 10 00 00 00 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t T4;        // 77 00 72 77 00 00 10 02 01 90 00 00 00 00 10 00 00 00 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a18;       // 00 72 77 00 00 10 02 01 90 00 00 00 00 10 00 00 00 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t T5;        // 72 77 00 00 10 02 01 90 00 00 00 00 10 00 00 00 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t T6;        // 77 00 00 10 02 01 90 00 00 00 00 10 00 00 00 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a21;       // 00 00 10 02 01 90 00 00 00 00 10 00 00 00 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a22;       // 00 10 02 01 90 00 00 00 00 10 00 00 00 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a23;       // 10 02 01 90 00 00 00 00 10 00 00 00 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a24;       // 02 01 90 00 00 00 00 10 00 00 00 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint16_t counter;  // 01 90 00 00 00 00 10 00 00 00 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a27;       // 00 00 00 00 10 00 00 00 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a28;       // 00 00 00 10 00 00 00 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a29;       // 00 00 10 00 00 00 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a30;       // 00 10 00 00 00 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a31;       // 10 00 00 00 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a32;       // 00 00 00 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a33;       // 00 00 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a34;       // 00 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a35;       // 00 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a36;       // 00 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a37;       // 00 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a38;       // 00 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a39;       // 11 C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a40;       // C8 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a41;       // 0F 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t hh;        // 47 BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t mm;        // BB BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t ss;        // BB 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a45;       // 97 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a46;       // 15 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a47;       // 00 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a48;       // 00 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a49;       // 00 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a50;       // 00 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a51;       // 30 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t T7;        // 99 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a53;       // 00 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a54;       // 00 00 00 02 00 00 00 00 06 00 00
    uint8_t a55;       // 00 00 02 00 00 00 00 06 00 00
    uint8_t a56;       // 00 02 00 00 00 00 06 00 00
    uint8_t a57;       // 02 00 00 00 00 06 00 00
    uint8_t a58;       // 00 00 00 00 06 00 00
    uint8_t a59;       // 00 00 00 06 00 00
    uint8_t a60;       // 00 00 06 00 00
    uint8_t a61;       // 00 06 00 00
    uint8_t a62;       // 06 00 00
    uint8_t a63;       // 00 00
    uint8_t a64;       // 00
};

// clang-format off
static float extract_T1(const uint8_t *b) { return b[9] - 100; }
static float extract_T2(const uint8_t *b) { return b[10] - 100; }
static float extract_T3(const uint8_t *b) { return b[12] - 100; }
static float extract_T4(const uint8_t *b) { return b[17] - 100; }
static float extract_T5(const uint8_t *b) { return b[19] - 100; }
static float extract_T6(const uint8_t *b) { return b[20] - 100; }
static float extract_T7(const uint8_t *b) { return b[52] - 100; }
static float extract_T8(const uint8_t *b) { return b[45] - 100; }
static float extract_25_26(const uint8_t *b) { return b[25] << 8 | b[26]; }
static float extract_ss(const uint8_t *b){return b[44] >> 2;}                       //44 ssss ssmm
static float extract_mm(const uint8_t *b){return b[44] << 4 & 0x30 | b[43] >> 4;}   //43 mmmm hhhh
static float extract_hh(const uint8_t *b){return b[43] << 1 & 0x1E | b[42] >> 7;}   //42 hddd ddmm
static float extract_dd(const uint8_t *b){return b[42] >> 2 & 0x1F; }               //41 mmyy yyyy
static float extract_MM(const uint8_t *b){return b[42] << 2 & 0x0F | b[41] >> 6;}   //40 yyyy yyyy
//   yy yyyy yy
// 0000 1111 1100 1000

#define EXTRACT_BYTE(idx) [](const uint8_t *b) -> float { return b[idx]; }
// clang-format on

static const SensorField2 sensorMap[] = {
    {"T1", "Air inlet temperature", "°C", "temperature", extract_T1, 1},
    {"T2", "Top tank water temperature", "°C", "temperature", extract_T2, 1},
    {"3c_11", "3c[11]", "", "", EXTRACT_BYTE(11)},
    {"T3", "Bottom tank water temperature", "°C", "temperature", extract_T3, 1},
    {"3c_13", "3c[13]", "", "", EXTRACT_BYTE(13)},
    {"3c_14", "3c[14]", "", "", EXTRACT_BYTE(14)},
    {"3c_15", "3c[15]", "", "", EXTRACT_BYTE(15)},
    {"3c_16", "3c[16]", "", "", EXTRACT_BYTE(16)},
    {"T4", "Discharge gas temperature", "°C", "temperature", extract_T4, 1},
    {"3c_18", "3c[18]", "", "", EXTRACT_BYTE(18)},
    {"T5", "Air outlet temperature", "°C", "temperature", extract_T5, 1},
    {"T6", "Suction gas temperature", "°C", "temperature", extract_T6, 1},
    {"3c_21", "3c[21]", "", "", EXTRACT_BYTE(21)},
    {"3c_22", "3c[22]", "", "", EXTRACT_BYTE(22)},
    {"3c_23", "3c[23]", "", "", EXTRACT_BYTE(23)},
    {"3c_24", "3c[24]", "", "", EXTRACT_BYTE(24)},
    {"counter", "Expansion valve", "", "", extract_25_26},
    {"3c_27", "3c[27]", "", "", EXTRACT_BYTE(27)},
    {"3c_28", "3c[28]", "", "", EXTRACT_BYTE(28)},
    {"3c_29", "3c[29]", "", "", EXTRACT_BYTE(29)},
    {"3c_30", "3c[30]", "", "", EXTRACT_BYTE(30)},
    {"3c_31", "3c[31] counter?", "", "", EXTRACT_BYTE(31)},
    {"3c_32", "3c[32] state b4=FanOn b8=CompOn, F", "", "", EXTRACT_BYTE(32)},
    {"3c_33", "3c[33] 02==boost?", "", "", EXTRACT_BYTE(33)},
    {"3c_34", "3c[34]", "", "", EXTRACT_BYTE(34)},
    {"3c_35", "3c[35]", "", "", EXTRACT_BYTE(35)},
    {"3c_36", "3c[36]", "", "", EXTRACT_BYTE(36)},
    {"3c_37", "3c[37]", "", "", EXTRACT_BYTE(37)},
    {"3c_38", "3c[38] state b6=CompOn", "", "", EXTRACT_BYTE(38)},
    {"3c_39", "3c[39]", "", "", EXTRACT_BYTE(39)},
    {"3c_40", "3c[40] temp related?", "", "", EXTRACT_BYTE(40)},
    {"3c_41", "3c[41]", "", "", EXTRACT_BYTE(41)},
    {"MM", "Month", "", "", extract_MM},
    {"dd", "Day", "", "", extract_dd},
    {"hh", "Hours", "", "", extract_hh},
    {"mm", "Minutes", "", "", extract_mm},
    {"ss", "Seconds", "", "", extract_ss},
    {"T8", "Top tank water temperature", "°C", "temperature", extract_T8, 1},
    {"3c_46", "3c[46]", "", "", EXTRACT_BYTE(46)},
    {"3c_47", "3c[47]", "", "", EXTRACT_BYTE(47)},
    {"3c_48", "3c[48]", "", "", EXTRACT_BYTE(48)},
    {"3c_49", "3c[49]", "", "", EXTRACT_BYTE(49)},
    {"3c_50", "3c[50]", "", "", EXTRACT_BYTE(50)},
    {"3c_51", "3c[51] day counter?", "", "", EXTRACT_BYTE(51)},
    {"T7", "Target temperature", "°C", "temperature", extract_T7, 1},
    {"3c_53", "3c[53]", "", "", EXTRACT_BYTE(53)},
    {"3c_54", "3c[54]", "", "", EXTRACT_BYTE(54)},
    {"3c_55", "3c[55]", "", "", EXTRACT_BYTE(55)},
    {"3c_56", "3c[56]", "", "", EXTRACT_BYTE(56)},
    {"3c_57", "3c[57]", "", "", EXTRACT_BYTE(57)},
    {"3c_58", "3c[58]", "", "", EXTRACT_BYTE(58)},
    {"3c_59", "3c[59]", "", "", EXTRACT_BYTE(59)},
    {"3c_60", "3c[60]", "", "", EXTRACT_BYTE(60)},
    {"3c_61", "3c[61]", "", "", EXTRACT_BYTE(61)},
    {"3c_62", "3c[62] weekday?", "", "", EXTRACT_BYTE(62)},
    {"3c_63", "3c[63]", "", "", EXTRACT_BYTE(63)},
    {"3c_64", "3c[64]", "", "", EXTRACT_BYTE(64)},
};

void read_3c(const uint8_t *buffer)
{
    static const int length = sizeof(sensorMap) / sizeof(sensorMap[0]);
    static float lastValues[length] = {0};
    static bool discoveryPublished[length] = {0};
    publishSensors(sensorMap, buffer, lastValues, discoveryPublished, length);
}
