#pragma once

#include "app_config.hpp"
#include "cd74hc4067.hpp"
#include "esp_err.h"
#include "mcp4725.hpp"
#include "ynv_hal.hpp"

namespace app
{
namespace hal
{

class HAL : public ynv::driver::HALBase
{
   public:
    // Get the singleton instance
    static HAL& getInstance()
    {
        static HAL instance;
        return instance;
    }

    // Delete copy constructor and assignment operator
    HAL(const HAL&)            = delete;
    HAL& operator=(const HAL&) = delete;

    // Public destructor
    ~HAL() = default;

    esp_err_t init(ynv::app::AppConfig_t* appConfig, const app::hal::CD74HC4067::Config_t& cd74hc4067Config,
                   const app::hal::MCP4725::Config_t& mcp4725Config);  // Initialize the HAL

    esp_err_t digitalWrite(int pin, bool high, int delay = 10, int common = 0) override;
    int       analogRead(int pin) override;

   private:
    // Private constructor
    HAL() : m_mux(), m_dac() { }

    app::hal::CD74HC4067 m_mux;  // CD74HC4067 multiplexer instance
    app::hal::MCP4725    m_dac;  // MCP4725 DAC instance

    // Private members for the application-specific HAL implementation
};

}  // namespace hal
}  // namespace app
