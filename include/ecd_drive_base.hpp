#pragma once

#include <array>

#include "esp_log.h"
#include "ynv_hal.hpp"

namespace ynv
{
namespace ecd
{

struct ECDConfig_t
{
    static constexpr const char* TAG = "ECDConfig";

    int maxAnalogValue;  // Maximum analog value for the ECD

    // Color & Bleach Configs
    int coloringVoltage;  // V - Color Pulse Voltage mapped to ADC_DAC_MAX_LSB
    int coloringTime;     // ms - Color time

    int bleachingVoltage;  // V - Bleach Pulse Voltage mapped to ADC_DAC_MAX_LSB
    int bleachingTime;     // ms - Bleach time

    // Refresh Configs
    int refreshColoringVoltage;  // V - Refresh Coloring Pulses Voltage mapped to ADC_DAC_MAX_LSB
    int refreshColorPulseTime;   // ms - Delay between each Refresh Pulse

    int refreshColorLimitHVoltage;
    int refreshColorLimitLVoltage;

    int refreshBleachingVoltage;  // V - Refresh Bleaching Pulses Voltage mapped to ADC_DAC_MAX_LSB
    int refreshBleachPulseTime;

    int refreshBleachLimitHVoltage;
    int refreshBleachLimitLVoltage;

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

template <int SEGMENT_COUNT>
class ECDDriveBase
{
   public:
    ECDDriveBase(const ECDConfig_t*                    config,  // ECD config
                 const std::array<int, SEGMENT_COUNT>* pins,    // ECD pins
                 ynv::driver::HALBase*                 hal      // underlying hardware to control pins
                 )
        : m_config(config), m_pins(pins), m_hal(hal)
    {
    }

    virtual ~ECDDriveBase() = default;

    virtual void drive(std::array<bool, SEGMENT_COUNT>&       currentStates,  // current states of the segments
                       const std::array<bool, SEGMENT_COUNT>& nextStates      // next states of the segments
                       ) = 0;

   protected:
    const ECDConfig_t*                    m_config;  // Configuration for the ECD
    const std::array<int, SEGMENT_COUNT>* m_pins;    // Pin numbers for the segments
    ynv::driver::HALBase*                 m_hal;     // Underlying hardware to control pins
};
}  // namespace ecd
}  // namespace ynv