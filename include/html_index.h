#ifndef HTML_INDEX_H
#define HTML_INDEX_H

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>AG SimDash Remote Configurator</title>
    <link href="https://fonts.googleapis.com/css2?family=Outfit:wght@300;400;500;600;700&display=swap" rel="stylesheet">
    <style>
        :root {
            --bg-primary: #0b0f19;
            --bg-secondary: #161c2a;
            --bg-card: rgba(30, 41, 59, 0.4);
            --border-color: rgba(255, 255, 255, 0.08);
            --text-primary: #f8fafc;
            --text-secondary: #94a3b8;
            --accent-purple: #8b5cf6;
            --accent-cyan: #06b6d4;
            --accent-green: #10b981;
            --accent-red: #f43f5e;
            --accent-orange: #f97316;
            --shadow-glow: 0 0 20px rgba(168, 85, 247, 0.2);
            --transition-speed: 0.3s;
        }

        * {
            box-sizing: border-box;
            margin: 0;
            padding: 0;
        }

        body {
            font-family: 'Outfit', sans-serif;
            background-color: var(--bg-primary);
            color: var(--text-primary);
            line-height: 1.5;
            overflow-x: hidden;
            background-image: 
                radial-gradient(circle at 10% 10%, rgba(168, 85, 247, 0.1) 0%, transparent 40%),
                radial-gradient(circle at 90% 90%, rgba(6, 182, 212, 0.1) 0%, transparent 40%);
            background-attachment: fixed;
        }

        .container {
            max-width: 1000px;
            margin: 0 auto;
            padding: 2rem 1rem;
        }

        header {
            display: flex;
            justify-content: space-between;
            align-items: center;
            margin-bottom: 2.5rem;
            padding-bottom: 1.5rem;
            border-bottom: 1px solid var(--border-color);
        }

        .logo {
            display: flex;
            align-items: center;
            gap: 0.75rem;
        }

        .logo-icon {
            width: 36px;
            height: 36px;
            background: linear-gradient(135deg, var(--accent-purple), var(--accent-cyan));
            border-radius: 10px;
            display: flex;
            align-items: center;
            justify-content: center;
            font-weight: 700;
            font-size: 1.25rem;
            box-shadow: var(--shadow-glow);
        }

        .logo-text {
            font-weight: 700;
            font-size: 1.5rem;
            letter-spacing: -0.5px;
            background: linear-gradient(to right, #ffffff, var(--text-secondary));
            -webkit-background-clip: text;
            -webkit-text-fill-color: transparent;
        }

        .card {
            background-color: var(--bg-secondary);
            border-radius: 16px;
            border: 1px solid var(--border-color);
            padding: 2rem;
            margin-bottom: 2rem;
            backdrop-filter: blur(10px);
            box-shadow: 0 10px 30px rgba(0,0,0,0.2);
        }

        .card-title {
            font-size: 1.25rem;
            font-weight: 700;
            margin-bottom: 0.75rem;
            background: linear-gradient(to right, #ffffff, var(--text-secondary));
            -webkit-background-clip: text;
            -webkit-text-fill-color: transparent;
            display: flex;
            justify-content: space-between;
            align-items: center;
        }

        .description {
            color: var(--text-secondary);
            font-size: 0.95rem;
            margin-bottom: 1.5rem;
        }

        /* Config Connection Section */
        .connect-bar {
            display: flex;
            gap: 1rem;
            margin-bottom: 1.5rem;
        }

        .connect-bar input {
            flex: 1;
        }

        /* Control/Settings layouts */
        .tabs {
            display: flex;
            gap: 1rem;
            margin-bottom: 1.5rem;
            background-color: rgba(22, 28, 42, 0.6);
            padding: 0.4rem;
            border-radius: 12px;
            border: 1px solid var(--border-color);
            width: fit-content;
        }

        .tab-btn {
            background: none;
            border: none;
            color: var(--text-secondary);
            padding: 0.6rem 1.5rem;
            border-radius: 8px;
            cursor: pointer;
            font-weight: 500;
            font-size: 0.95rem;
            font-family: inherit;
            transition: all var(--transition-speed);
        }

        .tab-btn:hover {
            color: var(--text-primary);
        }

        .tab-btn.active {
            background: linear-gradient(135deg, var(--accent-purple), var(--accent-cyan));
            color: var(--text-primary);
            box-shadow: var(--shadow-glow);
        }

        .tab-content {
            display: none;
        }

        .tab-content.active {
            display: block;
            animation: fadeIn 0.4s ease;
        }

        @keyframes fadeIn {
            from { opacity: 0; transform: translateY(10px); }
            to { opacity: 1; transform: translateY(0); }
        }

        /* Inputs & Form styling */
        input[type="text"],
        input[type="password"],
        input[type="number"],
        select {
            width: 100%;
            padding: 0.75rem 1rem;
            background-color: rgba(11, 15, 25, 0.8);
            border: 1px solid var(--border-color);
            border-radius: 8px;
            color: var(--text-primary);
            font-family: inherit;
            font-size: 0.95rem;
            transition: all var(--transition-speed);
        }

        input[type="text"]:focus,
        input[type="password"]:focus,
        input[type="number"]:focus,
        select:focus {
            outline: none;
            border-color: var(--accent-purple);
            box-shadow: 0 0 0 2px rgba(168, 85, 247, 0.25);
        }

        label {
            display: block;
            color: var(--text-secondary);
            margin-bottom: 0.5rem;
            font-size: 0.9rem;
            font-weight: 500;
        }

        .settings-grid {
            display: grid;
            grid-template-columns: 1fr;
            gap: 1.5rem;
        }

        @media (min-width: 768px) {
            .settings-grid {
                grid-template-columns: 1fr 1fr;
            }
        }

        .form-group {
            margin-bottom: 1.25rem;
        }

        .btn {
            padding: 0.75rem 1.5rem;
            border-radius: 8px;
            font-weight: 600;
            font-size: 0.95rem;
            cursor: pointer;
            transition: all var(--transition-speed);
            font-family: inherit;
            border: none;
            display: inline-flex;
            align-items: center;
            justify-content: center;
        }

        .btn-primary {
            background: linear-gradient(135deg, var(--accent-purple), var(--accent-cyan));
            color: var(--text-primary);
            box-shadow: var(--shadow-glow);
        }

        .btn-primary:hover {
            opacity: 0.9;
            transform: translateY(-1px);
        }

        .btn-secondary {
            background-color: #334155;
            color: var(--text-primary);
            border: 1px solid rgba(255,255,255,0.05);
        }

        .btn-secondary:hover {
            background-color: #475569;
        }

        .btn-danger {
            background-color: rgba(244, 63, 94, 0.2);
            color: var(--accent-red);
            border: 1px solid rgba(244, 63, 94, 0.3);
        }

        .btn-danger:hover {
            background-color: var(--accent-red);
            color: var(--text-primary);
        }

        .range-slider-group {
            margin-bottom: 1.5rem;
        }

        .range-label-container {
            display: flex;
            justify-content: space-between;
            margin-bottom: 0.5rem;
        }

        .range-value {
            color: var(--accent-cyan);
            font-weight: 600;
        }

        input[type="range"] {
            width: 100%;
            height: 6px;
            background: #334155;
            border-radius: 5px;
            outline: none;
            -webkit-appearance: none;
        }

        input[type="range"]::-webkit-slider-thumb {
            -webkit-appearance: none;
            width: 18px;
            height: 18px;
            border-radius: 50%;
            background: var(--accent-cyan);
            cursor: pointer;
            box-shadow: 0 0 5px rgba(0,0,0,0.5);
        }

        .toggle-container {
            display: flex;
            align-items: center;
            justify-content: space-between;
            margin-bottom: 1.25rem;
            padding: 0.75rem;
            background-color: rgba(11, 15, 25, 0.4);
            border-radius: 8px;
            border: 1px solid var(--border-color);
        }

        .toggle-switch {
            position: relative;
            display: inline-block;
            width: 50px;
            height: 26px;
        }

        .toggle-switch input {
            opacity: 0;
            width: 0;
            height: 0;
        }

        .slider-round {
            position: absolute;
            cursor: pointer;
            top: 0;
            left: 0;
            right: 0;
            bottom: 0;
            background-color: #334155;
            transition: .4s;
            border-radius: 34px;
        }

        .slider-round:before {
            position: absolute;
            content: "";
            height: 18px;
            width: 18px;
            left: 4px;
            bottom: 4px;
            background-color: white;
            transition: .4s;
            border-radius: 50%;
        }

        input:checked + .slider-round {
            background: linear-gradient(135deg, var(--accent-purple), var(--accent-cyan));
        }

        input:checked + .slider-round:before {
            transform: translateX(24px);
        }

        /* Dashboard & Preview Widgets */
        .dash-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(280px, 1fr));
            gap: 1.5rem;
            margin-bottom: 1.5rem;
        }

        .dash-card {
            background-color: rgba(11, 15, 25, 0.4);
            border-radius: 12px;
            border: 1px solid var(--border-color);
            padding: 1.25rem;
        }

        .dash-card-title {
            font-size: 0.9rem;
            color: var(--text-secondary);
            font-weight: 500;
            margin-bottom: 0.5rem;
            text-transform: uppercase;
            letter-spacing: 0.5px;
        }

        .dash-card-value {
            font-size: 2rem;
            font-weight: 700;
        }

        .gear-val {
            font-size: 4rem;
            font-weight: 800;
            text-align: center;
            background: linear-gradient(135deg, var(--accent-purple), var(--accent-cyan));
            -webkit-background-clip: text;
            -webkit-text-fill-color: transparent;
            line-height: 1;
        }

        .rpm-bar-bg {
            height: 20px;
            background-color: rgba(255, 255, 255, 0.05);
            border-radius: 6px;
            overflow: hidden;
            border: 1px solid var(--border-color);
            margin-top: 0.5rem;
        }

        .rpm-bar-fill {
            height: 100%;
            width: 0%;
            background: linear-gradient(to right, var(--accent-green) 0%, var(--accent-green) 60%, var(--accent-orange) 60%, var(--accent-orange) 85%, var(--accent-red) 85%, var(--accent-red) 100%);
            transition: width 0.1s;
        }

        .led-preview-strip {
            display: flex;
            justify-content: space-between;
            gap: 4px;
            background-color: #0b0f19;
            padding: 10px;
            border-radius: 8px;
            margin-bottom: 1.25rem;
            border: 1px solid var(--border-color);
        }

        .led-node {
            flex: 1;
            height: 12px;
            border-radius: 50%;
            background-color: #1e293b;
            box-shadow: inset 0 1px 3px rgba(0,0,0,0.5);
            transition: all 0.1s;
        }

        .toast {
            position: fixed;
            bottom: 20px;
            right: 20px;
            padding: 1rem 1.5rem;
            border-radius: 8px;
            background-color: var(--bg-secondary);
            border-left: 4px solid var(--accent-purple);
            box-shadow: 0 10px 25px rgba(0,0,0,0.5);
            transform: translateY(100px);
            opacity: 0;
            transition: all 0.3s cubic-bezier(0.175, 0.885, 0.32, 1.275);
            z-index: 1000;
        }

        .toast.show {
            transform: translateY(0);
            opacity: 1;
        }

        .status-badge {
            padding: 0.35rem 0.75rem;
            border-radius: 12px;
            font-size: 0.8rem;
            font-weight: 600;
            border: 1px solid rgba(255,255,255,0.08);
            background-color: rgba(244, 63, 94, 0.1);
            color: var(--accent-red);
        }

        .status-badge.connected {
            background-color: rgba(16, 185, 129, 0.1);
            color: var(--accent-green);
        }
    </style>
