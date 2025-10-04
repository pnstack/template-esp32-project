# Project Folder Structure

This document describes the organization of files in the ESP32 Template Project.

```
template-esp32-project/
│
├── .vscode/                        # Visual Studio Code settings
│   ├── extensions.json            # Recommended extensions (PlatformIO)
│   └── settings.json              # Editor settings
│
├── data/                           # Web interface files (uploaded to SPIFFS)
│   └── index.html                 # Main configuration web page
│
├── docs/                           # Documentation
│   ├── API.md                     # HTTP API documentation
│   ├── EXAMPLES.md                # Usage examples and code snippets
│   ├── OTA_UPDATES.md             # OTA firmware update guide
│   └── SETUP.md                   # Installation and setup instructions
│
├── include/                        # Header files (.h)
│   ├── config.h                   # Configuration constants and settings
│   ├── credentials.h.example      # Example credentials file (template)
│   ├── http_client.h              # HTTP client interface
│   ├── logger.h                   # Serial logging interface
│   ├── ota_manager.h              # OTA update manager interface
│   ├── web_server.h               # Web server interface
│   └── wifi_manager.h             # WiFi management interface
│
├── src/                            # Source files (.cpp)
│   ├── http_client.cpp            # HTTP client implementation
│   ├── logger.cpp                 # Serial logging implementation
│   ├── main.cpp                   # Main application entry point
│   ├── ota_manager.cpp            # OTA update manager implementation
│   ├── web_server.cpp             # Web server implementation
│   └── wifi_manager.cpp           # WiFi management implementation
│
├── .gitignore                      # Git ignore rules (build artifacts, credentials)
├── CONTRIBUTING.md                 # Contribution guidelines
├── FOLDER_STRUCTURE.md            # This file
├── LICENSE                         # MIT License
├── platformio.ini                  # PlatformIO project configuration
└── README.md                       # Main project documentation

```

## Directory Purposes

### `/.vscode/`
Contains Visual Studio Code configuration files:
- **extensions.json**: Recommends PlatformIO IDE extension
- **settings.json**: Editor preferences for C++ development

**Note**: These files are tracked in git but editor-specific generated files are ignored.

### `/data/`
Contains files that will be uploaded to the ESP32's SPIFFS filesystem:
- **index.html**: Web-based configuration interface
- Add additional web assets here (CSS, JS, images)

**Upload to ESP32**: Use `pio run --target uploadfs`

### `/docs/`
Comprehensive documentation for the project:
- **API.md**: HTTP endpoints and API usage
- **EXAMPLES.md**: Code examples for common tasks
- **OTA_UPDATES.md**: Over-the-air update procedures
- **SETUP.md**: Installation and first-time setup

### `/include/`
C++ header files defining interfaces and configurations:
- **config.h**: System-wide configuration constants
- **credentials.h.example**: Template for WiFi/API credentials
- **\*_manager.h**: Interface definitions for each module

**Note**: Copy `credentials.h.example` to `credentials.h` for local credentials (this file is gitignored).

### `/src/`
C++ implementation files:
- **main.cpp**: Application entry point, setup() and loop()
- **\*_manager.cpp**: Implementation of each module

## Build Artifacts (Not in Git)

The following directories are created during build but not tracked in git:

```
.pio/                              # PlatformIO build directory
├── build/                         # Compiled binaries
│   └── esp32dev/
│       ├── firmware.bin           # Main firmware binary
│       ├── firmware.elf           # ELF debug file
│       └── spiffs.bin             # Filesystem image
├── libdeps/                       # Downloaded libraries
│   └── esp32dev/
│       ├── ArduinoJson/
│       ├── AsyncTCP/
│       └── ESP AsyncWebServer/
└── ...                            # Other build files
```

## File Types

### Source Code
- **`.cpp`**: C++ source implementation files
- **`.h`**: C++ header files (interfaces, declarations)

### Configuration
- **`.ini`**: PlatformIO project configuration
- **`.json`**: JSON configuration files (VS Code, web data)

### Documentation
- **`.md`**: Markdown documentation files

### Web
- **`.html`**: Web interface pages
- **`.css`**: Stylesheets (if added)
- **`.js`**: JavaScript files (if separated from HTML)

## Adding New Files

### Adding a New Module

1. Create header in `include/`:
   ```
   include/my_module.h
   ```

2. Create implementation in `src/`:
   ```
   src/my_module.cpp
   ```

3. Include in `main.cpp`:
   ```cpp
   #include "my_module.h"
   ```

### Adding Web Assets

1. Create files in `data/`:
   ```
   data/style.css
   data/script.js
   data/logo.png
   ```

2. Reference in HTML:
   ```html
   <link rel="stylesheet" href="/style.css">
   <script src="/script.js"></script>
   <img src="/logo.png">
   ```

3. Upload filesystem:
   ```bash
   pio run --target uploadfs
   ```

### Adding Documentation

1. Create markdown file in `docs/`:
   ```
   docs/MY_GUIDE.md
   ```

2. Link from README.md:
   ```markdown
   - [My Guide](docs/MY_GUIDE.md)
   ```

## File Naming Conventions

### C++ Files
- **snake_case**: Use lowercase with underscores
  - `wifi_manager.cpp`, `http_client.h`

### Classes
- **PascalCase**: Capitalize each word
  - `WiFiManager`, `HTTPClientManager`

### Constants
- **UPPER_SNAKE_CASE**: Uppercase with underscores
  - `WIFI_MAX_RETRY`, `WEBSERVER_PORT`

### Variables
- **camelCase**: First word lowercase, rest capitalized
  - `lastReconnectAttempt`, `isConfigured`

### Files
- **lowercase**: Markdown and config files
  - `readme.md`, `platformio.ini`
- **UPPERCASE**: Special files
  - `README.md`, `LICENSE`, `CONTRIBUTING.md`

## Configuration Files

### `platformio.ini`
Main project configuration:
- Board type: `esp32dev`
- Framework: `arduino`
- Libraries: Dependencies
- Upload settings: Serial port, speed
- Build flags: Compilation options

### `.gitignore`
Prevents committing:
- Build artifacts (`.pio/`, `*.bin`)
- IDE files (`.vscode/.browse*`)
- Credentials (`credentials.h`)
- OS files (`.DS_Store`)

### `extensions.json`
Recommends installing PlatformIO IDE for optimal development experience.

### `settings.json`
Configures VS Code for C++ development with PlatformIO.

## Best Practices

1. **Keep related code together**: Each module has its `.h` and `.cpp` file
2. **Separate concerns**: Web, WiFi, OTA, HTTP are independent modules
3. **Document public APIs**: Headers describe interfaces, implementation in `.cpp`
4. **Use meaningful names**: File names match class names
5. **Organize by function**: Group related functionality in modules
6. **Version control**: Commit source, ignore build artifacts
7. **Document changes**: Update relevant docs when changing features

## Size Considerations

### Firmware Size
- Maximum: ~1.3 MB (depends on partition scheme)
- Typical: 200-500 KB for this template
- Monitor: Check after build in PlatformIO output

### SPIFFS Size
- Default: 1.5 MB (configurable in platformio.ini)
- Current usage: ~11 KB (index.html)
- Monitor: Check with SPIFFS.totalBytes() and SPIFFS.usedBytes()

### RAM Usage
- Total: 520 KB (ESP32)
- Monitor: Use ESP.getFreeHeap() to check available memory

## Further Reading

- [PlatformIO Project Structure](https://docs.platformio.org/en/latest/projectconf/index.html)
- [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)
- [Project README](README.md)
