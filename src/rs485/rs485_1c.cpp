#include "rs485_1c.h"
#include <cstdint>
#include "rs485.h"
#include <cstring>

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
    uint8_t a0;        // 15 2c c0 01 00 00 10 ff ff 00 00 af b4 04 4b 73 5f 5a 3a 4b 00 00 00 00
    uint8_t a1;        // 2c c0 01 00 00 10 ff ff 00 00 af b4 04 4b 73 5f 5a 3a 4b 00 00 00 00
    uint8_t a2;        // c0 01 00 00 10 ff ff 00 00 af b4 04 4b 73 5f 5a 3a 4b 00 00 00 00
    uint8_t a3;        // 01 00 00 10 ff ff 00 00 af b4 04 4b 73 5f 5a 3a 4b 00 00 00 00
    uint8_t a4;        // 00 00 10 ff ff 00 00 af b4 04 4b 73 5f 5a 3a 4b 00 00 00 00
    uint8_t fan;       // 80 10 ff ff 00 00 af b4 04 4b 73 5f 5a 3a 4b 00 00 00 00
    uint8_t a6;        // 10 ff ff 00 00 af b4 04 4b 73 5f 5a 3a 4b 00 00 00 00
    uint8_t a7;        // ff ff 00 00 af b4 04 4b 73 5f 5a 3a 4b 00 00 00 00
    uint8_t a8;        // ff 00 00 af b4 04 4b 73 5f 5a 3a 4b 00 00 00 00
    uint8_t a9;        // 00 00 af b4 04 4b 73 5f 5a 3a 4b 00 00 00 00
    uint8_t a10;       // 00 af b4 04 4b 73 5f 5a 3a 4b 00 00 00 00
    uint8_t a11;       // af b4 04 4b 73 5f 5a 3a 4b 00 00 00 00
    uint8_t a12;       // b4 04 4b 73 5f 5a 3a 4b 00 00 00 00
    uint8_t a13;       // 04 4b 73 5f 5a 3a 4b 00 00 00 00
    uint8_t a14;       // 4b 73 5f 5a 3a 4b 00 00 00 00
    uint8_t a15;       // 73 5f 5a 3a 4b 00 00 00 00
    uint8_t a16;       // 5f 5a 3a 4b 00 00 00 00
    uint8_t a17;       // 5a 3a 4b 00 00 00 00
    uint8_t a18;       // 3a 4b 00 00 00 00
    uint8_t a19;       // 4b 00 00 00 00
    uint8_t a20;       // 00 00 00 00
    uint8_t a21;       // 00 00 00
    uint8_t a22;       // 00 00
    uint8_t a23;       // 00
};

Frame1c previousFrame;

void read_1c(const uint8_t *buffer)
{
    const Frame1c *frame = reinterpret_cast<const Frame1c *>(buffer);
    previousFrame = *frame;
}

void write_1c(const uint8_t *buffer)
{
    Frame1c frame = previousFrame;
    frame.src = SRC;
    frame.fan = previousFrame.fan | 0x80;
}