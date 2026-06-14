#include "RpmShiftLightModule.h"

RpmShiftLightModule::RpmShiftLightModule(const ShiftLightSettings& settings) 
    : settings(settings), strip(nullptr), isInitialized(false), flashState(false), lastFlashTimeMs(0) {
}

RpmShiftLightModule::~RpmShiftLightModule() {
    isInitialized = false;
    stop();
    if (strip) {
        strip->updateLength(0);
        strip->show();
        delete strip;
        strip = nullptr;
    }
}

void RpmShiftLightModule::begin() {
    if (!settings.enabled) {
        return;
    }

    if (strip) {
        strip->updateLength(0);
        strip->show();
        delete strip;
        strip = nullptr;
    }

    // Allocate the NeoPixel strip dynamically
    strip = new Adafruit_NeoPixel(settings.numLeds, settings.pin, NEO_GRB + NEO_KHZ800);
    strip->begin();
    strip->setBrightness(settings.brightness);
    strip->clear();
    strip->show();

    isInitialized = true;
    flashState = false;
    lastFlashTimeMs = millis();
}

void RpmShiftLightModule::stop() {
    if (isInitialized && strip) {
        strip->clear();
        strip->show();
    }
}

void RpmShiftLightModule::reconfigure(const ShiftLightSettings& newSettings) {
    isInitialized = false;
    stop();
    settings = newSettings;
    begin();
}

void RpmShiftLightModule::update(const TelemetryData& data) {
    if (!settings.enabled || !isInitialized || !strip) {
        return;
    }

    // If telemetry has stopped updating, clear the LEDs
    if (!data.isUpdating || !data.isRaceOn) {
        stop();
        return;
    }

    updateLeds(data.currentRpm, data.maxRpm, data.idleRpm);
}

void RpmShiftLightModule::updateLeds(float currentRpm, float maxRpm, float idleRpm) {
    if (maxRpm <= idleRpm) {
        strip->clear();
        strip->show();
        return;
    }

    // Calculate RPM thresholds
    float startRpm = maxRpm * (settings.startRpmPercent / 100.0f);
    float endRpm = maxRpm * (settings.endRpmPercent / 100.0f);

    // If RPM is below start threshold, turn off all LEDs
    if (currentRpm < startRpm) {
        strip->clear();
        strip->show();
        return;
    }

    // If RPM is above shift/end threshold, flash all LEDs to prompt user to shift
    if (currentRpm >= endRpm) {
        uint32_t now = millis();
        if (now - lastFlashTimeMs >= 80) { // 80ms flash interval
            flashState = !flashState;
            lastFlashTimeMs = now;
        }

        if (flashState) {
            // Flash color based on scheme (Red or Blue/Purple or White)
            uint32_t flashColor = strip->Color(255, 0, 0); // Red default
            if (settings.colorScheme == 1) {
                flashColor = strip->Color(255, 0, 255); // Magenta
            } else if (settings.colorScheme == 2) {
                flashColor = strip->Color(255, 255, 255); // White
            }
            
            for (int i = 0; i < settings.numLeds; i++) {
                strip->setPixelColor(i, flashColor);
            }
        } else {
            strip->clear();
        }
        strip->show();
        return;
    }

    // Normal linear shift light range
    float range = endRpm - startRpm;
    float currentOffset = currentRpm - startRpm;
    float fillFraction = currentOffset / range;
    if (fillFraction > 1.0f) fillFraction = 1.0f;
    if (fillFraction < 0.0f) fillFraction = 0.0f;

    int numLedsToLight = (int)(fillFraction * settings.numLeds + 0.5f);

    for (int i = 0; i < settings.numLeds; i++) {
        if (i < numLedsToLight) {
            strip->setPixelColor(i, getColorForLed(i, settings.numLeds, fillFraction));
        } else {
            strip->setPixelColor(i, 0); // Turn off
        }
    }
    strip->show();
}

uint32_t RpmShiftLightModule::getColorForLed(uint8_t ledIndex, uint8_t totalLeds, float percent) {
    if (!strip) return 0;

    // Normalize LED index position (0.0 to 1.0)
    float pos = (float)ledIndex / (float)(totalLeds - 1);

    if (settings.colorScheme == 0) {
        // Scheme 0: Green -> Yellow -> Red
        if (pos < 0.5f) {
            return strip->Color(0, 255, 0); // Green
        } else if (pos < 0.8f) {
            return strip->Color(255, 180, 0); // Yellow/Orange
        } else {
            return strip->Color(255, 0, 0); // Red
        }
    } else if (settings.colorScheme == 1) {
        // Scheme 1: Blue -> Cyan -> Magenta
        if (pos < 0.5f) {
            return strip->Color(0, 0, 255); // Blue
        } else if (pos < 0.8f) {
            return strip->Color(0, 255, 255); // Cyan
        } else {
            return strip->Color(255, 0, 255); // Magenta
        }
    } else {
        // Scheme 2: Rainbow (HSV mapping)
        // Hue range from red (0) to violet (approx 50000)
        uint16_t hue = (uint16_t)(pos * 50000.0f);
        return strip->ColorHSV(hue, 255, 255);
    }
}

void RpmShiftLightModule::testPattern(float percent, uint32_t color) {
    if (!strip || !isInitialized) return;

    int numLedsToLight = (int)(percent * settings.numLeds);
    
    for (int i = 0; i < settings.numLeds; i++) {
        if (i < numLedsToLight) {
            if (color != 0) {
                strip->setPixelColor(i, color);
            } else {
                strip->setPixelColor(i, getColorForLed(i, settings.numLeds, percent));
            }
        } else {
            strip->setPixelColor(i, 0);
        }
    }
    strip->show();
}
