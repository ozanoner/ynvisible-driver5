#include "app_hal.hpp"

#include "app_check.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace app
{
namespace hal
{

esp_err_t HAL::init(ynv::app::AppConfig_t* appConfig, const app::hal::CD74HC4067::Config_t& cd74hc4067Config,
                    const app::hal::MCP4725::Config_t& mcp4725Config)
{
    assert(appConfig != nullptr);
    m_appConfig = appConfig;
    // Set the HAL pointer in the application configuration
    m_appConfig->hal = this;
    // Set high pin voltage based on resolution
    m_appConfig->highPinVoltage = (1 << m_appConfig->analogResolution) - 1;
    // Set max segment voltage based on resolution
    m_appConfig->maxSegmentVoltage = m_appConfig->highPinVoltage * ynv::app::AppConfig_t::MAX_SEGMENT_VOLTAGE /
                                     ynv::app::AppConfig_t::HIGH_PIN_VOLTAGE;
    ESP_LOGI(TAG, "Initializing HAL...");

    esp_err_t err = ESP_OK;

    err = m_mux.init(cd74hc4067Config);
    APP_RETURN_ON_ERROR(err, TAG, "Failed to initialize CD74HC4067");

    err = m_dac.init(mcp4725Config);
    APP_RETURN_ON_ERROR(err, TAG, "Failed to initialize MCP4725");

    ESP_LOGI(TAG, "HAL initialized");

    return err;
}

esp_err_t HAL::digitalWrite(int pin, bool high, int delay, int common)
{
    esp_err_t err = ESP_OK;

    ESP_LOGI(TAG, "digitalWrite: pin=%d, high=%s, delay=%d, common=%d", pin, high ? "true" : "false", delay, common);
    assert(common > 0);
    assert(delay > 0);
    assert(pin > 0 && pin < 16);  // CD74HC4067 has 16 channels (0-15), pin-0 won't be used

    if (high && (m_appConfig->highPinVoltage - common > m_appConfig->maxSegmentVoltage))
    {
        ESP_LOGW(TAG, "common voltage out of range for HIGH pin, adjusting: (%d->%d)", common,
                 m_appConfig->highPinVoltage - m_appConfig->maxSegmentVoltage);
        common = m_appConfig->highPinVoltage - m_appConfig->maxSegmentVoltage;
    }
    if (!high && common > m_appConfig->maxSegmentVoltage)
    {
        ESP_LOGW(TAG, "common voltage out of range for LOW pin, adjusting: (%d->%d)", common,
                 m_appConfig->maxSegmentVoltage);
        common = m_appConfig->maxSegmentVoltage;
    }

    // set the reference voltage on the DAC
    err = m_dac.write((uint16_t)common);
    APP_RETURN_ON_ERROR(err, TAG, "Failed to write common");

    err = m_mux.select(pin);
    APP_RETURN_ON_ERROR(err, TAG, "Failed to select mux channel");

    err = m_mux.enable();
    APP_RETURN_ON_ERROR(err, TAG, "Failed to enable mux");

    err = m_mux.write(high);

    if (err == ESP_OK)
    {
        vTaskDelay(pdMS_TO_TICKS(delay));  // wait for the specified delay
    }
    else
    {
        ESP_LOGE(TAG, "Failed to write to mux");
    }

    (void)m_mux.disable();

    return err;
}

int HAL::analogRead(int pin)
{
    esp_err_t err = ESP_OK;
    uint16_t  val = 0;

    assert(pin > 0 && pin < 16);  // CD74HC4067 has 16 channels (0-15), pin-0 won't be used

    if ((m_mux.select(pin) == ESP_OK) && (m_mux.enable() == ESP_OK))
    {
        err = m_mux.read(val);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to read from mux");
            return -1;
        }

        (void)m_mux.disable();
    }
    else
    {
        ESP_LOGE(TAG, "Failed to configure mux");
        return -1;
    }
    ESP_LOGI(TAG, "analogRead: pin=%d val=%d", pin, val);

    return val;
}

}  // namespace hal
}  // namespace app
