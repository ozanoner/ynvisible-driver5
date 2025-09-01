
#include <cinttypes>

#include "app_check.h"
#include "bsp/esp-bsp.h"
#include "cd74hc4067.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mcp4725.hpp"

static const char* TAG = "test1";

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Hello World!");

    app::hal::CD74HC4067 mux;
    // app::hal::MCP4725    dac;

    ESP_ERROR_CHECK(mux.init({.s0     = GPIO_NUM_11,
                              .s1     = GPIO_NUM_9,
                              .s2     = GPIO_NUM_14,
                              .s3     = GPIO_NUM_13,
                              .signal = GPIO_NUM_10,
                              .enable = GPIO_NUM_12}));

    // dac.init(
    //     {.i2cPort = 0, .i2cAddr = 0x60, .i2cSdaGpio = GPIO_NUM_20, .i2cSclGpio = GPIO_NUM_21, .i2cFreqHz = 100000});

    bool     writeVal = true;
    uint16_t dacValue = 0;

    while (true)
    {
        // read from channel-1
        ESP_LOGI(TAG, "Reading from channel-1");
        if ((mux.select(1) == ESP_OK) && (mux.configureRead() == ESP_OK) && (mux.enable() == ESP_OK))
        {
            uint16_t val = mux.read();
            mux.disable();
            mux.releaseRead();
            ESP_LOGI(TAG, "Channel-1 ADC value: %" PRIu16, val);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
        // write to channel-2
        ESP_LOGI(TAG, "Writing to channel-2");
        if ((mux.select(2) == ESP_OK) && (mux.configureWrite() == ESP_OK) && (mux.enable() == ESP_OK))
        {
            mux.write(writeVal);
            writeVal = !writeVal;
            vTaskDelay(pdMS_TO_TICKS(3000));
            mux.disable();
            mux.releaseWrite();
        }
        vTaskDelay(pdMS_TO_TICKS(10));

        // write to DAC, ie common electrode
        // dac.write(dacValue);
        // dacValue += 100;
        // if (dacValue >= (1 << 12))  // 12 bits
        // {
        //     dacValue = 0;
        // }

        ESP_LOGI(TAG, "--------------------------------------------------");
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}
