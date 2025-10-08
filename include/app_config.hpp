/**
 * @file app_config.h
 * @brief  Application configuration header
 * This header defines the application configuration structure used throughout the library.
 * @version 0.1
 * @date 2025-08-08
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

namespace ynv
{
namespace app
{
struct AppConfig_t
{
    // Specify the display type attached to the EvalKit v5
    int displayIndex;  // Use ynv::ecd::EvalkitDisplays::ECDEvalkitDisplay_t enum for this
    // Define the ECD driving algorithm. true for active driving, false for passive driving
    bool activeDriving;
    // ADC/DAC resolution
    // Don't forget to set maxSegmentVoltage and highPinVoltage accordingly
    int analogResolution;
    // Max segment voltage mapped to the DAC resolution (mV)
    int maxSegmentVoltage;
    // High pin voltage (mV) mapped to the DAC resolution
    int highPinVoltage;
    // Pointer to hardware abstraction layer (HAL) instance
    void *hal;

    static const int MAX_SEGMENT_VOLTAGE = 1400;  // Maximum voltage to apply on an ECD segment (mV)
    static const int HIGH_PIN_VOLTAGE    = 3300;  // ESP32 pin HIGH value (mV)
};
}  // namespace app
}  // namespace ynv
