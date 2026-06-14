#!/usr/bin/env python3
import socket
import time
import struct
import argparse
import sys
import math

def run_emulator(target_ip, target_port, loop_mode):
    print(f"Forza Horizon Telemetry Emulator started.")
    print(f"Sending UDP packets to {target_ip}:{target_port}")
    print(f"Press Ctrl+C to stop.\n")

    # Create UDP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # Initialize telemetry values
    is_race_on = 1
    max_rpm = 8000.0
    idle_rpm = 1000.0
    current_rpm = idle_rpm
    speed = 0.0 # m/s
    gear = 2    # 1st gear (0=R, 1=N, 2=1st, 3=2nd...)
    accel = 255 # full throttle
    brake = 0
    boost = 0.0

    start_time = time.time()
    packet_count = 0

    try:
        while True:
            # Calculate elapsed time
            elapsed = time.time() - start_time
            timestamp_ms = int(elapsed * 1000) & 0xFFFFFFFF

            # Simulate driving behavior
            if loop_mode == "drag":
                # Simulate a drag strip acceleration and gear shifts
                cycle_time = elapsed % 15.0  # 15-second cycles
                
                if cycle_time < 0.5:
                    # Launch control/idle
                    current_rpm = 3500.0
                    gear = 1  # Neutral
                    speed = 0.0
                    accel = 200
                    brake = 100
                    boost = 50000.0 # Pascals
                elif cycle_time < 3.5:
                    # 1st gear acceleration
                    gear = 2
                    accel = 255
                    brake = 0
                    t = cycle_time - 0.5
                    current_rpm = idle_rpm + (max_rpm - idle_rpm) * (t / 3.0)
                    speed = 20.0 * (t / 3.0)
                    boost = 120000.0
                elif cycle_time < 3.7:
                    # Shift to 2nd gear (brief lift)
                    gear = 3
                    accel = 50
                    current_rpm = 5000.0
                    boost = 30000.0
                elif cycle_time < 7.0:
                    # 2nd gear acceleration
                    gear = 3
                    accel = 255
                    t = (cycle_time - 3.7) / 3.3
                    current_rpm = 5000.0 + (max_rpm - 5000.0) * t
                    speed = 20.0 + 30.0 * t
                    boost = 150000.0
                elif cycle_time < 7.2:
                    # Shift to 3rd gear
                    gear = 4
                    accel = 50
                    current_rpm = 5500.0
                    boost = 40000.0
                elif cycle_time < 11.0:
                    # 3rd gear acceleration
                    gear = 4
                    accel = 255
                    t = (cycle_time - 7.2) / 3.8
                    current_rpm = 5500.0 + (max_rpm - 5500.0) * t
                    speed = 50.0 + 40.0 * t
                    boost = 160000.0
                elif cycle_time < 13.0:
                    # Heavy braking
                    accel = 0
                    brake = 255
                    t = (cycle_time - 11.0) / 2.0
                    current_rpm = max_rpm - (max_rpm - idle_rpm) * t
                    speed = 90.0 - 80.0 * t
                    boost = 0.0
                else:
                    # Stationary idle
                    gear = 1
                    accel = 0
                    brake = 255
                    current_rpm = idle_rpm
                    speed = 0.0
                    boost = 0.0
            
            elif loop_mode == "revving":
                # Sine wave revving in neutral
                sine_val = (math.sin(elapsed * 2.0) + 1.0) / 2.0 # 0.0 to 1.0
                current_rpm = idle_rpm + (max_rpm - idle_rpm) * sine_val
                gear = 1 # Neutral
                speed = 0.0
                accel = int(sine_val * 255)
                brake = 0
                boost = sine_val * 20000.0

            # Build a 324-byte packet filled with zeros
            packet = bytearray(324)

            # Write values into packet buffer at their exact byte offsets
            struct.pack_into("<i", packet, 0, is_race_on)            # Offset 0: IsRaceOn (int32)
            struct.pack_into("<I", packet, 4, timestamp_ms)          # Offset 4: TimestampMS (uint32)
            struct.pack_into("<f", packet, 8, max_rpm)              # Offset 8: EngineMaxRpm (float)
            struct.pack_into("<f", packet, 12, idle_rpm)            # Offset 12: EngineIdleRpm (float)
            struct.pack_into("<f", packet, 16, current_rpm)          # Offset 16: CurrentEngineRpm (float)
            struct.pack_into("<f", packet, 256, speed)               # Offset 256: Speed (float)
            struct.pack_into("<f", packet, 284, boost)               # Offset 284: Boost (float)
            
            struct.pack_into("<B", packet, 315, accel)               # Offset 315: Accel (uint8)
            struct.pack_into("<B", packet, 316, brake)               # Offset 316: Brake (uint8)
            struct.pack_into("<B", packet, 319, gear)                # Offset 319: Gear (uint8)

            # Send packet via UDP
            sock.sendto(packet, (target_ip, target_port))
            
            packet_count += 1
            if packet_count % 60 == 0:
                print(f"Sent {packet_count} packets. Current RPM: {current_rpm:.0f}, Gear: {gear-1}, Speed: {speed*3.6:.1f} km/h", end='\r')

            # Sleep to simulate ~60Hz output (16.6ms)
            time.sleep(0.0166)

    except KeyboardInterrupt:
        print("\nEmulator stopped by user.")
    finally:
        sock.close()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Forza Horizon Telemetry UDP Emulator")
    parser.add_argument("--ip", default="127.0.0.1", help="Target IP address of the ESP32 (default: 127.0.0.1)")
    parser.add_argument("--port", type=int, default=5300, help="Target UDP port of the ESP32 (default: 5300)")
    parser.add_argument("--mode", choices=["drag", "revving"], default="drag", help="Simulation mode (default: drag)")
    
    args = parser.parse_args()
    run_emulator(args.ip, args.port, args.mode)
