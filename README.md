# Ynvisible EvalKit-5 ECD Driver

ESP-IDF component for driving Ynvisible electrochromic displays (ECDs) on ESP32 platforms. This component provides a comprehensive hardware abstraction layer and animation system for controlling various types of electrochromic displays.

## Features

- **Multiple ECD Support**: Compatible with 9 different Ynvisible EvalKit display types
- **Hardware Abstraction**: Unified interface for ECD control hardware
- **Animation System**: Built-in animations including toggle, count up/down, and test patterns
- **Flexible Configuration**: Support for both active and passive driving modes
- **Thread-Safe**: FreeRTOS compatible with proper synchronization
- **Modern C++**: Leverages C++17 features

## Supported Displays

### Ynvisible ECD Types
- **Single Segment**: Basic on/off display functionality
- **Multi-Segment Bar**: 3-segment and 7-segment bar displays
- **Numeric Displays**: Decimal numbers, dot matrix, and signed numbers

## Compatibility
ESP-IDF 5.0 or newer


## Quick Start

### Basic Usage

```cpp
#include "evalkit_displays.hpp"
#include "evalkit_anims.hpp"
#include "app_hal.hpp"

extern "C" void app_main(void)
{
    // Initialize display manager
    auto& displays = ynv::ecd::EvalkitDisplays::getInstance();
    auto& anims = ynv::anim::EvalkitAnims::getInstance();

    // Configure application (implement your HAL)
    ynv::app::AppConfig_t config = {
        .activeDriving = true,
        .analogResolution = 12,
        .maxSegmentVoltage = ynv::app::AppConfig_t::MAX_SEGMENT_VOLTAGE,
        .highPinVoltage = ynv::app::AppConfig_t::HIGH_PIN_VOLTAGE,
        .hal = your_hal_instance  // Implement ynv::driver::HALBase
    };

    // Initialize displays
    displays.init(&config);

    // Start animation
    anims.select(ynv::ecd::EvalkitDisplays::EVALKIT_DISP_SINGLE_SEGMENT_DISPLAY,
                 ynv::anim::EvalkitAnims::ANIM_TOGGLE);

    // Animation loop
    while (true) {
        if (anims.isSelected()) {
            anims.getCurrentAnim().update();
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```

### Implementing Hardware Abstraction Layer

Create your own HAL implementation by inheriting from `ynv::driver::HALBase`:

```cpp
#include "ynv_hal.hpp"

class MyHAL : public ynv::driver::HALBase
{
public:
    esp_err_t digitalWrite(int pin, bool high, int delay = 10, int common = 0) override
    {
        // Implement your digital write with timing and common electrode control
        return ESP_OK;
    }

    int analogRead(int pin) override
    {
        // Implement your analog read functionality
        return analog_value;
    }
};
```

## API Reference

### Core Classes

#### `EvalkitDisplays`
Manages ECD hardware initialization and selection:
```cpp
auto& displays = ynv::ecd::EvalkitDisplays::getInstance();
displays.init(&config);
auto display = displays.selectDisplay(EVALKIT_DISP_SINGLE_SEGMENT_DISPLAY);
```

#### `EvalkitAnims`
Controls display animations:
```cpp
auto& anims = ynv::anim::EvalkitAnims::getInstance();
anims.select(displayType, animationType);
anims.getCurrentAnim().update();
```

#### `HALBase`
Hardware abstraction interface (implement in your application):
```cpp
class YourHAL : public ynv::driver::HALBase {
    // Implement digitalWrite() and analogRead()
};
```

### Display Types

| Display Type | Description | Segments |
|-------------|-------------|----------|
| `EVALKIT_DISP_SINGLE_SEGMENT_DISPLAY` | Single segment | 1 |
| `EVALKIT_DISP_THREE_SEGMENT_BAR_DISPLAY` | 3-segment bar | 3 |
| `EVALKIT_DISP_SEVEN_SEGMENT_BAR_DISPLAY` | 7-segment bar | 7 |
| `EVALKIT_DISP_DOT_NUMBER_DISPLAY` | Dot number | 8 |
| `EVALKIT_DISP_DECIMAL_NUMBER_DISPLAY` | Decimal number | 15 |
| `EVALKIT_DISP_SIGNED_NUMBER_DISPLAY` | Signed number | 15 |

### Animation Types

| Animation | Description | Supported Displays |
|-----------|-------------|-------------------|
| `ANIM_TOGGLE` | ON/OFF switching | All displays |
| `ANIM_UP` | Count up sequence | Multi-segment displays |
| `ANIM_DOWN` | Count down sequence | Multi-segment displays |
| `ANIM_TEST` | Test pattern | All displays |

### Configuration Structure

```cpp
struct AppConfig_t {
    bool activeDriving;          // Active (precise) vs passive (basic) driving
    int analogResolution;        // ADC/DAC resolution in bits
    int maxSegmentVoltage;       // Maximum segment voltage (ADC units)
    int highPinVoltage;          // High pin voltage level (ADC units)
    void* hal;                   // HAL implementation pointer
    
    static const int MAX_SEGMENT_VOLTAGE = 1400;  // Safe ECD voltage limit
    static const int HIGH_PIN_VOLTAGE = 3300;     // GPIO high level
};
```

## Examples

The component includes reference implementations:

- **Display Test**: Minimal ECD validation without GUI
- **GUI Demo**: Complete touchscreen interface using LVGL  
- **HAL Test**: Hardware component testing examples

*Note: Examples show specific hardware implementations for reference. Adapt the HAL layer to your hardware setup.*

## Architecture

```
Your Application
├── Your HAL Implementation (ynv::driver::HALBase)
├── Ynvisible ECD Driver Component
│   ├── Display Management (EvalkitDisplays)
│   ├── Animation System (EvalkitAnims) 
│   ├── ECD Classes (Single/Multi-segment)
│   └── Drive Algorithms (Active/Passive)
└── Your Hardware (Multiplexer, DAC, ECDs)
```

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Support

- **Issues**: [GitHub Issues](https://github.com/ozanoner/ynvisible-driver5/issues)

## References

- [Ynvisible Arduino Library](https://github.com/Ynvisible-Electronics/YNV-Driver-v5-Arduino-Library)
- [Ynvisible Evalution Kit-5 Guide](https://www.ynvisible.com/product/e-paper-display-kit)
