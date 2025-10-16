#pragma once

#include <array>
#include <vector>

#include "ecd_drive_base.hpp"

namespace ynv
{
namespace ecd
{
template <int SEGMENT_COUNT>
class ECDDriveActive : public ECDDriveBase<SEGMENT_COUNT>
{
   public:
    ~ECDDriveActive() = default;

    using ECDDriveBase<SEGMENT_COUNT>::ECDDriveBase;  // Inherit constructors
    using ECDDriveBase<SEGMENT_COUNT>::m_pins;
    using ECDDriveBase<SEGMENT_COUNT>::m_config;
    using ECDDriveBase<SEGMENT_COUNT>::m_hal;

    static constexpr int MAX_REFRESH_RETRIES = 30;  // Maximum retries for refresh operations

    void drive(std::array<bool, SEGMENT_COUNT>&       currentStates,
               const std::array<bool, SEGMENT_COUNT>& nextStates) override
    {
        std::vector<int> tmp(SEGMENT_COUNT);

        m_bleachPins.clear();
        m_colorPins.clear();
        m_bleachRefreshPins.clear();
        m_colorRefreshPins.clear();

        int analogVal = 0;

        for (int i = 0; i < SEGMENT_COUNT; ++i)
        {
            if (currentStates[i] == nextStates[i])
            {  // refresh
                analogVal = m_hal->analogRead((*m_pins)[i]);

                if (currentStates[i] && (analogVal < m_config->refreshColorLimitLVoltage))
                {
                    m_colorRefreshPins.push_back((*m_pins)[i]);
                }
                else if (!currentStates[i] && (analogVal > m_config->refreshBleachLimitHVoltage))
                {
                    m_bleachRefreshPins.push_back((*m_pins)[i]);
                }
            }
            else
            {  // change state
                if (nextStates[i])
                {
                    m_colorPins.push_back((*m_pins)[i]);
                }
                else
                {
                    m_bleachPins.push_back((*m_pins)[i]);
                }
                currentStates[i] = nextStates[i];  // Update the current state
            }
        }

        if (m_colorPins.size() > 0)
        {
            for (const auto& pin : m_colorPins)
            {
                m_hal->digitalWrite(pin, true, m_config->coloringTime, m_config->coloringVoltage);
            }
        }

        if (m_bleachPins.size() > 0)
        {
            for (const auto& pin : m_bleachPins)
            {
                m_hal->digitalWrite(pin, false, m_config->bleachingTime, m_config->bleachingVoltage);
            }
        }

        if (m_colorRefreshPins.size() > 0)
        {
            bool done {false};
            int  retries {0};

            while (!done && retries < MAX_REFRESH_RETRIES)
            {
                for (const auto& pin : m_colorRefreshPins)
                {
                    m_hal->digitalWrite(pin, true, m_config->refreshColorPulseTime, m_config->refreshColoringVoltage);
                }

                tmp = m_colorRefreshPins;

                m_colorRefreshPins.clear();
                for (const auto& pin : tmp)
                {
                    analogVal = m_hal->analogRead(pin);
                    if (analogVal < m_config->refreshColorLimitHVoltage)
                    {
                        m_colorRefreshPins.push_back(pin);
                    }
                }

                done = (m_colorRefreshPins.size() == 0);
                ++retries;
            }

            if (!done)
            {
                // Serial.println("WARNING: Color refresh failed");
            }
        }

        if (m_bleachRefreshPins.size() > 0)
        {
            bool done {false};
            int  retries {0};

            while (!done && retries < MAX_REFRESH_RETRIES)
            {
                for (const auto& pin : m_bleachRefreshPins)
                {
                    m_hal->digitalWrite(pin, false, m_config->refreshBleachPulseTime,
                                        m_config->refreshBleachingVoltage);
                }

                tmp = m_bleachRefreshPins;

                m_bleachRefreshPins.clear();
                for (const auto& pin : tmp)
                {
                    analogVal = m_hal->analogRead(pin);
                    if (analogVal > m_config->refreshBleachLimitLVoltage)
                    {
                        m_bleachRefreshPins.push_back(pin);
                    }
                }

                done = (m_bleachRefreshPins.size() == 0);
                ++retries;
            }

            if (!done)
            {
                // Serial.println("WARNING: Bleach refresh failed");
            }
        }
    }
};
}  // namespace ecd
}  // namespace ynv