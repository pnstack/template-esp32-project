# API Documentation

This document describes the HTTP API endpoints available on the ESP32 web server.

## Base URL

```
http://<device-ip>/
```

Replace `<device-ip>` with your ESP32's IP address (e.g., `192.168.1.100`).

## Endpoints

### 1. Get Device Status

Retrieves current device status including WiFi connection, IP address, and system information.

**Endpoint**: `/api/status`

**Method**: `GET`

**Response**: JSON

**Example Request**:
```bash
curl http://192.168.1.100/api/status
```

**Example Response**:
```json
{
  "device_name": "ESP32-Device",
  "uptime": 123456,
  "wifi_connected": true,
  "ssid": "MyWiFiNetwork",
  "ip_address": "192.168.1.100",
  "signal_strength": -45,
  "free_heap": 245678,
  "chip_model": "ESP32-D0WDQ6",
  "chip_cores": 2,
  "sdk_version": "v4.4.2"
}
```

**Response Fields**:
- `device_name` (string): Device identifier
- `uptime` (number): Milliseconds since boot
- `wifi_connected` (boolean): WiFi connection status
- `ssid` (string): Connected WiFi network name
- `ip_address` (string): Device IP address
- `signal_strength` (number): WiFi signal strength in dBm
- `free_heap` (number): Free heap memory in bytes
- `chip_model` (string): ESP32 chip model
- `chip_cores` (number): Number of CPU cores
- `sdk_version` (string): ESP-IDF SDK version

---

### 2. Get Configuration

Retrieves the current WiFi configuration.

**Endpoint**: `/api/config`

**Method**: `GET`

**Response**: JSON

**Example Request**:
```bash
curl http://192.168.1.100/api/config
```

**Example Response**:
```json
{
  "ssid": "MyWiFiNetwork",
  "password": "********"
}
```

**Note**: Password is returned for configuration purposes but should be handled securely.

---

### 3. Update Configuration

Updates the WiFi configuration and reconnects to the network.

**Endpoint**: `/api/config`

**Method**: `POST`

**Content-Type**: `application/x-www-form-urlencoded`

**Parameters**:
- `ssid` (required): WiFi network name
- `password` (optional): WiFi password

**Example Request**:
```bash
curl -X POST http://192.168.1.100/api/config \
  -d "ssid=MyNewNetwork" \
  -d "password=MySecurePassword123"
```

**Example Response (Success)**:
```json
{
  "success": true,
  "message": "Configuration saved. Device will reconnect."
}
```

**Example Response (Error)**:
```json
{
  "error": "SSID is required"
}
```

**HTTP Status Codes**:
- `200 OK`: Configuration saved successfully
- `400 Bad Request`: Missing or invalid parameters
- `500 Internal Server Error`: Failed to save configuration

---

### 4. Web Interface

Serves the HTML configuration interface.

**Endpoint**: `/`

**Method**: `GET`

**Response**: HTML

**Example**:
```
http://192.168.1.100/
```

Opens the web-based configuration interface in your browser.

---

## HTTP Client Usage

### Sending Sensor Data

The ESP32 can send data to external servers using the HTTP client.

**Example in Code**:
```cpp
#include "http_client.h"

HTTPClientManager httpClient;

void sendData() {
    float temperature = 25.5;
    float humidity = 60.0;
    
    const char* endpoint = "http://your-server.com/api/data";
    bool success = httpClient.sendSensorData(endpoint, temperature, humidity);
    
    if (success) {
        Logger::info("Data sent successfully");
    }
}
```

**Request Format** (sent by ESP32):
```http
POST /api/data HTTP/1.1
Host: your-server.com
Content-Type: application/json

{
  "temperature": 25.5,
  "humidity": 60.0,
  "timestamp": 123456
}
```

### Custom GET Request

```cpp
HTTPClientManager httpClient;
String response;

int httpCode = httpClient.sendGET("http://api.example.com/data", response);

if (httpCode == 200) {
    Logger::info("Response: " + response);
}
```

### Custom POST Request

```cpp
HTTPClientManager httpClient;
String response;

const char* payload = "{\"sensor\":\"temp\",\"value\":25.5}";
int httpCode = httpClient.sendPOST("http://api.example.com/data", payload, response);

if (httpCode == 200) {
    Logger::info("Response: " + response);
}
```

---

## WebSocket Support (Future)

WebSocket support can be added for real-time bidirectional communication.

**Potential Endpoints**:
- `/ws` - WebSocket connection for real-time updates

**Use Cases**:
- Real-time sensor data streaming
- Live configuration updates
- Device control commands

---

## Authentication (Optional Implementation)

For secure deployments, you can add API authentication:

### API Key Authentication

**Example Implementation**:
```cpp
server.on("/api/status", HTTP_GET, [](AsyncWebServerRequest *request){
    if (!request->hasHeader("X-API-Key")) {
        request->send(401, "application/json", "{\"error\":\"Unauthorized\"}");
        return;
    }
    
    String apiKey = request->header("X-API-Key");
    if (apiKey != "your-secret-api-key") {
        request->send(403, "application/json", "{\"error\":\"Forbidden\"}");
        return;
    }
    
    // Process request...
});
```

