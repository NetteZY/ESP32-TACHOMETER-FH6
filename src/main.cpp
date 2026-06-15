#include <Arduino.h>
#include <WiFi.h>
#include <AsyncUDP.h>
#include <algorithm>

#include "ConfigManager.h"
#include "TelemetryData.h"
#include "RpmShiftLightModule.h"
#include "WebConfigServer.h"

// Global instances
ConfigManager config;
TelemetryData telData;
AsyncUDP udp;

RpmShiftLightModule* shiftLightModule = nullptr;
WebConfigServer* webServer = nullptr;

unsigned long lastStatusUpdateMs = 0;
bool isUdpConnected = false;

// Connect to WiFi or initialize Access Point
void setupWiFi() {
    if (strlen(config.wifi.ssid) > 0) {
        Serial.printf("Connecting to Wi-Fi: %s\n", config.wifi.ssid);
        WiFi.mode(WIFI_STA);
        WiFi.begin(config.wifi.ssid, config.wifi.password);
        
        // Wait up to 15 seconds for connection
        int timeout = 30; 
        while (WiFi.status() != WL_CONNECTED && timeout > 0) {
            delay(500);
            Serial.print(".");
            timeout--;
        }
        Serial.println();

        if (WiFi.status() == WL_CONNECTED) {
            Serial.printf("Connected to Wi-Fi! IP Address: %s\n", WiFi.localIP().toString().c_str());
            return;
        }
        
        Serial.println("Wi-Fi connection failed. Falling back to Access Point Mode...");
    }

    // Access Point Mode fallback
    WiFi.mode(WIFI_AP);
    bool apCreated = WiFi.softAP("SimDash-ESP32-AP");

    if (apCreated) {
        Serial.printf("Access Point created successfully!\n");
        Serial.printf("SSID: SimDash-ESP32-AP\n");
        Serial.printf("IP Address: %s\n", WiFi.softAPIP().toString().c_str());
    } else {
        Serial.println("Failed to start Access Point.");
    }
}

