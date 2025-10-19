# HAL Test

A testing project for Hardware Abstraction Layer (HAL).

## Overview

This project contains tests and utilities for validating HAL functionality, specifically testing drivers for:
- **CD74HC4067** - 16-channel analog multiplexer/demultiplexer
- **MCP4725** - 12-bit DAC with I2C interface

Driver implementations are located in `examples/common`.

## Getting Started

1. Clone the repository
2. Navigate to the project directory
3. Build & flash the project (```idf.py flash monitor```)
4. Observe the serial output

## Requirements
- ESP32/ESP32-S series microcontroller
- CD74HC4067 and MCP4725 hardware components

## Hardware Components

### CD74HC4067
16-channel analog multiplexer for switching between multiple analog inputs/outputs.   
Datasheet: https://www.ti.com/lit/ds/symlink/cd74hc4067.pdf

### MCP4725
12-bit digital-to-analog converter with I2C communication interface.   
Datasheet: https://ww1.microchip.com/downloads/en/devicedoc/22039d.pdf

## Contributing

Please follow the project's coding standards and include appropriate tests for new features.