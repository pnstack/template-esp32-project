#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

// Log levels
enum LogLevel {
    LOG_ERROR = 0,
    LOG_WARN = 1,
    LOG_INFO = 2,
    LOG_DEBUG = 3
};

class Logger {
public:
    static void begin(unsigned long baudRate);
    static void setLogLevel(LogLevel level);
    
    static void error(const char* message);
    static void warn(const char* message);
    static void info(const char* message);
    static void debug(const char* message);
    
    static void error(const String& message);
    static void warn(const String& message);
    static void info(const String& message);
    static void debug(const String& message);

private:
    static LogLevel _logLevel;
    static void log(LogLevel level, const char* message);
    static const char* levelToString(LogLevel level);
};

#endif // LOGGER_H
