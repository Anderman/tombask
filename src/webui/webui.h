#pragma once

#include <ArduinoJson.h>
#include <WebServer.h>

// Parse JSON body from POST, return true if ok, false if error (en stuurt zelf foutmelding)
bool parseJsonBody(JsonDocument &doc);

void setupWebUi();
void webUiLoop();
extern WebServer server;

inline void sendCorsHeaders()
{
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type, Accept");
}

#define SEND_CORS() sendCorsHeaders()