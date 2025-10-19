/**
 * @file main.cpp
 * @brief HAL test application for CD74HC4067 multiplexer and MCP4725 DAC
 * @date 2025-10-19
 * @copyright Copyright (c) 2025
 *
 * This application shows the hardware abstraction layer usage,
 * specifically the CD74HC4067 16-channel multiplexer and
 * MCP4725 12-bit I2C DAC. It demonstrates basic read/write operations and
 * provides a foundation for writing ECD driving apps.
 */

#include <cinttypes>

#include "app_check.h"
#include "bsp/esp-bsp.h"
#include "cd74hc4067.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mcp4725.hpp"

/** @brief Maximum DAC value for 12-bit resolution (4096) */
#define DAC_MAX_VALUE (1ull << 12)  // 12 bits

/** @brief Log tag for ESP-IDF logging system */
static const char* TAG = "hal_test";

/**
 * @brief Main application entry point for HAL testing
 *
 * Initializes and tests the CD74HC4067 multiplexer and MCP4725 DAC components
 * in a continuous loop. The app performs the following operations:
 *
 * 1. Reads analog values from multiplexer channel 1 (ADC input)
 * 2. Writes incrementing values to the DAC (common electrode simulation)
 * 3. Writes alternating digital values to multiplexer channel 2 (segment control)
 *
 * @note Hardware Configuration:
 *       - CD74HC4067: GPIOs 9,11,13,14 (select) + 10 (signal) + 12 (enable)
 *       - MCP4725: I2C (SDA: GPIO38, SCL: GPIO41, Address: 0x60, 100kHz)
 *
 * @note Test Pattern:
 *       - Channel 1: ADC reading every cycle
 *       - DAC: Increments by 1023 each cycle (0→1023→2046→3069→4092→0...)
 *       - Channel 2: Alternates HIGH/LOW every 3 seconds
 */
extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Hello World!");

    // Initialize hardware components
    app::hal::CD74HC4067 mux;  ///< 16-channel analog multiplexer
    app::hal::MCP4725    dac;  ///< 12-bit I2C DAC for common electrode

    // Configure multiplexer with ESP32-S3-Box3 GPIO assignments
    ESP_ERROR_CHECK(mux.init({
        .s0     = GPIO_NUM_11,  ///< Select bit 0
        .s1     = GPIO_NUM_9,   ///< Select bit 1
        .s2     = GPIO_NUM_14,  ///< Select bit 2
        .s3     = GPIO_NUM_13,  ///< Select bit 3
        .signal = GPIO_NUM_10,  ///< Analog signal pin
        .enable = GPIO_NUM_12   ///< Enable/disable pin
    }));

    // Configure DAC with I2C parameters
    ESP_ERROR_CHECK(dac.init({
        .busHandle  = nullptr,      ///< Use BSP I2C bus
        .i2cPort    = 0,            ///< I2C port 0
        .i2cAddr    = 0x60,         ///< MCP4725 default address
        .i2cSdaGpio = GPIO_NUM_38,  ///< I2C data line
        .i2cSclGpio = GPIO_NUM_41,  ///< I2C clock line
        .i2cFreqHz  = 100000        ///< 100kHz I2C frequency
    }));

    // Test loop variables
    bool     writeVal = true;  ///< Alternating digital output value
    uint16_t dacValue = 0;     ///< Current DAC output value

    /**
     * Main test loop - cycles through HAL operations
     *
     * This infinite loop demonstrates typical ECD driving operations:
     * - Reading segment voltages (via multiplexer ADC)
     * - Setting common electrode voltage (via DAC)
     * - Controlling individual segments (via multiplexer digital output)
     */
    while (true)
    {
        // Test ADC reading through multiplexer channel 1
        ESP_LOGI(TAG, "Reading from channel-1");
        if ((mux.select(1) == ESP_OK) && (mux.enable() == ESP_OK))
        {
            uint16_t val;
            if (mux.read(val) == ESP_OK)
            {
                ESP_LOGI(TAG, "Channel-1 ADC value: %" PRIu16, val);
            }
            mux.disable();  // Always disable after use to prevent interference
        }
        vTaskDelay(pdMS_TO_TICKS(10));

        // Test DAC output for common electrode voltage control
        ESP_LOGI(TAG, "Writing to DAC: %" PRIu16 " %f", dacValue, 3.3 * (((float)dacValue) / DAC_MAX_VALUE));
        dac.write(dacValue);

        // Increment DAC value in steps for voltage sweep testing
        dacValue += 1023;  // Quarter-scale increments
        if (dacValue >= DAC_MAX_VALUE)
        {
            dacValue = 0;  // Wrap around to 0V
        }

        // Test digital output through multiplexer channel 2
        ESP_LOGI(TAG, "Writing to channel-2: %s", writeVal ? "HIGH" : "LOW");
        if ((mux.select(2) == ESP_OK) && (mux.enable() == ESP_OK))
        {
            mux.write(writeVal);
            writeVal = !writeVal;             // Toggle for next cycle
            vTaskDelay(pdMS_TO_TICKS(3000));  // Hold state for 3 seconds
            mux.disable();
        }
        vTaskDelay(pdMS_TO_TICKS(10));

        ESP_LOGI(TAG, "--------------------------------------------------");
        vTaskDelay(pdMS_TO_TICKS(3000));  // 3-second cycle time
    }
}
