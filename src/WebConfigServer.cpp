#include "WebConfigServer.h"
#include "html_index.h"
#include <ArduinoJson.h>

extern void restartTelemetry();

WebConfigServer::WebConfigServer(ConfigManager& config, TelemetryData& telData, RpmShiftLightModule& shiftLight)
    : server(80), config(config), telData(telData), shiftLight(shiftLight) {
}

void WebConfigServer::begin() {
    const char* headerkeys[] = {"Origin", "Access-Control-Request-Headers"};
    server.collectHeaders(headerkeys, 2);

    server.on("/", HTTP_GET, std::bind(&WebConfigServer::handleRoot, this));
    
    // API GET/POST handlers
    server.on("/api/status", HTTP_GET, std::bind(&WebConfigServer::handleGetStatus, this));
    server.on("/api/config", HTTP_GET, std::bind(&WebConfigServer::handleGetConfig, this));
    server.on("/api/save", HTTP_POST, std::bind(&WebConfigServer::handleSaveConfig, this));
    server.on("/api/test_leds", HTTP_GET, std::bind(&WebConfigServer::handleTestLeds, this));
    server.on("/api/scan_wifi", HTTP_GET, std::bind(&WebConfigServer::handleScanWifi, this));
    server.on("/api/reboot", HTTP_POST, std::bind(&WebConfigServer::handleReboot, this));
    server.on("/api/reset", HTTP_POST, std::bind(&WebConfigServer::handleReset, this));
    
    // Explicit OPTIONS handlers for CORS preflight
    server.on("/api/status", HTTP_OPTIONS, std::bind(&WebConfigServer::handleOptions, this));
    server.on("/api/config", HTTP_OPTIONS, std::bind(&WebConfigServer::handleOptions, this));
    server.on("/api/save", HTTP_OPTIONS, std::bind(&WebConfigServer::handleOptions, this));
    server.on("/api/test_leds", HTTP_OPTIONS, std::bind(&WebConfigServer::handleOptions, this));
    server.on("/api/scan_wifi", HTTP_OPTIONS, std::bind(&WebConfigServer::handleOptions, this));
    server.on("/api/reboot", HTTP_OPTIONS, std::bind(&WebConfigServer::handleOptions, this));
    server.on("/api/reset", HTTP_OPTIONS, std::bind(&WebConfigServer::handleOptions, this));
    
    server.onNotFound(std::bind(&WebConfigServer::handleNotFound, this));
    
    server.begin();
    Serial.println("Web Config Server started on port 80");
}

void WebConfigServer::handle() {
    server.handleClient();
}

void WebConfigServer::handleRoot() {
    server.send_P(200, "text/html", INDEX_HTML);
}

void WebConfigServer::handleGetStatus() {
    JsonDocument doc;
    doc["isRaceOn"] = telData.isRaceOn;
    doc["currentRpm"] = telData.currentRpm;
    doc["maxRpm"] = telData.maxRpm;
    doc["idleRpm"] = telData.idleRpm;
    doc["speedKmh"] = telData.speedKmh;
    doc["speedMph"] = telData.speedMph;
    doc["gear"] = telData.gear;
    doc["accelPercent"] = telData.accelPercent;
    doc["brakePercent"] = telData.brakePercent;
    doc["boost"] = telData.boost;
    doc["tireSlipMax"] = telData.tireSlipMax;
    doc["isUpdating"] = telData.isUpdating;
    doc["packetsReceived"] = telData.packetsReceived;
    doc["uptimeSeconds"] = millis() / 1000;
    doc["isApMode"] = (WiFi.getMode() == WIFI_AP) || (WiFi.status() != WL_CONNECTED);

    String response;
    serializeJson(doc, response);
    sendJSON(200, response);
}

