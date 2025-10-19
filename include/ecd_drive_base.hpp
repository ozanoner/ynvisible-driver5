/**
 * @file ecd_drive_base.hpp
 * @brief Base classes for electrochromic display driving implementations
 */
#pragma once

#include <array>

#include "esp_log.h"
#include "ynv_hal.hpp"

namespace ynv
{
namespace ecd
{

/**
 * @brief Configuration parameters for ECD driving operations
 *
 * Contains voltage levels, timing parameters, and refresh settings
 * for both coloring and bleaching operations.
 */
struct ECDConfig_t
{
    static constexpr const char* TAG = "ECDConfig";

    int maxAnalogValue;  ///< Maximum analog value based on ADC/DAC resolution

    // Color & Bleach Configs
    int coloringVoltage;  ///< Voltage for coloring operation (analog units)
    int coloringTime;     ///< Duration for coloring pulse (ms)

    int bleachingVoltage;  ///< Voltage for bleaching operation (analog units)
    int bleachingTime;     ///< Duration for bleaching pulse (ms)

    // Refresh Configs
    int refreshColoringVoltage;  ///< Voltage for color refresh pulses
    int refreshColorPulseTime;   ///< Duration of color refresh pulse (ms)

    int refreshColorLimitHVoltage;  ///< High voltage threshold for color refresh
    int refreshColorLimitLVoltage;  ///< Low voltage threshold for color refresh

    int refreshBleachingVoltage;  ///< Voltage for bleach refresh pulses
    int refreshBleachPulseTime;   ///< Duration of bleach refresh pulse (ms)

    int refreshBleachLimitHVoltage;  ///< High voltage threshold for bleach refresh
    int refreshBleachLimitLVoltage;  ///< Low voltage threshold for bleach refresh

    /**
     * @brief Print configuration parameters to log
     */
    void print() const
    {
        ESP_LOGI(TAG, "-------------------------------------------------------------");
        ESP_LOGI(TAG, "Parameter                  | Value");
        ESP_LOGI(TAG, "-------------------------------------------------------------");
        ESP_LOGI(TAG, "maxAnalogValue             | %d", maxAnalogValue);
        ESP_LOGI(TAG, "coloringVoltage            | %d", coloringVoltage);
        ESP_LOGI(TAG, "coloringTime               | %d", coloringTime);
        ESP_LOGI(TAG, "bleachingVoltage           | %d", bleachingVoltage);
        ESP_LOGI(TAG, "bleachingTime              | %d", bleachingTime);
        ESP_LOGI(TAG, "refreshColoringVoltage     | %d", refreshColoringVoltage);
        ESP_LOGI(TAG, "refreshColorPulseTime      | %d", refreshColorPulseTime);
        ESP_LOGI(TAG, "refreshColorLimitHVoltage  | %d", refreshColorLimitHVoltage);
        ESP_LOGI(TAG, "refreshColorLimitLVoltage  | %d", refreshColorLimitLVoltage);
        ESP_LOGI(TAG, "refreshBleachingVoltage    | %d", refreshBleachingVoltage);
        ESP_LOGI(TAG, "refreshBleachPulseTime     | %d", refreshBleachPulseTime);
        ESP_LOGI(TAG, "refreshBleachLimitHVoltage | %d", refreshBleachLimitHVoltage);
        ESP_LOGI(TAG, "refreshBleachLimitLVoltage | %d", refreshBleachLimitLVoltage);
        ESP_LOGI(TAG, "-------------------------------------------------------------");
    }
};

/**
 * @brief Abstract base class for ECD driving implementations
 * @tparam SEGMENT_COUNT Number of display segments
 *
 * Provides common interface and data members for different driving
 * strategies (active, passive). Derived classes implement specific
 * driving algorithms.
 */
template <int SEGMENT_COUNT>
class ECDDriveBase
{
   public:
    /**
     * @brief Constructor
     * @param config ECD configuration parameters
     * @param pins Array of GPIO pin numbers for segments
     * @param hal Hardware abstraction layer instance
     */
    explicit ECDDriveBase(const ECDConfig_t* config, const std::array<int, SEGMENT_COUNT>* pins,
                          ynv::driver::HALBase* hal)
        : m_config(config), m_pins(pins), m_hal(hal)
    {
    }

    virtual ~ECDDriveBase() = default;

    /**
     * @brief Drive ECD segments from current to next states
     * @param currentStates Current segment states (modified in-place)
     * @param nextStates Target segment states
     *
     * Pure virtual function implemented by derived classes to perform
     * the actual driving operations based on their specific algorithms.
     */
    virtual void drive(std::array<bool, SEGMENT_COUNT>&       currentStates,
                       const std::array<bool, SEGMENT_COUNT>& nextStates) = 0;

   protected:
    static constexpr const char* TAG = "ECDDrive";

    const ECDConfig_t*                    m_config;  ///< ECD configuration parameters
    const std::array<int, SEGMENT_COUNT>* m_pins;    ///< GPIO pin assignments for segments
    ynv::driver::HALBase*                 m_hal;     ///< Hardware abstraction layer
};
}  // namespace ecd
}  // namespace ynv