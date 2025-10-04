# Contributing to ESP32 Template Project

Thank you for your interest in contributing to this project! This document provides guidelines for contributing.

## How to Contribute

### Reporting Bugs

If you find a bug, please create an issue with:
- Clear description of the bug
- Steps to reproduce
- Expected behavior vs actual behavior
- ESP32 board model and version
- PlatformIO version
- Serial monitor output (if applicable)

### Suggesting Features

Feature requests are welcome! Please create an issue with:
- Clear description of the feature
- Use case and benefits
- Example code or pseudocode (if applicable)

### Pull Requests

1. **Fork the repository**
2. **Create a feature branch**: `git checkout -b feature/your-feature-name`
3. **Make your changes**:
   - Follow the existing code style
   - Add comments for complex logic
   - Update documentation if needed
4. **Test your changes**:
   - Build the project: `pio run`
   - Upload and test on ESP32: `pio run --target upload`
   - Verify web interface still works
5. **Commit your changes**: `git commit -m "Add feature: description"`
6. **Push to your fork**: `git push origin feature/your-feature-name`
7. **Create a Pull Request**

## Code Style Guidelines

### C++ Code

- Use descriptive variable and function names
- Follow existing indentation (4 spaces)
- Add comments for complex logic
- Use header guards in `.h` files
- Keep functions focused and small

Example:
```cpp
// Good
void WiFiManager::handleReconnect() {
    if (WiFi.status() == WL_CONNECTED) {
        return;
    }
    // ... reconnection logic
}

// Avoid
void h() {
    // unclear function name
}
```

### HTML/CSS/JavaScript

- Use consistent indentation (4 spaces)
- Add comments for complex functionality
- Keep JavaScript modular and readable
- Test in multiple browsers

### Documentation

- Use clear, concise language
- Provide examples where helpful
- Keep documentation up-to-date with code changes
- Use proper Markdown formatting

## Project Structure

When adding new features, maintain the project structure:

```
src/        - Implementation files (.cpp)
include/    - Header files (.h)
data/       - Web interface files
docs/       - Documentation
```

## Testing

Before submitting a PR, please test:

1. **Build**: `pio run` - no compilation errors
2. **Upload**: `pio run --target upload` - successful upload
3. **Functionality**: Verify your changes work as expected
4. **Web Interface**: Test web UI still functions correctly
5. **OTA**: If applicable, test OTA updates work
6. **Serial Logs**: Check for errors in serial monitor

## Documentation Updates

If your changes affect:
- Setup process → Update `docs/SETUP.md`
- API endpoints → Update `docs/API.md`
- OTA process → Update `docs/OTA_UPDATES.md`
- Usage → Update `README.md`

## Commit Messages

Use clear, descriptive commit messages:

```
Good:
- "Add MQTT support for sensor data"
- "Fix WiFi reconnection timeout issue"
- "Update API documentation with new endpoints"

Avoid:
- "Fixed stuff"
- "Update"
- "Changes"
```

## Code Review Process

1. Maintainers will review your PR
2. Address any feedback or requested changes
3. Once approved, your PR will be merged
4. Your contribution will be credited

## Questions?

If you have questions:
- Check existing documentation
- Search existing issues
- Create a new issue for discussion

## License

By contributing, you agree that your contributions will be licensed under the same license as the project.

Thank you for contributing! 🎉
