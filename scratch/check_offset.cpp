#include <iostream>
#include <cstdint>
#include <cstddef>

#pragma pack(push, 1)
struct ForzaTelemetryPacket {
    int32_t IsRaceOn;                  // 0 = menu, 1 = race
    uint32_t TimestampMS;              // Can overflow
    float EngineMaxRpm;
    float EngineIdleRpm;
    float CurrentEngineRpm;
    float AccelerationX;               // In car's local space (X=right, Y=up, Z=forward)
    float AccelerationY;
    float AccelerationZ;
    float VelocityX;
    float VelocityY;
    float VelocityZ;
    float AngularVelocityX;            // X=pitch, Y=yaw, Z=roll
    float AngularVelocityY;
    float AngularVelocityZ;
    float Yaw;
    float Pitch;
    float Roll;
    float NormalizedSuspensionTravelFrontLeft; // 0.0 = max stretch, 1.0 = max compression
    float NormalizedSuspensionTravelFrontRight;
    float NormalizedSuspensionTravelRearLeft;
    float NormalizedSuspensionTravelRearRight;
    float TireSlipRatioFrontLeft;      // 0 = 100% grip, |ratio| > 1.0 = loss of grip
    float TireSlipRatioFrontRight;
    float TireSlipRatioRearLeft;
    float TireSlipRatioRearRight;
    float WheelRotationSpeedFrontLeft; // Wheel speed rad/s
    float WheelRotationSpeedFrontRight;
    float WheelRotationSpeedRearLeft;
    float WheelRotationSpeedRearRight;
    int32_t WheelOnRumbleStripFrontLeft; // 1 = on rumble strip, 0 = off
    int32_t WheelOnRumbleStripFrontRight;
    int32_t WheelOnRumbleStripRearLeft;
    int32_t WheelOnRumbleStripRearRight;
    float WheelInPuddleDepthFrontLeft; // 0 to 1
    float WheelInPuddleDepthFrontRight;
    float WheelInPuddleDepthRearLeft;
    float WheelInPuddleDepthRearRight;
    float SurfaceRumbleFrontLeft;      // 0 to 1
    float SurfaceRumbleFrontRight;
    float SurfaceRumbleRearLeft;
    float SurfaceRumbleRearRight;
    float TireSlipAngleFrontLeft;      // 0 = 100% grip, |angle| > 1.0 = loss of grip
    float TireSlipAngleFrontRight;
    float TireSlipAngleRearLeft;
    float TireSlipAngleRearRight;
    float TireCombinedSlipFrontLeft;
    float TireCombinedSlipFrontRight;
    float TireCombinedSlipRearLeft;
    float TireCombinedSlipRearRight;
    float SuspensionTravelMetersFrontLeft;
    float SuspensionTravelMetersFrontRight;
    float SuspensionTravelMetersRearLeft;
    float SuspensionTravelMetersRearRight;
    int32_t CarOrdinal;                // Unique ID of car make/model
    int32_t CarClass;                  // 0 (D) to 7 (X)
    int32_t CarPerformanceIndex;       // 100 to 999
    int32_t DrivetrainType;            // 0=FWD, 1=RWD, 2=AWD
    int32_t NumCylinders;
    int32_t CarCategory;
    uint32_t HorizonUnknown1;
    uint32_t HorizonUnknown2;
    float PositionX;
    float PositionY;
    float PositionZ;
    float Speed;                       // m/s
    float Power;                       // Watts
    float Torque;                      // N-m
    float TireTempFrontLeft;
    float TireTempFrontRight;
    float TireTempRearLeft;
    float TireTempRearRight;
    float Boost;
    float Fuel;
    float DistanceTraveled;
    float BestLap;
    float LastLap;
    float CurrentLap;
    float CurrentRaceTime;
    uint16_t LapNumber;
    uint8_t RacePosition;
    uint8_t Accel;                     // 0 to 255
    uint8_t Brake;                     // 0 to 255
    uint8_t Clutch;                    // 0 to 255
    uint8_t HandBrake;                 // 0 to 255
    uint8_t Gear;                      // 0 = Reverse, 1 = Neutral, 2 = 1st, 3 = 2nd, etc.
    int8_t Steer;                      // -127 to 127
    int8_t NormalizedDrivingLine;
    int8_t NormalizedAIBrakeDifference;
    uint8_t HorizonTrailingUnknown;    // Extra trailing byte
};
#pragma pack(pop)

int main() {
    std::cout << "Size: " << sizeof(ForzaTelemetryPacket) << std::endl;
    std::cout << "Speed: " << offsetof(ForzaTelemetryPacket, Speed) << std::endl;
    std::cout << "Gear: " << offsetof(ForzaTelemetryPacket, Gear) << std::endl;
    std::cout << "CarOrdinal: " << offsetof(ForzaTelemetryPacket, CarOrdinal) << std::endl;
    return 0;
}
