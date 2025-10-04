#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

class WebServerManager {
public:
    WebServerManager();
    
    // Initialize web server
    void begin();
    
    // Handle web server
    void handle();
    
    // Set callbacks
    void onConfigUpdate(std::function<void(const char*, const char*)> callback);
    void onGetStatus(std::function<String()> callback);

private:
    AsyncWebServer* _server;
    std::function<void(const char*, const char*)> _configUpdateCallback;
    std::function<String()> _statusCallback;
    
    void setupRoutes();
    void handleRoot(AsyncWebServerRequest* request);
    void handleConfig(AsyncWebServerRequest* request);
    void handleStatus(AsyncWebServerRequest* request);
    void handleSaveConfig(AsyncWebServerRequest* request);
    void handleNotFound(AsyncWebServerRequest* request);
};

#endif // WEB_SERVER_H
