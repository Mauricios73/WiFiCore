#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

#include "config.h"
#include "types.h"

AsyncWebServer server(WEB_PORT);

static const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">

<title>WiFiCore</title>

<style>
body {
    margin: 0;
    background: #101010;
    color: #eee;
    font-family: Arial, sans-serif;
}

header {
    background: #181818;
    padding: 18px;
    text-align: center;
    font-size: 24px;
    font-weight: bold;
}

.container {
    padding: 15px;
    max-width: 800px;
    margin: auto;
}

.card {
    background: #1b1b1b;
    border-radius: 10px;
    padding: 15px;
    margin-bottom: 12px;
}

.grid {
    display: grid;
    grid-template-columns: repeat(2,1fr);
    gap: 10px;
}

.value {
    font-size: 28px;
    font-weight: bold;
}

.label {
    color: #999;
    font-size: 13px;
}

button {
    padding: 12px;
    width: 100%;
    margin-top: 10px;
    border: 0;
    border-radius: 6px;
    background: #303030;
    color: white;
}
</style>

</head>

<body>

<header>
WiFiCore
</header>

<div class="container">

<div class="card">

<div class="grid">

<div>
<div class="label">APs</div>
<div class="value" id="aps">0</div>
</div>

<div>
<div class="label">Clients</div>
<div class="value" id="clients">0</div>
</div>

<div>
<div class="label">Frames</div>
<div class="value" id="frames">0</div>
</div>

<div>
<div class="label">EAPOL</div>
<div class="value" id="eapol">0</div>
</div>

</div>

</div>

<div class="card">

<div class="label">Channel</div>
<div class="value" id="channel">6</div>

<div class="label">RSSI</div>
<div class="value" id="rssi">0</div>

<div class="label">Uptime</div>
<div class="value" id="uptime">0</div>

</div>

<div class="card">

<button onclick="location.href='/files'">
FILES
</button>

<button onclick="location.href='/statistics'">
STATISTICS
</button>

</div>

</div>

<script>

async function updateStatus() {

    try {

        const response =
            await fetch('/api/status');

        const data =
            await response.json();

        document.getElementById('aps').textContent =
            data.aps;

        document.getElementById('clients').textContent =
            data.clients;

        document.getElementById('frames').textContent =
            data.frames;

        document.getElementById('eapol').textContent =
            data.eapol;

        document.getElementById('channel').textContent =
            data.channel;

        document.getElementById('rssi').textContent =
            data.rssi;

        document.getElementById('uptime').textContent =
            data.uptime + ' s';

    } catch (error) {

        console.log(error);

    }
}

updateStatus();

setInterval(
    updateStatus,
    1000
);

</script>

</body>
</html>
)rawliteral";


// ============================================================
// Statistics
// ============================================================

extern StatisticsData getStatistics();


// ============================================================
// Web Portal
// ============================================================

void webPortalBegin() {

    Serial.println(
        "[WEB] Starting WebServer..."
    );

    Serial.print(
        "[WEB] AP IP: "
    );

    Serial.println(
        WiFi.softAPIP()
    );

    // --------------------------------------------------------
    // Main page
    // --------------------------------------------------------

    server.on(
        "/",
        HTTP_GET,
        [](AsyncWebServerRequest* request) {

            request->send_P(
                200,
                "text/html",
                INDEX_HTML
            );
        }
    );

    // --------------------------------------------------------
    // API status
    // --------------------------------------------------------

    server.on(
        "/api/status",
        HTTP_GET,
        [](AsyncWebServerRequest* request) {

            StatisticsData stats =
                getStatistics();

            String json;

            json.reserve(256);

            json += "{";

            json += "\"aps\":";
            json += stats.aps;

            json += ",\"clients\":";
            json += stats.clients;

            json += ",\"frames\":";
            json += stats.frames;

            json += ",\"management\":";
            json += stats.management;

            json += ",\"control\":";
            json += stats.control;

            json += ",\"data\":";
            json += stats.data;

            json += ",\"eapol\":";
            json += stats.eapol;

            json += ",\"channel\":";
            json += stats.channel;

            json += ",\"rssi\":";
            json += stats.rssi;

            json += ",\"uptime\":";
            json += stats.uptime;

            json += "}";

            request->send(
                200,
                "application/json",
                json
            );
        }
    );

    // --------------------------------------------------------
    // Test endpoint
    // --------------------------------------------------------

    server.on(
        "/test",
        HTTP_GET,
        [](AsyncWebServerRequest* request) {

            request->send(
                200,
                "text/plain",
                "WiFiCore WebServer OK"
            );
        }
    );

    // --------------------------------------------------------
    // 404
    // --------------------------------------------------------

    server.onNotFound(
        [](AsyncWebServerRequest* request) {

            request->send(
                404,
                "text/plain",
                "WiFiCore: Not Found"
            );
        }
    );

    // --------------------------------------------------------
    // Start
    // --------------------------------------------------------

    server.begin();

    Serial.println(
        "[WEB] WebServer started"
    );

    Serial.print(
        "[WEB] URL: http://"
    );

    Serial.println(
        WiFi.softAPIP()
    );
}