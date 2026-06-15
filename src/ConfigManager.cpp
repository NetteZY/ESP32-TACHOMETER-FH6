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
    shiftLight.ledMode = preferences.getUChar("sl_mode", shiftLight.ledMode);
    shiftLight.matrixSerpentine = preferences.getBool("sl_serp", shiftLight.matrixSerpentine);
    shiftLight.matrixStartRow = preferences.getUChar("sl_mstart", shiftLight.matrixStartRow);
    shiftLight.matrixRowCount = preferences.getUChar("sl_mrows", shiftLight.matrixRowCount);
    shiftLight.gearStartRow = preferences.getUChar("sl_gstart", shiftLight.gearStartRow);
    shiftLight.gearRowCount = preferences.getUChar("sl_grows", shiftLight.gearRowCount);
}

void ConfigManager::save() {
    // WiFi config
    preferences.putBytes("wifi_ssid", wifi.ssid, sizeof(wifi.ssid));
    preferences.putBytes("wifi_pass", wifi.password, sizeof(wifi.password));

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
    preferences.putUChar("sl_mode", shiftLight.ledMode);
    preferences.putBool("sl_serp", shiftLight.matrixSerpentine);
    preferences.putUChar("sl_mstart", shiftLight.matrixStartRow);
    preferences.putUChar("sl_mrows", shiftLight.matrixRowCount);
    preferences.putUChar("sl_gstart", shiftLight.gearStartRow);
    preferences.putUChar("sl_grows", shiftLight.gearRowCount);
}

void ConfigManager::reset() {
    // Reset to defaults
    memset(&wifi, 0, sizeof(wifi));
    strncpy(wifi.ssid, "", sizeof(wifi.ssid) - 1);
    strncpy(wifi.password, "", sizeof(wifi.password) - 1);

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
    shiftLight.ledMode = 0;
    shiftLight.matrixSerpentine = false;
    shiftLight.matrixStartRow = 0;
    shiftLight.matrixRowCount = 1;
    shiftLight.gearStartRow = 1;
    shiftLight.gearRowCount = 7;
}
