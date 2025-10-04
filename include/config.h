#ifndef CONFIG_H
#define CONFIG_H

// WiFi Configuration
#define WIFI_SSID_MAX_LENGTH 32
#define WIFI_PASSWORD_MAX_LENGTH 64
#define WIFI_RECONNECT_INTERVAL 5000  // ms
#define WIFI_MAX_RETRY 20

// Web Server Configuration
#define WEBSERVER_PORT 80

// OTA Configuration
#define OTA_HOSTNAME "esp32-device"
#define OTA_PASSWORD "admin"

// HTTP Client Configuration
#define HTTP_TIMEOUT 5000  // ms

// Serial Configuration
#define SERIAL_BAUD_RATE 115200

// SPIFFS Configuration
#define FORMAT_SPIFFS_IF_FAILED true

// Application Settings
#define CONFIG_FILE "/config.json"
#define DEFAULT_DEVICE_NAME "ESP32-Device"

#endif // CONFIG_H
