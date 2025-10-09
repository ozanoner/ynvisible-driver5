
#pragma once

#include "ecd.hpp"

namespace ynv
{
namespace ecd
{
class Disp7SegBar : public ECD<7>
{
   public:
    using ECD<7>::ECD;  // Inherit constructors from ECD<7>

    // Function to toggle the state of the segments
    void increment()
    {
        ++m_pos;
        updateNextStates();
    }

    void decrement()
    {
        --m_pos;
        updateNextStates();
    }

    void resetPos() { m_pos = 0; }
    int  getPos() const { return m_pos; }

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

   private:
    int m_pos = 0;

    void updateNextStates()
    {
        m_pos %= 7;  // Wrap around if exceeds segment count
        for (int i = 0; i < m_pos; ++i)
        {
            m_nextStates[i] = true;  // Set segments to color state
        }
        for (int i = m_pos; i < 7; ++i)
        {
            m_nextStates[i] = false;  // Set remaining segments to bleach state
        }
    }
};
}  // namespace ecd
}  // namespace ynv