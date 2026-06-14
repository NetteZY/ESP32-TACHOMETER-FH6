# ESP32 Forza Horizon Tachometer & Shift Light (SimDash-like)

This project is a custom, highly configurable ESP32 firmware that receives UDP telemetry from **Forza Horizon (4, 5, and 6)** over the local network and drives a WS2812B (NeoPixel) addressable LED strip to act as a real-time RPM shift light.

It features a built-in, premium **Web Dashboard** hosted directly on the ESP32 for easy configuration of WiFi networks, hardware pins, LED counts, brightness, and shift light thresholds without needing to modify or re-flash the code.

Additionally, it includes dedicated web tools to flash and configure the ESP32 directly from your browser.

---

## 🚀 In-Browser Installer & Configurator

You can flash and configure your ESP32 directly from any Web Serial-capable browser (Chrome, Edge, or Opera):

1. **Flash Firmware**: Open [flasher.html](flasher.html) in your browser. Connect the ESP32 to your PC via USB:
   - **Cloud Install**: Click **⚡ Install Firmware** to download and flash the latest precompiled unified binary (`merged-firmware.bin`) directly from GitHub releases to offset `0x0`.
   - **Local File Upload**: Switch to the local upload tab, select a downloaded `.bin` file, and flash it instantly (bypassing CORS and API rate limits).
2. **Network Configurator**: Open [web-tool.html](web-tool.html) to configure the ESP32 over WiFi. You can set the operation mode (Client or Access Point), update shift light properties, test the addressable LEDs, and monitor live tachometer dials.

---

## Features

- **Binary Telemetry Parsing**: Listens on a UDP port, parses the official 324-byte Forza "Car Dash" packet structure, and handles data at ~60Hz.
- **Dynamic LED Configuration**: Configure the GPIO pin, number of LEDs, brightness, and color profile (Classic GT, Modern Hypercar, or Rainbow Sweep) on-the-fly.
- **Adjustable Shift Thresholds**: Slide controls to customize when the LEDs start lighting up (activation RPM %) and when they flash (shift prompt redline %).
- **WiFi Station & AP Fallback**: Automatically creates its own WiFi Access Point (`SimDash-ESP32-AP`) if it cannot connect to your home Wi-Fi, allowing easy network setup.
- **Live UI Dials**: A dark, responsive, glassmorphic Web UI showing real-time RPM, Speed, Gear, Throttle/Brake percentages, Boost, Max Tire Slip, and Uptime.

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
- Click the **PlatformIO Build** and **Upload** buttons in the status bar (or run `pio run -t upload` from the terminal).

### 2. Configure Network & Hardware
1. Connect your PC or smartphone to the WiFi network broadcast by the ESP32: **`SimDash-ESP32-AP`** (no password).
2. Open a web browser and go to `http://192.168.4.1` (or use [web-tool.html](web-tool.html) connected to `192.168.4.1`).
3. Go to the **Wi-Fi & System** tab, select **Client Mode (Connect to Home Router)**, enter your SSID and Password, and click **Save Wi-Fi Settings**.
4. Click **Reboot Device**. The ESP32 will reboot and connect to your home network.
5. Re-access the dashboard by navigating to the ESP32's new IP address (which can be found in your router's client list) or using [web-tool.html](web-tool.html) with the new IP address.
6. Go to the **Shift Light** tab to customize your GPIO pin, number of LEDs, brightness, and colors. Use the **Test** buttons to check your physical LEDs.

### 3. Connect to Forza Horizon
1. Open **Forza Horizon** (4, 5, or 6).
2. Go to **Settings > HUD and Gameplay**.
3. Scroll to the bottom and configure:
   - **Data Out**: `ON`
   - **Data Out IP Address**: `<ESP32_IP_ADDRESS>` (the local IP address of your ESP32)
   - **Data Out IP Port**: `5300` (or your configured port)
4. Start driving. The shift lights will instantly sync with your in-game tachometer!
