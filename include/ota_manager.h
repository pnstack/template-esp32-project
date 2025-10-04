#ifndef OTA_MANAGER_H
#define OTA_MANAGER_H

#include <ArduinoOTA.h>
#include <Arduino.h>

class OTAManager {
public:
    OTAManager();
    
    // Initialize OTA updates
    void begin(const char* hostname, const char* password);
    
    // Handle OTA updates (call in loop)
    void handle();
    
    // Check if OTA update is in progress
    bool isUpdating();

private:
    bool _updating;
    
    void setupCallbacks();
};

#endif // OTA_MANAGER_H
