# ESP32 Template Project

A comprehensive ESP32 project template with WiFi configuration, web server, OTA updates, HTTP client, and serial logging. Built with Arduino framework and PlatformIO for easy development and deployment.

## 🚀 Features

- ✅ **WiFi Management**: Auto-reconnect, configurable credentials via web interface
- ✅ **Web Server**: Async web server with responsive HTML UI
- ✅ **OTA Updates**: Over-the-air firmware updates for remote devices
- ✅ **HTTP Client**: Send data to external APIs/servers
- ✅ **Serial Logging**: Comprehensive logging with multiple log levels
- ✅ **SPIFFS**: File system for storing configuration and web files
- ✅ **Clean Architecture**: Well-organized code structure with separation of concerns
- ✅ **PlatformIO Support**: Complete development environment setup

## 📁 Project Structure

```
template-esp32-project/
├── src/                        # Source files
│   ├── main.cpp               # Main application entry point
│   ├── wifi_manager.cpp       # WiFi connection and management
│   ├── web_server.cpp         # Web server implementation
│   ├── ota_manager.cpp        # OTA update handling
│   ├── http_client.cpp        # HTTP client for API calls
│   └── logger.cpp             # Serial logging implementation
├── include/                    # Header files
│   ├── config.h               # Configuration constants
│   ├── wifi_manager.h         # WiFi manager interface
│   ├── web_server.h           # Web server interface
│   ├── ota_manager.h          # OTA manager interface
│   ├── http_client.h          # HTTP client interface
│   ├── logger.h               # Logger interface
│   └── credentials.h.example  # Example credentials file
├── data/                       # Web files (uploaded to SPIFFS)
│   └── index.html             # Web configuration interface
├── docs/                       # Documentation
│   ├── SETUP.md               # Setup and installation guide
│   ├── OTA_UPDATES.md         # OTA update instructions
│   └── API.md                 # HTTP API documentation
├── platformio.ini             # PlatformIO configuration
├── .gitignore                 # Git ignore file
└── README.md                  # This file
```

## 🛠️ Quick Start

### Prerequisites

