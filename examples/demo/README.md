# YnVisible EvalKit Demo with ESP32-S3-Box3

This demo showcases the YnVisible EvalKit using ESP32-S3-Box3 development board with custom driver hardware instead of the original YnVisible driver board.

## Overview

The YnVisible EvalKit contains various Electrochromic Display (ECD) samples that can be controlled to demonstrate different visual effects and animations. This implementation uses ESP32-S3-Box3 as the main controller with additional hardware components to drive the ECDs.

## Hardware Setup

### Main Components
- **ESP32-S3-Box3**: Main development board with integrated display and touch interface
- **CD74HC4067**: 16-channel analog multiplexer for ECD selection
- **MCP4725**: 12-bit I2C DAC for true analog output
- **YnVisible EvalKit ECDs**: Various electrochromic display samples

### ECD Samples in EvalKit
The evalkit contains the following ECD types:
- **DISP431V2PV1**: 7-Segment Bar Display with Toggle, Count up, Count down animations
- **DISP433V1PV1**: Decimal Number Display with Toggle, Count up, Count down animations  
- **DISP434V1PV1**: Dot Number Display with Toggle, Count up, Count down animations
- **DISP437V2PV1**: Single Segment Display with Toggle animation only
- **DISP438V2PV1**: 3-Segment Bar Display with Toggle, Count up, Count down animations
- **DISP440V2PV1**: Single Segment Display with Toggle animation only
- **DISP442V2PV1**: Single Segment Display with Toggle animation only  
- **DISP443V2PV1**: Single Segment Display with Toggle animation only
- **DISP444V1PV2**: Signed Number Display with Toggle, Count up, Count down animations

See [the evalkit documentation](../../docs/evalkit.pdf)

### Wiring Connections
```
ESP32-S3-Box3 ↔ CD74HC4067:
- GPIO11 ↔ S0 (Select bit 0)
- GPIO9 ↔ S1 (Select bit 1) 
- GPIO14 ↔ S2 (Select bit 2)
- GPIO13 ↔ S3 (Select bit 3)
- GPIO12 ↔ EN (Enable/Disable)
- GPIO10 ↔ Signal

ESP32-S3-Box3 ↔ MCP4725 (address: 0x60):
- GPIO38 ↔ SDA (I2C Data)
- GPIO41 ↔ SCL (I2C Clock)

CD74HC4067 ↔ ECDs:
- C1-C15 ↔ ECD channels (C0 is NOT connected)
- COM ↔ MCP4725 output (voltage reference)
```

## Software Features

### User Interface
- **Touch-based GUI**: Navigate through ECDs using the ESP32-S3-Box3 touchscreen
- **ECD Preview**: Display images of each ECD type for easy identification
- **Animation Control**: Select and control different animations per ECD
- **Status Display**: Real-time feedback on current animation state

### Supported Animations
1. **Toggle**: Switch between two visual states
2. **Count Up**: Progressive state changes in ascending order
3. **Count Down**: Progressive state changes in descending order

### Driver Architecture
- **Hardware Abstraction Layer**: Clean interface for ECD control
- **Singleton Pattern**: Thread-safe hardware resource management
- **LVGL Integration**: Modern GUI framework for smooth user experience
- **Modular Design**: Easy to extend for additional ECD types

## Building and Running

### Prerequisites
- **ESP-IDF v5.0 or later** (tested with v5.4)
- **ESP32-S3-Box3** development board
- **Hardware components**:
  - CD74HC4067 16-channel multiplexer
  - MCP4725 12-bit I2C DAC
  - YnVisible EvalKit ECDs
  - Breadboard and jumper wires
- **Development environment**: VSCode with ESP-IDF extension (recommended)

### Build Instructions
```bash
# Navigate to project directory
cd examples/demo1

# Configure project (optional)
idf.py menuconfig

# Build project
idf.py build

# Flash to device
idf.py flash monitor

# Serial monitoring
idf.py monitor --print-filter="*:I HAL:W gpio:W ECDDrive:W" -p /dev/ttyACM0 
```

### Configuration Options
- I2C frequency for MCP4725 communication
- GPIO pin assignments for multiplexer control
- ECD voltage levels and timing parameters
- GUI display settings

## Usage

1. **Power on** the ESP32-S3-Box3 with connected hardware
2. **Touch the display** to navigate through available ECDs
3. **Select an ECD** to view available animations
4. **Choose animation** to start the electrochromic effect
5. **Monitor status** through the on-screen indicators


## Project Structure

```
.
├── main
│   ├── disp_images
│   │   ├── disp431v2pv1.c
│   │   ├── disp433v1pv1.c
│   │   ├── disp434v1pv1.c
│   │   ├── disp437v2pv1.c
│   │   ├── disp438v2pv1.c
│   │   ├── disp440v2pv1.c
│   │   ├── disp442v2pv1.c
│   │   ├── disp443v2pv1.c
│   │   └── disp444v1pv2.c
│   ├── CMakeLists.txt
│   ├── app_disp_info.hpp
│   ├── app_gui.cpp
│   ├── app_gui.hpp
│   ├── disp_images.hpp
│   ├── idf_component.yml
│   └── main.cpp
├── CMakeLists.txt
├── README.md
├── partitions.csv
└── sdkconfig.defaults
```
```main.cpp``` has the application entry point.

## Technical Notes

### Voltage Control
- **12-bit resolution**: 0-4095 digital values map to 0-3.3V output
- **ECD-specific levels**: Each ECD type may require different voltage ranges

### Analog Operations
#### CD74HC4067
- **4-bit addressing**: Supports up to 16 pins (can drive any ECD in the evalkit)
- **Bi-directional**: Can read from/write to any selected pin.

#### MCP4725
- **True analog output**: 12-bit true analog output to protect ECDs (ESP32S3 supports PWM only)

## Troubleshooting

### Common Issues
- **No ECD response**: Check wiring connections and voltage levels
- **GUI not responding**: Verify LVGL configuration and touch calibration  
- **I2C communication errors**: Check SDA/SCL connections and pull-up resistors
- **Multiplexer issues**: Verify select pin connections and logic levels

### Debug Options
- Enable detailed logging in `menuconfig`
- Use serial monitor for real-time diagnostics
- Check I2C bus scanner for device detection

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Contributing

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## Support

For questions and support:
- Check the troubleshooting section
- Review ESP-IDF documentation
- Consult YnVisible EvalKit documentation