
#pragma once

#include "ecd.hpp"

namespace ynv
{
namespace ecd
{
class Disp3SegBar : public ECD<3>
{
   public:
    using ECD<3>::ECD;  // Inherit constructors from ECD<3>

    static constexpr std::array<int, 3> PINS {PIN_SEG_2, PIN_SEG_1, PIN_SEG_3};

    // Function to toggle the state of the segments
    void increment()
    {
        ++m_pos;
        updateNextStatesIncDec();
    }

    void decrement()
    {
        --m_pos;
        updateNextStatesIncDec();
    }

    void position(int pos)
    {
        m_pos = pos % 3;  // Ensure position is within bounds
        for (int i = 0; i < 3; ++i)
        {
            m_nextStates[i] = (i == m_pos);  // Set segments based on position
        }
    }

    int getPos() const { return m_pos; }

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

    void updateNextStatesIncDec()
    {
        m_pos %= 3;  // Wrap around if exceeds segment count
        for (int i = 0; i < m_pos; ++i)
        {
            m_nextStates[i] = true;  // Set segments to color state
        }
        for (int i = m_pos; i < 3; ++i)
        {
            m_nextStates[i] = false;  // Set remaining segments to bleach state
        }
    }
};
}  // namespace ecd
}  // namespace ynv