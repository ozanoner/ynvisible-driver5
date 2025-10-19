/**
 * @file ynv_hal.hpp
 * @brief Hardware Abstraction Layer (HAL) base class for ECD operations
 */
#pragma once

#include "app_config.hpp"
#include "esp_err.h"

namespace ynv
{
namespace driver
{
/**
 * @brief Abstract base class for hardware abstraction layer
 *
 * Provides unified interface for digital/analog I/O operations
 * required for electrochromic display control.
 */
class HALBase
{
   public:
    /**
     * @brief Write digital value to a pin with timing control
     * @param pin Pin number to write to
     * @param high Logic level (true=HIGH, false=LOW)
     * @param delay Duration to hold state (milliseconds)
     * @param common Common electrode voltage (DAC units)
     * @return ESP_OK on success, error code otherwise
     */
    virtual esp_err_t digitalWrite(int pin, bool high, int delay = 10, int common = 0) = 0;

    /**
     * @brief Read analog value from a pin
     * @param pin Pin number to read from
     * @return Analog value (DAC units), or <0 on error
     */
    virtual int analogRead(int pin) = 0;

    static constexpr const char* TAG = "HAL";

   protected:
    ynv::app::AppConfig_t* m_appConfig;  ///< Application configuration pointer
};

}  // namespace driver
}  // namespace ynv
