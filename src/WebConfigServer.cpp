#include "WebConfigServer.h"
#include "html_index.h"
#include <ArduinoJson.h>

WebConfigServer::WebConfigServer(ConfigManager& config, TelemetryData& telData, RpmShiftLightModule& shiftLight)
    : server(80), config(config), telData(telData), shiftLight(shiftLight) {
}

void WebConfigServer::begin() {
    server.on("/", HTTP_GET, std::bind(&WebConfigServer::handleRoot, this));
    
    // API GET/POST handlers
    server.on("/api/status", HTTP_GET, std::bind(&WebConfigServer::handleGetStatus, this));
    server.on("/api/config", HTTP_GET, std::bind(&WebConfigServer::handleGetConfig, this));
    server.on("/api/save", HTTP_POST, std::bind(&WebConfigServer::handleSaveConfig, this));
    server.on("/api/test_leds", HTTP_GET, std::bind(&WebConfigServer::handleTestLeds, this));
    server.on("/api/reboot", HTTP_POST, std::bind(&WebConfigServer::handleReboot, this));
    server.on("/api/reset", HTTP_POST, std::bind(&WebConfigServer::handleReset, this));
    
    // Explicit OPTIONS handlers for CORS preflight
    server.on("/api/status", HTTP_OPTIONS, std::bind(&WebConfigServer::handleOptions, this));
    server.on("/api/config", HTTP_OPTIONS, std::bind(&WebConfigServer::handleOptions, this));
    server.on("/api/save", HTTP_OPTIONS, std::bind(&WebConfigServer::handleOptions, this));
    server.on("/api/test_leds", HTTP_OPTIONS, std::bind(&WebConfigServer::handleOptions, this));
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

    String response;
    serializeJson(doc, response);
    sendJSON(200, response);
}

void WebConfigServer::handleGetConfig() {
    JsonDocument doc;
    
    // WiFi
    JsonObject wifi = doc["wifi"].to<JsonObject>();
    wifi["ssid"] = config.wifi.ssid;
    wifi["apMode"] = config.wifi.apMode;
    wifi["apName"] = config.wifi.apName;
    wifi["password"] = (strlen(config.wifi.password) > 0) ? "••••••••" : "";
    wifi["apPassword"] = (strlen(config.wifi.apPassword) > 0) ? "••••••••" : "";

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
        if (wifi.containsKey("apMode")) config.wifi.apMode = wifi["apMode"];
        if (wifi.containsKey("apName")) strncpy(config.wifi.apName, wifi["apName"], sizeof(config.wifi.apName) - 1);
        if (wifi.containsKey("apPassword")) {
            const char* apPass = wifi["apPassword"];
            if (strcmp(apPass, "••••••••") != 0) {
                strncpy(config.wifi.apPassword, apPass, sizeof(config.wifi.apPassword) - 1);
            }
        }
    }

    // Parse Telemetry settings
    if (doc.containsKey("telemetry")) {
        JsonObject tel = doc["telemetry"];
        if (tel.containsKey("port")) config.telemetry.port = tel["port"];
        if (tel.containsKey("enabled")) config.telemetry.enabled = tel["enabled"];
        if (tel.containsKey("gameType")) config.telemetry.gameType = tel["gameType"];
        if (tel.containsKey("pcIpAddress")) strncpy(config.telemetry.pcIpAddress, tel["pcIpAddress"], sizeof(config.telemetry.pcIpAddress) - 1);
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
        
        shiftLightChanged = true;
    }

    config.save();

    // If shift light settings were updated, reconfigure the running module instantly
    if (shiftLightChanged) {
        shiftLight.reconfigure(config.shiftLight);
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
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
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
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.sendHeader("Access-Control-Allow-Private-Network", "true");
    server.send(code, contentType, content);
}

void WebConfigServer::sendJSON(int code, const String& json) {
    sendCORS(code, "application/json", json);
}
