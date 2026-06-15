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
    uint16_t numLeds = (settings.ledMode == 1) ? 64 : settings.numLeds;
    strip = new Adafruit_NeoPixel(numLeds, settings.pin, NEO_GRB + NEO_KHZ800);
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

    updateLeds(data);
}

// 5x7 Font bitmaps for R, N, 1-9, and - (dash)
static const uint8_t GEAR_BITMAPS[12][7] = {
    // 0: R
    { 0b01111100, 0b01100110, 0b01100110, 0b01111100, 0b01101100, 0b01100110, 0b01100110 },
    // 1: N
    { 0b01100110, 0b01110110, 0b01111110, 0b01101110, 0b01100110, 0b01100110, 0b01100110 },
    // 2: 1
    { 0b00011000, 0b00111000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00111100 },
    // 3: 2
    { 0b00111100, 0b01100110, 0b00000110, 0b00011100, 0b00110000, 0b01100000, 0b01111110 },
    // 4: 3
    { 0b00111100, 0b01100110, 0b00000110, 0b00011100, 0b00000110, 0b01100110, 0b00111100 },
    // 5: 4
    { 0b00001100, 0b00011100, 0b00101100, 0b01001100, 0b01111110, 0b00001100, 0b00001100 },
    // 6: 5
    { 0b01111110, 0b01100000, 0b01111100, 0b00000110, 0b00000110, 0b01100110, 0b00111100 },
    // 7: 6
    { 0b00111100, 0b01100000, 0b01111100, 0b01100110, 0b01100110, 0b01100110, 0b00111100 },
    // 8: 7
    { 0b01111110, 0b00000110, 0b00001100, 0b00011000, 0b00110000, 0b01100000, 0b01100000 },
    // 9: 8
    { 0b00111100, 0b01100110, 0b01100110, 0b00111100, 0b01100110, 0b01100110, 0b00111100 },
    // 10: 9
    { 0b00111100, 0b01100110, 0b01100110, 0b00111110, 0b00000110, 0b01100110, 0b00111100 },
    // 11: - (dash)
    { 0b00000000, 0b00000000, 0b00000000, 0b00111100, 0b00000000, 0b00000000, 0b00000000 }
};

int RpmShiftLightModule::getMatrixLedIndex(int x, int y, bool serpentine) {
    if (x < 0 || x > 7 || y < 0 || y > 7) return -1;
    if (serpentine) {
        return (y % 2 == 0) ? (y * 8 + x) : (y * 8 + (7 - x));
    } else {
        return y * 8 + x;
    }
}

void RpmShiftLightModule::drawGearMatrix(uint8_t gear, uint8_t startRow, uint8_t rowCount, uint32_t color, bool serpentine) {
    if (rowCount == 0 || !strip) return;

    // Get the 7-row bitmap for the gear
    const uint8_t* bitmap = GEAR_BITMAPS[gear < 12 ? gear : 11];

    int bmpStartRow = 0;
    int physStartRow = startRow;
    int linesToDraw = rowCount;

    if (rowCount >= 7) {
        // Center vertically
        physStartRow = startRow + (rowCount - 7) / 2;
        linesToDraw = 7;
        bmpStartRow = 0;
    } else {
        // Crop the bitmap (prefer keeping the middle/important lines)
        bmpStartRow = (7 - rowCount) / 2;
        linesToDraw = rowCount;
        physStartRow = startRow;
    }

    for (int y = 0; y < linesToDraw; y++) {
        uint8_t rowByte = bitmap[bmpStartRow + y];
        int physicalY = physStartRow + y;
        if (physicalY < 0 || physicalY > 7) continue;

        for (int x = 0; x < 8; x++) {
            // Bit 7 is column 0, bit 0 is column 7
            bool pixelOn = (rowByte & (0x80 >> x)) != 0;
            if (pixelOn) {
                int ledIdx = getMatrixLedIndex(x, physicalY, serpentine);
                if (ledIdx != -1) {
                    strip->setPixelColor(ledIdx, color);
                }
            }
        }
    }
}

