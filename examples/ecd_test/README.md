# YnVisible EvalKit Display Test

A minimal test application for validating electrochromic display (ECD) functionality without GUI overhead. This project provides a bare-bones environment for testing display drivers and animation systems.

## Overview

This application demonstrates basic ECD operation by:
- Initializing hardware abstraction layer (HAL)
- Setting up display management system
- Running continuous toggle animation on test display
- Providing minimal overhead for hardware validation

## Hardware Requirements

### Development Board
- **ESP32-S3-Box3** development board

### Components
- **CD74HC4067** 16-channel analog multiplexer
- **MCP4725** 12-bit I2C DAC
- **YnVisible EvalKit ECDs** (any type for basic testing)
- Breadboard and jumper wires for connections

### Wiring Configuration

| Component | Pin | ESP32-S3-Box3 GPIO |
|-----------|-----|---------------------|
| **CD74HC4067** | S0 | GPIO 11 |
| | S1 | GPIO 9 |
| | S2 | GPIO 14 |
| | S3 | GPIO 13 |
| | SIG | GPIO 10 |
| | EN | GPIO 12 |
| **MCP4725** | SDA | GPIO 38 |
| | SCL | GPIO 41 |
| | VCC | 3.3V |
| | GND | GND |

**MCP4725 I2C Address:** `0x60` (default)  
**I2C Frequency:** 100kHz

## Software Configuration

### Build Configuration

Configure the project using ESP-IDF menuconfig:

```bash
idf.py menuconfig
```

Key configuration options:
- **Application → ECD Driving Mode**
  - `Active Driving`: Precise voltage control (recommended)
  - `Passive Driving`: Basic switching mode
- **Application → Voltage Settings**
  - Maximum segment voltage
  - High pin voltage level

### Project Structure

```
.
├── main
│   ├── CMakeLists.txt
│   ├── idf_component.yml
│   └── main.cpp
├── CMakeLists.txt
├── README.md
├── partitions.csv
└── sdkconfig.defaults
```

## Building and Running

### Prerequisites
- ESP-IDF v5.0 or later (tested with v5.4)
- ESP32-S3-Box3 development board
- USB cable for programming and serial monitoring

### Build Commands

```bash
# Navigate to project directory
cd /workspaces/examples/disp_test

# Configure the project
idf.py menuconfig

# Build the project
idf.py build

# Flash to device
idf.py flash

# Monitor serial output
idf.py monitor
```

### Combined Build and Flash
```bash
idf.py flash monitor
```

## Expected Output

The application will display:
1. Hardware initialization messages
2. Display system setup confirmation
3. Animation update logs every second
4. ECD state changes 

## Test Procedure

1. **Hardware Setup**
   - Connect components according to wiring table
   - Ensure all connections are secure
   - Power on ESP32-S3-Box3

2. **Software Testing**
   - Flash the application
   - Monitor serial output for initialization
   - Observe animation updates every second

## Troubleshooting

| Issue | Symptoms | Solution |
|-------|----------|----------|
| No serial output | Blank monitor screen | Check USB cable, press reset button |
| I2C errors | `ESP_ERR_TIMEOUT` messages | Add 4.7kΩ pull-up resistors to SDA/SCL |
| DAC not responding | Constant voltage output | Verify I2C address (0x60), check wiring |
| Multiplexer errors | GPIO operation failures | Verify GPIO connections, check power supply |
| Build errors | Compilation failures | Run `idf.py reconfigure`, check ESP-IDF version |

## Configuration Options


### Runtime Configuration
The application uses these default settings:
- **Animation Type**: Toggle (ON/OFF switching)
- **Update Interval**: 1 second
- **Display Type**: EVALKIT_DISP_TEST
- **I2C Frequency**: 100kHz

## Development Notes

### Testing Different Displays
To test different ECD types, modify `main.cpp`:

```cpp
// Change display type
anims.select(ynv::ecd::EvalkitDisplays::ECDEvalkitDisplay_t::EVALKIT_DISP_SINGLE_SEGMENT_DISPLAY,
             ynv::anim::EvalkitAnims::Anim_t::ANIM_TOGGLE);
```

### Testing Different Animations
```cpp
// Test count-up animation
anims.select(ynv::ecd::EvalkitDisplays::ECDEvalkitDisplay_t::EVALKIT_DISP_TEST,
             ynv::anim::EvalkitAnims::Anim_t::ANIM_UP);
```

### Debug Logging
Enable debug output:
```bash
idf.py monitor --print-filter="*:D"
```

## Related Projects

- **[demo](../demo/)**: Full GUI demo with touchscreen interface
- **[hal_test](../hal_test/)**: Hardware abstraction layer component testing

## Support

For issues and questions:
1. Check the troubleshooting section above
2. Review ESP-IDF documentation
3. Consult YnVisible EvalKit hardware documentation
4. Check component datasheets (CD74HC4067, MCP4725)