**Example Request**:
```bash
curl http://192.168.1.100/api/status \
  -H "X-API-Key: your-secret-api-key"
```

---

## Error Handling

### Common HTTP Status Codes

- `200 OK`: Request successful
- `400 Bad Request`: Invalid request parameters
- `401 Unauthorized`: Authentication required
- `403 Forbidden`: Invalid credentials
- `404 Not Found`: Endpoint or resource not found
- `500 Internal Server Error`: Server-side error

### Error Response Format

```json
{
  "error": "Description of the error"
}
```

---

## Rate Limiting (Recommended for Production)

Consider implementing rate limiting to prevent abuse:

```cpp
// Simple rate limiting example
unsigned long lastRequestTime = 0;
const unsigned long REQUEST_INTERVAL = 1000; // 1 request per second

server.on("/api/status", HTTP_GET, [](AsyncWebServerRequest *request){
    unsigned long currentTime = millis();
    
    if (currentTime - lastRequestTime < REQUEST_INTERVAL) {
        request->send(429, "application/json", 
                     "{\"error\":\"Too many requests\"}");
        return;
    }
    
    lastRequestTime = currentTime;
    // Process request...
});
```

---

## CORS Support (Optional)

For web applications hosted on different domains:

```cpp
server.on("/api/status", HTTP_OPTIONS, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse(200);
    response->addHeader("Access-Control-Allow-Origin", "*");
    response->addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    response->addHeader("Access-Control-Allow-Headers", "Content-Type");
    request->send(response);
});
```

---

## Testing the API

### Using cURL

```bash
# Get status
curl http://192.168.1.100/api/status

# Update configuration
curl -X POST http://192.168.1.100/api/config \
  -d "ssid=TestNetwork" \
  -d "password=TestPassword"

# Get configuration
curl http://192.168.1.100/api/config
```

### Using Python

```python
import requests
import json

# Device IP
base_url = "http://192.168.1.100"

# Get status
response = requests.get(f"{base_url}/api/status")
print(response.json())

# Update configuration
config_data = {
    "ssid": "NewNetwork",
    "password": "NewPassword"
}
response = requests.post(f"{base_url}/api/config", data=config_data)
print(response.json())
```

### Using JavaScript (Browser)

```javascript
// Get status
fetch('http://192.168.1.100/api/status')
  .then(response => response.json())
  .then(data => console.log(data))
  .catch(error => console.error('Error:', error));

// Update configuration
const formData = new FormData();
formData.append('ssid', 'NewNetwork');
formData.append('password', 'NewPassword');

fetch('http://192.168.1.100/api/config', {
  method: 'POST',
  body: formData
})
  .then(response => response.json())
  .then(data => console.log(data))
  .catch(error => console.error('Error:', error));
```

---

## Extending the API

### Adding New Endpoints

1. **Define the route in `web_server.cpp`**:

```cpp
void WebServerManager::setupRoutes() {
    // Existing routes...
    
    // New custom endpoint
    _server->on("/api/custom", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(200, "application/json", "{\"message\":\"Custom endpoint\"}");
    });
}
```

2. **Handle parameters**:

```cpp
_server->on("/api/led", HTTP_POST, [](AsyncWebServerRequest* request) {
    if (request->hasParam("state", true)) {
        String state = request->getParam("state", true)->value();
        
        if (state == "on") {
            digitalWrite(LED_PIN, HIGH);
            request->send(200, "application/json", "{\"led\":\"on\"}");
        } else if (state == "off") {
            digitalWrite(LED_PIN, LOW);
            request->send(200, "application/json", "{\"led\":\"off\"}");
        } else {
            request->send(400, "application/json", "{\"error\":\"Invalid state\"}");
        }
    } else {
        request->send(400, "application/json", "{\"error\":\"State parameter required\"}");
    }
});
```

3. **Handle JSON requests**:

```cpp
_server->on("/api/sensor", HTTP_POST, 
    [](AsyncWebServerRequest* request){}, 
    NULL,
    [](AsyncWebServerRequest* request, uint8_t *data, size_t len, 
       size_t index, size_t total) {
        
        StaticJsonDocument<200> doc;
        DeserializationError error = deserializeJson(doc, (const char*)data);
        
        if (error) {
            request->send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
            return;
        }
        
        float value = doc["value"];
        // Process value...
        
        request->send(200, "application/json", "{\"success\":true}");
    }
);
```

---

## Security Considerations

1. **Use HTTPS** in production (requires additional setup)
2. **Implement authentication** for sensitive endpoints
3. **Validate all input** parameters
4. **Sanitize user input** to prevent injection attacks
5. **Use strong passwords** for WiFi and OTA
6. **Limit API access** to trusted networks when possible
7. **Monitor for suspicious activity** in logs
8. **Keep firmware updated** to patch security vulnerabilities

---

## Support

For questions or issues with the API:
- Check the examples in the code
- Review the serial monitor logs
- Create an issue on GitHub
- Consult ESP32 AsyncWebServer documentation
