#include "rs485_2d.h"
#include <cstdint>
#include "rs485.h"

struct __attribute__((packed)) Frame2c
{
    uint8_t start1;    // 7E 7E F0 F0 04 2D 90 31 02 15 21 99 40 4A 02 C0 02 05 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t start2;    // 7E F0 F0 04 2D 90 31 02 15 21 99 40 4A 02 C0 02 05 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t dst;       // F0 F0 04 2D 90 31 02 15 21 99 40 4A 02 C0 02 05 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t src;       // F0 04 2D 90 31 02 15 21 99 40 4A 02 C0 02 05 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t type;      // 04 2D 90 31 02 15 21 99 40 4A 02 C0 02 05 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t lengte;    // 2D 90 31 02 15 21 99 40 4A 02 C0 02 05 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint16_t protocol; // 90 31 02 15 21 99 40 4A 02 C0 02 05 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t func;      // 02 15 21 99 40 4A 02 C0 02 05 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a0;        // 15 21 99 40 4A 02 C0 02 05 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a1;        // 21 99 40 4A 02 C0 02 05 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t Tsetpoint; // 99 40 4A 02 C0 02 05 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a3;        // 40 4A 02 C0 02 05 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a4;        // 4A 02 C0 02 05 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a5;        // 02 C0 02 05 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a6;        // C0 02 05 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a7;        // 02 05 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a8;        // 05 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a9;        // 01 C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a10;       // C7 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a11;       // 01 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a12;       // 0C 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a13;       // 02 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a14;       // 0E 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a15;       // 02 C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a16;       // C4 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a17;       // 00 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a18;       // 24 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a19;       // 01 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a20;       // 17 B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a21;       // B4 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a22;       // 00 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a23;       // 00 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a24;       // 00 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a25;       // 00 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a26;       // 55 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a27;       // 8E 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a28;       // 5D A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a29;       // A7 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a30;       // 15 B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a31;       // B0 8E 00 AA FF FF FF FF FF 73
    uint8_t a32;       // 8E 00 AA FF FF FF FF FF 73
    uint8_t a33;       // 00 AA FF FF FF FF FF 73
    uint8_t a34;       // AA FF FF FF FF FF 73
    uint8_t a35;       // FF FF FF FF FF 73
    uint8_t a36;       // FF FF FF FF 73
    uint8_t a37;       // FF FF FF 73
    uint8_t a38;       // FF FF 73
    uint8_t a39;       // FF 73
    uint8_t a40;       // 73
};

static Frame2c previousFrame;

void read_2d(const uint8_t *buffer)
{
    const Frame2c *frame = reinterpret_cast<const Frame2c *>(buffer);
    previousFrame = *frame;
}

void write_2d(const uint8_t *buffer)
{
    Frame2c frame = previousFrame;
    frame.src = SRC;
    frame.Tsetpoint = buffer[10];
}
