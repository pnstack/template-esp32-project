#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <HTTPClient.h>
#include <Arduino.h>
#include <ArduinoJson.h>

class HTTPClientManager {
public:
    HTTPClientManager();
    
    // Send GET request
    int sendGET(const char* url, String& response);
    
    // Send POST request with JSON payload
    int sendPOST(const char* url, const char* jsonPayload, String& response);
    
    // Send sensor data (example)
    bool sendSensorData(const char* url, float temperature, float humidity);

private:
    HTTPClient _http;
    
    bool isValidURL(const char* url);
};

#endif // HTTP_CLIENT_H
