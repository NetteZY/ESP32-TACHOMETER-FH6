#ifndef TELEMETRY_MODULE_H
#define TELEMETRY_MODULE_H

#include "TelemetryData.h"

// Base class interface for all output features/modules (similar to SimDash modules)
class TelemetryModule {
public:
    virtual ~TelemetryModule() {}
    
    // Initialize the module (e.g. setup GPIOs, initialize libraries)
    virtual void begin() = 0;
    
    // Update the module with new telemetry data (called at ~60Hz or when new data arrives)
    virtual void update(const TelemetryData& data) = 0;
    
    // Stop the module (e.g. turn off LEDs when telemetry is inactive)
    virtual void stop() = 0;
    
    // Get the display name of the module
    virtual const char* getName() const = 0;
    
    // Check if the module is currently enabled in settings
    virtual bool isEnabled() const = 0;
};

#endif // TELEMETRY_MODULE_H
