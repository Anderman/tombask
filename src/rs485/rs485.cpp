#include <HardwareSerial.h>
#include "globals.h"
#include "rs485.h"
#include "rs485_1c.h"
#include "rs485_2d.h"
#include "rs485_25.h"
#include "rs485_40.h"
#include "rs485_3c.h"

static HardwareSerial RS485(2);
static uint8_t buffer[0x48];
static int bufferPos = 0;
static uint8_t length = 8;

static bool verifyCrc(const uint8_t *data, size_t len);
static void frameReader();

void setupRs485()
{
    buffer[0] = 0x7e;
    buffer[1] = 0x7e;
    RS485.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN); // RX=16, TX=17 (pas aan naar jouw hardware)
}

void rs485Loop()
{
    while (RS485.available())
    {
        uint8_t c = RS485.read();

        static int bufferPos = 0;
        if (bufferPos == 0 && c == 0x7E)
        {
            bufferPos++;
        }
        else if (bufferPos == 1 && c == 0x7E)
        {
            bufferPos++;
        }
        else if (bufferPos > 1 && bufferPos < length)
        {
            if (bufferPos == 5)
            {
                length = 5 + c + 3;
            }
            buffer[bufferPos++] = c;
            if (bufferPos == length)
            {
                for (int i = 0; i < length; i++)
                {
                    Serial.print(buffer[i], HEX);
                    Serial.print(" ");
                }
                bool crcOk = verifyCrc(buffer, length);
                Serial.println(crcOk ? "OK" : "ERROR");
                if (crcOk)
                {
                    if (buffer[2] == 0xf0 && buffer[3] == 0xf0)
                    {
                        frameReader();
                    }
                    if (buffer[2] == 0x02 && buffer[3] == 0xf0 && length == 0x04)
                    {
                    }
                }
                bufferPos = 0;
                length = 8;
            }
        }
    }
}

static uint16_t crc16_modbus(const uint8_t *data, size_t len)
{
    uint16_t crc = 0xFFFF;
    for (size_t i = 0; i < len; i++)
    {
        crc ^= data[i];
        for (int j = 0; j < 8; j++)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xA001;
            else
                crc >>= 1;
        }
    }
    return crc;
}

static bool verifyCrc(const uint8_t *data, size_t len)
{
    if (len < 3)
        return false; // Minimaal 1 byte data + 2 bytes CRC
    uint16_t receivedCrc = data[len - 2] | (data[len - 1] << 8);
    uint16_t calculatedCrc = crc16_modbus(data, len - 2);
    return receivedCrc == calculatedCrc;
}

static void frameReader()
{
    switch (length)
    {
    case 0x1c:
        return read_1c(buffer);
    case 0x25:
        return read_25(buffer);
    case 0x2d:
        return read_2d(buffer);
    case 0x3c:
        return read_3c(buffer);
    case 0x40:
        return read_40(buffer);
    }
    Serial.println(String("Unknown frame length: ") + String(length, HEX));
}
