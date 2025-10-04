# Project Setup Checklist

Use this checklist to track your ESP32 project setup and ensure all components are working correctly.

## Initial Setup

### Software Installation
- [ ] Visual Studio Code installed
- [ ] PlatformIO IDE extension installed
- [ ] Repository cloned to local machine
- [ ] Project opened in VS Code
- [ ] PlatformIO initialized (should see icon in sidebar)

### Hardware Setup
- [ ] ESP32 board connected via USB
- [ ] USB drivers installed (if needed)
- [ ] Serial port identified (COM port or /dev/tty*)
- [ ] Serial port configured in `platformio.ini` (optional)

## First Upload

### Filesystem Upload
- [ ] SPIFFS filesystem image built successfully
- [ ] `data/index.html` exists
- [ ] Filesystem uploaded to ESP32 (`pio run --target uploadfs`)
- [ ] No errors during upload

### Firmware Upload
- [ ] Project builds without errors (`pio run`)
- [ ] Firmware uploaded successfully (`pio run --target upload`)
- [ ] Serial monitor opened (`pio device monitor`)
- [ ] Boot messages visible in serial monitor

### Initial Boot Verification
- [ ] Logger initialization message shown
- [ ] WiFi manager initialized
- [ ] SPIFFS mounted successfully
- [ ] Web server started on port 80
- [ ] No critical errors in logs

## WiFi Configuration

### Connection Setup
- [ ] WiFi credentials prepared (SSID and password)
- [ ] Device IP address identified (from serial monitor or router)
- [ ] Web interface accessible in browser
- [ ] Configuration page loads correctly

### WiFi Connection
- [ ] SSID entered in web interface
- [ ] Password entered (if required)
- [ ] Configuration saved successfully
- [ ] Device reconnected to WiFi
- [ ] New IP address assigned
- [ ] Can access web interface at new IP

### Connection Verification
- [ ] WiFi status shows "Connected"
- [ ] IP address displayed correctly
- [ ] Signal strength visible (RSSI in dBm)
- [ ] Uptime counter incrementing
- [ ] Device pingable from computer

## Feature Testing

### Web Server
- [ ] Can access http://\<device-ip\>/
- [ ] Web interface loads without errors
- [ ] Status updates in real-time
- [ ] Configuration form works
- [ ] API endpoint `/api/status` returns JSON
- [ ] API endpoint `/api/config` accessible

### Serial Logging
- [ ] Log messages visible in serial monitor
- [ ] Timestamps present on each message
- [ ] Log levels working (INFO, WARN, ERROR, DEBUG)
- [ ] Messages are readable and formatted correctly

### WiFi Auto-Reconnect
- [ ] Device reconnects after router reboot
- [ ] Reconnection logged in serial monitor
- [ ] Web interface accessible after reconnect
- [ ] No manual intervention required

### HTTP Client (if configured)
- [ ] External API endpoint configured
- [ ] Test data sent successfully
- [ ] HTTP response codes logged
- [ ] Error handling works for failed requests

### OTA Updates
- [ ] OTA hostname set in configuration
- [ ] OTA password configured
- [ ] `platformio.ini` updated for OTA upload
- [ ] Device discoverable on network for OTA
- [ ] Test firmware uploaded via OTA
- [ ] Update completed successfully
- [ ] Device rebooted with new firmware

## Code Customization

### Configuration
- [ ] Reviewed `include/config.h`
- [ ] Adjusted constants if needed (timeouts, intervals)
- [ ] Updated device name
- [ ] Set appropriate log level

### Credentials
- [ ] Copied `credentials.h.example` to `credentials.h`
- [ ] Updated WiFi credentials (if hardcoding)
- [ ] Set API endpoints (if using HTTP client)
- [ ] Verified `credentials.h` is in .gitignore

### Custom Features
- [ ] Identified features to add (sensors, actuators, etc.)
- [ ] Added required libraries to `platformio.ini`
- [ ] Created custom modules (if needed)
- [ ] Updated documentation for changes

## Documentation Review

