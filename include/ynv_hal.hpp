#pragma once

#include "app_config.hpp"
#include "esp_err.h"

namespace ynv
{
namespace driver
{
class HALBase
{
   public:
    /**
     * @brief Write a digital value to a pin
     * @param pin Pin number to write to
     * @param high Logic level to write (true for HIGH, false for LOW)
     * @param delay Duration to hold the state (in milliseconds)
     * @param common Common voltage level for the ECD segment (in analog resolution units - not mV)
     * @return esp_err_t ESP_OK on success, error code otherwise
     */
    virtual esp_err_t digitalWrite(int pin, bool high, int delay = 10, int common = 0) = 0;

    /**
     * @brief Read an analog value from a pin
     * @param pin Pin number to read from
     * @return int Analog value read from the pin (in analog resolution units), or <0 on error
     */
    virtual int analogRead(int pin) = 0;

    static constexpr const char* TAG = "HAL";

   protected:
    ynv::app::AppConfig_t* m_appConfig;  // Pointer to application configuration
};

}  // namespace driver
}  // namespace ynv
