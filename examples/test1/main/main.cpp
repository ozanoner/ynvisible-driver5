
#include <cinttypes>

#include "app_check.h"
#include "bsp/esp-bsp.h"
#include "cd74hc4067.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mcp4725.hpp"

#define DAC_MAX_VALUE (1ull << 12)  // 12 bits

static const char* TAG = "test1";

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Hello World!");

    app::hal::CD74HC4067 mux;
    app::hal::MCP4725    dac;

    ESP_ERROR_CHECK(mux.init({.s0     = GPIO_NUM_11,
                              .s1     = GPIO_NUM_9,
                              .s2     = GPIO_NUM_14,
                              .s3     = GPIO_NUM_13,
                              .signal = GPIO_NUM_10,
                              .enable = GPIO_NUM_12}));

    ESP_ERROR_CHECK(dac.init({.busHandle  = nullptr,  // reuse from BSP in the demo
                              .i2cPort    = 0,
                              .i2cAddr    = 0x60,
                              .i2cSdaGpio = GPIO_NUM_38,
                              .i2cSclGpio = GPIO_NUM_41,
                              .i2cFreqHz  = 100000}));

    bool     writeVal = true;
    uint16_t dacValue = 0;

    while (true)
    {
        // read from channel-1
        ESP_LOGI(TAG, "Reading from channel-1");
        if ((mux.select(1) == ESP_OK) && (mux.enable() == ESP_OK))
        {
            uint16_t val;
            if (mux.read(val) == ESP_OK)
            {
                ESP_LOGI(TAG, "Channel-1 ADC value: %" PRIu16, val);
            }
            mux.disable();
        }
        vTaskDelay(pdMS_TO_TICKS(10));

        // write to DAC, ie common electrode
        ESP_LOGI(TAG, "Writing to DAC: %" PRIu16 " %f", dacValue, 3.3 * (((float)dacValue) / DAC_MAX_VALUE));
        dac.write(dacValue);
        dacValue += 1023;
        if (dacValue >= DAC_MAX_VALUE)
        {
            dacValue = 0;
        }

        // write to channel-2
        ESP_LOGI(TAG, "Writing to channel-2: %s", writeVal ? "HIGH" : "LOW");
        if ((mux.select(2) == ESP_OK) && (mux.enable() == ESP_OK))
        {
            mux.write(writeVal);
            writeVal = !writeVal;
            vTaskDelay(pdMS_TO_TICKS(3000));
            mux.disable();
        }
        vTaskDelay(pdMS_TO_TICKS(10));

        ESP_LOGI(TAG, "--------------------------------------------------");
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}
