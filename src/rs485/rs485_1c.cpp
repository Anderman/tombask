#include <cstdint>
#include <cstring>
#include <Arduino.h>
#include "rs485_1c.h"
#include "rs485.h"
#include "Settings.h"
#include "sensors.h"

struct __attribute__((packed)) Frame1c
{
    uint8_t start1;
    uint8_t start2;
    uint8_t dst;
    uint8_t src;
    uint8_t type;
    uint8_t lengte;
    uint16_t protocol; // 90 31
    uint8_t func;      // 02
    uint8_t a9;        // 15 2c c0 01 00 00 10 ff ff 00 00 af b4 04 4b 73 5f 5a 3a 4b 00 00 00 00
    uint8_t a10;       // 2c c0 01 00 00 10 ff ff 00 00 af b4 04 4b 73 5f 5a 3a 4b 00 00 00 00
    uint8_t a11;       // c0 01 00 00 10 ff ff 00 00 af b4 04 4b 73 5f 5a 3a 4b 00 00 00 00
    uint8_t a12;       // 01 00 00 10 ff ff 00 00 af b4 04 4b 73 5f 5a 3a 4b 00 00 00 00
    uint8_t a13;       // 00 00 10 ff ff 00 00 af b4 04 4b 73 5f 5a 3a 4b 00 00 00 00
    uint8_t fan;       // 80 10 ff ff 00 00 af b4 04 4b 73 5f 5a 3a 4b 00 00 00 00
    uint8_t a15;       // 10 ff ff 00 00 af b4 04 4b 73 5f 5a 3a 4b 00 00 00 00
    uint8_t a16;       // ff ff 00 00 af b4 04 4b 73 5f 5a 3a 4b 00 00 00 00
    uint8_t a17;       // ff 00 00 af b4 04 4b 73 5f 5a 3a 4b 00 00 00 00
    uint8_t a18;       // 00 00 af b4 04 4b 73 5f 5a 3a 4b 00 00 00 00
    uint8_t a19;       // 00 af b4 04 4b 73 5f 5a 3a 4b 00 00 00 00
    uint8_t a20;       // af b4 04 4b 73 5f 5a 3a 4b 00 00 00 00
    uint8_t a21;       // b4 04 4b 73 5f 5a 3a 4b 00 00 00 00
    uint8_t a22;       // 04 4b 73 5f 5a 3a 4b 00 00 00 00
    uint8_t a23;       // 4b 73 5f 5a 3a 4b 00 00 00 00
    uint8_t a24;       // 73 5f 5a 3a 4b 00 00 00 00
    uint8_t a25;       // 5f 5a 3a 4b 00 00 00 00
    uint8_t a26;       // 5a 3a 4b 00 00 00 00
    uint8_t a27;       // 3a 4b 00 00 00 00
    uint8_t a28;       // 4b 00 00 00 00
    uint8_t a29;       // 00 00 00 00
    uint8_t a30;       // 00 00 00
    uint8_t a31;       // 00 00
    uint8_t a32;       // 00
    uint8_t crc1;
    uint8_t crc2;
    uint8_t stop;
};
static float extract_fan(const uint8_t *b) { return b[14] & 0x80 ? 1 : 0; }
#define EXTRACT_BYTE(idx) [](const uint8_t *b) -> float { return b[idx]; }

static const SensorField2 sensorMap[] = {
    {"1c_9", "1c[9]", "", "", EXTRACT_BYTE(9)},
    {"1c_10", "1c[10]", "", "", EXTRACT_BYTE(10)},
    {"1c_11", "1c[11]", "", "", EXTRACT_BYTE(11)},
    {"1c_12", "1c[12]", "", "", EXTRACT_BYTE(12)},
    {"1c_13", "1c[13]", "", "", EXTRACT_BYTE(13)},
    {"fan", "fan", "", "", extract_fan, 0, 0, 0, 0, true, true},
    {"1c_15", "1c[15]", "", "", EXTRACT_BYTE(15)},
    {"1c_16", "1c[16]", "", "", EXTRACT_BYTE(16)},
    {"1c_17", "1c[17]", "", "", EXTRACT_BYTE(17)},
    {"1c_18", "1c[18]", "", "", EXTRACT_BYTE(18)},
    {"1c_19", "1c[19]", "", "", EXTRACT_BYTE(19)},
    {"1c_20", "1c[20]", "", "", EXTRACT_BYTE(20)},
    {"1c_21", "1c[21]", "", "", EXTRACT_BYTE(21)},
    {"1c_22", "1c[22]", "", "", EXTRACT_BYTE(22)},
    {"1c_23", "1c[23]", "", "", EXTRACT_BYTE(23)},
    {"1c_24", "1c[24]", "", "", EXTRACT_BYTE(24)},
    {"1c_25", "1c[25]", "", "", EXTRACT_BYTE(25)},
    {"1c_26", "1c[26]", "", "", EXTRACT_BYTE(26)},
    {"1c_27", "1c[27]", "", "", EXTRACT_BYTE(27)},
    {"1c_28", "1c[28]", "", "", EXTRACT_BYTE(28)},
    {"1c_29", "1c[29]", "", "", EXTRACT_BYTE(29)},
    {"1c_30", "1c[30]", "", "", EXTRACT_BYTE(30)},
    {"1c_31", "1c[31]", "", "", EXTRACT_BYTE(31)},
    {"1c_32", "1c[32]", "", "", EXTRACT_BYTE(32)},
};

static Frame1c previousFrame;

void read_1c(const uint8_t *buffer)
{
    memcpy(&previousFrame, buffer, sizeof(Frame1c));

    static const int length = sizeof(sensorMap) / sizeof(sensorMap[0]);
    static float lastValues[length] = {0};
    static bool discoveryPublished[length] = {0};
    publishSensors(sensorMap, buffer, lastValues, discoveryPublished, length);
    if (!ControlValueChanged)
    {
        controlValues.Fan = (buffer[14] & 0x80) != 0;
    }
}

uint8_t get_1cFrame(uint8_t *buffer)
{
    previousFrame.dst = 0xf0;
    previousFrame.src = SRC;
    previousFrame.type = 0x04;
    previousFrame.func = 0x02;
    previousFrame.fan = controlValues.Fan ? previousFrame.fan | 0x80 : previousFrame.fan & ~0x80;
    memcpy(buffer, &previousFrame, sizeof(Frame1c));
    return sizeof(Frame1c);
}