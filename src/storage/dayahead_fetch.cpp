#include <WiFiClientSecure.h>
#include "../secrets.h"
#include <time.h>
#include <Arduino.h>
#include "dayahead_fetch.h"
#include "dayahead_storage.h"

DayAheadPrices fetchDayAheadFromEntsoe()
{
    DayAheadPrices result = {0};
    WiFiClientSecure client;
    client.setInsecure();
    const char *host = "web-api.tp.entsoe.eu";
    const int httpsPort = 443;
    time_t now = time(nullptr);
    struct tm tm_start = *localtime(&now);
    int minute = tm_start.tm_min;
    int rounded = (minute / 15) * 15;
    tm_start.tm_min = rounded;
    tm_start.tm_sec = 0;
    char periodStart[13], periodEnd[13];
    snprintf(periodStart, sizeof(periodStart), "%04d%02d%02d%02d%02d",
             tm_start.tm_year + 1900, tm_start.tm_mon + 1, tm_start.tm_mday, tm_start.tm_hour, tm_start.tm_min);
    struct tm tm_end = tm_start;
    tm_end.tm_mday += 1;
    tm_end.tm_hour = 23;
    tm_end.tm_min = 0;
    mktime(&tm_end);
    snprintf(periodEnd, sizeof(periodEnd), "%04d%02d%02d2300",
             tm_end.tm_year + 1900, tm_end.tm_mon + 1, tm_end.tm_mday);
    if (client.connect(host, httpsPort))
    {
        String url = "/api?securityToken=";
        url += ENSTSO_API_KEY;
        url += "&documentType=A44&in_Domain=10YNL----------L&out_Domain=10YNL----------L";
        url += "&periodStart=";
        url += periodStart;
        url += "&periodEnd=";
        url += periodEnd;
        client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                     "Host: " + host + "\r\n" +
                     "Connection: close\r\n\r\n");
        unsigned long timeout = millis();
        while (client.connected() && !client.available() && millis() - timeout < 5000)
        {
        }
        String response;
        while (client.available())
        {
            String line = client.readStringUntil('\n');
            response += line + "\n";
        }
        int tsIdx = 0;
        while (true)
        {
            int tsStart = response.indexOf("<TimeSeries>", tsIdx);
            if (tsStart < 0)
                break;
            int tsEnd = response.indexOf("</TimeSeries>", tsStart);
            if (tsEnd < 0)
                break;
            int periodIdx = tsStart;
            while (true)
            {
                int pStart = response.indexOf("<Period>", periodIdx);
                if (pStart < 0 || pStart > tsEnd)
                    break;
                int pEnd = response.indexOf("</Period>", pStart);
                if (pEnd < 0 || pEnd > tsEnd)
                    break;
                int t0idx = response.indexOf("<start>", pStart);
                int t0end = response.indexOf("</start>", t0idx);
                String t0str = (t0idx >= 0 && t0end > t0idx && t0end < pEnd) ? response.substring(t0idx + 7, t0end) : "";
                struct tm t0 = {0};
                time_t t0_epoch = 0;
                if (t0str.length() >= 16)
                {
                    sscanf(t0str.c_str(), "%4d-%2d-%2dT%2d:%2dZ", &t0.tm_year, &t0.tm_mon, &t0.tm_mday, &t0.tm_hour, &t0.tm_min);
                    t0.tm_year -= 1900;
                    t0.tm_mon -= 1;
                    t0.tm_sec = 0;
                    t0.tm_isdst = -1;
                    t0_epoch = mktime(&t0);
                }
                int pos = 0;
                float price = 0;
                for (int i = pStart; i < pEnd; ++i)
                {
                    if (response.substring(i, i + 10) == "<position>")
                    {
                        int j = response.indexOf("</position>", i);
                        if (j > i && j < pEnd)
                        {
                            pos = response.substring(i + 10, j).toInt();
                            i = j;
                        }
                    }
                    else if (response.substring(i, i + 14) == "<price.amount>")
                    {
                        int j = response.indexOf("</price.amount>", i);
                        if (j > i && j < pEnd)
                        {
                            price = response.substring(i + 14, j).toFloat();
                            i = j;
                            if (pos >= 1 && t0_epoch > 0 && result.count < 192)
                            {
                                result.startEpoch = (result.count == 0) ? t0_epoch : result.startEpoch;
                                result.prices[result.count++] = price;
                            }
                        }
                    }
                }
                periodIdx = pEnd + 8;
            }
            tsIdx = tsEnd + 12;
        }
    }
    result.valid = (result.count > 0 && result.startEpoch != 0);
    return result;
}

DayAheadPrices getDayAheadPrices()
{
    DayAheadPrices prices = {0};
    if (loadDayAheadPrices(prices))
    {
        time_t now = time(nullptr); // Altijd UTC
        struct tm today12 = *gmtime(&now);
        today12.tm_hour = 12;
        today12.tm_min = 0;
        today12.tm_sec = 0;
        time_t epoch12 = mktime(&today12);
        if (now >= epoch12 && prices.startEpoch >= epoch12)
        {
            return prices;
        }
    }
    // Fetch als cache ongeldig, niet van vandaag, of na 15:00 en startEpoch niet na 15:00
    prices = fetchDayAheadFromEntsoe();
    if (prices.valid)
    {
        saveDayAheadPrices(prices);
    }
    return prices;
}
