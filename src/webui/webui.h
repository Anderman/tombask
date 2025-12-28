#include <ArduinoJson.h>

// Parse JSON body from POST, return true if ok, false if error (en stuurt zelf foutmelding)
bool parseJsonBody(JsonDocument &doc);
#pragma once
#include <WebServer.h>

void setupWebUi();
void webUiLoop();
extern WebServer server;
#define SEND_CORS() server.sendHeader("Access-Control-Allow-Origin", "*")