void RpmShiftLightModule::updateLeds(const TelemetryData& data) {
    if (!strip) return;

    float currentRpm = data.currentRpm;
    float maxRpm = data.maxRpm;
    float idleRpm = data.idleRpm;
    uint8_t gear = data.gear;

    if (maxRpm <= idleRpm) {
        strip->clear();
        strip->show();
        return;
    }

    // Calculate RPM thresholds
    float startRpm = maxRpm * (settings.startRpmPercent / 100.0f);
    float endRpm = maxRpm * (settings.endRpmPercent / 100.0f);

    // Get the flash color based on the selected scheme
    uint32_t flashColor = strip->Color(255, 0, 0); // Red default
    if (settings.colorScheme == 1) {
        flashColor = strip->Color(255, 0, 255); // Magenta
    } else if (settings.colorScheme == 2) {
        flashColor = strip->Color(255, 255, 255); // White
    }

    // If RPM is above shift/end threshold, flash active zones
    if (currentRpm >= endRpm) {
        uint32_t now = millis();
        if (now - lastFlashTimeMs >= 80) { // 80ms flash interval
            flashState = !flashState;
            lastFlashTimeMs = now;
        }

        strip->clear();
        if (flashState) {
            if (settings.ledMode == 0) {
                // Flash all strip LEDs
                for (int i = 0; i < settings.numLeds; i++) {
                    strip->setPixelColor(i, flashColor);
                }
            } else {
                // Flash both shift light rows and gear indicator rows
                int totalShiftLeds = settings.matrixRowCount * 8;
                for (int i = 0; i < totalShiftLeds; i++) {
                    int row = settings.matrixStartRow + (i / 8);
                    int col = i % 8;
                    int ledIdx = getMatrixLedIndex(col, row, settings.matrixSerpentine);
                    if (ledIdx != -1) {
                        strip->setPixelColor(ledIdx, flashColor);
                    }
                }
                // Draw gear fully in flashColor
                if (settings.gearRowCount > 0) {
                    drawGearMatrix(gear, settings.gearStartRow, settings.gearRowCount, flashColor, settings.matrixSerpentine);
                }
            }
        }
        strip->show();
        return;
    }

    // If RPM is below start threshold, turn off shift lights, but still display gear in matrix mode
    if (currentRpm < startRpm) {
        strip->clear();
        if (settings.ledMode == 1 && settings.gearRowCount > 0) {
            uint32_t gearColor = strip->Color(0, 170, 255); // Premium Ice Blue
            drawGearMatrix(gear, settings.gearStartRow, settings.gearRowCount, gearColor, settings.matrixSerpentine);
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

    strip->clear();

    if (settings.ledMode == 0) {
        // Strip mode
        int numLedsToLight = (int)(fillFraction * settings.numLeds + 0.5f);
        for (int i = 0; i < settings.numLeds; i++) {
            if (i < numLedsToLight) {
                strip->setPixelColor(i, getColorForLed(i, settings.numLeds, fillFraction));
            }
        }
    } else {
        // Matrix mode
        int totalShiftLeds = settings.matrixRowCount * 8;
        int numLedsToLight = (int)(fillFraction * totalShiftLeds + 0.5f);
        for (int i = 0; i < totalShiftLeds; i++) {
            if (i < numLedsToLight) {
                int row = settings.matrixStartRow + (i / 8);
                int col = i % 8;
                int ledIdx = getMatrixLedIndex(col, row, settings.matrixSerpentine);
                if (ledIdx != -1) {
                    strip->setPixelColor(ledIdx, getColorForLed(i, totalShiftLeds, fillFraction));
                }
            }
        }
        // Draw the gear centered
        if (settings.gearRowCount > 0) {
            uint32_t gearColor = strip->Color(0, 170, 255); // Premium Ice Blue
            drawGearMatrix(gear, settings.gearStartRow, settings.gearRowCount, gearColor, settings.matrixSerpentine);
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

    strip->clear();

    if (settings.ledMode == 0) {
        // Strip Mode
        int numLedsToLight = (int)(percent * settings.numLeds);
        for (int i = 0; i < settings.numLeds; i++) {
            if (i < numLedsToLight) {
                if (color != 0) {
                    strip->setPixelColor(i, color);
                } else {
                    strip->setPixelColor(i, getColorForLed(i, settings.numLeds, percent));
                }
            }
        }
    } else {
        // Matrix Mode
        int totalShiftLeds = settings.matrixRowCount * 8;
        int numLedsToLight = (int)(percent * totalShiftLeds);

        // Flash behavior for test pattern if percent is 1.0 (Redline)
        uint32_t activeColor = color;
        if (percent >= 1.0f) {
            activeColor = strip->Color(255, 0, 0); // Red default
            if (settings.colorScheme == 1) activeColor = strip->Color(255, 0, 255);
            else if (settings.colorScheme == 2) activeColor = strip->Color(255, 255, 255);
        }

        // Draw Shift Lights
        for (int i = 0; i < totalShiftLeds; i++) {
            if (i < numLedsToLight) {
                int row = settings.matrixStartRow + (i / 8);
                int col = i % 8;
                int ledIdx = getMatrixLedIndex(col, row, settings.matrixSerpentine);
                if (ledIdx != -1) {
                    if (activeColor != 0) {
                        strip->setPixelColor(ledIdx, activeColor);
                    } else {
                        strip->setPixelColor(ledIdx, getColorForLed(i, totalShiftLeds, percent));
                    }
                }
            }
        }

        // Draw Gear '8' (as it's a test pattern)
        if (settings.gearRowCount > 0) {
            uint32_t gearColor = (activeColor != 0) ? activeColor : strip->Color(0, 170, 255);
            drawGearMatrix(9, settings.gearStartRow, settings.gearRowCount, gearColor, settings.matrixSerpentine); // Index 9 is '8'
        }
    }

    strip->show();
}
