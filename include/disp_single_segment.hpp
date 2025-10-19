/**
 * @file disp_single_segment.hpp
 * @brief Single segment ECD driver
 */

#pragma once

#include "ecd.hpp"

namespace ynv
{
namespace ecd
{
class DispSingleSegment : public ECD<1>
{
   public:
    using ECD<1>::ECD;  // Inherit constructors from ECD<1>

    static constexpr std::array<int, 1> PINS {PIN_SEG_1};

    void on()
    {
        m_nextStates[0] = true;  // Set the single segment to color state
    }

    void off()
    {
        m_nextStates[0] = false;  // Set the single segment to bleach state
    }

   protected:
    void initConfig() override
    {
        m_config.coloringVoltage            = (int)(m_config.maxAnalogValue / 2);
        m_config.coloringTime               = 500;
        m_config.bleachingVoltage           = (int)(m_config.maxAnalogValue / 2 * 0.8f);
        m_config.bleachingTime              = 500;
        m_config.refreshColoringVoltage     = (int)(m_config.maxAnalogValue / 2 * 0.8f);
        m_config.refreshColorPulseTime      = 50;
        m_config.refreshColorLimitHVoltage  = (int)(m_config.maxAnalogValue * 0.8f);
        m_config.refreshColorLimitLVoltage  = (int)(m_config.maxAnalogValue * 0.6f);
        m_config.refreshBleachingVoltage    = (int)(m_config.maxAnalogValue / 2 * 0.6f);
        m_config.refreshBleachPulseTime     = 50;
        m_config.refreshBleachLimitHVoltage = (int)(m_config.maxAnalogValue * 0.4f);
        m_config.refreshBleachLimitLVoltage = (int)(m_config.maxAnalogValue * 0.3f);
    }
};
}  // namespace ecd
}  // namespace ynv