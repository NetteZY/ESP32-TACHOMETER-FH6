#include "ConfigManager.h"

ConfigManager::ConfigManager() {
    // Set safe defaults
    reset();
}

bool ConfigManager::begin() {
    bool ok = preferences.begin("simdash", false);
    if (ok) {
        load();
    }
    return ok;
}

void ConfigManager::load() {
    // WiFi config
    preferences.getBytes("wifi_ssid", wifi.ssid, sizeof(wifi.ssid));
    preferences.getBytes("wifi_pass", wifi.password, sizeof(wifi.password));
    wifi.apMode = preferences.getBool("wifi_ap", wifi.apMode);
    preferences.getBytes("wifi_apname", wifi.apName, sizeof(wifi.apName));
    preferences.getBytes("wifi_appass", wifi.apPassword, sizeof(wifi.apPassword));

    // Telemetry config
    telemetry.port = preferences.getUShort("tel_port", telemetry.port);
    telemetry.enabled = preferences.getBool("tel_en", telemetry.enabled);
    telemetry.gameType = preferences.getUChar("tel_game", telemetry.gameType);
    preferences.getBytes("tel_pcip", telemetry.pcIpAddress, sizeof(telemetry.pcIpAddress));

    // Shift Light config
    shiftLight.enabled = preferences.getBool("sl_en", shiftLight.enabled);
    shiftLight.pin = preferences.getUChar("sl_pin", shiftLight.pin);
    shiftLight.numLeds = preferences.getUChar("sl_numleds", shiftLight.numLeds);
    shiftLight.startRpmPercent = preferences.getFloat("sl_startrpm", shiftLight.startRpmPercent);
    shiftLight.endRpmPercent = preferences.getFloat("sl_endrpm", shiftLight.endRpmPercent);
    shiftLight.brightness = preferences.getUChar("sl_bright", shiftLight.brightness);
    shiftLight.colorScheme = preferences.getUChar("sl_scheme", shiftLight.colorScheme);
}

void ConfigManager::save() {
    // WiFi config
    preferences.putBytes("wifi_ssid", wifi.ssid, sizeof(wifi.ssid));
    preferences.putBytes("wifi_pass", wifi.password, sizeof(wifi.password));
    preferences.putBool("wifi_ap", wifi.apMode);
    preferences.putBytes("wifi_apname", wifi.apName, sizeof(wifi.apName));
    preferences.putBytes("wifi_appass", wifi.apPassword, sizeof(wifi.apPassword));

    // Telemetry config
    preferences.putUShort("tel_port", telemetry.port);
    preferences.putBool("tel_en", telemetry.enabled);
    preferences.putUChar("tel_game", telemetry.gameType);
    preferences.putBytes("tel_pcip", telemetry.pcIpAddress, sizeof(telemetry.pcIpAddress));

    // Shift Light config
    preferences.putBool("sl_en", shiftLight.enabled);
    preferences.putUChar("sl_pin", shiftLight.pin);
    preferences.putUChar("sl_numleds", shiftLight.numLeds);
    preferences.putFloat("sl_startrpm", shiftLight.startRpmPercent);
    preferences.putFloat("sl_endrpm", shiftLight.endRpmPercent);
    preferences.putUChar("sl_bright", shiftLight.brightness);
    preferences.putUChar("sl_scheme", shiftLight.colorScheme);
}

void ConfigManager::reset() {
    // Reset to defaults
    memset(&wifi, 0, sizeof(wifi));
    strncpy(wifi.ssid, "", sizeof(wifi.ssid) - 1);
    strncpy(wifi.password, "", sizeof(wifi.password) - 1);
    wifi.apMode = true;
    strncpy(wifi.apName, "SimDash-ESP32-AP", sizeof(wifi.apName) - 1);
    strncpy(wifi.apPassword, "", sizeof(wifi.apPassword) - 1);

    telemetry.port = 5300;
    telemetry.enabled = true;
    telemetry.gameType = GAME_FORZA;
    strncpy(telemetry.pcIpAddress, "192.168.1.100", sizeof(telemetry.pcIpAddress) - 1);

    shiftLight.enabled = true;
    shiftLight.pin = 15;
    shiftLight.numLeds = 16;
    shiftLight.startRpmPercent = 70.0f;
    shiftLight.endRpmPercent = 95.0f;
    shiftLight.brightness = 64;
    shiftLight.colorScheme = 0;
}
