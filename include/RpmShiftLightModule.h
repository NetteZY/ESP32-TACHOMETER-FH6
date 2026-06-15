#ifndef RPM_SHIFT_LIGHT_MODULE_H
#define RPM_SHIFT_LIGHT_MODULE_H

#include "TelemetryModule.h"
#include "ConfigManager.h"
#include <Adafruit_NeoPixel.h>

class RpmShiftLightModule : public TelemetryModule {
public:
    RpmShiftLightModule(const ShiftLightSettings& settings);
    virtual ~RpmShiftLightModule();

    virtual void begin() override;
    virtual void update(const TelemetryData& data) override;
    virtual void stop() override;
    virtual const char* getName() const override { return "RPM Shift Light"; }
    virtual bool isEnabled() const override { return settings.enabled; }

    // Reinitializes the NeoPixel strip with new settings (pin, count, brightness) at runtime
    void reconfigure(const ShiftLightSettings& newSettings);

    // Dynamic test function to light up a specific percentage of LEDs
    void testPattern(float percent, uint32_t color = 0);

private:
    ShiftLightSettings settings;
    Adafruit_NeoPixel* strip;
    bool isInitialized;
    bool flashState;
    uint32_t lastFlashTimeMs;

    void updateLeds(const TelemetryData& data);
    uint32_t getColorForLed(uint8_t ledIndex, uint8_t totalLeds, float percent);
    int getMatrixLedIndex(int x, int y, bool serpentine);
    void drawGearMatrix(uint8_t gear, uint8_t startRow, uint8_t rowCount, uint32_t color, bool serpentine);
};

#endif // RPM_SHIFT_LIGHT_MODULE_H