### Read Documentation
- [ ] Read [QUICKSTART.md](QUICKSTART.md) for quick setup
- [ ] Read [README.md](README.md) for project overview
- [ ] Read [docs/SETUP.md](docs/SETUP.md) for detailed setup
- [ ] Read [docs/API.md](docs/API.md) for API documentation
- [ ] Read [docs/OTA_UPDATES.md](docs/OTA_UPDATES.md) for OTA guide
- [ ] Read [docs/EXAMPLES.md](docs/EXAMPLES.md) for code examples
- [ ] Read [ARCHITECTURE.md](ARCHITECTURE.md) for system design
- [ ] Read [FOLDER_STRUCTURE.md](FOLDER_STRUCTURE.md) for file organization

## Development Workflow

### Version Control
- [ ] Git initialized (if starting fresh)
- [ ] Remote repository configured
- [ ] `.gitignore` working (credentials not tracked)
- [ ] Initial commit made
- [ ] Pushed to remote repository

### Build Process
- [ ] Familiar with PlatformIO commands
- [ ] Can build project: `pio run`
- [ ] Can upload firmware: `pio run --target upload`
- [ ] Can upload filesystem: `pio run --target uploadfs`
- [ ] Can monitor serial: `pio device monitor`

### Debugging
- [ ] Serial monitor working
- [ ] Can see debug messages
- [ ] Understand log levels
- [ ] Can enable/disable debug output
- [ ] Exception decoder working (if needed)

## Production Readiness

### Security
- [ ] Changed default OTA password
- [ ] Strong WiFi password used
- [ ] Credentials not committed to repository
- [ ] API authentication added (if needed)
- [ ] Considered HTTPS for production

### Stability
- [ ] Device runs for extended period without crashes
- [ ] Memory leaks checked (heap monitoring)
- [ ] WiFi reconnection tested
- [ ] OTA updates tested multiple times
- [ ] Error handling verified

### Performance
- [ ] Boot time acceptable
- [ ] Web interface responsive
- [ ] API response times acceptable
- [ ] WiFi signal strength adequate
- [ ] Memory usage within limits

### Deployment
- [ ] Tested on actual hardware
- [ ] Tested in target environment
- [ ] Backup firmware saved
- [ ] Rollback plan in place
- [ ] Monitoring solution ready (if needed)

## Optional Enhancements

### Advanced Features
- [ ] MQTT client added (if needed)
- [ ] Additional sensors integrated
- [ ] Custom web pages created
- [ ] Database integration (if applicable)
- [ ] Cloud service integration (if needed)

### Optimization
- [ ] Code reviewed and optimized
- [ ] Memory usage minimized
- [ ] Power consumption optimized (if battery powered)
- [ ] Network efficiency improved
- [ ] Build size reduced (if needed)

### Documentation
- [ ] Custom features documented
- [ ] API changes documented
- [ ] Configuration options documented
- [ ] Troubleshooting guide updated
- [ ] Examples added for new features

## Troubleshooting Completed

### Common Issues Resolved
- [ ] Upload failures resolved
- [ ] Serial port permissions fixed (Linux)
- [ ] WiFi connection issues debugged
- [ ] Web interface loading problems fixed
- [ ] OTA update issues resolved
- [ ] Memory issues addressed

## Project Status

**Current Status**: ⬜ Not Started | ⬜ In Progress | ⬜ Testing | ⬜ Production

**Notes**:
```
[Add your notes here about project status, issues, or next steps]




```

**Last Updated**: [Date]

**Next Steps**:
1. 
2. 
3. 

---

## Quick Reference

### Essential Commands
```bash
# Build
pio run

# Upload firmware
pio run --target upload

# Upload filesystem
pio run --target uploadfs

# Serial monitor
pio device monitor

# Clean build
pio run --target clean

# OTA upload (after configuring platformio.ini)
pio run --target upload --upload-port 192.168.1.100
```

### Important Files
- `platformio.ini` - Project configuration
- `src/main.cpp` - Main application
- `include/config.h` - Configuration constants
- `data/index.html` - Web interface
- `.gitignore` - Git ignore rules

### Important URLs
- Device: `http://<device-ip>/`
- Status API: `http://<device-ip>/api/status`
- Config API: `http://<device-ip>/api/config`

### Support
- 📖 [Documentation](docs/)
- 🐛 [GitHub Issues](https://github.com/pnstack/template-esp32-project/issues)
- 💬 [Discussions](https://github.com/pnstack/template-esp32-project/discussions)

---

**Tip**: Print this checklist and check off items as you complete them, or keep a digital copy and update it regularly!
