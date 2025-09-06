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
        static std::vector<int> bleachRefreshPins(SEGMENT_COUNT);
        static std::vector<int> bleachPins(SEGMENT_COUNT);
        static std::vector<int> colorRefreshPins(SEGMENT_COUNT);
        static std::vector<int> colorPins(SEGMENT_COUNT);
        static std::vector<int> tmp(SEGMENT_COUNT);

        bleachPins.clear();
        colorPins.clear();
        bleachRefreshPins.clear();
        colorRefreshPins.clear();

        int analogVal = 0;

        for (int i = 0; i < SEGMENT_COUNT; ++i)
        {
            if (currentStates[i] == nextStates[i])
            {  // refresh
                analogVal = m_hal->analogRead((*m_pins)[i]);

                if (currentStates[i] && (analogVal < m_config->refreshColorLimitLVoltage))
                {
                    colorPins.push_back((*m_pins)[i]);
                }
                else if (!currentStates[i] && (analogVal > m_config->refreshBleachLimitHVoltage))
                {
                    bleachPins.push_back((*m_pins)[i]);
                }
            }
            else
            {  // change state
                if (nextStates[i])
                {
                    colorPins.push_back((*m_pins)[i]);
                }
                else
                {
                    bleachPins.push_back((*m_pins)[i]);
                }
                currentStates[i] = nextStates[i];  // Update the current state
            }
        }

        if (colorPins.size() > 0)
        {
            for (const auto& pin : colorPins)
            {
                m_hal->digitalWrite(pin, true, m_config->coloringTime, m_config->coloringVoltage);
            }
        }

        if (bleachPins.size() > 0)
        {
            for (const auto& pin : bleachPins)
            {
                m_hal->digitalWrite(pin, false, m_config->bleachingTime, m_config->bleachingVoltage);
            }
        }

        if (colorRefreshPins.size() > 0)
        {
            bool done {false};
            int  retries {0};

            while (!done && retries < MAX_REFRESH_RETRIES)
            {
                for (const auto& pin : colorRefreshPins)
                {
                    m_hal->digitalWrite(pin, true, m_config->refreshColorPulseTime, m_config->refreshColoringVoltage);
                }

                tmp = colorRefreshPins;

                colorRefreshPins.clear();
                for (const auto& pin : tmp)
                {
                    analogVal = m_hal->analogRead(pin);
                    if (analogVal < m_config->refreshColorLimitHVoltage)
                    {
                        colorRefreshPins.push_back(pin);
                    }
                }

                done = (colorRefreshPins.size() == 0);
                ++retries;
            }

            if (!done)
            {
                // Serial.println("WARNING: Color refresh failed");
            }
        }

        if (bleachRefreshPins.size() > 0)
        {
            bool done {false};
            int  retries {0};

            while (!done && retries < MAX_REFRESH_RETRIES)
            {
                for (const auto& pin : bleachRefreshPins)
                {
                    m_hal->digitalWrite(pin, false, m_config->refreshBleachPulseTime,
                                        m_config->refreshBleachingVoltage);
                }

                tmp = bleachRefreshPins;

                bleachRefreshPins.clear();
                for (const auto& pin : tmp)
                {
                    analogVal = m_hal->analogRead(pin);
                    if (analogVal > m_config->refreshBleachLimitLVoltage)
                    {
                        bleachRefreshPins.push_back(pin);
                    }
                }

                done = (bleachRefreshPins.size() == 0);
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