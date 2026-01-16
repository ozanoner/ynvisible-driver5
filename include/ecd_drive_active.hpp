/**
 * @file ecd_drive_active.hpp
 * @brief Active driving implementation for ECDs with voltage feedback and refresh control.
 */

#pragma once

#include <algorithm>
#include <array>
#include <vector>

#include "ecd_drive_base.hpp"
#include "esp_log.h"

namespace ynv
{
namespace ecd
{
/**
 * @brief Active ECD driver with voltage feedback and refresh control
 * @tparam SEGMENT_COUNT Number of display segments
 *
 * Provides precise voltage control with analog feedback monitoring
 * and automatic refresh operations to maintain display state.
 */
template <int SEGMENT_COUNT>
class ECDDriveActive : public ECDDriveBase<SEGMENT_COUNT>
{
   private:
    std::vector<int> m_colorPins;          ///< Pins requiring coloring operation
    std::vector<int> m_bleachPins;         ///< Pins requiring bleaching operation
    std::vector<int> m_colorRefreshPins;   ///< Pins needing color refresh
    std::vector<int> m_bleachRefreshPins;  ///< Pins needing bleach refresh

    void doColorBleach()
    {
        bool done {m_colorPins.size() == 0 && m_bleachPins.size() == 0};
        int  retries {0};

        if (done)
        {
            ESP_LOGI(TAG, "No segments to color or bleach");
            return;
        }

        // Execute state changes
        for (const auto& pin : m_colorPins)
        {
            m_hal->digitalWrite(pin, true, m_config->coloringTime,
                                (m_config->maxAnalogValue - m_config->coloringVoltage));
        }
        for (const auto& pin : m_bleachPins)
        {
            m_hal->digitalWrite(pin, false, m_config->bleachingTime, m_config->bleachingVoltage);
        }

        // measure OCP and apply color/bleach voltage if needed
        do
        {
            // Remove segments that have reached target voltage
            m_colorPins.erase(std::remove_if(m_colorPins.begin(), m_colorPins.end(),
                                             [&](int pin)
                                             {
                                                 int analogVal = m_hal->analogRead(pin);
                                                 return analogVal >= m_config->coloringVoltage;
                                             }),
                              m_colorPins.end());

            m_bleachPins.erase(std::remove_if(m_bleachPins.begin(), m_bleachPins.end(),
                                              [&](int pin)
                                              {
                                                  int analogVal = m_hal->analogRead(pin);
                                                  return analogVal <= m_config->bleachingVoltage;
                                              }),
                               m_bleachPins.end());

            done = (m_colorPins.size() == 0 && m_bleachPins.size() == 0);

            if (!done)
            {
                // Re-apply color/bleach pulses to remaining segments
                for (const auto& pin : m_colorPins)
                {
                    m_hal->digitalWrite(pin, true, m_config->coloringTime,
                                        (m_config->maxAnalogValue - m_config->coloringVoltage));
                }

                for (const auto& pin : m_bleachPins)
                {
                    m_hal->digitalWrite(pin, false, m_config->bleachingTime, m_config->bleachingVoltage);
                }
            }
        } while (!done && ++retries < MAX_RETRIES);

        if (!done)
        {
            ESP_LOGW(TAG, "Color/Bleach operation did not complete within %d retries", MAX_RETRIES);
        }
    }

