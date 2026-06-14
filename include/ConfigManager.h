#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <Arduino.h>
#include <Preferences.h>

struct WiFiSettings {
    char ssid[32];
    char password[64];
    bool apMode;
    char apName[32];
    char apPassword[64];
};

enum GameType {
    GAME_FORZA = 0,
    GAME_F1 = 1,
    GAME_DIRT_RALLY = 2,
    GAME_ASSETTO_CORSA = 3
};

struct TelemetrySettings {
    uint16_t port;
    bool enabled;
    uint8_t gameType;          // GameType enum
    char pcIpAddress[16];      // IPv4 string e.g. "192.168.1.100"
};

struct ShiftLightSettings {
    bool enabled;
    uint8_t pin;
    uint8_t numLeds;
    float startRpmPercent;       // percentage of max RPM to start lighting up (e.g. 70%)
    float endRpmPercent;         // percentage of max RPM to flash shift indicator (e.g. 95%)
    uint8_t brightness;          // 0-255
    uint8_t colorScheme;         // 0 = Green->Yellow->Red, 1 = Blue->Cyan->Magenta, 2 = Rainbow
};

class ConfigManager {
public:
    ConfigManager();
    bool begin();
    
    void load();
    void save();
    void reset();

    WiFiSettings wifi;
    TelemetrySettings telemetry;
    ShiftLightSettings shiftLight;

private:
    Preferences preferences;
};

#endif // CONFIG_MANAGER_H
