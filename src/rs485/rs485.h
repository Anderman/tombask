#pragma once
void setupRs485();
void rs485Loop();

void WriteFrame(const uint8_t len, const bool isLast);

static const uint8_t SRC  = 0x02; // source address for this device

