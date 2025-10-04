#include "web_server.h"
#include "config.h"
#include "logger.h"

WebServerManager::WebServerManager() {
    _server = new AsyncWebServer(WEBSERVER_PORT);
}

void WebServerManager::begin() {
    // Initialize SPIFFS
    if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
        Logger::error("SPIFFS Mount Failed");
        return;
    }
    
    Logger::info("SPIFFS mounted successfully");
    
    // Setup routes
    setupRoutes();
    
    // Start server
    _server->begin();
    Logger::info("Web Server started on port " + String(WEBSERVER_PORT));
}

void WebServerManager::handle() {
    // AsyncWebServer handles requests automatically
}

void WebServerManager::onConfigUpdate(std::function<void(const char*, const char*)> callback) {
    _configUpdateCallback = callback;
}

void WebServerManager::onGetStatus(std::function<String()> callback) {
    _statusCallback = callback;
}

void WebServerManager::setupRoutes() {
    // Serve static files from SPIFFS
    _server->serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
    
    // API endpoints
    _server->on("/api/status", HTTP_GET, [this](AsyncWebServerRequest* request) {
        handleStatus(request);
    });
    
    _server->on("/api/config", HTTP_GET, [this](AsyncWebServerRequest* request) {
        handleConfig(request);
    });
    
    _server->on("/api/config", HTTP_POST, [this](AsyncWebServerRequest* request) {
        handleSaveConfig(request);
    });
    
    // 404 handler
    _server->onNotFound([this](AsyncWebServerRequest* request) {
        handleNotFound(request);
    });
}

void WebServerManager::handleRoot(AsyncWebServerRequest* request) {
    request->send(SPIFFS, "/index.html", "text/html");
}

void WebServerManager::handleConfig(AsyncWebServerRequest* request) {
    // Read config from SPIFFS
    File file = SPIFFS.open(CONFIG_FILE, "r");
    if (!file) {
        request->send(404, "application/json", "{\"error\":\"Config file not found\"}");
        return;
    }
    
    String content = file.readString();
    file.close();
    
    request->send(200, "application/json", content);
}

void WebServerManager::handleStatus(AsyncWebServerRequest* request) {
    if (_statusCallback) {
        String status = _statusCallback();
        request->send(200, "application/json", status);
    } else {
        request->send(200, "application/json", "{\"status\":\"ok\"}");
    }
}

void WebServerManager::handleSaveConfig(AsyncWebServerRequest* request) {
    String ssid = "";
    String password = "";
    
    // Get parameters
    if (request->hasParam("ssid", true)) {
        ssid = request->getParam("ssid", true)->value();
    }
    if (request->hasParam("password", true)) {
        password = request->getParam("password", true)->value();
    }
    
    if (ssid.length() == 0) {
        request->send(400, "application/json", "{\"error\":\"SSID is required\"}");
        return;
    }
    
    // Save to SPIFFS
    StaticJsonDocument<256> doc;
    doc["ssid"] = ssid;
    doc["password"] = password;
    
    File file = SPIFFS.open(CONFIG_FILE, "w");
    if (!file) {
        request->send(500, "application/json", "{\"error\":\"Failed to save config\"}");
        return;
    }
    
    serializeJson(doc, file);
    file.close();
    
    Logger::info("Configuration saved");
    
    // Trigger callback
    if (_configUpdateCallback) {
        _configUpdateCallback(ssid.c_str(), password.c_str());
    }
    
    request->send(200, "application/json", "{\"success\":true,\"message\":\"Configuration saved. Device will reconnect.\"}");
}

void WebServerManager::handleNotFound(AsyncWebServerRequest* request) {
    request->send(404, "text/plain", "Not found");
}
