#include "wifi_manager.h"
#include "config.h"
#include "logger.h"

WiFiManager::WiFiManager() 
    : _lastReconnectAttempt(0), _retryCount(0) {
}

void WiFiManager::begin() {
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(false);  // We'll handle reconnection manually
    Logger::info("WiFi Manager initialized");
}

bool WiFiManager::connect(const char* ssid, const char* password) {
    if (ssid == nullptr || strlen(ssid) == 0) {
        Logger::error("WiFi: SSID cannot be empty");
        return false;
    }
    
    _ssid = String(ssid);
    _password = String(password);
    _retryCount = 0;
    
    Logger::info("Connecting to WiFi: " + _ssid);
    WiFi.disconnect();
    delay(100);
    
    WiFi.begin(ssid, password);
    
    // Wait for connection
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < WIFI_MAX_RETRY) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    Serial.println();
    
    if (WiFi.status() == WL_CONNECTED) {
        logStatus();
        return true;
    } else {
        Logger::error("WiFi connection failed");
        return false;
    }
}

void WiFiManager::handleReconnect() {
    if (WiFi.status() == WL_CONNECTED) {
        _retryCount = 0;
        return;
    }
    
    unsigned long currentMillis = millis();
    
    if (currentMillis - _lastReconnectAttempt >= WIFI_RECONNECT_INTERVAL) {
        _lastReconnectAttempt = currentMillis;
        _retryCount++;
        
        if (_retryCount <= WIFI_MAX_RETRY) {
            Logger::info("Attempting to reconnect to WiFi... (Attempt " + 
                        String(_retryCount) + "/" + String(WIFI_MAX_RETRY) + ")");
            WiFi.disconnect();
            delay(100);
            WiFi.begin(_ssid.c_str(), _password.c_str());
        } else {
            Logger::error("WiFi reconnection failed after maximum retries");
            _retryCount = 0;  // Reset for next cycle
        }
    }
}

bool WiFiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

String WiFiManager::getSSID() {
    return _ssid;
}

String WiFiManager::getIPAddress() {
    if (isConnected()) {
        return WiFi.localIP().toString();
    }
    return "Not connected";
}

void WiFiManager::disconnect() {
    WiFi.disconnect();
    Logger::info("WiFi disconnected");
}

void WiFiManager::setCredentials(const char* ssid, const char* password) {
    _ssid = String(ssid);
    _password = String(password);
}

void WiFiManager::logStatus() {
    Logger::info("WiFi connected!");
    Logger::info("SSID: " + String(WiFi.SSID()));
    Logger::info("IP Address: " + WiFi.localIP().toString());
    Logger::info("Signal Strength (RSSI): " + String(WiFi.RSSI()) + " dBm");
}
