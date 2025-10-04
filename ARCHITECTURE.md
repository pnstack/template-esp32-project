# System Architecture

This document describes the architecture and component interactions of the ESP32 Template Project.

## High-Level Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                         ESP32 Device                         │
│                                                              │
│  ┌──────────────┐  ┌──────────────┐  ┌─────────────────┐  │
│  │   Logger     │  │ WiFi Manager │  │  OTA Manager    │  │
│  │              │  │              │  │                 │  │
│  │ - Serial log │  │ - Connect    │  │ - OTA updates   │  │
│  │ - Log levels │  │ - Reconnect  │  │ - Auth check    │  │
│  │ - Timestamps │  │ - Status     │  │ - Progress      │  │
│  └──────┬───────┘  └──────┬───────┘  └────────┬────────┘  │
│         │                  │                    │           │
│         └──────────────────┴────────────────────┘           │
│                            │                                │
│  ┌─────────────────────────┴─────────────────────────────┐ │
│  │                    main.cpp                            │ │
│  │  - Application lifecycle (setup/loop)                  │ │
│  │  - Component coordination                              │ │
│  │  - Configuration management                            │ │
│  └────┬───────────────────────────────────────────────┬──┘ │
│       │                                                │    │
│  ┌────┴────────────┐                        ┌─────────┴───┐│
│  │  Web Server     │                        │ HTTP Client ││
│  │                 │                        │             ││
│  │ - Serve UI      │                        │ - GET/POST  ││
│  │ - Handle config │                        │ - Send data ││
│  │ - API endpoints │                        │ - JSON      ││
│  └────┬────────────┘                        └─────────┬───┘│
│       │                                                │    │
│  ┌────┴────────────┐                                  │    │
│  │     SPIFFS      │                                  │    │
│  │                 │                                  │    │
│  │ - index.html    │                                  │    │
│  │ - config.json   │                                  │    │
│  └─────────────────┘                                  │    │
└────────────────────────────────────────────────────────────┘
         │                                              │
         │                                              │
         ▼                                              ▼
  ┌─────────────┐                              ┌──────────────┐
  │  Web Browser│                              │External APIs │
  │             │                              │              │
  │ - Config UI │                              │- Data server │
  │ - Status    │                              │- Cloud APIs  │
  └─────────────┘                              └──────────────┘
```

## Component Interaction Flow

### 1. Startup Sequence

```
Power On
   │
   ├─> Initialize Logger
   │      └─> Open serial communication (115200 baud)
   │
   ├─> Initialize WiFi Manager
   │      └─> Set WiFi mode to Station (STA)
   │
   ├─> Load Configuration from SPIFFS
   │      ├─> Read config.json
   │      ├─> Extract WiFi credentials
   │      └─> Attempt connection
   │
   ├─> Initialize Web Server
   │      ├─> Mount SPIFFS
   │      ├─> Setup routes
   │      │     ├─> / (index.html)
   │      │     ├─> /api/status
   │      │     ├─> /api/config
   │      │     └─> POST /api/config
   │      └─> Start server on port 80
   │
   ├─> Initialize OTA Manager (if WiFi connected)
   │      ├─> Set hostname
   │      ├─> Set password
   │      └─> Setup callbacks
   │
   └─> Enter main loop
```

### 2. Main Loop Execution

```
loop()
   │
   ├─> Check WiFi Status
   │      ├─> If disconnected: handleReconnect()
   │      └─> If connected: Continue
   │
   ├─> Handle OTA Updates
   │      ├─> Listen for OTA requests
   │      └─> Process if OTA in progress
   │
   ├─> Handle Web Server Requests
   │      └─> AsyncWebServer handles automatically
   │
   ├─> Periodic Tasks (if WiFi connected)
   │      ├─> Send sensor data
   │      ├─> Update status
   │      └─> Other scheduled operations
   │
   └─> Small delay (10ms) for stability
```

### 3. WiFi Configuration Flow

```
User Opens Web Browser
   │
   ├─> Navigate to http://<device-ip>/
   │      └─> Web Server serves index.html from SPIFFS
   │
   ├─> Page Loads
   │      ├─> Fetch /api/status
   │      └─> Display current WiFi status
   │
   ├─> User Enters Credentials
   │      ├─> SSID: "MyNetwork"
   │      └─> Password: "MyPassword"
   │
   ├─> Submit Configuration
   │      └─> POST /api/config with form data
   │
   ├─> Web Server Processes Request
   │      ├─> Validate parameters
   │      ├─> Save to config.json in SPIFFS
   │      └─> Trigger WiFi reconnection callback
   │
   ├─> WiFi Manager Reconnects
   │      ├─> Disconnect current connection
   │      ├─> Connect with new credentials
   │      └─> Report status via Logger
   │
   └─> User Sees Success Message
          └─> Device now connected to WiFi