    void doRefresh()
    {
        // Remove segments that are already within target voltage ranges
        m_colorRefreshPins.erase(std::remove_if(m_colorRefreshPins.begin(), m_colorRefreshPins.end(),
                                                [&](int pin)
                                                {
                                                    int analogVal = m_hal->analogRead(pin);
                                                    return analogVal >= m_config->refreshColorLimitLVoltage;
                                                }),
                                 m_colorRefreshPins.end());

        m_bleachRefreshPins.erase(std::remove_if(m_bleachRefreshPins.begin(), m_bleachRefreshPins.end(),
                                                 [&](int pin)
                                                 {
                                                     int analogVal = m_hal->analogRead(pin);
                                                     return analogVal <= m_config->refreshBleachLimitHVoltage;
                                                 }),
                                  m_bleachRefreshPins.end());

        // Refresh loop with voltage monitoring
        bool done {m_colorRefreshPins.size() == 0 && m_bleachRefreshPins.size() == 0};
        int  retries {0};

        if (done)
        {
            ESP_LOGI(TAG, "No segments to refresh");
            return;
        }

        do
        {
            // Apply refresh pulses to remaining segments
            for (const auto& pin : m_colorRefreshPins)
            {
                m_hal->digitalWrite(pin, true, m_config->refreshColorPulseTime,
                                    (m_config->maxAnalogValue - m_config->refreshColoringVoltage));
            }

            for (const auto& pin : m_bleachRefreshPins)
            {
                m_hal->digitalWrite(pin, false, m_config->refreshBleachPulseTime, m_config->refreshBleachingVoltage);
            }

            // Remove segments that have reached target voltage
            m_colorRefreshPins.erase(std::remove_if(m_colorRefreshPins.begin(), m_colorRefreshPins.end(),
                                                    [&](int pin)
                                                    {
                                                        int analogVal = m_hal->analogRead(pin);
                                                        return analogVal >= m_config->refreshColorLimitHVoltage;
                                                    }),
                                     m_colorRefreshPins.end());

            m_bleachRefreshPins.erase(std::remove_if(m_bleachRefreshPins.begin(), m_bleachRefreshPins.end(),
                                                     [&](int pin)
                                                     {
                                                         int analogVal = m_hal->analogRead(pin);
                                                         return analogVal <= m_config->refreshBleachLimitLVoltage;
                                                     }),
                                      m_bleachRefreshPins.end());

            done = (m_colorRefreshPins.size() == 0 && m_bleachRefreshPins.size() == 0);
        } while (!done && ++retries < MAX_REFRESH_RETRIES);

        if (!done)
        {
            ESP_LOGW(TAG, "Refresh operation did not complete within %d retries", MAX_REFRESH_RETRIES);
        }
    }

   public:
    ~ECDDriveActive() = default;

    using ECDDriveBase<SEGMENT_COUNT>::TAG;
    using ECDDriveBase<SEGMENT_COUNT>::ECDDriveBase;  // Inherit constructors
    using ECDDriveBase<SEGMENT_COUNT>::m_pins;
    using ECDDriveBase<SEGMENT_COUNT>::m_config;
    using ECDDriveBase<SEGMENT_COUNT>::m_hal;

    /** @brief Maximum refresh attempts before timeout */
    static constexpr int MAX_REFRESH_RETRIES = 30;

    /** @brief Maximum retries for color/bleach operations */
    static constexpr int MAX_RETRIES = 3;

    /**
     * @brief Drive ECD segments with active voltage monitoring
     * @param currentStates Current segment states (modified in-place)
     * @param nextStates Target segment states
     *
     * Performs state transitions with voltage feedback and automatic
     * refresh operations to ensure reliable display operation.
     */
    void drive(std::array<bool, SEGMENT_COUNT>&       currentStates,
               const std::array<bool, SEGMENT_COUNT>& nextStates) override
    {
        // Clear and reserve pin vectors for efficiency
        m_colorPins.clear();
        m_bleachPins.clear();
        m_colorRefreshPins.clear();
        m_bleachRefreshPins.clear();

        m_colorPins.reserve(SEGMENT_COUNT);
        m_bleachPins.reserve(SEGMENT_COUNT);
        m_colorRefreshPins.reserve(SEGMENT_COUNT);
        m_bleachRefreshPins.reserve(SEGMENT_COUNT);

        // Categorize segments by required operation
        for (int i = 0; i < SEGMENT_COUNT; ++i)
        {
            if (currentStates[i] == nextStates[i])
            {  // Refresh existing state
                if (currentStates[i])
                {
                    m_colorRefreshPins.push_back((*m_pins)[i]);
                }
                else
                {
                    m_bleachRefreshPins.push_back((*m_pins)[i]);
                }
            }
            else
            {  // Change state
                if (nextStates[i])
                {
                    m_colorPins.push_back((*m_pins)[i]);
                }
                else
                {
                    m_bleachPins.push_back((*m_pins)[i]);
                }
                currentStates[i] = nextStates[i];
            }
        }

        doColorBleach();
        doRefresh();
    }
};
}  // namespace ecd
}  // namespace ynv