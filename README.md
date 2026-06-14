# Multi-Game ESP32 Sim Racing Shift Light & Telemetry Dashboard

This project is a highly configurable, high-performance ESP32 firmware designed for custom DIY sim racing dash displays and shift light indicators. It listens for real-time UDP telemetry streams from multiple major racing games and drives addressable WS2812B (NeoPixel) LED strips to act as an RPM tachometer / shift light.

It features a premium **Web Dashboard** hosted directly on the ESP32 for on-the-fly configuration of WiFi networks, hardware pins, LED counts, brightness, and shift light thresholds. It also supports local auto-connection and direct access.

---

## 🎮 Supported Games & Configuration

This firmware can dynamically switch between different UDP telemetry protocols. Configure your game type in the **Wi-Fi & System** tab under **Telemetry Listener**.

| Game / Engine | Default Port | Telemetry Config Instructions |
| --- | --- | --- |
| **Forza Horizon 4 / 5 / 6 & Motorsport** | `5300` or `5555` | Set **Data Out** to `ON`, enter the ESP32's IP address, and set the port to `5300` (or your configured port). |
| **EA Sports F1 23 / 24** | `20777` | Go to **Telemetry Settings**. Turn UDP Telemetry `ON`, enter the ESP32's IP address, set the port to `20777`, and set the format to F1 23/24. |
| **DiRT Rally 2.0** | `10001` | Edit `hardware_settings_config.xml` (located in `Documents/My Games/DiRT Rally 2.0/hardwaresettings/`). Find the `<motion>` element and configure it: `<motion enabled="true" ip="<ESP32_IP>" port="10001" delay="1" extradata="3" />`. |
| **Assetto Corsa / ACC** | `9996` | Enter your gaming PC's IP address in the **Game PC IP Address** field in the ESP32 Web Config UI. The ESP32 will automatically send periodic handshake packets to port `9996` to start the stream. |

---

## 🚀 In-Browser Installer & Configurator

You can flash and configure your ESP32 directly from any Web Serial-capable browser (Chrome, Edge, or Opera):

1. **Flash Firmware**: Open [flasher.html](flasher.html) in your browser. Connect the ESP32 to your PC via USB:
   - **Cloud Install**: Click **⚡ Install Firmware** to flash the latest precompiled unified binary (`merged-firmware.bin`) directly from GitHub releases to offset `0x0`.
   - **Local File Upload**: Switch to the local upload tab, select a downloaded `.bin` file, and flash it instantly.
2. **Network Configurator**: Open [web-tool.html](web-tool.html) to configure the ESP32 over WiFi. When connected directly to the device's hotspot or IP, the configurator will **auto-connect** and open the dashboard directly.

---

## Features

- **Multi-Protocol UDP Parsing**: Parses official telemetry structures (Forza Sled/Dash, Codemasters F1, Codemasters DiRT, and Assetto Corsa update packets) at ~60Hz.
- **Dynamic LED Configuration**: Customize the GPIO pin, number of LEDs, brightness, and color profile (Classic GT, Modern Hypercar, or Rainbow Sweep) on-the-fly.
- **Adjustable Shift Thresholds**: Slide controls to customize when the LEDs start lighting up (activation RPM %) and when they flash (shift prompt redline %).
- **WiFi Station & AP Fallback**: Automatically creates its own WiFi Access Point (`SimDash-ESP32-AP`) if it cannot connect to your home Wi-Fi, allowing easy network setup.
- **Live UI Dials**: A dark, responsive, glassmorphic Web UI showing real-time RPM, Speed, Gear, Throttle/Brake percentages, Boost, Max Tire Slip, and Uptime.
- **Hot-Reloading Sockets**: Telemetry ports and game type changes are updated instantly in memory without requiring a manual reboot of the ESP32.

---

## Hardware Requirements

- **Microcontroller**: ESP32 (or ESP32-S3) development board.
- **LED Strip**: WS2812B (NeoPixel) addressable RGB LED strip.
- **Power**: A 5V power source (external or USB) capable of powering your LED strip. (A 16-LED strip at full brightness can draw up to ~1A, so keeping brightness low/moderate is recommended if powered directly from the ESP32's 5V pin).

---

## Setup & Usage (Manual Build)

If you prefer to compile and upload the source code manually rather than using the in-browser installer:

### 1. Build and Flash
This project is built using PlatformIO.
- Install VS Code and the **PlatformIO IDE** extension.
- Open the project directory in VS Code.
- Connect your ESP32 to your PC.
- Click the **PlatformIO Build** and **Upload** buttons in the status bar.

### 2. Configure Network & Hardware
1. Connect your PC or smartphone to the WiFi network broadcast by the ESP32: **`SimDash-ESP32-AP`** (no password).
2. Open a web browser and go to `http://192.168.4.1` (or use [web-tool.html](web-tool.html) connected to `192.168.4.1` which will auto-connect).
3. Go to the **Wi-Fi & System** tab, select **Client Mode (Connect to Home Router)**, enter your SSID and Password, and click **Save Wi-Fi Settings**.
4. Click **Reboot Device**. The ESP32 will reboot and connect to your home network.
5. Re-access the dashboard by navigating to the ESP32's new IP address (which can be found in your router's client list) or using [web-tool.html](web-tool.html) with the new IP address.
6. Go to the **Shift Light** tab to customize your GPIO pin, number of LEDs, brightness, and colors. Use the **Test** buttons to check your physical LEDs.
