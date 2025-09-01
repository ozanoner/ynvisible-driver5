#pragma once

#include <cinttypes>

#include "driver/gpio.h"
#include "driver/i2c_master.h"
#include "esp_err.h"

namespace app
{
namespace hal
{

class MCP4725
{
   public:
    struct Config
    {
        uint8_t    i2cPort;     // I2C port number
        uint8_t    i2cAddr;     // I2C address of the MCP4725
        gpio_num_t i2cSdaGpio;  // GPIO number for I2C SDA
        gpio_num_t i2cSclGpio;  // GPIO number for I2C SCL
        uint32_t   i2cFreqHz;   // I2C frequency in Hz
    };

    MCP4725() : m_config(), m_initialized(false), m_i2cBusHandle(nullptr), m_devHandle(nullptr) { }
    ~MCP4725();

    esp_err_t init(const Config& config);

    esp_err_t write(uint16_t value);

   private:
    Config                  m_config;
    bool                    m_initialized;
    i2c_master_bus_handle_t m_i2cBusHandle;
    i2c_master_dev_handle_t m_devHandle;
};

}  // namespace hal
}  // namespace app