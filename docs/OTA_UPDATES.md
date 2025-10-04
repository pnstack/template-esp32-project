# Over-The-Air (OTA) Firmware Updates

This guide explains how to perform OTA firmware updates on your ESP32 device without physically connecting it via USB.

## Overview

OTA updates allow you to upload new firmware to the ESP32 over WiFi. This is particularly useful for:
- Devices in remote or hard-to-reach locations
- Production deployments
- Quick testing and iteration during development
- Updating multiple devices on the same network

## Prerequisites

1. ESP32 must be connected to WiFi
2. Computer and ESP32 must be on the same network
3. OTA must be enabled in the firmware (enabled by default in this template)
4. Know the device's hostname or IP address

## OTA Configuration

### Default Settings

The OTA configuration is defined in `include/config.h`:

```cpp
#define OTA_HOSTNAME "esp32-device"
#define OTA_PASSWORD "admin"
```

### Customizing OTA Settings

You can change these settings:

1. **Edit `include/config.h`**:
   ```cpp
   #define OTA_HOSTNAME "my-custom-device"
   #define OTA_PASSWORD "my-secure-password"
   ```

2. **Or modify in `src/main.cpp`**:
   ```cpp
   otaManager.begin("my-device-name", "my-password");
   ```

## Performing OTA Updates

### Method 1: Using PlatformIO (Recommended)

#### Step 1: Configure Upload Port

Edit `platformio.ini` to add OTA upload configuration:

```ini
[env:esp32dev]
; ... existing configuration ...

; OTA Upload Configuration
upload_protocol = espota
upload_port = 192.168.1.100      ; Your ESP32's IP address
upload_flags = 
    --auth=admin                  ; OTA password (must match firmware)
```

#### Step 2: Upload Firmware via OTA

```bash
pio run --target upload
```

Or in VS Code: **PlatformIO > Upload**

### Method 2: Using Arduino IDE

1. Open Arduino IDE
2. Go to **Tools > Port**
3. Select network port: `esp32-device at 192.168.1.100` (hostname at IP)
4. Click **Upload**
5. Enter OTA password when prompted

### Method 3: Using espota.py Script (Manual)

The `espota.py` script is included with the ESP32 Arduino framework.

```bash
# Find the script location
find ~/.platformio -name espota.py

# Upload firmware
python ~/.platformio/packages/framework-arduinoespressif32/tools/espota.py \
    -i 192.168.1.100 \
    -p 3232 \
    --auth=admin \
    -f .pio/build/esp32dev/firmware.bin
```

### Method 4: Using PlatformIO Core with Auto-Discovery

PlatformIO can automatically discover ESP32 devices on your network:

```bash
# List available OTA devices
pio device list --mdns

# Upload to discovered device
pio run --target upload --upload-port esp32-device.local
```

## OTA Update Process

### What Happens During Update

1. **Initiation**: Upload command is sent to ESP32
2. **Authentication**: OTA password is verified
3. **Upload**: New firmware is transferred over WiFi
4. **Verification**: Firmware integrity is checked
5. **Flash**: New firmware is written to flash memory
6. **Reboot**: ESP32 restarts with new firmware

### Serial Monitor Output

During OTA update, you'll see logs like:

```
[INFO] OTA Update Started: sketch
[INFO] OTA Progress: 10%
[INFO] OTA Progress: 20%
[INFO] OTA Progress: 30%
...
[INFO] OTA Progress: 100%
[INFO] OTA Update Completed!
```

## Troubleshooting

### Device Not Found

**Issue**: Cannot find ESP32 on network

**Solutions**:
1. Verify ESP32 is connected to WiFi (check serial monitor)
2. Ping the device:
   ```bash
   ping 192.168.1.100
   # or
   ping esp32-device.local
   ```
3. Check firewall settings (allow port 3232)
4. Ensure computer and ESP32 are on the same network/VLAN

### Authentication Failed

**Issue**: OTA password rejected

**Solutions**:
1. Verify password in `platformio.ini` matches firmware
2. Re-upload firmware via USB with correct password
3. Check for typos in password

### Upload Failed or Timeout

**Issue**: Upload starts but fails to complete

**Solutions**:
1. Move closer to WiFi router (improve signal strength)
2. Restart ESP32 and try again
3. Check available flash space
4. Verify firmware size isn't too large
5. Temporarily disable other network-heavy operations

### OTA Not Available

**Issue**: OTA port doesn't appear