void WebConfigServer::handleGetConfig() {
    JsonDocument doc;
    doc["isApMode"] = (WiFi.getMode() == WIFI_AP) || (WiFi.status() != WL_CONNECTED);
    
    // WiFi
    JsonObject wifi = doc["wifi"].to<JsonObject>();
    wifi["ssid"] = config.wifi.ssid;
    wifi["password"] = (strlen(config.wifi.password) > 0) ? "••••••••" : "";

    // Telemetry
    JsonObject telemetry = doc["telemetry"].to<JsonObject>();
    telemetry["port"] = config.telemetry.port;
    telemetry["enabled"] = config.telemetry.enabled;
    telemetry["gameType"] = config.telemetry.gameType;
    telemetry["pcIpAddress"] = config.telemetry.pcIpAddress;

    // Shift Light
    JsonObject sl = doc["shiftLight"].to<JsonObject>();
    sl["enabled"] = config.shiftLight.enabled;
    sl["pin"] = config.shiftLight.pin;
    sl["numLeds"] = config.shiftLight.numLeds;
    sl["startRpmPercent"] = config.shiftLight.startRpmPercent;
    sl["endRpmPercent"] = config.shiftLight.endRpmPercent;
    sl["brightness"] = config.shiftLight.brightness;
    sl["colorScheme"] = config.shiftLight.colorScheme;
    sl["ledMode"] = config.shiftLight.ledMode;
    sl["matrixSerpentine"] = config.shiftLight.matrixSerpentine;
    sl["matrixStartRow"] = config.shiftLight.matrixStartRow;
    sl["matrixRowCount"] = config.shiftLight.matrixRowCount;
    sl["gearStartRow"] = config.shiftLight.gearStartRow;
    sl["gearRowCount"] = config.shiftLight.gearRowCount;

    String response;
    serializeJson(doc, response);
    sendJSON(200, response);
}

void WebConfigServer::handleSaveConfig() {
    if (server.hasArg("plain") == false) {
        sendCORS(400, "text/plain", "Body missing");
        return;
    }

    String body = server.arg("plain");
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, body);

    if (error) {
        sendCORS(400, "text/plain", "JSON parse error");
        return;
    }

    bool shiftLightChanged = false;

    // Parse WiFi settings
    if (doc.containsKey("wifi")) {
        JsonObject wifi = doc["wifi"];
        if (wifi.containsKey("ssid")) strncpy(config.wifi.ssid, wifi["ssid"], sizeof(config.wifi.ssid) - 1);
        if (wifi.containsKey("password")) {
            const char* pass = wifi["password"];
            if (strcmp(pass, "••••••••") != 0) {
                strncpy(config.wifi.password, pass, sizeof(config.wifi.password) - 1);
            }
        }
    }

    // Parse Telemetry settings
    bool telemetryChanged = false;
    if (doc.containsKey("telemetry")) {
        JsonObject tel = doc["telemetry"];
        uint16_t oldPort = config.telemetry.port;
        bool oldEnabled = config.telemetry.enabled;
        uint8_t oldGameType = config.telemetry.gameType;

        if (tel.containsKey("port")) config.telemetry.port = tel["port"];
        if (tel.containsKey("enabled")) config.telemetry.enabled = tel["enabled"];
        if (tel.containsKey("gameType")) config.telemetry.gameType = tel["gameType"];
        if (tel.containsKey("pcIpAddress")) strncpy(config.telemetry.pcIpAddress, tel["pcIpAddress"], sizeof(config.telemetry.pcIpAddress) - 1);

        if (config.telemetry.port != oldPort || 
            config.telemetry.enabled != oldEnabled || 
            config.telemetry.gameType != oldGameType) {
            telemetryChanged = true;
        }
    }

    // Parse Shift Light settings
    if (doc.containsKey("shiftLight")) {
        JsonObject sl = doc["shiftLight"];
        if (sl.containsKey("enabled")) config.shiftLight.enabled = sl["enabled"];
        if (sl.containsKey("pin")) config.shiftLight.pin = sl["pin"];
        if (sl.containsKey("numLeds")) config.shiftLight.numLeds = sl["numLeds"];
        if (sl.containsKey("startRpmPercent")) config.shiftLight.startRpmPercent = sl["startRpmPercent"];
        if (sl.containsKey("endRpmPercent")) config.shiftLight.endRpmPercent = sl["endRpmPercent"];
        if (sl.containsKey("brightness")) config.shiftLight.brightness = sl["brightness"];
        if (sl.containsKey("colorScheme")) config.shiftLight.colorScheme = sl["colorScheme"];
        if (sl.containsKey("ledMode")) config.shiftLight.ledMode = sl["ledMode"];
        if (sl.containsKey("matrixSerpentine")) config.shiftLight.matrixSerpentine = sl["matrixSerpentine"];
        if (sl.containsKey("matrixStartRow")) config.shiftLight.matrixStartRow = sl["matrixStartRow"];
        if (sl.containsKey("matrixRowCount")) config.shiftLight.matrixRowCount = sl["matrixRowCount"];
        if (sl.containsKey("gearStartRow")) config.shiftLight.gearStartRow = sl["gearStartRow"];
        if (sl.containsKey("gearRowCount")) config.shiftLight.gearRowCount = sl["gearRowCount"];
        
        shiftLightChanged = true;
    }

    config.save();

    // If shift light settings were updated, reconfigure the running module instantly
    if (shiftLightChanged) {
        shiftLight.reconfigure(config.shiftLight);
    }

    // If telemetry settings changed, restart the listener dynamically
    if (telemetryChanged) {
        restartTelemetry();
    }

    sendCORS(200, "text/plain", "OK");
}

