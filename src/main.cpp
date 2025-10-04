#include <Arduino.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include "config.h"
#include "logger.h"
#include "wifi_manager.h"
#include "web_server.h"
#include "ota_manager.h"
#include "http_client.h"

// Global objects
WiFiManager wifiManager;
WebServerManager webServer;
OTAManager otaManager;
HTTPClientManager httpClient;

// Application state
bool isConfigured = false;
unsigned long lastDataSend = 0;
const unsigned long DATA_SEND_INTERVAL = 60000; // Send data every 60 seconds

// Function prototypes
void loadConfiguration();
void saveConfiguration(const char* ssid, const char* password);
String getStatusJSON();
void sendExampleData();

void setup() {
    // Initialize logger
    Logger::begin(SERIAL_BAUD_RATE);
    Logger::setLogLevel(LOG_INFO);
    
    Logger::info("===========================================");
    Logger::info("ESP32 Template Project");
    Logger::info("===========================================");
    Logger::info("Starting system initialization...");
    
    // Initialize WiFi Manager
    wifiManager.begin();
    
    // Load configuration from SPIFFS
    loadConfiguration();
    
    // Initialize Web Server
    webServer.begin();
    
    // Setup web server callbacks
    webServer.onConfigUpdate([](const char* ssid, const char* password) {
        Logger::info("Configuration updated via web interface");
        saveConfiguration(ssid, password);
        
        // Reconnect with new credentials
        delay(1000);
        wifiManager.connect(ssid, password);
    });
    
    webServer.onGetStatus([]() {
        return getStatusJSON();
    });
    
    // Initialize OTA (only if WiFi is connected)
    if (wifiManager.isConnected()) {
        otaManager.begin(OTA_HOSTNAME, OTA_PASSWORD);
    }
    
    Logger::info("System initialization completed!");
    Logger::info("===========================================\n");
    
    if (!isConfigured) {
        Logger::warn("WiFi not configured. Please connect to the device and configure WiFi.");
        Logger::info("Access the web interface at: http://" + WiFi.softAPIP().toString());
    }
}

void loop() {
    // Handle WiFi reconnection
    if (isConfigured) {
        wifiManager.handleReconnect();
    }
    
    // Handle OTA updates (only if WiFi is connected)
    if (wifiManager.isConnected()) {
        otaManager.handle();
    }
    
    // Handle web server
    webServer.handle();
    
    // Send example data periodically (only if WiFi is connected and not updating)
    if (wifiManager.isConnected() && !otaManager.isUpdating()) {
        unsigned long currentMillis = millis();
        if (currentMillis - lastDataSend >= DATA_SEND_INTERVAL) {
            lastDataSend = currentMillis;
            sendExampleData();
        }
    }
    
    // Small delay to prevent watchdog issues
    delay(10);
}

void loadConfiguration() {
    // Initialize SPIFFS
    if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
        Logger::error("Failed to mount SPIFFS");
        return;
    }
    
    // Try to load configuration
    File file = SPIFFS.open(CONFIG_FILE, "r");
    if (!file) {
        Logger::warn("Configuration file not found. Using default AP mode.");
        
        // Create default config file
        StaticJsonDocument<256> doc;
        doc["ssid"] = "";
        doc["password"] = "";
        
        File newFile = SPIFFS.open(CONFIG_FILE, "w");
        if (newFile) {
            serializeJson(doc, newFile);
            newFile.close();
        }
        
        isConfigured = false;
        return;
    }
    
    // Parse configuration
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();
    
    if (error) {
        Logger::error("Failed to parse configuration file");
        isConfigured = false;
        return;
    }
    
    const char* ssid = doc["ssid"];
    const char* password = doc["password"];
    
    if (ssid == nullptr || strlen(ssid) == 0) {
        Logger::warn("No WiFi credentials found in configuration");
        isConfigured = false;
        return;
    }
    
    Logger::info("Configuration loaded successfully");
    
    // Connect to WiFi
    if (wifiManager.connect(ssid, password)) {
        isConfigured = true;
    } else {
        Logger::warn("Failed to connect with stored credentials");
        isConfigured = false;
    }
}

void saveConfiguration(const char* ssid, const char* password) {
    StaticJsonDocument<256> doc;
    doc["ssid"] = ssid;
    doc["password"] = password;
    
    File file = SPIFFS.open(CONFIG_FILE, "w");
    if (!file) {
        Logger::error("Failed to save configuration");
        return;
    }
    
    serializeJson(doc, file);
    file.close();
    
    Logger::info("Configuration saved to SPIFFS");
    isConfigured = true;
}

String getStatusJSON() {
    StaticJsonDocument<512> doc;
    
    doc["device_name"] = DEFAULT_DEVICE_NAME;
    doc["uptime"] = millis();
    doc["wifi_connected"] = wifiManager.isConnected();
    
    if (wifiManager.isConnected()) {
        doc["ssid"] = wifiManager.getSSID();
        doc["ip_address"] = wifiManager.getIPAddress();
        doc["signal_strength"] = WiFi.RSSI();
    } else {
        doc["ssid"] = "Not connected";
        doc["ip_address"] = "N/A";
        doc["signal_strength"] = 0;
    }
    
    doc["free_heap"] = ESP.getFreeHeap();
    doc["chip_model"] = ESP.getChipModel();
    doc["chip_cores"] = ESP.getChipCores();
    doc["sdk_version"] = ESP.getSdkVersion();
    
    String output;
    serializeJson(doc, output);
    
    return output;
}

void sendExampleData() {
    // Example: Send simulated sensor data
    float temperature = 22.5 + (random(-50, 50) / 10.0); // Simulated temperature
    float humidity = 55.0 + (random(-100, 100) / 10.0);   // Simulated humidity
    
    Logger::debug("Preparing to send sensor data...");
    
    // Replace this URL with your actual endpoint
    const char* endpoint = "http://your-server.com/api/data";
    
    // Uncomment to actually send data
    // bool success = httpClient.sendSensorData(endpoint, temperature, humidity);
    // if (success) {
    //     Logger::info("Data sent successfully");
    // } else {
    //     Logger::warn("Failed to send data");
    // }
    
    Logger::debug("Temperature: " + String(temperature) + "°C, Humidity: " + String(humidity) + "%");
}
