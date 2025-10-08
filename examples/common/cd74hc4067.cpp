
#include "cd74hc4067.hpp"

#include <cassert>

#include "app_check.h"
#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"

namespace app
{
namespace hal
{

esp_err_t CD74HC4067::init(const Config_t& config)
{
    esp_err_t err = ESP_OK;

    m_config = config;

    // configure Enable pin
    gpio_config_t ioConf = {};
    ioConf.pin_bit_mask  = (1ull << (int)m_config.enable);
    ioConf.intr_type     = GPIO_INTR_DISABLE;
    ioConf.mode          = GPIO_MODE_OUTPUT;
    ioConf.pull_down_en  = GPIO_PULLDOWN_DISABLE;
    ioConf.pull_up_en    = GPIO_PULLUP_ENABLE;
    err                  = gpio_config(&ioConf);
    APP_RETURN_ON_ERROR(err, TAG, "Failed to configure Enable pin");

    // configure Select pins
    ioConf              = {};
    ioConf.pin_bit_mask = (1ull << (int)m_config.s0) | (1ull << (int)m_config.s1) | (1ull << (int)m_config.s2) |
                          (1ull << (int)m_config.s3);
    ioConf.intr_type    = GPIO_INTR_DISABLE;
    ioConf.mode         = GPIO_MODE_OUTPUT;
    ioConf.pull_down_en = GPIO_PULLDOWN_ENABLE;
    ioConf.pull_up_en   = GPIO_PULLUP_DISABLE;
    err                 = gpio_config(&ioConf);
    APP_RETURN_ON_ERROR(err, TAG, "Failed to configure Select pins");

    // Signal can be analog input or digital output.
    // read and write will configure the Signal pin when using it
    err = adc_oneshot_io_to_channel(m_config.signal, &m_adcUnit, &m_adcChannel);
    APP_RETURN_ON_ERROR(err, TAG, "Failed to configure Signal pin");

    m_initialised = true;

    return err;
}

esp_err_t CD74HC4067::select(uint8_t channel)
{
    assert(m_initialised);
    assert(channel < 16);

    esp_err_t err = ESP_OK;

    err = gpio_set_level(m_config.s0, (channel >> 0) & 0x01);
    APP_RETURN_ON_ERROR(err, TAG, "Failed to set S0 pin");

    err = gpio_set_level(m_config.s1, (channel >> 1) & 0x01);
    APP_RETURN_ON_ERROR(err, TAG, "Failed to set S1 pin");

    err = gpio_set_level(m_config.s2, (channel >> 2) & 0x01);
    APP_RETURN_ON_ERROR(err, TAG, "Failed to set S2 pin");

    err = gpio_set_level(m_config.s3, (channel >> 3) & 0x01);
    APP_RETURN_ON_ERROR(err, TAG, "Failed to set S3 pin");

    return err;
}

esp_err_t CD74HC4067::enable()
{
    assert(m_initialised);

    esp_err_t err = gpio_set_level(m_config.enable, 0);
    APP_RETURN_ON_ERROR(err, TAG, "enable failed");

    return err;
}

esp_err_t CD74HC4067::disable()
{
    assert(m_initialised);

    esp_err_t err = gpio_set_level(m_config.enable, 1);
    APP_RETURN_ON_ERROR(err, TAG, "disable failed");

    return err;
}

esp_err_t CD74HC4067::configureRead()
{
    assert(m_initialised);

    esp_err_t err = ESP_OK;

    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = m_adcUnit,
    };
    err = adc_oneshot_new_unit(&init_config1, &m_adcHandle);
    APP_RETURN_ON_ERROR(err, TAG, "Failed to create ADC unit");

    //-------------ADC1 Config_t---------------//
    adc_oneshot_chan_cfg_t config = {
        .atten    = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_12,
    };
    err = adc_oneshot_config_channel(m_adcHandle, m_adcChannel, &config);
    APP_RETURN_ON_ERROR(err, TAG, "Failed to configure ADC channel");

    return ESP_OK;
}

esp_err_t CD74HC4067::read(uint16_t& value)
{
    assert(m_initialised);
    assert(m_adcHandle != nullptr);

    int       val;
    esp_err_t err = adc_oneshot_read(m_adcHandle, m_adcChannel, &val);
    APP_RETURN_ON_ERROR(err, TAG, "Failed to read ADC channel");

    value = (uint16_t)val;
    return ESP_OK;
}

esp_err_t CD74HC4067::releaseRead()
{
    assert(m_initialised);

    esp_err_t err = adc_oneshot_del_unit(m_adcHandle);
    APP_RETURN_ON_ERROR(err, TAG, "Failed to release ADC unit");
    m_adcHandle = nullptr;

    return ESP_OK;
}

esp_err_t CD74HC4067::configureWrite()
{
    assert(m_initialised);
    assert(m_adcHandle == nullptr);

    gpio_config_t ioConf = {};
    ioConf.pin_bit_mask  = (1ull << (int)m_config.signal);
    ioConf.intr_type     = GPIO_INTR_DISABLE;
    ioConf.mode          = GPIO_MODE_OUTPUT;
    ioConf.pull_down_en  = GPIO_PULLDOWN_DISABLE;
    ioConf.pull_up_en    = GPIO_PULLUP_ENABLE;

    esp_err_t err = gpio_config(&ioConf);
    APP_RETURN_ON_ERROR(err, TAG, "Failed to configure Signal pin for writing");

    return err;
}

esp_err_t CD74HC4067::write(bool high)
{
    assert(m_initialised);

    esp_err_t err = gpio_set_level(m_config.signal, high);
    APP_RETURN_ON_ERROR(err, TAG, "Failed to set Signal pin");

    return err;
}

esp_err_t CD74HC4067::releaseWrite()
{
    assert(m_initialised);

    esp_err_t err = ESP_OK;

    err = gpio_reset_pin(m_config.signal);
    APP_RETURN_ON_ERROR(err, TAG, "Failed to reset Signal pin");

    err = gpio_set_pull_mode(m_config.signal, GPIO_FLOATING);
    APP_RETURN_ON_ERROR(err, TAG, "Failed to set pull mode for Signal pin");

    err = gpio_set_direction(m_config.signal, GPIO_MODE_DISABLE);
    APP_RETURN_ON_ERROR(err, TAG, "Failed to disable Signal pin");

    return ESP_OK;
}

}  // namespace hal
}  // namespace app
