# ESP32 Project Setup Guide

## Prerequisites

### Hardware Requirements
- ESP32 development board (ESP32-WROOM, ESP32-DevKitC, or similar)
- USB cable for programming and power
- Computer with USB port

### Software Requirements
1. **Visual Studio Code** (recommended) or any text editor
2. **PlatformIO IDE** extension for VS Code
   - Install from VS Code Extensions marketplace
   - Or use PlatformIO Core CLI

3. **Python 3.x** (usually included with PlatformIO)

## Installation Steps

### 1. Clone the Repository

```bash
git clone https://github.com/pnstack/template-esp32-project.git
cd template-esp32-project
```

### 2. Open Project in VS Code

```bash
code .
```

Or open VS Code and select **File > Open Folder** and choose the project directory.

### 3. Install Dependencies

PlatformIO will automatically install all required dependencies when you first build the project. The dependencies include:

- ESP32 Arduino Framework
- ESP Async WebServer
- AsyncTCP
- ArduinoJson

These are defined in `platformio.ini` and will be downloaded automatically.

### 4. Configure Serial Port (Optional)

Edit `platformio.ini` to match your serial port:

```ini
upload_port = /dev/ttyUSB0    ; Linux
; upload_port = COM3          ; Windows
; upload_port = /dev/cu.usbserial-*  ; macOS

monitor_port = /dev/ttyUSB0   ; Same as upload_port
```

To find your serial port:
- **Linux**: `ls /dev/ttyUSB*` or `ls /dev/ttyACM*`
- **Windows**: Check Device Manager > Ports (COM & LPT)
- **macOS**: `ls /dev/cu.*`

## Building the Project

### Using VS Code + PlatformIO

1. Click the PlatformIO icon in the sidebar
2. Under "Project Tasks", click:
   - **Build** to compile the firmware
   - **Upload** to flash the ESP32
   - **Monitor** to view serial output
   - **Upload Filesystem Image** to upload web files

### Using PlatformIO CLI

```bash
# Build the project
pio run

# Upload firmware
pio run --target upload

# Monitor serial output
pio device monitor

# Upload filesystem (SPIFFS) with web files
pio run --target uploadfs

# Build and upload in one command
pio run --target upload && pio device monitor
```

## First-Time Setup

### 1. Upload Filesystem Image

**IMPORTANT**: Before uploading the main firmware, upload the filesystem image containing the web interface:

```bash
pio run --target uploadfs
```

Or in VS Code: **PlatformIO > Upload Filesystem Image**

### 2. Upload Firmware

```bash
pio run --target upload
```

Or in VS Code: **PlatformIO > Upload**

### 3. Open Serial Monitor

```bash
pio device monitor
```

Or in VS Code: **PlatformIO > Monitor**

You should see initialization logs like:

```
===================================
ESP32 System Logger Initialized
===================================

[INFO] ESP32 Template Project
[INFO] Starting system initialization...
[INFO] WiFi Manager initialized
[INFO] SPIFFS mounted successfully
[INFO] Web Server started on port 80
[INFO] System initialization completed!
```

### 4. Configure WiFi

#### Option A: Via Web Interface (Recommended)

1. The ESP32 will create a WiFi access point (if not configured)
2. Connect your computer/phone to the ESP32's WiFi network
3. Open a web browser and navigate to: `http://192.168.4.1` (default AP IP)
4. Enter your WiFi credentials in the configuration page
5. Click "Save Configuration"
6. The ESP32 will restart and connect to your WiFi network

#### Option B: Via Serial Monitor

You can also view logs and monitor the connection status through the serial monitor.

### 5. Access the Device

Once connected to your WiFi network, find the device's IP address from the serial monitor:

```
[INFO] WiFi connected!
[INFO] IP Address: 192.168.1.100
```

Open a web browser and navigate to this IP address to access the configuration interface.

## Development Workflow

### Typical Development Cycle

1. **Edit code** in `src/` or `include/` directories
2. **Build** to check for compilation errors
   ```bash
   pio run
   ```
3. **Upload** to ESP32
   ```bash
   pio run --target upload
   ```
4. **Monitor** serial output for debugging
   ```bash
   pio device monitor
   ```

### Modifying Web Interface

1. Edit files in `data/` directory
2. Upload filesystem image:
   ```bash
   pio run --target uploadfs
   ```
3. Refresh your browser

### Tips

- Use `Ctrl+C` to exit the serial monitor
- Press the `EN` (reset) button on ESP32 after upload if it doesn't restart automatically
- Check serial output for debugging information
- The web server runs on port 80 by default

## Troubleshooting

### Cannot Connect to Serial Port

**Issue**: Permission denied or port not found

**Solution (Linux)**:
```bash
sudo usermod -a -G dialout $USER
# Log out and log back in
```

**Solution (Windows)**:
- Install CH340/CP2102 USB driver
- Check Device Manager for the correct COM port

### Upload Failed

**Issue**: Failed to connect to ESP32

**Solutions**:
1. Hold the `BOOT` button while uploading
2. Try a different USB cable (data cable, not charge-only)
3. Reduce upload speed in `platformio.ini`:
   ```ini
   upload_speed = 115200
   ```

### SPIFFS Mount Failed

**Issue**: Web server cannot serve files

**Solution**:
1. Upload filesystem image first:
   ```bash
   pio run --target uploadfs
   ```
2. Check that `data/` directory contains `index.html`

### WiFi Connection Failed

**Issue**: Cannot connect to WiFi

**Solutions**:
1. Verify SSID and password are correct
2. Check that WiFi network is 2.4GHz (ESP32 doesn't support 5GHz)
3. Check router settings (MAC filtering, etc.)
4. View serial monitor for detailed error messages

### Out of Memory

**Issue**: Compilation fails due to insufficient memory

**Solution**:
- Reduce log level in code
- Remove unused libraries
- Optimize code size

## Next Steps

- Read [OTA_UPDATES.md](OTA_UPDATES.md) for over-the-air update instructions
- Check [API.md](API.md) for HTTP API documentation
- Customize the project for your specific needs
- Add your own sensors and actuators

## Support

For issues and questions:
- Check the GitHub Issues page
- Review PlatformIO documentation: https://docs.platformio.org
- ESP32 Arduino documentation: https://docs.espressif.com
