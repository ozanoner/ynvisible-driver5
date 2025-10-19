/**
 * @file main.cpp
 * @brief Application entry point for YnVisible EvalKit Demo
 * @date 2025-10-19
 * @copyright Copyright (c) 2025
 *
 * This file contains the main application logic for the YnVisible EvalKit demonstration
 * using ESP32-S3-Box3 development board. It initializes hardware components, sets up
 * the GUI, and manages electrochromic display animations.
 */

#include "app_gui.hpp"
#include "app_hal.hpp"
#include "bsp/esp-bsp.h"
#include "esp_log.h"
#include "evalkit_anims.hpp"
#include "evalkit_displays.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/**
 * @brief Anonymous namespace containing global singleton instances
 *
 * This namespace encapsulates the singleton instances used throughout the application
 * to prevent global namespace pollution while maintaining easy access.
 */
namespace
{
/** @brief GUI singleton instance for user interface management */
app::GUI& m_gui = app::GUI::getInstance();

/** @brief Application configuration structure */
ynv::app::AppConfig_t appConfig;

/** @brief Animation manager singleton instance */
auto& anims = ynv::anim::EvalkitAnims::getInstance();

/** @brief Display manager singleton instance */
auto& displays = ynv::ecd::EvalkitDisplays::getInstance();

/** @brief Hardware abstraction layer singleton instance */
auto& hal = app::hal::HAL::getInstance();
}  // namespace

/**
 * @brief FreeRTOS task for updating electrochromic display animations
 *
 * This task runs continuously and updates the current animation state every second.
 * It checks if an animation is selected and calls its update method to advance
 * the animation sequence.
 *
 * @param pvParameters Task parameters (unused)
 *
 * @note This task has a 1-second update interval (1000ms)
 * @note Task priority is set to 5 in app_main()
 * @note Task stack size is 4096 bytes
 */
static void animTask(void* pvParameters);

/**
 * @brief Main application entry point
 *
 * Initializes all hardware components and software modules required for the
 * YnVisible EvalKit demonstration:
 *
 * 1. Configures application settings (driving mode, voltage levels, etc.)
 * 2. Initializes I2C bus for communication with DAC
 * 3. Sets up GUI system with touch interface
 * 4. Configures HAL with multiplexer and DAC settings
 * 5. Initializes ECD display management
 * 6. Registers button event handlers for animation control
 * 7. Creates FreeRTOS task for animation updates
 *
 * @note Hardware Configuration:
 *       - CD74HC4067 multiplexer on GPIOs 9,11,13,14 (select) + 10 (signal) + 12 (enable)
 *       - MCP4725 DAC on I2C (SDA: GPIO38, SCL: GPIO41, Address: 0x60)
 *       - I2C frequency: 100kHz
 *
 * @note Software Configuration:
 *       - Active driving mode enabled
 *       - 12-bit analog resolution for voltage measurements
 *       - Maximum segment voltage and high pin voltage set to default values
 */
extern "C" void app_main(void)
{
    // Configure application settings
    appConfig.hal               = &hal;
    appConfig.activeDriving     = true;  ///< Enable active driving for precise ECD control
    appConfig.analogResolution  = 12;    ///< Use 12-bit ADC resolution
    appConfig.maxSegmentVoltage = ynv::app::AppConfig_t::MAX_SEGMENT_VOLTAGE;  ///< Set maximum allowed segment voltage
    appConfig.highPinVoltage    = ynv::app::AppConfig_t::HIGH_PIN_VOLTAGE;     ///< Set high pin voltage level

    // Initialize I2C bus for DAC communication
    bsp_i2c_init();

    // Initialize graphical user interface
    m_gui.init();

    // Initialize hardware abstraction layer with multiplexer and DAC configuration
    hal.init(&appConfig,
             // CD74HC4067 16-channel multiplexer configuration
             {.s0     = GPIO_NUM_11,   ///< Multiplexer select bit 0
              .s1     = GPIO_NUM_9,    ///< Multiplexer select bit 1
              .s2     = GPIO_NUM_14,   ///< Multiplexer select bit 2
              .s3     = GPIO_NUM_13,   ///< Multiplexer select bit 3
              .signal = GPIO_NUM_10,   ///< Multiplexer analog signal pin
              .enable = GPIO_NUM_12},  ///< Multiplexer enable pin
             // MCP4725 12-bit I2C DAC configuration
             {.busHandle  = nullptr,      ///< I2C bus handle (will be initialized)
              .i2cPort    = 0,            ///< I2C port number
              .i2cAddr    = 0x60,         ///< MCP4725 I2C device address
              .i2cSdaGpio = GPIO_NUM_38,  ///< I2C data line GPIO
              .i2cSclGpio = GPIO_NUM_41,  ///< I2C clock line GPIO
              .i2cFreqHz  = 100000});      ///< I2C bus frequency (100kHz)

    // Initialize electrochromic display management
    displays.init(&appConfig);

    // Register GUI button event handler for animation selection
    m_gui.registerButtonHandler(
        [](const app::disp::DisplayAnimInfo_t* info)
        {
            if (info != nullptr)
            {
                // Start the selected animation on the specified display
                anims.select(info->displayType, info->animType);
            }
        });

    // Create FreeRTOS task for animation updates
    (void)xTaskCreate(animTask,       ///< Task function pointer
                      "anim-update",  ///< Task name for debugging
                      4096,           ///< Stack size in bytes
                      nullptr,        ///< Task parameters (none)
                      5,              ///< Task priority (0-24, higher = more priority)
                      nullptr);       ///< Task handle (not needed)
}

static void animTask(void* pvParameters)
{
    /**
     * Main animation update loop
     * Continuously checks for active animations and updates their state.
     */
    while (true)
    {
        // Check if an animation is currently selected and active
        if (anims.isSelected())
        {
            // Update the current animation state (advance to next frame/step)
            anims.getCurrentAnim().update();
        }

        // Wait 1 second before next update (1000ms in FreeRTOS ticks)
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
