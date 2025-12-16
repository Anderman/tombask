#include <HardwareSerial.h>
#include "globals.h"
#include "rs485.h"
#include "rs485_1c.h"
#include "rs485_2d.h"
#include "rs485_25.h"
#include "rs485_40.h"
#include "rs485_3c.h"
#include <webui/webLogger.h>

// ESP32-C3 only has UART0/1 available via HardwareSerial (0/1)
static HardwareSerial RS485(1);
static uint8_t buffer[0x48];
static int bufferPos = 0;
static uint8_t length = 8;

static bool verifyCrc(const uint8_t *data, size_t len);
static void frameReader();
static bool formatFrameLine(const uint8_t *data, size_t len, char *out, size_t outSize);

void setupRs485()
{
    buffer[0] = 0x7e;
    buffer[1] = 0x7e;
    RS485.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
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
                char line[3 * 0x48 + 32];
                const bool crcOk = formatFrameLine(buffer, length, line, sizeof(line));
                Serial.println(line);
                webLoggerWriteLine(line);
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
    uint16_t receivedCrc = data[len - 3] | (data[len - 2] << 8);
    uint16_t calculatedCrc = crc16_modbus(data, len - 3);
    return receivedCrc == calculatedCrc;
}

static void frameReader()
{
    switch (buffer[5])
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
static bool formatFrameLine(const uint8_t *data, size_t len, char *out, size_t outSize)
{
    if (!out || outSize == 0)
        return false;

    size_t pos = 0;

    for (size_t i = 0; i < len && pos + 3 < outSize; i++)
        pos += snprintf(out + pos, outSize - pos, "%02X ", data[i]);

    if (pos > 0 && out[pos - 1] == ' ')
        out[pos - 1] = '\0'; // laatste spatie weg
    else if (pos == 0)
        out[0] = '\0';

    const bool crcOk = verifyCrc(data, len);

    const size_t used = strlen(out);
    if (used < outSize)
        snprintf(out + used, outSize - used, " | CRC %s", crcOk ? "OK" : "ERROR");

    return crcOk;
}