// Start listening for Forza Horizon UDP telemetry packets
void setupTelemetry() {
    if (!config.telemetry.enabled) {
        Serial.println("Telemetry listener is disabled in settings.");
        return;
    }

    if (udp.listen(config.telemetry.port)) {
        Serial.printf("UDP Telemetry listener started on port %d\n", config.telemetry.port);
        isUdpConnected = true;
        
        udp.onPacket([](AsyncUDPPacket packet) {
            const uint8_t* rawData = packet.data();
            size_t len = packet.length();

            switch (config.telemetry.gameType) {
                case GAME_FORZA: {
                    if (len >= 323 && len <= 332) {
                        ForzaTelemetryPacket forzaPacket;
                        memcpy(&forzaPacket, rawData, std::min(len, sizeof(ForzaTelemetryPacket)));

                        telData.isRaceOn = (forzaPacket.IsRaceOn == 1);
                        telData.timestampMs = forzaPacket.TimestampMS;
                        telData.currentRpm = forzaPacket.CurrentEngineRpm;
                        telData.maxRpm = forzaPacket.EngineMaxRpm;
                        telData.idleRpm = forzaPacket.EngineIdleRpm;
                        
                        float speedMs = sqrt(forzaPacket.VelocityX * forzaPacket.VelocityX +
                                             forzaPacket.VelocityY * forzaPacket.VelocityY +
                                             forzaPacket.VelocityZ * forzaPacket.VelocityZ);
                        telData.speedKmh = speedMs * 3.6f;
                        telData.speedMph = speedMs * 2.23694f;
                        
                        if (forzaPacket.Gear == 0) telData.gear = 1; // N
                        else if (forzaPacket.Gear >= 11 || forzaPacket.Gear == 255) telData.gear = 0; // R
                        else telData.gear = forzaPacket.Gear + 1; // 1st -> 2
                        
                        telData.accelPercent = (uint8_t)((forzaPacket.Accel / 255.0f) * 100.0f);
                        telData.brakePercent = (uint8_t)((forzaPacket.Brake / 255.0f) * 100.0f);
                        
                        float boostPsi = forzaPacket.Boost * 0.000145038f;
                        telData.boost = (boostPsi > 0.0f) ? boostPsi : 0.0f;
                        
                        float slipFL = std::abs(forzaPacket.TireSlipRatioFrontLeft);
                        float slipFR = std::abs(forzaPacket.TireSlipRatioFrontRight);
                        float slipRL = std::abs(forzaPacket.TireSlipRatioRearLeft);
                        float slipRR = std::abs(forzaPacket.TireSlipRatioRearRight);
                        telData.tireSlipMax = std::max({slipFL, slipFR, slipRL, slipRR});

                        telData.lastPacketTimeMs = millis();
                        telData.isUpdating = true;
                        telData.packetsReceived++;
                    }
                    break;
                }
                case GAME_F1: {
                    if (len >= sizeof(F1PacketHeader)) {
                        F1PacketHeader header;
                        memcpy(&header, rawData, sizeof(F1PacketHeader));
                        
                        // Car Telemetry (Packet ID 6)
                        if (header.m_packetId == 6 && len >= sizeof(F1PacketCarTelemetryData)) {
                            F1PacketCarTelemetryData f1Packet;
                            memcpy(&f1Packet, rawData, sizeof(F1PacketCarTelemetryData));
                            
                            uint8_t playerIdx = header.m_playerCarIndex;
                            F1CarTelemetryData& carData = f1Packet.m_carTelemetryData[playerIdx];
                            
                            telData.isRaceOn = true;
                            telData.timestampMs = (uint32_t)(header.m_sessionTime * 1000.0f);
                            
                            // Mock max RPM to 100 and use revLightsPercent as currentRpm for accurate shift light
                            telData.maxRpm = 100.0f;
                            telData.idleRpm = 0.0f;
                            telData.currentRpm = (float)carData.m_revLightsPercent;
                            
                            telData.speedKmh = (float)carData.m_speed;
                            telData.speedMph = telData.speedKmh * 0.621371f;
                            
                            if (carData.m_gear == -1) telData.gear = 0;
                            else if (carData.m_gear == 0) telData.gear = 1;
                            else telData.gear = carData.m_gear + 1;
                            
                            telData.accelPercent = (uint8_t)(carData.m_throttle * 100.0f);
                            telData.brakePercent = (uint8_t)(carData.m_brake * 100.0f);
                            telData.boost = 0.0f;
                            telData.tireSlipMax = 0.0f;
                            
                            telData.lastPacketTimeMs = millis();
                            telData.isUpdating = true;
                            telData.packetsReceived++;
                        }
                    }
                    break;
                }
                case GAME_DIRT_RALLY: {
                    if (len >= 264) {
                        DirtRallyTelemetryPacket drPacket;
                        memcpy(&drPacket, rawData, sizeof(DirtRallyTelemetryPacket));
                        
                        telData.isRaceOn = true;
                        telData.timestampMs = (uint32_t)(drPacket.time * 1000.0f);
                        telData.currentRpm = drPacket.engine_rpm * 10.0f;
                        telData.maxRpm = drPacket.max_rpm * 10.0f;
                        telData.idleRpm = drPacket.idle_rpm * 10.0f;
                        
                        telData.speedKmh = drPacket.speed * 3.6f;
                        telData.speedMph = drPacket.speed * 2.23694f;
                        
                        int drGear = (int)drPacket.gear;
                        if (drGear == 10 || drGear == -1) telData.gear = 0;
                        else if (drGear == 0) telData.gear = 1;
                        else telData.gear = drGear + 1;
                        
                        telData.accelPercent = (uint8_t)(drPacket.throttle * 100.0f);
                        telData.brakePercent = (uint8_t)(drPacket.brake * 100.0f);
                        telData.boost = 0.0f;
                        telData.tireSlipMax = 0.0f;
                        
                        telData.lastPacketTimeMs = millis();
                        telData.isUpdating = true;
                        telData.packetsReceived++;
                    }
                    break;
                }
                case GAME_ASSETTO_CORSA: {
                    if (len >= sizeof(AssettoCorsaCarUpdate)) {
                        AssettoCorsaCarUpdate acPacket;
                        memcpy(&acPacket, rawData, sizeof(AssettoCorsaCarUpdate));
                        
                        telData.isRaceOn = true;
                        telData.timestampMs = millis();
                        telData.currentRpm = acPacket.engineRpm;
                        telData.maxRpm = acPacket.maxEngineRpm;
                        telData.idleRpm = 1000.0f;
                        
                        telData.speedKmh = acPacket.speed_Kmh;
                        telData.speedMph = acPacket.speed_Kmh * 0.621371f;
                        
                        telData.gear = acPacket.gear;
                        telData.accelPercent = (uint8_t)(acPacket.gas * 100.0f);
                        telData.brakePercent = (uint8_t)(acPacket.brake * 100.0f);
                        telData.boost = acPacket.turboBoost * 14.5038f;
                        telData.tireSlipMax = 0.0f;
                        
                        telData.lastPacketTimeMs = millis();
                        telData.isUpdating = true;
                        telData.packetsReceived++;
                    }
                    break;
                }
            }
        });
    } else {
        Serial.printf("Failed to bind UDP port %d!\n", config.telemetry.port);
    }
}

