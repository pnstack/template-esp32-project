#include "logger.h"

LogLevel Logger::_logLevel = LOG_INFO;

void Logger::begin(unsigned long baudRate) {
    Serial.begin(baudRate);
    while (!Serial && millis() < 5000) {
        ; // Wait for serial port to connect
    }
    Serial.println("\n===================================");
    Serial.println("ESP32 System Logger Initialized");
    Serial.println("===================================\n");
}

void Logger::setLogLevel(LogLevel level) {
    _logLevel = level;
}

void Logger::error(const char* message) {
    log(LOG_ERROR, message);
}

void Logger::warn(const char* message) {
    log(LOG_WARN, message);
}

void Logger::info(const char* message) {
    log(LOG_INFO, message);
}

void Logger::debug(const char* message) {
    log(LOG_DEBUG, message);
}

void Logger::error(const String& message) {
    log(LOG_ERROR, message.c_str());
}

void Logger::warn(const String& message) {
    log(LOG_WARN, message.c_str());
}

void Logger::info(const String& message) {
    log(LOG_INFO, message.c_str());
}

void Logger::debug(const String& message) {
    log(LOG_DEBUG, message.c_str());
}

void Logger::log(LogLevel level, const char* message) {
    if (level > _logLevel) {
        return;
    }
    
    char timestamp[32];
    snprintf(timestamp, sizeof(timestamp), "[%10lu] ", millis());
    
    Serial.print(timestamp);
    Serial.print("[");
    Serial.print(levelToString(level));
    Serial.print("] ");
    Serial.println(message);
}

const char* Logger::levelToString(LogLevel level) {
    switch (level) {
        case LOG_ERROR: return "ERROR";
        case LOG_WARN:  return "WARN ";
        case LOG_INFO:  return "INFO ";
        case LOG_DEBUG: return "DEBUG";
        default:        return "UNKN ";
    }
}
