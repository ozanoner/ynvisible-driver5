/**
 * @file app_config.hpp
 * @brief Application configuration header
 * @version 0.1
 * @date 2025-08-08
 * @copyright Copyright (c) 2025
 */
#pragma once

namespace ynv
{
namespace app
{
/**
 * @brief Application configuration structure for ECD operations
 *
 * Contains hardware and voltage settings for electrochromic display control
 */
struct AppConfig_t
{
    /** @brief ECD driving mode (true=active, false=passive) */
    bool activeDriving;

    /** @brief ADC/DAC resolution in bits */
    int analogResolution;

    /** @brief Maximum segment voltage in DAC units */
    int maxSegmentVoltage;

    /** @brief High pin voltage level in DAC units */
    int highPinVoltage;

    /** @brief Pointer to HAL instance */
    void* hal;

    /** @brief Maximum safe ECD segment voltage (1400mV) */
    static const int MAX_SEGMENT_VOLTAGE = 1400;

    /** @brief ESP32 GPIO high voltage level (3300mV) */
    static const int HIGH_PIN_VOLTAGE = 3300;
};
}  // namespace app
}  // namespace ynv
