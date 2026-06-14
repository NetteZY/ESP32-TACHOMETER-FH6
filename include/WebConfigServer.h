#ifndef WEB_CONFIG_SERVER_H
#define WEB_CONFIG_SERVER_H

#include <Arduino.h>
#include <WebServer.h>
#include "ConfigManager.h"
#include "TelemetryData.h"
#include "RpmShiftLightModule.h"

class WebConfigServer {
public:
    WebConfigServer(ConfigManager& config, TelemetryData& telData, RpmShiftLightModule& shiftLight);
    
    void begin();
    void handle();

private:
    WebServer server;
    ConfigManager& config;
    TelemetryData& telData;
    RpmShiftLightModule& shiftLight;

    // HTTP Route Handlers
    void handleRoot();
    void handleGetStatus();
    void handleGetConfig();
    void handleSaveConfig();
    void handleTestLeds();
    void handleReboot();
    void handleReset();
    void handleNotFound();
    void handleOptions();

    // Helpers to send responses with CORS and PNA headers
    void sendCORS(int code, const String& contentType, const String& content);
    void sendJSON(int code, const String& json);
};

#endif // WEB_CONFIG_SERVER_H
