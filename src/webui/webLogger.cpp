#include <WebServer.h>
#include "webui.h"
#include "webLogger.h"

static WiFiClient sseClient;

void webLoggerWriteLine(const char *line)
{
    char buf[256];
    strlcpy(buf, line, sizeof(buf));
    if (!sseClient || !sseClient.connected())
    {
        return;
    }
    sseClient.print("data: ");
    sseClient.print(buf);
    sseClient.print("\n\n");
}

void handleSseLogs()
{
    if (sseClient)
    {
        sseClient.stop();
    }
    sseClient = server.client();
    sseClient.print(
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/event-stream\r\n"
        "Cache-Control: no-cache\r\n"
        "Connection: keep-alive\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "\r\n");

    sseClient.setNoDelay(true);
}
