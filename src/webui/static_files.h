#pragma once
#include <Arduino.h>

extern bool spiffsOk;
#pragma once
#include <Arduino.h>


bool tryServeFromSpiffs(const String &uri);
const char *contentTypeForPath(const String &path);
void handleNotFound();
