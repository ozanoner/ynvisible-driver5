
#include "mcp4725.hpp"

#include <cassert>

#include "app_check.h"
#include "driver/i2c_master.h"

namespace app
{
namespace hal
{

MCP4725::~MCP4725()
{
    if (m_initialized && m_config.busHandle == nullptr)
    {
        // Deinitialize the I2C bus
        i2c_del_master_bus(m_i2cBusHandle);
    }
}

esp_err_t MCP4725::init(const Config& config)
{
    esp_err_t err = ESP_OK;

    if (m_initialized)
    {
        return ESP_OK;
    }

    m_config = config;

    if (m_config.busHandle != nullptr)
    {
        m_i2cBusHandle = m_config.busHandle;
        ESP_LOGI(TAG, "Reusing existing I2C bus");
    }
    else
    {
        i2c_master_bus_config_t masterCfg = {
            .i2c_port          = m_config.i2cPort,
            .sda_io_num        = m_config.i2cSdaGpio,
            .scl_io_num        = m_config.i2cSclGpio,
            .clk_source        = I2C_CLK_SRC_DEFAULT,
            .glitch_ignore_cnt = 7,
        };

        err = i2c_new_master_bus(&masterCfg, &m_i2cBusHandle);
        APP_RETURN_ON_ERROR(err, TAG, "Failed to create I2C master bus");
    }

    i2c_device_config_t devCfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address  = (uint16_t)m_config.i2cAddr,
        .scl_speed_hz    = m_config.i2cFreqHz,
    };

    err = i2c_master_bus_add_device(m_i2cBusHandle, &devCfg, &m_devHandle);
    APP_RETURN_ON_ERROR(err, TAG, "Failed to add I2C device");

    // Initialize the I2C bus and the MCP4725
    m_initialized = true;
    return err;
}

esp_err_t MCP4725::write(uint16_t value)
{
    assert(m_initialized);
    assert(value < (1 << 12));  // 12-bit DAC

    // see the datasheet, p25: Write DAC Register: (C2, C1, C0)=0,1,0,0 -> first byte is 0x40
    uint8_t data[] = {(0x40), (uint8_t)(value >> 4), (uint8_t)(value << 4)};

    ESP_ERROR_CHECK(i2c_master_transmit(m_devHandle, data, sizeof(data), -1));
    return ESP_OK;
}

}  // namespace hal
}  // namespace app
