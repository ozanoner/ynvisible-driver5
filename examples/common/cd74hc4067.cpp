
#include "cd74hc4067.hpp"

#include <cassert>

#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_check.h"
#include "esp_err.h"
#include "esp_log.h"

namespace app
{
namespace hal
{

esp_err_t CD74HC4067::init(const Config& config)
{
    m_config = config;

    // configure Enable pin
    gpio_config_t ioConf = {};
    ioConf.pin_bit_mask  = (1ull << (int)m_config.enable);
    ioConf.intr_type     = GPIO_INTR_DISABLE;
    ioConf.mode          = GPIO_MODE_OUTPUT;
    ioConf.pull_down_en  = GPIO_PULLDOWN_DISABLE;
    ioConf.pull_up_en    = GPIO_PULLUP_ENABLE;
    gpio_config(&ioConf);

    // configure Select pins
    ioConf              = {};
    ioConf.pin_bit_mask = (1ull << (int)m_config.s0) | (1ull << (int)m_config.s1) | (1ull << (int)m_config.s2) |
                          (1ull << (int)m_config.s3);
    ioConf.intr_type    = GPIO_INTR_DISABLE;
    ioConf.mode         = GPIO_MODE_OUTPUT;
    ioConf.pull_down_en = GPIO_PULLDOWN_ENABLE;
    ioConf.pull_up_en   = GPIO_PULLUP_DISABLE;
    gpio_config(&ioConf);

    // Signal can be analog input or digital output.
    // read and write will configure the Signal pin when using it
    ESP_ERROR_CHECK(adc_oneshot_io_to_channel(m_config.signal, &m_adcUnit, &m_adcChannel));
    m_initialised = true;

    return ESP_OK;
}

esp_err_t CD74HC4067::select(uint8_t channel)
{
    assert(m_initialised);
    assert(channel < 16);

    gpio_set_level(m_config.s0, (channel >> 0) & 0x01);
    gpio_set_level(m_config.s1, (channel >> 1) & 0x01);
    gpio_set_level(m_config.s2, (channel >> 2) & 0x01);
    gpio_set_level(m_config.s3, (channel >> 3) & 0x01);
    return ESP_OK;
}

esp_err_t CD74HC4067::enable()
{
    assert(m_initialised);

    gpio_set_level(m_config.enable, 0);
    return ESP_OK;
}

esp_err_t CD74HC4067::disable()
{
    assert(m_initialised);

    gpio_set_level(m_config.enable, 1);
    return ESP_OK;
}

esp_err_t CD74HC4067::configureRead()
{
    assert(m_initialised);

    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = m_adcUnit,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &m_adcHandle));

    //-------------ADC1 Config---------------//
    adc_oneshot_chan_cfg_t config = {
        .atten    = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_12,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(m_adcHandle, m_adcChannel, &config));
    return ESP_OK;
}

uint16_t CD74HC4067::read()
{
    assert(m_initialised);

    int val;
    ESP_ERROR_CHECK(adc_oneshot_read(m_adcHandle, m_adcChannel, &val));
    return (uint16_t)val;
}

esp_err_t CD74HC4067::releaseRead()
{
    assert(m_initialised);

    ESP_ERROR_CHECK(adc_oneshot_del_unit(m_adcHandle));
    return ESP_OK;
}

esp_err_t CD74HC4067::configureWrite()
{
    assert(m_initialised);

    gpio_config_t ioConf = {};
    ioConf.pin_bit_mask  = (1ull << (int)m_config.signal);
    ioConf.intr_type     = GPIO_INTR_DISABLE;
    ioConf.mode          = GPIO_MODE_OUTPUT;
    ioConf.pull_down_en  = GPIO_PULLDOWN_DISABLE;
    ioConf.pull_up_en    = GPIO_PULLUP_ENABLE;
    return gpio_config(&ioConf);
}

esp_err_t CD74HC4067::write(bool high)
{
    assert(m_initialised);

    return gpio_set_level(m_config.signal, high);
}

esp_err_t CD74HC4067::releaseWrite()
{
    assert(m_initialised);

    ESP_ERROR_CHECK(gpio_reset_pin(m_config.signal));
    ESP_ERROR_CHECK(gpio_set_pull_mode(m_config.signal, GPIO_FLOATING));
    ESP_ERROR_CHECK(gpio_set_direction(m_config.signal, GPIO_MODE_DISABLE));
    return ESP_OK;
}

}  // namespace hal
}  // namespace app
