# Usage Examples

This document provides practical examples for common use cases with the ESP32 template project.

## Table of Contents

1. [Basic Usage](#basic-usage)
2. [WiFi Management](#wifi-management)
3. [HTTP Client](#http-client)
4. [Web Server Customization](#web-server-customization)
5. [Logging](#logging)
6. [OTA Updates](#ota-updates)
7. [Sensor Integration](#sensor-integration)
8. [Advanced Examples](#advanced-examples)

## Basic Usage

### Minimal Setup

```cpp
#include <Arduino.h>
#include "logger.h"
#include "wifi_manager.h"

WiFiManager wifiManager;

void setup() {
    Logger::begin(115200);
    Logger::info("Starting device...");
    
    wifiManager.begin();
    wifiManager.connect("YourSSID", "YourPassword");
}

void loop() {
    wifiManager.handleReconnect();
    delay(10);
}
```

## WiFi Management

### Connect to WiFi with Credentials

```cpp
#include "wifi_manager.h"

WiFiManager wifiManager;

void setup() {
    wifiManager.begin();
    
    if (wifiManager.connect("MyNetwork", "MyPassword")) {
        Logger::info("Connected successfully!");
        Logger::info("IP: " + wifiManager.getIPAddress());
    } else {
        Logger::error("Connection failed");
    }
}
```

### Check WiFi Status

```cpp
void loop() {
    if (wifiManager.isConnected()) {
        Logger::info("WiFi is connected");
        Logger::info("Signal: " + String(WiFi.RSSI()) + " dBm");
    } else {
        Logger::warn("WiFi disconnected, attempting reconnect...");
        wifiManager.handleReconnect();
    }
    
    delay(5000);
}
```

### Manually Trigger Reconnection

```cpp
void reconnectWiFi() {
    Logger::info("Manually reconnecting WiFi...");
    wifiManager.disconnect();
    delay(1000);
    wifiManager.connect("NewSSID", "NewPassword");
}
```

## HTTP Client

### Send GET Request

```cpp
#include "http_client.h"

HTTPClientManager httpClient;

void fetchData() {
    String response;
    int httpCode = httpClient.sendGET("http://api.example.com/data", response);
    
    if (httpCode == 200) {
        Logger::info("Response: " + response);
    } else {
        Logger::error("GET failed with code: " + String(httpCode));
    }
}
```

### Send POST Request with JSON

```cpp
#include "http_client.h"
#include <ArduinoJson.h>

HTTPClientManager httpClient;

void sendData() {
    StaticJsonDocument<200> doc;
    doc["sensor"] = "temperature";
    doc["value"] = 25.5;
    doc["unit"] = "celsius";
    
    String jsonString;
    serializeJson(doc, jsonString);
    
    String response;
    int httpCode = httpClient.sendPOST(
        "http://api.example.com/sensor",
        jsonString.c_str(),
        response
    );
    
    if (httpCode == 200 || httpCode == 201) {
        Logger::info("Data sent successfully");
    }
}
```

### Send Sensor Data (Built-in Helper)

```cpp
void sendSensorReading() {
    float temperature = 22.5;
    float humidity = 55.0;
    
    bool success = httpClient.sendSensorData(
        "http://your-server.com/api/sensors",
        temperature,
        humidity
    );
    
    if (success) {
        Logger::info("Sensor data sent");
    }
}
```

### Periodic Data Sending

```cpp
unsigned long lastSend = 0;
const unsigned long SEND_INTERVAL = 60000; // 60 seconds

void loop() {
    unsigned long now = millis();
    
    if (now - lastSend >= SEND_INTERVAL) {
        lastSend = now;
        
        if (wifiManager.isConnected()) {
            float temp = readTemperature();
            float humidity = readHumidity();
            httpClient.sendSensorData("http://api.example.com/data", temp, humidity);
        }
    }
}
```

## Web Server Customization

### Add Custom API Endpoint

Edit `src/web_server.cpp`:

```cpp
void WebServerManager::setupRoutes() {
    // Existing routes...
    
    // Custom endpoint
    _server->on("/api/temperature", HTTP_GET, [](AsyncWebServerRequest* request) {
        float temp = readTemperature();
        
        StaticJsonDocument<100> doc;
        doc["temperature"] = temp;
        doc["unit"] = "celsius";
        
        String response;
        serializeJson(doc, response);
        
        request->send(200, "application/json", response);
    });
}
```

### Handle POST with Parameters

```cpp
_server->on("/api/led", HTTP_POST, [](AsyncWebServerRequest* request) {
    if (request->hasParam("state", true)) {
        String state = request->getParam("state", true)->value();
        
        if (state == "on") {
            digitalWrite(LED_PIN, HIGH);
            request->send(200, "application/json", "{\"status\":\"LED ON\"}");
        } else if (state == "off") {
            digitalWrite(LED_PIN, LOW);
            request->send(200, "application/json", "{\"status\":\"LED OFF\"}");
        }
    } else {
        request->send(400, "application/json", "{\"error\":\"Missing parameter\"}");
    }
});
```

### Serve Static Files

```cpp
// Serve additional static files from SPIFFS
_server->serveStatic("/images", SPIFFS, "/images/");
_server->serveStatic("/css", SPIFFS, "/css/");
```

## Logging

### Basic Logging

```cpp
#include "logger.h"

void example() {
    Logger::error("This is an error message");
    Logger::warn("This is a warning");
    Logger::info("This is an info message");
    Logger::debug("This is a debug message");
}
```

### Log Variables

```cpp
void logSensorData() {
    float temperature = 25.5;
    int sensorId = 1;
    
    Logger::info("Sensor " + String(sensorId) + 
                 " temperature: " + String(temperature) + "°C");
}
```

### Change Log Level

```cpp
void setup() {
    Logger::begin(115200);
    
    // Only show errors and warnings
    Logger::setLogLevel(LOG_WARN);
    
    // Show all messages including debug
    Logger::setLogLevel(LOG_DEBUG);
}
```

### Conditional Logging

```cpp
void processData(float value) {
    if (value > 100) {
        Logger::error("Value exceeds threshold: " + String(value));
    } else if (value > 80) {
        Logger::warn("Value approaching threshold: " + String(value));
    } else {
        Logger::debug("Value normal: " + String(value));
    }
}
```

## OTA Updates

### Enable OTA with Custom Settings

```cpp
#include "ota_manager.h"

OTAManager otaManager;

void setup() {
    // Connect to WiFi first
    wifiManager.connect("SSID", "Password");
    
    // Initialize OTA with custom hostname and password
    otaManager.begin("my-esp32-device", "secure-password");
}

void loop() {
    otaManager.handle();
}
```

### Check OTA Status

```cpp
void loop() {
    if (otaManager.isUpdating()) {
        Logger::info("OTA update in progress...");
        // Skip other operations during update
        return;
    }
    
    // Normal operations
    otaManager.handle();
    // ... other code
}
```

## Sensor Integration

### DHT Temperature & Humidity Sensor

First, add DHT library to `platformio.ini`:
```ini
lib_deps = 
    ...
    DHT sensor library
```

Then in your code:

```cpp
#include <DHT.h>
#include "http_client.h"

#define DHT_PIN 4
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);
HTTPClientManager httpClient;

void setup() {
    dht.begin();
}

void readAndSendSensorData() {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    
    if (isnan(temperature) || isnan(humidity)) {
        Logger::error("Failed to read from DHT sensor");
        return;
    }
    
    Logger::info("Temp: " + String(temperature) + "°C, Humidity: " + String(humidity) + "%");
    
    httpClient.sendSensorData(
        "http://your-server.com/api/data",
        temperature,
        humidity
    );
}
```

### BMP280 Pressure Sensor

Add to `platformio.ini`:
```ini
lib_deps = 
    ...
    Adafruit BMP280 Library
```

Code:

```cpp
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp;

void setup() {
    if (!bmp.begin(0x76)) {
        Logger::error("BMP280 not found");
        return;
    }
    Logger::info("BMP280 initialized");
}

void readPressure() {
    float pressure = bmp.readPressure() / 100.0F; // hPa
    float altitude = bmp.readAltitude(1013.25);   // meters
    
    Logger::info("Pressure: " + String(pressure) + " hPa");
    Logger::info("Altitude: " + String(altitude) + " m");
}
```

## Advanced Examples

### Non-Blocking Task Scheduler

```cpp
struct Task {
    unsigned long lastRun;
    unsigned long interval;
    void (*function)();
};

Task tasks[] = {
    {0, 1000, readSensor},      // Every 1 second
    {0, 60000, sendData},       // Every 60 seconds
    {0, 300000, checkUpdates},  // Every 5 minutes
};

void loop() {
    unsigned long now = millis();
    
    for (int i = 0; i < sizeof(tasks) / sizeof(Task); i++) {
        if (now - tasks[i].lastRun >= tasks[i].interval) {
            tasks[i].lastRun = now;
            tasks[i].function();
        }
    }
    
    wifiManager.handleReconnect();
    otaManager.handle();
    delay(10);
}
```

### Configuration from SPIFFS

```cpp
#include <ArduinoJson.h>

void loadCustomConfig() {
    File file = SPIFFS.open("/custom_config.json", "r");
    if (!file) {
        Logger::warn("Custom config not found");
        return;
    }
    
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();
    
    if (error) {
        Logger::error("Failed to parse config");
        return;
    }
    
    // Read configuration values
    int sensorInterval = doc["sensor_interval"] | 60;
    String apiEndpoint = doc["api_endpoint"] | "http://default.com/api";
    bool enableOTA = doc["enable_ota"] | true;
    
    Logger::info("Config loaded successfully");
}
```

### LED Status Indicator

```cpp
#define LED_PIN 2

enum LEDStatus {
    LED_CONNECTING,
    LED_CONNECTED,
    LED_ERROR,
    LED_UPDATING
};

void setLEDStatus(LEDStatus status) {
    static unsigned long lastBlink = 0;
    static bool ledState = false;
    unsigned long now = millis();
    
    switch (status) {
        case LED_CONNECTING:
            // Fast blink
            if (now - lastBlink >= 200) {
                ledState = !ledState;
                digitalWrite(LED_PIN, ledState);
                lastBlink = now;
            }
            break;
            
        case LED_CONNECTED:
            // Solid on
            digitalWrite(LED_PIN, HIGH);
            break;
            
        case LED_ERROR:
            // Slow blink
            if (now - lastBlink >= 1000) {
                ledState = !ledState;
                digitalWrite(LED_PIN, ledState);
                lastBlink = now;
            }
            break;
            
        case LED_UPDATING:
            // Very fast blink
            if (now - lastBlink >= 100) {
                ledState = !ledState;
                digitalWrite(LED_PIN, ledState);
                lastBlink = now;
            }
            break;
    }
}

void loop() {
    if (otaManager.isUpdating()) {
        setLEDStatus(LED_UPDATING);
    } else if (wifiManager.isConnected()) {
        setLEDStatus(LED_CONNECTED);
    } else {
        setLEDStatus(LED_CONNECTING);
    }
}
```

### Deep Sleep Mode

```cpp
#define SLEEP_DURATION 60e6  // 60 seconds in microseconds

void enterDeepSleep() {
    Logger::info("Entering deep sleep for " + String(SLEEP_DURATION / 1e6) + " seconds");
    
    // Cleanup before sleep
    wifiManager.disconnect();
    delay(100);
    
    // Configure wake-up
    esp_sleep_enable_timer_wakeup(SLEEP_DURATION);
    
    // Enter deep sleep
    esp_deep_sleep_start();
}

void loop() {
    // Do work
    readSensorData();
    sendData();
    
    // Sleep until next reading
    enterDeepSleep();
}
```

### Button Input with Debounce

```cpp
#define BUTTON_PIN 0
#define DEBOUNCE_DELAY 50

bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;

void checkButton() {
    bool reading = digitalRead(BUTTON_PIN);
    
    if (reading != lastButtonState) {
        lastDebounceTime = millis();
    }
    
    if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
        if (reading == LOW) {  // Button pressed
            Logger::info("Button pressed!");
            // Trigger action
            reconnectWiFi();
        }
    }
    
    lastButtonState = reading;
}

void loop() {
    checkButton();
    delay(10);
}
```

## Testing Your Code

### Test WiFi Connection

```cpp
void testWiFi() {
    Logger::info("Testing WiFi connection...");
    
    if (wifiManager.connect("TestNetwork", "TestPassword")) {
        Logger::info("✓ WiFi connection successful");
        Logger::info("✓ IP: " + wifiManager.getIPAddress());
        Logger::info("✓ Signal: " + String(WiFi.RSSI()) + " dBm");
    } else {
        Logger::error("✗ WiFi connection failed");
    }
}
```

### Test HTTP Client

```cpp
void testHTTPClient() {
    Logger::info("Testing HTTP client...");
    
    String response;
    int code = httpClient.sendGET("http://httpbin.org/get", response);
    
    if (code == 200) {
        Logger::info("✓ HTTP GET successful");
    } else {
        Logger::error("✗ HTTP GET failed: " + String(code));
    }
}
```

### Test Web Server

```cpp
void testWebServer() {
    Logger::info("Testing web server...");
    Logger::info("Open browser and navigate to: http://" + wifiManager.getIPAddress());
    Logger::info("You should see the configuration page");
}
```

## Support

For more examples or questions:
- Check the main documentation
- Review the source code
- Create an issue on GitHub
