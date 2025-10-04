#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include <Arduino.h>

class WiFiManager {
public:
    WiFiManager();
    
    // Initialize WiFi manager
    void begin();
    
    // Connect to WiFi with stored credentials
    bool connect(const char* ssid, const char* password);
    
    // Handle WiFi reconnection
    void handleReconnect();
    
    // Check if WiFi is connected
    bool isConnected();
    
    // Get current SSID
    String getSSID();
    
    // Get IP address
    String getIPAddress();
    
    // Disconnect from WiFi
    void disconnect();
    
    // Set credentials
    void setCredentials(const char* ssid, const char* password);

private:
    String _ssid;
    String _password;
    unsigned long _lastReconnectAttempt;
    int _retryCount;
    
    void logStatus();
};

#endif // WIFI_MANAGER_H
