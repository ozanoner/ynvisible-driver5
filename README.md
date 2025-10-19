# YnVisible EvalKit ECD Driver

ESP-IDF component for driving YnVisible electrochromic displays (ECDs) on ESP32 platforms. This component provides a comprehensive hardware abstraction layer and animation system for controlling various types of electrochromic displays.

## Features

- **Multiple ECD Support**: Compatible with 9 different YnVisible EvalKit display types
- **Hardware Abstraction**: Unified interface for controller hardware
- **Animation System**: Built-in animations including toggle, count up/down, and test patterns
- **Flexible Configuration**: Support for both active and passive driving modes
- **Thread-Safe**: FreeRTOS compatible with proper synchronization
- **Modern C++**: Leverages C++17 features

## Supported Hardware

### Electrochromic Displays
- **Single Segment**: DISP437V2PV1, DISP440V2PV1, DISP442V2PV1, DISP443V2PV1
- **Multi-Segment**: DISP438V2PV1 (3-segment), DISP431V2PV1 (7-segment)
- **Numeric Displays**: DISP433V1PV1 (decimal), DISP434V1PV1 (dot), DISP444V1PV2 (signed)

## Compatibility
ESP-IDF 5.0 or newer


## Quick Start

### Installation

Add this component to your ESP-IDF project:

```bash
idf.py add-dependency "ynvisible/evalkit_ecd_driver"
```

Or add to `main/idf_component.yml`:

```yaml
dependencies:
  ynvisible/evalkit_ecd_driver: "^0.1.0"
```

### Basic Usage

```cpp
#include "evalkit_displays.hpp"
#include "evalkit_anims.hpp"
#include "app_hal.hpp"

// Initialize hardware
auto& hal = app::hal::HAL::getInstance();
auto& displays = ynv::ecd::EvalkitDisplays::getInstance();
auto& anims = ynv::anim::EvalkitAnims::getInstance();

// Configure application
ynv::app::AppConfig_t config = {
    .hal = &hal,
    .activeDriving = true,
    .analogResolution = 12,
    .maxSegmentVoltage = 2048,
    .highPinVoltage = 1638
};

// Initialize hardware (CD74HC4067 & MCP4725 in the demo app)
hal.init(&config,
    // Multiplexer pins
    {.s0 = GPIO_NUM_11, .s1 = GPIO_NUM_9, .s2 = GPIO_NUM_14, 
     .s3 = GPIO_NUM_13, .signal = GPIO_NUM_10, .enable = GPIO_NUM_12},
    // DAC configuration  
    {.i2cAddr = 0x60, .i2cSdaGpio = GPIO_NUM_38, 
     .i2cSclGpio = GPIO_NUM_41, .i2cFreqHz = 100000});

displays.init(&config);
anims.init(&config);

// Start animation
anims.select(ynv::ecd::EvalkitDisplays::EVALKIT_DISP_SINGLE_SEGMENT_DISPLAY,
             ynv::anim::EvalkitAnims::ANIM_TOGGLE);
```

### Configuration

Configure the component via menuconfig:

```bash
idf.py menuconfig
# Navigate to: Component config → YnVisible EvalKit
```

Key configuration options:
- **Driving Mode**: Active (precise) or Passive (basic) 
- **Voltage Levels**: Maximum segment and high pin voltages
- **Timing Parameters**: Refresh intervals and retry counts

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

#### `HAL` (Hardware Abstraction Layer)
Low-level hardware control:
```cpp
auto& hal = app::hal::HAL::getInstance();
hal.init(&config, muxConfig, dacConfig);
```

### Display Types

| Enum Value | Description | Segments |
|------------|-------------|----------|
| `EVALKIT_DISP_SINGLE_SEGMENT_DISPLAY` | Single segment | 1 |
| `EVALKIT_DISP_THREE_SEGMENT_BAR_DISPLAY` | 3-segment bar | 3 |
| `EVALKIT_DISP_SEVEN_SEGMENT_BAR_DISPLAY` | 7-segment bar | 7 |
| `EVALKIT_DISP_DOT_NUMBER_DISPLAY` | Dot matrix number | 12 |
| `EVALKIT_DISP_DECIMAL_NUMBER_DISPLAY` | Decimal number | 13 |
| `EVALKIT_DISP_SIGNED_NUMBER_DISPLAY` | Signed number | 15 |

### Animation Types

| Animation | Description | Supported Displays |
|-----------|-------------|-------------------|
| `ANIM_TOGGLE` | ON/OFF switching | All displays |
| `ANIM_UP` | Count up sequence | Multi-segment displays |
| `ANIM_DOWN` | Count down sequence | Multi-segment displays |
| `ANIM_TEST` | Test pattern | All displays |

## Examples

### Minimal Display Test
See [`examples/disp_test`](examples/disp_test/) for a basic ECD test without GUI.

### Full GUI Demo  
See [`examples/demo`](examples/demo/) for a complete touchscreen interface using LVGL.

### HAL Component Test
See [`examples/hal_test`](examples/hal_test/) for testing multiplexer and DAC functionality.

## License

This project is licensed under the Apache License 2.0 - see the LICENSE file for details.

## Support

- **Documentation**: [YnVisible EvalKit Guide](https://www.ynvisible.com/)
- **Component Registry**: [ESP Component Registry](https://components.espressif.com/)
- **Issues**: [GitHub Issues](https://github.com/your-repo/issues)

## References

- [YnVisible Arduino Library](https://github.com/Ynvisible-Electronics/YNV-Driver-v5-Arduino-Library)
- [YnVisible Official Website](https://www.ynvisible.com/)
- [ESP-IDF Programming Guide](https://docs.espressif.com/projects/esp-idf/en/latest/)