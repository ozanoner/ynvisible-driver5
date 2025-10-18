#pragma once

#include <cinttypes>

#include "ecd.hpp"

namespace ynv
{
namespace ecd
{
class DispTest : public ECD<15>
{
   public:
    using ECD<15>::ECD;  // Inherit constructors from ECD<15>

    static constexpr std::array<int, 15> PINS {PIN_SEG_1,  PIN_SEG_2,  PIN_SEG_3,  PIN_SEG_4,  PIN_SEG_5,
                                               PIN_SEG_6,  PIN_SEG_7,  PIN_SEG_8,  PIN_SEG_9,  PIN_SEG_10,
                                               PIN_SEG_11, PIN_SEG_12, PIN_SEG_13, PIN_SEG_14, PIN_SEG_15};

    void show(uint8_t pos)
    {
        m_nextStates.fill(false);
        // Set the specified segment to color state
        m_nextStates[pos % 15] = true;
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
        m_config.refreshColorLimitLVoltage = (int)(m_config.maxAnalogValue * 0.7f);

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