**Solutions**:
1. Verify ESP32 is running and connected to WiFi
2. Check OTA is initialized in firmware:
   ```cpp
   otaManager.begin(OTA_HOSTNAME, OTA_PASSWORD);
   ```
3. Wait ~30 seconds after ESP32 boots
4. Restart mDNS service on your computer

## Security Best Practices

### 1. Use Strong Passwords

```cpp
// Bad
#define OTA_PASSWORD "admin"

// Good
#define OTA_PASSWORD "MyStr0ng!P@ssw0rd123"
```

### 2. Disable OTA in Production (Optional)

For highly secure deployments, consider disabling OTA after initial setup:

```cpp
void setup() {
    // Only enable OTA in debug builds
    #ifdef DEBUG
    otaManager.begin(OTA_HOSTNAME, OTA_PASSWORD);
    #endif
}
```

### 3. Use mDNS for Hostname Resolution

Instead of hardcoding IP addresses, use the device hostname:

```bash
# Upload to hostname instead of IP
pio run --target upload --upload-port esp32-device.local
```

### 4. Monitor OTA Activity

Check serial logs regularly to detect unauthorized OTA attempts:

```
[ERROR] OTA Error[1]: Auth Failed
```

## Advanced OTA Features

### Filesystem Updates via OTA

You can also update the SPIFFS filesystem via OTA:

```cpp
// In platformio.ini
upload_protocol = espota
upload_flags = 
    --auth=admin
    --spiffs
```

Then upload filesystem:
```bash
pio run --target uploadfs
```

### Custom OTA Callbacks

Add custom behavior during OTA updates:

```cpp
ArduinoOTA.onStart([]() {
    // Turn on LED indicator
    digitalWrite(LED_PIN, HIGH);
});

ArduinoOTA.onEnd([]() {
    // Turn off LED indicator
    digitalWrite(LED_PIN, LOW);
});

ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    // Update progress indicator
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
});
```

### Rollback Protection

Implement version checking to prevent downgrade attacks:

```cpp
#define FIRMWARE_VERSION "1.2.0"

bool shouldAcceptUpdate(const String& newVersion) {
    // Compare versions and decide
    return compareVersions(newVersion, FIRMWARE_VERSION) > 0;
}
```

## OTA Update Workflow

### Development Workflow

1. **First Upload**: USB connection (required)
2. **Subsequent Updates**: OTA (convenient)
3. **Major Changes**: USB connection (safer)
4. **Minor Fixes**: OTA (faster)

### Production Workflow

1. **Test firmware** on development device via USB
2. **Validate** all features work correctly
3. **Perform OTA** update on staging device
4. **Monitor** for 24-48 hours
5. **Roll out** to production devices via OTA
6. **Keep backup** of previous firmware version

## Monitoring OTA Updates

### Via Serial Monitor

Keep serial monitor open during OTA:

```bash
pio device monitor
```

### Via Web Interface

Check device status at `http://device-ip/api/status` for uptime and version info.

### Via HTTP Endpoint

Create a custom endpoint to report firmware version:

```cpp
server.on("/api/version", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", FIRMWARE_VERSION);
});
```

## Common OTA Update Scenarios

### Scenario 1: Bug Fix

1. Fix bug in code
2. Increment version number
3. Build firmware: `pio run`
4. Upload via OTA: `pio run --target upload --upload-port device-ip`
5. Verify fix in serial monitor

### Scenario 2: Feature Addition

1. Develop new feature
2. Test locally via USB
3. Once stable, deploy via OTA to remote devices
4. Monitor serial logs for issues

### Scenario 3: Configuration Change

1. Modify `config.h` parameters
2. Rebuild and upload via OTA
3. Device applies new configuration on reboot

## Tips and Best Practices

1. **Always test OTA locally first** before deploying to remote devices
2. **Keep USB access available** as a fallback
3. **Use version numbers** to track firmware versions
4. **Test WiFi stability** before relying on OTA
5. **Have a rollback plan** in case of issues
6. **Document changes** in each firmware version
7. **Monitor first device** before rolling out widely
8. **Consider staged rollouts** for large deployments

## Further Reading

- [ESP32 OTA Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/ota.html)
- [Arduino OTA Library](https://github.com/esp8266/Arduino/tree/master/libraries/ArduinoOTA)
- [PlatformIO OTA Updates](https://docs.platformio.org/en/latest/platforms/espressif32.html#over-the-air-ota-update)