- [VS Code](https://code.visualstudio.com/) with [PlatformIO IDE](https://platformio.org/install/ide?install=vscode) extension
- ESP32 development board
- USB cable

### Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/pnstack/template-esp32-project.git
   cd template-esp32-project
   ```

2. **Open in VS Code**:
   ```bash
   code .
   ```

3. **Upload filesystem image** (contains web interface):
   ```bash
   pio run --target uploadfs
   ```
   Or in VS Code: **PlatformIO → Upload Filesystem Image**

4. **Build and upload firmware**:
   ```bash
   pio run --target upload
   ```
   Or in VS Code: **PlatformIO → Upload**

5. **Open serial monitor**:
   ```bash
   pio device monitor
   ```
   Or in VS Code: **PlatformIO → Monitor**

### First-Time Configuration

1. After uploading, the ESP32 will start and display its status in the serial monitor
2. If no WiFi is configured, you can:
   - **Option A**: Connect to ESP32's default IP (usually `192.168.4.1`) if it creates an AP
   - **Option B**: Connect it to your network and find its IP in the serial monitor
3. Open a web browser and navigate to the ESP32's IP address
4. Enter your WiFi credentials in the web interface
5. Click "Save Configuration"
6. The device will reconnect to your WiFi network

## 📖 Documentation

- **[Setup Guide](docs/SETUP.md)**: Detailed installation and setup instructions
- **[OTA Updates](docs/OTA_UPDATES.md)**: How to perform over-the-air firmware updates
- **[API Documentation](docs/API.md)**: HTTP API endpoints and usage examples

## 🎯 Usage Examples

### Configure WiFi via Web Interface

1. Access the device at `http://<device-ip>/`
2. View current status (WiFi connection, IP address, uptime, etc.)
3. Update WiFi credentials
4. Monitor device status in real-time

### Send Data to External Server

```cpp
#include "http_client.h"

HTTPClientManager httpClient;

void sendSensorData() {
    float temperature = 25.5;
    float humidity = 60.0;
    
    const char* endpoint = "http://your-server.com/api/data";
    bool success = httpClient.sendSensorData(endpoint, temperature, humidity);
    
    if (success) {
        Logger::info("Data sent successfully");
    }
}
```

### Update Firmware via OTA

```bash
# Configure upload port in platformio.ini
upload_protocol = espota
upload_port = 192.168.1.100

# Upload via OTA
pio run --target upload
```

### Access Device Status via API

```bash
curl http://192.168.1.100/api/status
```

Response:
```json
{
  "device_name": "ESP32-Device",
  "uptime": 123456,
  "wifi_connected": true,
  "ssid": "MyWiFiNetwork",
  "ip_address": "192.168.1.100",
  "signal_strength": -45
}
```

## 🔧 Configuration

### WiFi Settings

Edit `include/config.h`:
```cpp
#define WIFI_RECONNECT_INTERVAL 5000  // ms between reconnect attempts
#define WIFI_MAX_RETRY 20             // max reconnection attempts
```

### Web Server Port

```cpp
#define WEBSERVER_PORT 80
```

### OTA Settings

```cpp
#define OTA_HOSTNAME "esp32-device"
#define OTA_PASSWORD "admin"
```

### Serial Logging

```cpp
#define SERIAL_BAUD_RATE 115200

// In your code:
Logger::setLogLevel(LOG_DEBUG);  // LOG_ERROR, LOG_WARN, LOG_INFO, LOG_DEBUG
```

## 🧪 Development

### PlatformIO Commands

```bash
# Build project
pio run

# Upload firmware via USB
pio run --target upload

# Upload filesystem (web files)
pio run --target uploadfs

# Open serial monitor
pio device monitor

# Clean build files
pio run --target clean

# Update libraries
pio lib update
```

### Adding New Features

1. Create new `.cpp` and `.h` files in `src/` and `include/`
2. Add your functionality following the existing code structure
3. Use the Logger for debugging: `Logger::info("Your message")`
4. Test thoroughly via serial monitor

### Modifying Web Interface

1. Edit `data/index.html`
2. Upload filesystem: `pio run --target uploadfs`
3. Refresh your browser

## 📦 Dependencies

The following libraries are automatically installed by PlatformIO:

- **ESP32 Arduino Framework**: Core ESP32 support
- **ESP AsyncWebServer**: Asynchronous web server
- **AsyncTCP**: Async TCP library for ESP32
- **ArduinoJson**: JSON parsing and generation

## 🐛 Troubleshooting

### Cannot connect to serial port

```bash
# Linux: Add user to dialout group
sudo usermod -a -G dialout $USER
# Log out and log back in
```

### Upload fails

- Hold the BOOT button during upload
- Try a different USB cable
- Reduce upload speed in `platformio.ini`

### WiFi connection issues

- Verify credentials are correct
- Ensure network is 2.4GHz (ESP32 doesn't support 5GHz)
- Check router settings (MAC filtering, etc.)
- View detailed logs in serial monitor

### Web interface not loading

- Upload filesystem first: `pio run --target uploadfs`
- Verify `data/index.html` exists
- Check SPIFFS mount status in serial monitor

## 🔒 Security Notes

1. **Change default OTA password** before deployment
2. **Never commit credentials** to version control (use `credentials.h`, which is in `.gitignore`)
3. **Use strong WiFi passwords**
4. **Consider adding API authentication** for production
5. **Keep firmware updated** to patch security vulnerabilities

## 📝 License

This project is provided as a template. Feel free to use, modify, and distribute as needed.

## 🤝 Contributing

Contributions are welcome! Please feel free to submit issues and pull requests.

## 📧 Support

For questions and support:
- Check the [documentation](docs/)
- Review serial monitor logs for errors
- Create an issue on GitHub

## 🙏 Acknowledgments

Built with:
- [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)
- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
- [ArduinoJson](https://arduinojson.org/)
- [PlatformIO](https://platformio.org/)

## 🎓 Learning Resources

- [ESP32 Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)
- [PlatformIO Documentation](https://docs.platformio.org/)
- [Arduino Reference](https://www.arduino.cc/reference/en/)

---

Made with ❤️ for the ESP32 community