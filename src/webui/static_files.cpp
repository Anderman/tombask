
#include <WebServer.h>
#include "webui.h"
#include "static_files.h"
#include <SPIFFS.h>
#include <WebServer.h>

bool spiffsOk = false;
bool tryServeFromSpiffs(const String &uri);

void handleNotFound()
{
    extern WebServer server;
    if (tryServeFromSpiffs(server.uri())) return;
    SEND_CORS();
    server.send(404, "text/plain", "Not found");
}

const char *contentTypeForPath(const String &path)
{
    if (path.endsWith(".html"))
        return "text/html";
    if (path.endsWith(".css"))
        return "text/css";
    if (path.endsWith(".js"))
        return "application/javascript";
    if (path.endsWith(".json"))
        return "application/json";
    if (path.endsWith(".svg"))
        return "image/svg+xml";
    if (path.endsWith(".png"))
        return "image/png";
    if (path.endsWith(".ico"))
        return "image/x-icon";
    if (path.endsWith(".txt"))
        return "text/plain";
    if (path.endsWith(".map"))
        return "application/json";
    return "application/octet-stream";
}

bool tryServeFromSpiffs(const String &uri)
{
    if (!spiffsOk)
        return false;

    String path = uri;
    if (path == "/")
        path = "/index.html";

    if (SPIFFS.exists(path))
    {
        File f = SPIFFS.open(path, "r");
        if (!f)
            return false;
        extern WebServer server;
        server.streamFile(f, contentTypeForPath(path));
        f.close();
        return true;
    }

    // SPA fallback: if no extension, serve index.html
    if (path.indexOf('.') < 0 && SPIFFS.exists("/index.html"))
    {
        File f = SPIFFS.open("/index.html", "r");
        if (!f)
            return false;
        extern WebServer server;
        server.streamFile(f, "text/html");
        f.close();
        return true;
    }

    return false;
}
