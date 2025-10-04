# Quick Start Guide

Get your ESP32 project up and running in 5 minutes!

## Prerequisites

✅ ESP32 board  
✅ USB cable  
✅ VS Code + PlatformIO extension

## Step-by-Step Setup

### 1. Install VS Code & PlatformIO (5 minutes)

1. Download and install [Visual Studio Code](https://code.visualstudio.com/)
2. Open VS Code
3. Go to Extensions (Ctrl+Shift+X)
4. Search for "PlatformIO IDE"
5. Click Install
6. Wait for installation to complete
7. Reload VS Code

### 2. Clone and Open Project (1 minute)

```bash
git clone https://github.com/pnstack/template-esp32-project.git
cd template-esp32-project
code .
```

### 3. Connect ESP32 (30 seconds)

1. Connect ESP32 to your computer via USB
2. Wait for drivers to install (Windows)
3. Note the port:
   - **Linux**: `/dev/ttyUSB0` or `/dev/ttyACM0`
   - **Windows**: `COM3`, `COM4`, etc.
   - **macOS**: `/dev/cu.usbserial-*`

### 4. Configure Serial Port (Optional)

Edit `platformio.ini` if needed:

```ini
upload_port = /dev/ttyUSB0    ; Your port here
monitor_port = /dev/ttyUSB0   ; Same as upload_port
```

**Auto-detection**: PlatformIO usually auto-detects the port!

### 5. Upload Files (2 minutes)

Click the PlatformIO icon in the sidebar, then:

1. **Upload Filesystem Image** (contains web interface)
   - Wait for "SUCCESS" message
   
2. **Upload** (main firmware)
   - Wait for "SUCCESS" message

### 6. Open Serial Monitor (30 seconds)

Click **Monitor** in PlatformIO sidebar.

You should see:
```
===================================
ESP32 System Logger Initialized
===================================
[INFO] WiFi Manager initialized
[INFO] Web Server started on port 80
```

### 7. Configure WiFi (1 minute)

**Option A: Via Web Interface** (Recommended)

1. Find IP address in serial monitor:
   ```
   [INFO] IP Address: 192.168.4.1
   ```
2. Open browser: `http://192.168.4.1`
3. Enter your WiFi credentials
4. Click "Save Configuration"

**Option B: Hardcode Credentials** (For testing)

Edit `src/main.cpp`:
```cpp
wifiManager.connect("YourNetworkName", "YourPassword");
```

Then re-upload firmware.

### 8. Success! ✨

Once connected to WiFi, the serial monitor shows:
```
[INFO] WiFi connected!
[INFO] IP Address: 192.168.1.100
```

Open browser: `http://192.168.1.100`

## Common Issues & Fixes

### ❌ Upload Failed

**Problem**: "Failed to connect to ESP32"

**Solutions**:
- Hold BOOT button during upload
- Try different USB cable
- Check port in Device Manager/System
- Reduce upload speed in `platformio.ini`:
  ```ini
  upload_speed = 115200
  ```

### ❌ Permission Denied (Linux)

**Problem**: "cannot access port /dev/ttyUSB0"

**Solution**:
```bash
sudo usermod -a -G dialout $USER
# Log out and log back in
```

### ❌ Web Interface Not Loading

**Problem**: Cannot access web page

**Solutions**:
1. Verify you uploaded filesystem:
   ```bash
   pio run --target uploadfs
   ```
2. Check IP address in serial monitor
3. Ensure you're on the same network

### ❌ WiFi Won't Connect

**Problem**: WiFi connection fails

**Solutions**:
- Verify SSID and password are correct
- Check network is 2.4GHz (ESP32 doesn't support 5GHz)
- Move closer to router
- Check router doesn't block new devices

## What's Next?

### 🎯 Customize Your Project

1. **Add sensors**: See [EXAMPLES.md](docs/EXAMPLES.md)
2. **Modify web interface**: Edit `data/index.html`
3. **Add API endpoints**: Edit `src/web_server.cpp`
4. **Send data to server**: Use HTTP client examples

### 📚 Learn More

- **[Setup Guide](docs/SETUP.md)**: Detailed installation instructions
- **[API Documentation](docs/API.md)**: HTTP endpoints and usage
- **[OTA Updates](docs/OTA_UPDATES.md)**: Wireless firmware updates
- **[Examples](docs/EXAMPLES.md)**: Code snippets for common tasks

### 🔧 Development Workflow

1. **Edit code** in `src/` or `include/`
2. **Build**: PlatformIO → Build
3. **Upload**: PlatformIO → Upload
4. **Monitor**: PlatformIO → Monitor
5. **Repeat!**

## PlatformIO Commands (CLI)

Prefer command line? Use these:

```bash
# Build project
pio run

# Upload firmware
pio run --target upload

# Upload web files
pio run --target uploadfs

# Serial monitor
pio device monitor

# All in one
pio run --target upload && pio device monitor
```

## Testing Your Setup

### Test WiFi
```cpp
// In src/main.cpp
void loop() {
    if (wifiManager.isConnected()) {
        Logger::info("WiFi OK - IP: " + wifiManager.getIPAddress());
    } else {
        Logger::warn("WiFi disconnected");
    }
    delay(5000);
}
```

### Test Web Server
Open browser: `http://<your-esp32-ip>/api/status`

Should return JSON:
```json
{
  "wifi_connected": true,
  "ip_address": "192.168.1.100",
  ...
}
```

### Test OTA Updates
1. Edit some code
2. Build: `pio run`
3. Update `platformio.ini`:
   ```ini
   upload_protocol = espota
   upload_port = 192.168.1.100
   ```
4. Upload: `pio run --target upload`

## Tips & Tricks

💡 **Use Auto-Format**: Alt+Shift+F in VS Code  
💡 **Serial Monitor Shortcut**: Ctrl+Alt+S  
💡 **Build Shortcut**: Ctrl+Alt+B  
💡 **Upload Shortcut**: Ctrl+Alt+U  

💡 **View Serial Output**: Keep monitor open in split terminal  
💡 **Quick Restart**: Press EN button on ESP32  
💡 **Clear Output**: Click trash icon in terminal  

💡 **Change Log Level**:
```cpp
Logger::setLogLevel(LOG_DEBUG);  // More verbose
Logger::setLogLevel(LOG_ERROR);  // Quiet
```

💡 **Save Credentials Template**:
```bash
cp include/credentials.h.example include/credentials.h
# Edit credentials.h with your settings
```

## Getting Help

🐛 **Found a bug?** Create an issue on GitHub  
❓ **Have a question?** Check documentation first  
💬 **Want to contribute?** See [CONTRIBUTING.md](CONTRIBUTING.md)  

## Project Stats

- **Lines of Code**: ~1,500
- **Build Time**: ~30 seconds
- **Upload Time**: ~10 seconds
- **Firmware Size**: ~300 KB
- **RAM Usage**: ~100 KB

## Success Checklist

- [ ] PlatformIO installed
- [ ] Project cloned and opened
- [ ] ESP32 connected
- [ ] Filesystem uploaded
- [ ] Firmware uploaded
- [ ] Serial monitor working
- [ ] WiFi configured
- [ ] Web interface accessible
- [ ] Ready to customize!

---

**Congratulations! Your ESP32 is ready for development! 🎉**

Next: Check out [EXAMPLES.md](docs/EXAMPLES.md) to add sensors, customize the web interface, and build your IoT project!