```

### 4. OTA Update Flow

```
Developer Initiates OTA Upload
   │
   ├─> PlatformIO sends firmware to ESP32
   │      └─> Uses port 3232 (OTA default)
   │
   ├─> OTA Manager Receives Request
   │      ├─> Verify password
   │      └─> Begin update process
   │
   ├─> Update Progress
   │      ├─> onStart: Log "OTA Update Started"
   │      ├─> onProgress: Log progress % (every 10%)
   │      └─> Set _updating flag
   │
   ├─> Main Loop Behavior During Update
   │      ├─> Skip normal operations
   │      └─> Only handle OTA
   │
   ├─> Update Completion
   │      ├─> onEnd: Log "OTA Update Completed"
   │      ├─> Clear _updating flag
   │      └─> Restart ESP32
   │
   └─> Device Reboots with New Firmware
```

### 5. HTTP Data Transmission Flow

```
Sensor Reading Triggered
   │
   ├─> Read Sensor Values
   │      ├─> Temperature: 25.5°C
   │      └─> Humidity: 60%
   │
   ├─> Create JSON Payload
   │      └─> {"temperature": 25.5, "humidity": 60.0, "timestamp": 123456}
   │
   ├─> HTTP Client Sends POST
   │      ├─> Set Content-Type: application/json
   │      ├─> Connect to server
   │      └─> Send payload
   │
   ├─> Wait for Response
   │      ├─> Timeout: 5 seconds
   │      └─> Receive HTTP status code
   │
   ├─> Process Response
   │      ├─> Success (200-299): Log success
   │      └─> Error (400+): Log error
   │
   └─> Close Connection
```

## Data Flow Diagrams

### Configuration Storage (SPIFFS)

```
┌──────────────────┐
│   config.json    │
│                  │
│ {                │
│   "ssid": "...", │
│   "password": ".."│
│ }                │
└────────┬─────────┘
         │
         │ Read on startup
         │ Write on config change
         │
    ┌────┴────┐
    │ SPIFFS  │
    │ 1.5 MB  │
    └─────────┘
```

### Memory Layout

```
┌─────────────────────────────────┐
│        Flash Memory (4MB)        │
├──────────────────┬──────────────┤
│   Firmware       │   SPIFFS     │
│   ~1.3 MB        │   1.5 MB     │
│                  │              │
│ - Code           │ - index.html │
│ - Libraries      │ - config.json│
│ - Constants      │ - Assets     │
└──────────────────┴──────────────┘

┌─────────────────────────────────┐
│        RAM (520 KB)              │
├──────────────────────────────────┤
│ Heap: ~100 KB used               │
│ Stack: ~20 KB                    │
│ Free: ~400 KB                    │
└──────────────────────────────────┘
```

## Module Responsibilities

### Logger (`logger.cpp/h`)
**Purpose**: Centralized logging system

**Responsibilities**:
- Print messages to serial console
- Add timestamps
- Filter by log level (ERROR, WARN, INFO, DEBUG)
- Format messages consistently

**Dependencies**: None

### WiFi Manager (`wifi_manager.cpp/h`)
**Purpose**: Handle WiFi connectivity

**Responsibilities**:
- Connect to WiFi network
- Monitor connection status
- Auto-reconnect on disconnection
- Report signal strength and IP

**Dependencies**: Logger

### Web Server (`web_server.cpp/h`)
**Purpose**: Serve web interface and API

**Responsibilities**:
- Serve HTML files from SPIFFS
- Handle HTTP requests
- Process API calls
- Manage configuration updates

**Dependencies**: Logger, SPIFFS, AsyncWebServer

### OTA Manager (`ota_manager.cpp/h`)
**Purpose**: Enable firmware updates over WiFi

**Responsibilities**:
- Listen for OTA requests
- Authenticate updates
- Monitor upload progress
- Handle update completion/errors

**Dependencies**: Logger, WiFi Manager

### HTTP Client (`http_client.cpp/h`)
**Purpose**: Communicate with external servers

**Responsibilities**:
- Send GET/POST requests
- Handle JSON payloads
- Process responses
- Manage timeouts

**Dependencies**: Logger, WiFi Manager

### Main Application (`main.cpp`)
**Purpose**: Coordinate all components

**Responsibilities**:
- Initialize all modules
- Manage application lifecycle
- Handle configuration
- Coordinate component interactions

**Dependencies**: All other modules

## Communication Protocols

### Serial Communication
- **Baud Rate**: 115200
- **Purpose**: Logging and debugging
- **Format**: `[timestamp] [LEVEL] message`

### HTTP/HTTPS
- **Port**: 80 (HTTP)
- **Purpose**: Web interface and API
- **Methods**: GET, POST
- **Format**: JSON for API responses

### WiFi (802.11 b/g/n)
- **Frequency**: 2.4 GHz only
- **Mode**: Station (STA)
- **Security**: WPA/WPA2

### OTA Protocol
- **Port**: 3232 (default)
- **Purpose**: Firmware updates
- **Security**: Password authentication
- **Protocol**: espota

## Error Handling Strategy

### WiFi Errors
```
Connection Failed
   │
   ├─> Log error message
   ├─> Wait WIFI_RECONNECT_INTERVAL (5s)
   ├─> Retry (up to WIFI_MAX_RETRY times)
   │
   └─> If all retries fail:
          └─> Continue operation in disconnected mode
              (Web server still accessible if in AP mode)
