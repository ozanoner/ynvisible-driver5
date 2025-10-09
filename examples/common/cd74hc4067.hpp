// CD74HCx4067 High-Speed CMOS Logic 16-Channel Analog Multiplexer and Demultiplexer

#pragma once

#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_err.h"

namespace app
{
namespace hal
{

class CD74HC4067
{
   public:
    struct Config_t
    {
        gpio_num_t s0;
        gpio_num_t s1;
        gpio_num_t s2;
        gpio_num_t s3;
        gpio_num_t signal;
        gpio_num_t enable;
    };

    CD74HC4067() : m_initialised(false), m_adcHandle(nullptr) { }

    esp_err_t init(const Config_t& config);  // Configure pins
    esp_err_t select(uint8_t channel);       // select a channel by using Select pins
    esp_err_t enable();                      // enable a channel by taking Enable low
    esp_err_t disable();                     // high-z, default

    esp_err_t configureRead();        // prep analog read
    esp_err_t read(uint16_t& value);  // analog read
    esp_err_t releaseRead();          // release resources

    esp_err_t configureWrite();  // prep digital write
    esp_err_t write(bool high);  // digital write
    esp_err_t releaseWrite();    // release resources

    static constexpr const char* TAG = "CD74HC4067";

   private:
    Config_t m_config;
    bool     m_initialised;

    adc_oneshot_unit_handle_t m_adcHandle;
    adc_unit_t                m_adcUnit;
    adc_channel_t             m_adcChannel;
};

}  // namespace hal
}  // namespace app
