#include "http_client.h"
#include "config.h"
#include "logger.h"

HTTPClientManager::HTTPClientManager() {
}

int HTTPClientManager::sendGET(const char* url, String& response) {
    if (!isValidURL(url)) {
        Logger::error("Invalid URL");
        return -1;
    }
    
    _http.begin(url);
    _http.setTimeout(HTTP_TIMEOUT);
    
    int httpCode = _http.GET();
    
    if (httpCode > 0) {
        response = _http.getString();
        Logger::info("HTTP GET Response: " + String(httpCode));
    } else {
        Logger::error("HTTP GET Failed: " + String(_http.errorToString(httpCode).c_str()));
    }
    
    _http.end();
    return httpCode;
}

int HTTPClientManager::sendPOST(const char* url, const char* jsonPayload, String& response) {
    if (!isValidURL(url)) {
        Logger::error("Invalid URL");
        return -1;
    }
    
    _http.begin(url);
    _http.setTimeout(HTTP_TIMEOUT);
    _http.addHeader("Content-Type", "application/json");
    
    int httpCode = _http.POST(jsonPayload);
    
    if (httpCode > 0) {
        response = _http.getString();
        Logger::info("HTTP POST Response: " + String(httpCode));
    } else {
        Logger::error("HTTP POST Failed: " + String(_http.errorToString(httpCode).c_str()));
    }
    
    _http.end();
    return httpCode;
}

bool HTTPClientManager::sendSensorData(const char* url, float temperature, float humidity) {
    StaticJsonDocument<200> doc;
    doc["temperature"] = temperature;
    doc["humidity"] = humidity;
    doc["timestamp"] = millis();
    
    String jsonString;
    serializeJson(doc, jsonString);
    
    Logger::debug("Sending sensor data: " + jsonString);
    
    String response;
    int httpCode = sendPOST(url, jsonString.c_str(), response);
    
    return (httpCode >= 200 && httpCode < 300);
}

bool HTTPClientManager::isValidURL(const char* url) {
    if (url == nullptr || strlen(url) == 0) {
        return false;
    }
    
    String urlStr = String(url);
    return (urlStr.startsWith("http://") || urlStr.startsWith("https://"));
}