</head>
<body>
    <div class="container">
        <header>
            <div class="logo">
                <div class="logo-icon">AG</div>
                <div class="logo-text">Antigravity SimDash Dashboard</div>
            </div>
            <div id="connection-badge" class="status-badge">NOT CONNECTED</div>
        </header>

        <!-- Connection Card -->
        <div class="card">
            <div class="card-title">Connect to Device</div>
            <p class="description">
                Enter your ESP32 IP address below to manage settings and view live telemetry. 
                If the device is in setup mode, connect your PC/phone to the <b>SimDash-ESP32-AP</b> WiFi network and connect to <b>192.168.4.1</b>.
            </p>

            <div class="connect-bar">
                <input type="text" id="device-ip" value="192.168.4.1" placeholder="ESP32 IP Address (e.g. 192.168.1.15)">
                <button class="btn btn-primary" id="btn-connect" onclick="connectToDevice()">🔌 Connect to ESP32</button>
            </div>
        </div>

        <!-- Configuration Panels (Initially Hidden until Connected) -->
        <div id="config-panel" style="display: none;">
            <div class="tabs">
                <button class="tab-btn active" id="tab-dash-btn" onclick="switchTab('tab-dash')">Live Stats</button>
                <button class="tab-btn" id="tab-led-btn" onclick="switchTab('tab-led')">Shift Light Config</button>
                <button class="tab-btn" id="tab-net-btn" onclick="switchTab('tab-net')">Wi-Fi & System</button>
            </div>

            <!-- Live Stats Content -->
            <div id="tab-dash" class="tab-content active">
                <div class="dash-grid">
                    <div class="dash-card" style="grid-column: 1 / -1;">
                        <div class="dash-card-title">Live Engine RPM</div>
                        <div style="display:flex; justify-content:space-between; font-weight:700;">
                            <span id="dash-rpm-val">0 RPM</span>
                            <span id="dash-rpm-max" style="color:var(--accent-cyan);">0 RPM Max</span>
                        </div>
                        <div class="rpm-bar-bg">
                            <div id="dash-rpm-bar" class="rpm-bar-fill"></div>
                        </div>
                    </div>

                    <div class="dash-card" style="text-align: center;">
                        <div class="dash-card-title">Current Gear</div>
                        <div id="dash-gear" class="gear-val">N</div>
                    </div>

                    <div class="dash-card">
                        <div class="dash-card-title">Vehicle Speed</div>
                        <div class="dash-card-value" id="dash-speed">0 <span style="font-size:1rem; font-weight:normal; color:var(--text-secondary);">km/h</span></div>
                        <div style="margin-top:1rem; font-size:0.875rem; color:var(--text-secondary);">
                            MPH: <span id="dash-speed-mph" style="color:var(--text-primary); font-weight:600;">0</span> | 
                            Boost: <span id="dash-boost" style="color:var(--text-primary); font-weight:600;">0.0</span> psi
                        </div>
                    </div>

                    <div class="dash-card">
                        <div class="dash-card-title">Status Metrics</div>
                        <div style="font-size:0.9rem; color:var(--text-secondary); display:flex; flex-direction:column; gap:0.4rem;">
                            <div>Uptime: <span id="dash-uptime" style="color:var(--text-primary); font-weight:600;">0s</span></div>
                            <div>Packets: <span id="dash-packets" style="color:var(--text-primary); font-weight:600;">0</span></div>
                            <div>Max Tire Slip: <span id="dash-slip" style="color:var(--text-primary); font-weight:600;">0.00</span></div>
                        </div>
                    </div>
                </div>
            </div>

            <!-- Shift Light Config Content -->
            <div id="tab-led" class="tab-content">
                <div class="dash-card" style="margin-bottom:1.5rem;">
                    <div class="dash-card-title">Virtual LED Strip Preview</div>
                    <div id="led-preview" class="led-preview-strip"></div>
                    <div style="display:flex; gap:0.5rem; flex-wrap:wrap;">
                        <button class="btn btn-secondary" style="padding:0.4rem 1rem; font-size:0.85rem;" onclick="testLeds(0.25)">Test 25%</button>
                        <button class="btn btn-secondary" style="padding:0.4rem 1rem; font-size:0.85rem;" onclick="testLeds(0.50)">Test 50%</button>
                        <button class="btn btn-secondary" style="padding:0.4rem 1rem; font-size:0.85rem;" onclick="testLeds(0.90)">Test 90%</button>
                        <button class="btn btn-secondary" style="padding:0.4rem 1rem; font-size:0.85rem;" onclick="testLeds(1.00)">Test Redline</button>
                        <button class="btn btn-danger" style="padding:0.4rem 1rem; font-size:0.85rem;" onclick="testLeds(0)">Clear</button>
                    </div>
                </div>

                <form id="led-form" onsubmit="saveConfig(event, 'shiftLight')">
                    <div class="toggle-container">
                        <div>
                            <div style="font-weight: 600;">Enable LED Strip</div>
                            <div style="color: var(--text-secondary); font-size: 0.85rem;">Toggle the WS2812B shift light strip.</div>
                        </div>
                        <label class="toggle-switch">
                            <input type="checkbox" id="sl-enabled">
                            <span class="slider-round"></span>
                        </label>
                    </div>

                    <div class="settings-grid">
                        <div class="form-group">
                            <label for="sl-pin">GPIO Pin</label>
                            <select id="sl-pin">
                                <option value="2">GPIO 2 (Built-in LED)</option>
                                <option value="4">GPIO 4</option>
                                <option value="12">GPIO 12</option>
                                <option value="13">GPIO 13</option>
                                <option value="14">GPIO 14</option>
                                <option value="15">GPIO 15 (Recommended)</option>
                                <option value="16">GPIO 16</option>
                                <option value="17">GPIO 17</option>
                                <option value="18">GPIO 18</option>
                                <option value="21">GPIO 21</option>
                                <option value="22">GPIO 22</option>
                                <option value="23">GPIO 23</option>
                                <option value="25">GPIO 25</option>
                                <option value="26">GPIO 26</option>
                                <option value="27">GPIO 27</option>
                            </select>
                        </div>

                        <div class="form-group">
                            <label for="sl-numleds">Number of LEDs</label>
                            <input type="number" id="sl-numleds" min="1" max="64" onchange="drawLedPreview()">
                        </div>
                    </div>

                    <div class="range-slider-group">
                        <div class="range-label-container">
                            <label for="sl-startrpm">Activation Start RPM (%)</label>
                            <span id="sl-startrpm-val" class="range-value">70%</span>
                        </div>
                        <input type="range" id="sl-startrpm" min="10" max="95" oninput="updateSlider('sl-startrpm')">
                    </div>

                    <div class="range-slider-group">
                        <div class="range-label-container">
                            <label for="sl-endrpm">Shift Flash threshold (%)</label>
                            <span id="sl-endrpm-val" class="range-value">95%</span>
                        </div>
                        <input type="range" id="sl-endrpm" min="70" max="99" oninput="updateSlider('sl-endrpm')">
                    </div>

                    <div class="range-slider-group">
                        <div class="range-label-container">
                            <label for="sl-bright">LED Brightness (0-255)</label>
                            <span id="sl-bright-val" class="range-value">64</span>
                        </div>
                        <input type="range" id="sl-bright" min="5" max="255" oninput="updateSlider('sl-bright')">
                    </div>

                    <div class="form-group">
                        <label for="sl-scheme">Color Profile</label>
                        <select id="sl-scheme" onchange="drawLedPreview()">
                            <option value="0">Classic GT/Formula (Green -> Yellow -> Red)</option>
                            <option value="1">Modern Hypercar (Blue -> Cyan -> Magenta)</option>
                            <option value="2">Rainbow Sweep</option>
                        </select>
                    </div>

                    <button type="submit" class="btn btn-primary">Save Shift Light Config</button>
                </form>
            </div>

            <!-- WiFi & System Config Content -->
            <div id="tab-net" class="tab-content">
                <div class="settings-grid">
                    <div class="dash-card">
                        <div class="dash-card-title">Network Configuration</div>
                        <form id="wifi-form" onsubmit="saveConfig(event, 'wifi')">
                            
                            <div class="form-group">
                                <label for="wifi-opmode">Wi-Fi Operation Mode</label>
                                <select id="wifi-opmode" onchange="toggleWifiFields()">
                                    <option value="client">Client Mode (Connect to Home Router)</option>
                                    <option value="ap">Access Point Mode (Standalone Hotspot)</option>
                                </select>
                                <span style="font-size:0.8rem; color:var(--text-secondary);">
                                    Select Client Mode to connect the device to your home network and assign it its own IP address.
                                </span>
                            </div>

                            <div id="wifi-station-fields">
                                <div class="form-group">
                                    <label for="wifi-ssid">WiFi SSID (Home Network Name)</label>
                                    <input type="text" id="wifi-ssid" placeholder="Home Network SSID">
                                </div>
                                <div class="form-group">
                                    <label for="wifi-pass">WiFi Password</label>
                                    <input type="password" id="wifi-pass" placeholder="••••••••">
                                </div>
                            </div>

                            <div id="wifi-ap-fields" style="display:none;">
                                <div class="form-group">
                                    <label for="wifi-apname">AP Name (SSID broadcast)</label>
                                    <input type="text" id="wifi-apname" placeholder="SimDash-ESP32-AP">
                                </div>
                                <div class="form-group">
                                    <label for="wifi-appass">AP Password (WPA2)</label>
                                    <input type="password" id="wifi-appass" placeholder="Leave empty for open network">
                                </div>
                            </div>

                            <button type="submit" class="btn btn-primary">Save Wi-Fi Settings</button>
                        </form>
                    </div>

                    <div class="dash-card">
                        <div class="dash-card-title">Telemetry Listener</div>
                        <form id="tel-form" onsubmit="saveConfig(event, 'telemetry')">
                            <div class="toggle-container">
                                <div>
                                    <div style="font-weight: 600;">Enable UDP Listener</div>
                                    <div style="color: var(--text-secondary); font-size: 0.85rem;">Listen on network UDP port.</div>
                                </div>
                                <label class="toggle-switch">
                                    <input type="checkbox" id="tel-enabled">
                                    <span class="slider-round"></span>
                                </label>
                            </div>

                            <div class="form-group">
                                <label for="tel-game">Game Type</label>
                                <select id="tel-game" onchange="toggleGameFields()">
                                    <option value="0">Forza Horizon 4 / 5 / Motorsport</option>
                                    <option value="1">F1 23 / 24 (EA Sports)</option>
                                    <option value="2">DiRT Rally 2.0</option>
                                    <option value="3">Assetto Corsa</option>
                                </select>
                            </div>

                            <div class="form-group" id="tel-pcip-field" style="display:none;">
                                <label for="tel-pcip">Game PC IP Address (For Assetto Corsa Handshake)</label>
                                <input type="text" id="tel-pcip" placeholder="e.g. 192.168.1.100">
                            </div>

                            <div class="form-group">
                                <label for="tel-port">UDP Listen Port</label>
                                <input type="number" id="tel-port" min="1024" max="65535">
                            </div>

                            <button type="submit" class="btn btn-primary" style="margin-bottom:1.5rem;">Save System Settings</button>
                        </form>
                        
                        <div style="border-top: 1px solid var(--border-color); padding-top:1.5rem; margin-top:1.5rem; display:flex; gap:1rem;">
                            <button class="btn btn-danger" onclick="deviceAction('reboot')">Reboot Device</button>
                            <button class="btn btn-secondary" onclick="deviceAction('reset')">Reset Config</button>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>

    <!-- Notification Toast -->
    <div id="toast" class="toast">Settings updated successfully!</div>

    <script>
        let deviceIp = "";
        let pollTimer = null;
        let ledCount = 16;

        function showToast(text, isError = false) {
            const toast = document.getElementById('toast');
            toast.textContent = text;
            toast.style.borderLeftColor = isError ? 'var(--accent-red)' : 'var(--accent-purple)';
            toast.classList.add('show');
            setTimeout(() => toast.classList.remove('show'), 3000);
        }

        function switchTab(tabId) {
            document.querySelectorAll('.tab-content').forEach(el => el.classList.remove('active'));
            document.querySelectorAll('.tab-btn').forEach(el => el.classList.remove('active'));
            
            document.getElementById(tabId).classList.add('active');
            document.getElementById(tabId + '-btn').classList.add('active');

            if (tabId === 'tab-dash') {
                startPolling();
            } else {
                stopPolling();
            }
        }

        function updateSlider(id) {
            const input = document.getElementById(id);
            const span = document.getElementById(id + '-val');
            span.textContent = input.value + (id.includes('rpm') ? '%' : '');
            simulateLedPreview();
        }

        function toggleWifiFields() {
            const opMode = document.getElementById('wifi-opmode').value;
            const isAp = (opMode === 'ap');
            document.getElementById('wifi-station-fields').style.display = isAp ? 'none' : 'block';
            document.getElementById('wifi-ap-fields').style.display = isAp ? 'block' : 'none';
        }

        function toggleGameFields() {
            const gameType = document.getElementById('tel-game').value;
            const isAssetto = (gameType === '3');
            document.getElementById('tel-pcip-field').style.display = isAssetto ? 'block' : 'none';
        }

        function drawLedPreview() {
            const preview = document.getElementById('led-preview');
            ledCount = parseInt(document.getElementById('sl-numleds').value) || 16;
            
            preview.innerHTML = '';
            for (let i = 0; i < ledCount; i++) {
                const node = document.createElement('div');
                node.className = 'led-node';
                node.id = 'preview-led-' + i;
                preview.appendChild(node);
            }
            simulateLedPreview(0.5);
        }

        function getLedColor(index, count, scheme) {
            const pos = index / (count - 1);
            if (scheme === 0) {
                if (pos < 0.5) return 'rgb(16, 185, 129)';
                if (pos < 0.8) return 'rgb(249, 115, 22)';
                return 'rgb(244, 63, 94)';
            } else if (scheme === 1) {
                if (pos < 0.5) return 'rgb(59, 130, 246)';
                if (pos < 0.8) return 'rgb(6, 182, 212)';
                return 'rgb(168, 85, 247)';
            } else {
                return `hsl(${pos * 300}, 100%, 50%)`;
            }
        }

        function simulateLedPreview(percent = 0.5) {
            const scheme = parseInt(document.getElementById('sl-scheme').value);
            const litCount = Math.round(percent * ledCount);

            for (let i = 0; i < ledCount; i++) {
                const led = document.getElementById('preview-led-' + i);
                if (!led) continue;
                
                if (i < litCount) {
                    const color = getLedColor(i, ledCount, scheme);
                    led.style.backgroundColor = color;
                    led.style.boxShadow = `0 0 10px ${color}, inset 0 1px 2px rgba(255,255,255,0.4)`;
                } else {
                    led.style.backgroundColor = '#1e293b';
                    led.style.boxShadow = 'inset 0 1px 3px rgba(0,0,0,0.5)';
                }
            }
        }

        // Connection to device
        function connectToDevice() {
            const ipInput = document.getElementById('device-ip').value.trim();
            if (!ipInput) {
                showToast("Please enter a valid IP address", true);
                return;
            }
            
            deviceIp = ipInput;
            if (!deviceIp.startsWith("http://") && !deviceIp.startsWith("https://")) {
                deviceIp = "http://" + deviceIp;
            }

            const badge = document.getElementById('connection-badge');
            const btn = document.getElementById('btn-connect');
            
            btn.textContent = "Connecting...";
            badge.textContent = "CONNECTING...";
            badge.className = "status-badge";

            fetch(`${deviceIp}/api/config`)
                .then(res => {
                    if (!res.ok) throw new Error("Connection failed");
                    return res.json();
                })
                .then(cfg => {
                    populateUI(cfg);
                    document.getElementById('config-panel').style.display = 'block';
                    btn.textContent = "🔌 Re-Connect";
                    badge.textContent = "CONNECTED";
                    badge.className = "status-badge connected";
                    showToast("Successfully connected to ESP32!");
                    startPolling();
                })
                .catch(err => {
                    btn.textContent = "🔌 Connect to ESP32";
                    badge.textContent = "CONNECTION FAILED";
                    badge.className = "status-badge";
                    document.getElementById('config-panel').style.display = 'none';
                    stopPolling();
                    showToast("Failed to connect to ESP32. Check IP address and CORS status.", true);
                });
        }

        function populateUI(cfg) {
            document.getElementById('wifi-ssid').value = cfg.wifi.ssid;
            document.getElementById('wifi-opmode').value = cfg.wifi.apMode ? "ap" : "client";
            document.getElementById('wifi-apname').value = cfg.wifi.apName;

            document.getElementById('tel-enabled').checked = cfg.telemetry.enabled;
            document.getElementById('tel-port').value = cfg.telemetry.port;
            document.getElementById('tel-game').value = cfg.telemetry.gameType.toString();
            document.getElementById('tel-pcip').value = cfg.telemetry.pcIpAddress || "192.168.1.100";

            document.getElementById('sl-enabled').checked = cfg.shiftLight.enabled;
            document.getElementById('sl-pin').value = cfg.shiftLight.pin.toString();
            document.getElementById('sl-numleds').value = cfg.shiftLight.numLeds;
            document.getElementById('sl-startrpm').value = cfg.shiftLight.startRpmPercent;
            document.getElementById('sl-endrpm').value = cfg.shiftLight.endRpmPercent;
            document.getElementById('sl-bright').value = cfg.shiftLight.brightness;
            document.getElementById('sl-scheme').value = cfg.shiftLight.colorScheme.toString();

            updateSlider('sl-startrpm');
            updateSlider('sl-endrpm');
            updateSlider('sl-bright');
            toggleWifiFields();
            toggleGameFields();
            drawLedPreview();
        }

        function startPolling() {
            if (pollTimer) clearInterval(pollTimer);
            pollTimer = setInterval(pollTelemetry, 250);
        }

        function stopPolling() {
            if (pollTimer) {
                clearInterval(pollTimer);
                pollTimer = null;
            }
        }

        function pollTelemetry() {
            if (!deviceIp) return;
            fetch(`${deviceIp}/api/status`)
                .then(res => res.json())
                .then(data => {
                    document.getElementById('dash-rpm-val').textContent = `${Math.round(data.currentRpm)} / ${Math.round(data.maxRpm)} RPM`;
                    document.getElementById('dash-rpm-max').textContent = `${Math.round(data.maxRpm)} RPM Max`;
                    document.getElementById('dash-gear').textContent = data.gear === 0 ? 'R' : (data.gear === 1 ? 'N' : (data.gear - 1).toString());
                    document.getElementById('dash-speed').innerHTML = `${Math.round(data.speedKmh)} <span style="font-size:1rem; font-weight:normal; color:var(--text-secondary);">km/h</span>`;
                    document.getElementById('dash-speed-mph').textContent = Math.round(data.speedMph);
                    document.getElementById('dash-boost').textContent = data.boost.toFixed(1);
                    document.getElementById('dash-uptime').textContent = data.uptimeSeconds + 's';
                    document.getElementById('dash-packets').textContent = data.packetsReceived;
                    document.getElementById('dash-slip').textContent = data.tireSlipMax.toFixed(2);

                    let pct = 0;
                    if (data.maxRpm > data.idleRpm) {
                        pct = ((data.currentRpm - data.idleRpm) / (data.maxRpm - data.idleRpm)) * 100;
                    }
                    document.getElementById('dash-rpm-bar').style.width = Math.min(100, Math.max(0, pct)) + '%';

                    if (data.isUpdating) {
                        let slStart = parseFloat(document.getElementById('sl-startrpm').value);
                        let slEnd = parseFloat(document.getElementById('sl-endrpm').value);
                        let currentPct = (data.currentRpm / data.maxRpm) * 100;
                        
                        if (currentPct < slStart) {
                            simulateLedPreview(0);
                        } else if (currentPct >= slEnd) {
                            const isEven = Math.floor(Date.now() / 80) % 2 === 0;
                            simulateLedPreview(isEven ? 1.0 : 0);
                        } else {
                            let fillPct = (currentPct - slStart) / (slEnd - slStart);
                            simulateLedPreview(fillPct);
                        }
                    }
                })
                .catch(err => {
                    const badge = document.getElementById('connection-badge');
                    badge.textContent = "ESP32 DISCONNECTED";
                    badge.className = "status-badge";
                    stopPolling();
                });
        }

        function saveConfig(e, section) {
            e.preventDefault();
            const payload = {};

            if (section === 'shiftLight') {
                payload.shiftLight = {
                    enabled: document.getElementById('sl-enabled').checked,
                    pin: parseInt(document.getElementById('sl-pin').value),
                    numLeds: parseInt(document.getElementById('sl-numleds').value),
                    startRpmPercent: parseFloat(document.getElementById('sl-startrpm').value),
                    endRpmPercent: parseFloat(document.getElementById('sl-endrpm').value),
                    brightness: parseInt(document.getElementById('sl-bright').value),
                    colorScheme: parseInt(document.getElementById('sl-scheme').value)
                };
            } else if (section === 'wifi') {
                const opMode = document.getElementById('wifi-opmode').value;
                payload.wifi = {
                    ssid: document.getElementById('wifi-ssid').value,
                    password: document.getElementById('wifi-pass').value,
                    apMode: (opMode === 'ap'),
                    apName: document.getElementById('wifi-apname').value,
                    apPassword: document.getElementById('wifi-appass').value
                };
            } else if (section === 'telemetry') {
                payload.telemetry = {
                    enabled: document.getElementById('tel-enabled').checked,
                    port: parseInt(document.getElementById('tel-port').value),
                    gameType: parseInt(document.getElementById('tel-game').value),
                    pcIpAddress: document.getElementById('tel-pcip').value
                };
            }

            fetch(`${deviceIp}/api/save`, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify(payload)
            })
            .then(res => {
                if (res.ok) {
                    showToast("Settings saved successfully!");
                } else {
                    showToast("Failed to save settings", true);
                }
            })
            .catch(err => showToast("Connection error when saving settings", true));
        }

        function testLeds(percent) {
            stopPolling();
            if (percent === 0) {
                simulateLedPreview(0);
                fetch(`${deviceIp}/api/test_leds?percent=0`)
                    .then(() => startPolling());
                return;
            }

            simulateLedPreview(percent);
            fetch(`${deviceIp}/api/test_leds?percent=${percent}`)
                .catch(() => showToast("Test command failed", true));
        }

        function deviceAction(action) {
            if (!confirm(`Are you sure you want to perform: ${action}?`)) return;
            
            fetch(`${deviceIp}/api/${action}`, { method: 'POST' })
                .then(() => {
                    showToast(`Device ${action} triggered! Panel reloading...`);
                    stopPolling();
                    document.getElementById('config-panel').style.display = 'none';
                    setTimeout(() => location.reload(), 5000);
                })
                .catch(() => showToast("Request failed", true));
        }
    </script>
</body>
</html>
)rawliteral";

#endif // HTML_INDEX_H