```

### HTTP Errors
```
Request Failed
   │
   ├─> Log HTTP error code
   ├─> Return error to caller
   │
   └─> Caller decides:
          ├─> Retry later
          └─> Report to user
```

### OTA Errors
```
Update Failed
   │
   ├─> Log specific error:
   │      ├─> Auth Failed
   │      ├─> Begin Failed
   │      ├─> Receive Failed
   │      └─> End Failed
   │
   ├─> Abort update
   └─> Continue normal operation
          (Device not bricked, old firmware intact)
```

## Performance Characteristics

### Typical Operation
- **Boot Time**: ~2 seconds
- **WiFi Connection**: 3-5 seconds
- **Web Page Load**: <1 second
- **API Response Time**: <100ms
- **OTA Update Time**: ~30 seconds for 300KB firmware

### Resource Usage
- **CPU**: ~10% in idle (WiFi connected)
- **RAM**: ~100KB used, ~400KB free
- **Flash**: ~300KB firmware, ~11KB SPIFFS

### Network
- **WiFi Reconnect Interval**: 5 seconds
- **HTTP Timeout**: 5 seconds
- **Max Concurrent Connections**: 4 (web server)

## Scalability Considerations

### Adding New Features
1. Create new module (`.cpp/.h` files)
2. Initialize in `setup()`
3. Handle in `loop()` if needed
4. Add logging as appropriate

### Adding New API Endpoints
1. Edit `web_server.cpp`
2. Add route in `setupRoutes()`
3. Implement handler function
4. Document in `docs/API.md`

### Adding New Sensors
1. Add library to `platformio.ini`
2. Initialize in `setup()`
3. Read in `loop()` or periodic task
4. Send data via HTTP client

## Security Considerations

### Implemented
- ✅ OTA password authentication
- ✅ WiFi WPA/WPA2 encryption
- ✅ Credentials stored in SPIFFS (not in code)
- ✅ Credentials file in .gitignore

### Recommended Additions
- 🔒 HTTPS for web server (requires certificates)
- 🔒 API key authentication
- 🔒 Rate limiting on API endpoints
- 🔒 Input validation and sanitization
- 🔒 CORS configuration for web requests

## Future Enhancements

Potential additions to consider:
- WebSocket support for real-time data
- MQTT for IoT messaging
- mDNS for easier device discovery
- Deep sleep for battery operation
- NTP time synchronization
- Multiple WiFi credential storage
- Captive portal for initial setup
- Configuration backup/restore

## Debugging Guide

### Enable Debug Logs
```cpp
Logger::setLogLevel(LOG_DEBUG);
```

### Monitor Memory
```cpp
Logger::info("Free heap: " + String(ESP.getFreeHeap()));
```

### Check WiFi Signal
```cpp
Logger::info("RSSI: " + String(WiFi.RSSI()) + " dBm");
```

### Test API Endpoints
```bash
curl http://192.168.1.100/api/status
```

### Monitor Serial Output
```bash
pio device monitor --baud 115200
```

## References

- [ESP32 Technical Reference](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf)
- [Arduino ESP32 Core](https://github.com/espressif/arduino-esp32)
- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
- [ArduinoJson](https://arduinojson.org/)
- [PlatformIO Docs](https://docs.platformio.org/)
