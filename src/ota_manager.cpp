#include "ota_manager.h"
#include "config.h"
#include "logger.h"

OTAManager::OTAManager() : _updating(false) {
}

void OTAManager::begin(const char* hostname, const char* password) {
    ArduinoOTA.setHostname(hostname);
    
    if (password != nullptr && strlen(password) > 0) {
        ArduinoOTA.setPassword(password);
    }
    
    setupCallbacks();
    
    ArduinoOTA.begin();
    Logger::info("OTA initialized. Hostname: " + String(hostname));
}

void OTAManager::handle() {
    ArduinoOTA.handle();
}

bool OTAManager::isUpdating() {
    return _updating;
}

void OTAManager::setupCallbacks() {
    ArduinoOTA.onStart([this]() {
        _updating = true;
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";
        } else {  // U_SPIFFS
            type = "filesystem";
        }
        Logger::info("OTA Update Started: " + type);
    });
    
    ArduinoOTA.onEnd([this]() {
        _updating = false;
        Logger::info("\nOTA Update Completed!");
    });
    
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        static unsigned int lastProgress = 0;
        unsigned int currentProgress = (progress / (total / 100));
        
        // Only log every 10%
        if (currentProgress >= lastProgress + 10) {
            Logger::info("OTA Progress: " + String(currentProgress) + "%");
            lastProgress = currentProgress;
        }
    });
    
    ArduinoOTA.onError([this](ota_error_t error) {
        _updating = false;
        String errorMsg = "OTA Error[" + String(error) + "]: ";
        
        if (error == OTA_AUTH_ERROR) {
            errorMsg += "Auth Failed";
        } else if (error == OTA_BEGIN_ERROR) {
            errorMsg += "Begin Failed";
        } else if (error == OTA_CONNECT_ERROR) {
            errorMsg += "Connect Failed";
        } else if (error == OTA_RECEIVE_ERROR) {
            errorMsg += "Receive Failed";
        } else if (error == OTA_END_ERROR) {
            errorMsg += "End Failed";
        }
        
        Logger::error(errorMsg);
    });
}