void restartTelemetry() {
    Serial.println("Restarting telemetry UDP listener...");
    udp.close();
    isUdpConnected = false;
    memset(&telData, 0, sizeof(telData));
    telData.isUpdating = false;
    setupTelemetry();
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n--- SimDash ESP32 Core Booting ---");

    // Initialize config manager
    if (!config.begin()) {
        Serial.println("Warning: Config NVS initialization failed! Using defaults.");
    }

    // Connect WiFi (Station/AP)
    setupWiFi();

    // Initialize outputs
    shiftLightModule = new RpmShiftLightModule(config.shiftLight);
    shiftLightModule->begin();

    // Start Telemetry UDP listener
    memset(&telData, 0, sizeof(telData));
    telData.isUpdating = false;
    setupTelemetry();

    // Start Web configuration server
    webServer = new WebConfigServer(config, telData, *shiftLightModule);
    webServer->begin();

    Serial.println("Boot sequence complete. Running system loop...");
}

void loop() {
    // Process incoming HTTP requests for configuration
    if (webServer) {
        webServer->handle();
    }

    // Telemetry timeout detection (2 seconds of silence = offline)
    if (telData.isUpdating && (millis() - telData.lastPacketTimeMs > 2000)) {
        telData.isUpdating = false;
        Serial.println("Telemetry stream timed out. Entering idle state.");
        
        // Turn off outputs when telemetry disconnects
        if (shiftLightModule) {
            shiftLightModule->stop();
        }
    }

    // Assetto Corsa Periodic Handshake
    static unsigned long lastHandshakeMs = 0;
    if (!telData.isUpdating && config.telemetry.gameType == GAME_ASSETTO_CORSA && strlen(config.telemetry.pcIpAddress) > 0 && (millis() - lastHandshakeMs > 3000)) {
        lastHandshakeMs = millis();
        IPAddress pcIp;
        if (pcIp.fromString(config.telemetry.pcIpAddress)) {
            AssettoCorsaHandshake hs;
            hs.identifier = 1;
            hs.version = 1;
            hs.operationId = 1;
            udp.writeTo((uint8_t*)&hs, sizeof(hs), pcIp, 9996);
        }
    }

    // Update active modules with latest telemetry data
    if (shiftLightModule && telData.isUpdating) {
        shiftLightModule->update(telData);
    }

    // Yield to WiFi background tasks
    delay(2);
}
