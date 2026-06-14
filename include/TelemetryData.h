#ifndef TELEMETRY_DATA_H
#define TELEMETRY_DATA_H

#include <Arduino.h>
#include <cstdint>

#pragma pack(push, 1)
// Forza Horizon 4/5/6 324-byte UDP Telemetry packet structure ("Car Dash" format)
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

#pragma pack(push, 1)
// F1 23 Telemetry Packet Structures
struct F1PacketHeader {
    uint16_t m_packetFormat;
    uint8_t m_gameYear;
    uint8_t m_gameMajorVersion;
    uint8_t m_gameMinorVersion;
    uint8_t m_packetVersion;
    uint8_t m_packetId; // 6 = Car Telemetry
    uint64_t m_sessionUID;
    float m_sessionTime;
    uint32_t m_frameIdentifier;
    uint32_t m_overallFrameIdentifier;
    uint8_t m_playerCarIndex;
    uint8_t m_secondaryPlayerCarIndex;
};

struct F1CarTelemetryData {
    uint16_t m_speed;            // km/h
    float m_throttle;            // 0.0 - 1.0
    float m_steer;               // -1.0 - 1.0
    float m_brake;               // 0.0 - 1.0
    uint8_t m_clutch;            // 0 - 100
    int8_t m_gear;               // 1-8, N=0, R=-1
    uint16_t m_engineRPM;
    uint8_t m_drs;
    uint8_t m_revLightsPercent;
    uint16_t m_revLightsBitValue;
    uint16_t m_brakesTemperature[4];
    uint8_t m_tyresSurfaceTemperature[4];
    uint8_t m_tyresInnerTemperature[4];
    uint16_t m_engineTemperature;
    float m_tyresPressure[4];
    uint8_t m_surfaceType[4];
};

struct F1PacketCarTelemetryData {
    F1PacketHeader m_header;
    F1CarTelemetryData m_carTelemetryData[22];
    uint32_t m_mfdPanelIndex;
    uint32_t m_mfdPanelIndexSecondaryPlayer;
    int8_t m_suggestedGear;
};

// DiRT Rally 2.0 Telemetry Packet (extradata=3, 264+ bytes of floats)
struct DirtRallyTelemetryPacket {
    float time;              // 0
    float time_tot;          // 1
    float speed;             // 2 (m/s)
    float pos_x, pos_y, pos_z; // 3,4,5
    float vel_x, vel_y, vel_z; // 6,7,8
    float roll_x, roll_y, roll_z; // 9,10,11
    float pitch_x, pitch_y, pitch_z; // 12,13,14
    float susp_pos[4];       // 15,16,17,18
    float susp_vel[4];       // 19,20,21,22
    float wheel_speed[4];    // 23,24,25,26
    float throttle;          // 27 (0 to 1)
    float steer;             // 28
    float brake;             // 29 (0 to 1)
    float clutch;            // 30
    float gear;              // 31 (usually 0=N, 1=1st, 10=R)
    float g_force_lat;       // 32
    float g_force_lon;       // 33
    float lap;               // 34
    float engine_rpm;        // 35
    float sli_pro_support;   // 36
    float car_pos;           // 37
    float kers_lvl;          // 38
    float kers_max_lvl;      // 39
    float drs;               // 40
    float trac_ctrl;         // 41
    float abs;               // 42
    float fuel_in_tank;      // 43
    float fuel_capacity;     // 44
    float in_pits;           // 45
    float sector;            // 46
    float sector1_time;      // 47
    float sector2_time;      // 48
    float brakes_temp[4];    // 49,50,51,52
    float tyres_press[4];    // 53,54,55,56
    float team_info;         // 57
    float total_laps;        // 58
    float track_size;        // 59
    float last_lap_time;     // 60
    float max_rpm;           // 61
    float idle_rpm;          // 62
    float max_gears;         // 63
};

// Assetto Corsa Telemetry Packets
struct AssettoCorsaHandshake {
    int32_t identifier;
    int32_t version;
    int32_t operationId;
};

struct AssettoCorsaCarUpdate {
    int32_t identifier;
    int32_t size; 
    
    float speed_Kmh;
    float velocity[3];
    float localVelocity[3];
    float accel_G[3];
    float steer;
    float cgHeight;
    float pitch;
    float roll;
    float cgLocation[3];
    float carDamage[5];
    int32_t numberOfTyresOut;
    int32_t pitLimiterOn;
    float abs;
    float kersCharge;
    float kersInput;
    int32_t autoShifterOn;
    float rideHeight[2];
    float engineRpm;
    float maxEngineRpm;
    float turboBoost;
    float gas;
    float brake;
    float clutch;
    float engineMap;
    int32_t gear; // 0=R, 1=N, 2=1st
};
#pragma pack(pop)

// Parsed telemetry structure shared across the firmware modules
struct TelemetryData {
    bool isRaceOn;
    uint32_t timestampMs;
    float currentRpm;
    float maxRpm;
    float idleRpm;
    float speedKmh;                    // Converted to km/h (speed m/s * 3.6)
    float speedMph;                    // Converted to mph (speed m/s * 2.23694)
    uint8_t gear;                      // 0 = Reverse, 1 = Neutral, 2..N = Gears
    uint8_t accelPercent;              // 0-100%
    uint8_t brakePercent;              // 0-100%
    float boost;                       // Boost pressure
    float tireSlipMax;                 // Maximum tire slip ratio across all 4 wheels
    bool isUpdating;                   // True if telemetry packets are currently active
    uint32_t lastPacketTimeMs;         // Timestamp of last received packet
    uint32_t packetsReceived;          // Total UDP packets received
};

#endif // TELEMETRY_DATA_H
