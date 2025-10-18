
#pragma once

#include <array>
#include <cstdint>

#include "ecd.hpp"

namespace ynv
{
namespace ecd
{
class DispDotNumber : public ECD<8>
{
   public:
    using ECD<8>::ECD;  // Inherit constructors from ECD<8>

    static constexpr std::array<int, 8> PINS {PIN_SEG_6, PIN_SEG_8, PIN_SEG_1, PIN_SEG_2,
                                              PIN_SEG_3, PIN_SEG_4, PIN_SEG_5, PIN_SEG_7};

    void show(uint8_t number, bool dot = true)
    {
        number %= 10;           // Ensure number is within 0-9
        m_nextStates[0] = dot;  // Set the dot state
        for (int i = 0; i < 7; ++i)
        {
            m_nextStates[i + 1] = (numberMask()[number] >> i) & 1;
        }
    }

   protected:
    void initConfig() override
    {
        // initial coloring/bleaching voltage and times
        // coloring values: segment pin = maxAnalogValue (high), common = maxAnalogValue - coloringVoltage
        // bleaching values: segment pin = 0, common = bleachingVoltage
        m_config.coloringVoltage  = (int)(m_config.maxAnalogValue / 2 * 0.8f);
        m_config.coloringTime     = 300;
        m_config.bleachingVoltage = (int)(m_config.maxAnalogValue / 2 * 0.6f);
        m_config.bleachingTime    = 300;

        // refresh settings for coloring
        m_config.refreshColoringVoltage = (int)(m_config.maxAnalogValue / 2 * 0.8f);
        m_config.refreshColorPulseTime  = 50;
        // over half
        m_config.refreshColorLimitHVoltage = (int)(m_config.maxAnalogValue * 0.85f);
        m_config.refreshColorLimitLVoltage = (int)(m_config.maxAnalogValue * 0.6f);

        // refresh settings for bleaching
        m_config.refreshBleachingVoltage = (int)(m_config.maxAnalogValue / 2 * 0.6f);
        m_config.refreshBleachPulseTime  = 50;
        // under half
        m_config.refreshBleachLimitHVoltage = (int)(m_config.maxAnalogValue * 0.4f);
        m_config.refreshBleachLimitLVoltage = (int)(m_config.maxAnalogValue * 0.3f);
    }
};
}  // namespace ecd
}  // namespace ynv