void WebConfigServer::handleTestLeds() {
    if (!server.hasArg("percent")) {
        sendCORS(400, "text/plain", "Missing percent param");
        return;
    }

    float percent = server.arg("percent").toFloat();
    if (percent < 0.0f) percent = 0.0f;
    if (percent > 1.0f) percent = 1.0f;

    // Apply the test pattern to the NeoPixel strip
    shiftLight.testPattern(percent);
    sendCORS(200, "text/plain", "OK");
}

void WebConfigServer::handleReboot() {
    sendCORS(200, "text/plain", "Rebooting...");
    delay(500);
    ESP.restart();
}

void WebConfigServer::handleReset() {
    sendCORS(200, "text/plain", "Resetting settings and rebooting...");
    config.reset();
    config.save();
    delay(500);
    ESP.restart();
}

void WebConfigServer::handleOptions() {
    String origin = server.header("Origin");
    if (origin.length() == 0) {
        origin = "*";
    }
    server.sendHeader("Access-Control-Allow-Origin", origin);
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    
    String reqHeaders = server.header("Access-Control-Request-Headers");
    if (reqHeaders.length() == 0) {
        reqHeaders = "Content-Type";
    }
    server.sendHeader("Access-Control-Allow-Headers", reqHeaders);
    server.sendHeader("Access-Control-Allow-Private-Network", "true");
    server.send(204);
}

void WebConfigServer::handleNotFound() {
    if (server.method() == HTTP_OPTIONS) {
        handleOptions();
        return;
    }
    sendCORS(404, "text/plain", "Not Found");
}

void WebConfigServer::sendCORS(int code, const String& contentType, const String& content) {
    String origin = server.header("Origin");
    if (origin.length() == 0) {
        origin = "*";
    }
    server.sendHeader("Access-Control-Allow-Origin", origin);
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    
    String reqHeaders = server.header("Access-Control-Request-Headers");
    if (reqHeaders.length() == 0) {
        reqHeaders = "Content-Type";
    }
    server.sendHeader("Access-Control-Allow-Headers", reqHeaders);
    server.sendHeader("Access-Control-Allow-Private-Network", "true");
    server.send(code, contentType, content);
}

void WebConfigServer::sendJSON(int code, const String& json) {
    sendCORS(code, "application/json", json);
}

void WebConfigServer::handleScanWifi() {
    int n = WiFi.scanNetworks();
    JsonDocument doc;
    JsonArray arr = doc.to<JsonArray>();
    
    int limit = std::min(n, 15);
    for (int i = 0; i < limit; i++) {
        JsonObject net = arr.add<JsonObject>();
        net["ssid"] = WiFi.SSID(i);
        net["rssi"] = WiFi.RSSI(i);
        net["secure"] = (WiFi.encryptionType(i) != WIFI_AUTH_OPEN);
    }
    
    WiFi.scanDelete();

    String response;
    serializeJson(doc, response);
    sendJSON(200, response);
}
