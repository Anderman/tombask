#include <ArduinoJson.h>

// Parse JSON body from POST, return true if ok, false if error (en stuurt zelf foutmelding)
bool parseJsonBody(JsonDocument &doc);
#pragma once
#include <WebServer.h>

void setupWebUi();
void webUiLoop();
extern WebServer server;
#define SEND_CORS()                                                                \
    do                                                                             \
    {                                                                              \
        server.sendHeader("Access-Control-Allow-Origin", "*");                     \
        server.sendHeader("Access-Control-Allow-Methods", "POST, OPTIONS");        \
        server.sendHeader("Access-Control-Allow-Headers", "Content-Type, Accept"); \
    } while (0)