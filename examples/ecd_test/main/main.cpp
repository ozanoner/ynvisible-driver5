/**
 * @file main.cpp
 * @brief An ECD application that shows the component usage
 * @date 2025-10-19
 * @copyright Copyright (c) 2025
 *
 * This application tests electrochromic display (ECD) functionality without GUI,
 * providing a minimal test environment for validating display drivers and
 * animation systems.
 */

#include "app_hal.hpp"
#include "evalkit_anims.hpp"
#include "evalkit_displays.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/**
 * @brief Anonymous namespace containing singleton instances
 *
 * Encapsulates global instances to prevent namespace pollution while
 * maintaining easy access throughout the application.
 */
namespace
{
/** @brief Application configuration structure */
ynv::app::AppConfig_t appConfig;

/** @brief Animation manager singleton */
auto& anims = ynv::anim::EvalkitAnims::getInstance();

/** @brief Display manager singleton */
auto& displays = ynv::ecd::EvalkitDisplays::getInstance();

/** @brief Hardware abstraction layer singleton */
auto& hal = app::hal::HAL::getInstance();
}  // namespace

/**
 * @brief Main application entry point for display testing
 *
 * Initializes the hardware abstraction layer, display system, and runs
 * a continuous animation test loop. This provides a minimal environment
 * for testing ECD functionality without GUI overhead.
 *
 * Test Configuration:
 * - Uses EVALKIT_DISP_TEST display type
 * - Runs ANIM_TOGGLE animation continuously
 * - Updates every 1 second
 *
 * @note Hardware Configuration:
 *       - CD74HC4067 multiplexer on GPIOs 9,11,13,14 (select) + 10 (signal) + 12 (enable)
 *       - MCP4725 DAC on I2C (SDA: GPIO38, SCL: GPIO41, Address: 0x60, 100kHz)
 *
 * @note Driving mode is configurable via menuconfig (CONFIG_ECD_DRIVING_ACTIVE)
 */
extern "C" void app_main(void)
{
    // Configure application settings
    appConfig.hal = &hal;

    // Set driving mode based on build configuration
#ifdef CONFIG_ECD_DRIVING_ACTIVE
    appConfig.activeDriving = true;  ///< Enable active driving for precise control
#else
    appConfig.activeDriving = false;  ///< Use passive driving mode
#endif

    appConfig.analogResolution  = 12;                                          ///< 12-bit ADC resolution
    appConfig.maxSegmentVoltage = ynv::app::AppConfig_t::MAX_SEGMENT_VOLTAGE;  ///< Maximum segment voltage
    appConfig.highPinVoltage    = ynv::app::AppConfig_t::HIGH_PIN_VOLTAGE;     ///< High pin voltage level

    // Initialize hardware abstraction layer
    hal.init(&appConfig,
             // CD74HC4067 multiplexer configuration
             {.s0     = GPIO_NUM_11,   ///< Select bit 0
              .s1     = GPIO_NUM_9,    ///< Select bit 1
              .s2     = GPIO_NUM_14,   ///< Select bit 2
              .s3     = GPIO_NUM_13,   ///< Select bit 3
              .signal = GPIO_NUM_10,   ///< Analog signal pin
              .enable = GPIO_NUM_12},  ///< Enable control pin
             // MCP4725 DAC configuration
             {.busHandle  = nullptr,      ///< I2C bus handle (auto-init)
              .i2cPort    = 0,            ///< I2C port number
              .i2cAddr    = 0x60,         ///< MCP4725 device address
              .i2cSdaGpio = GPIO_NUM_38,  ///< I2C data line
              .i2cSclGpio = GPIO_NUM_41,  ///< I2C clock line
              .i2cFreqHz  = 100000});      ///< 100kHz I2C frequency

    // Initialize display management system
    displays.init(&appConfig);

    // Start test animation - toggle animation on test display
    anims.select(ynv::ecd::EvalkitDisplays::ECDEvalkitDisplay_t::EVALKIT_DISP_TEST,
                 ynv::anim::EvalkitAnims::Anim_t::ANIM_TOGGLE);

    /**
     * Main test loop - continuous animation updates
     *
     * Runs the selected animation indefinitely with 1-second intervals,
     * providing a simple test environment for validating ECD functionality.
     */
    while (true)
    {
        anims.getCurrentAnim().update();  // Update animation state
        vTaskDelay(pdMS_TO_TICKS(1000));  // Wait 1 second between updates
    }
}
