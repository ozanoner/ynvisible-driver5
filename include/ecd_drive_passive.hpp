
#pragma once

#include <array>

#include "ecd_drive_base.hpp"
#include "ynv_hal.hpp"

namespace ynv
{
namespace ecd
{
template <int SEGMENT_COUNT>
class ECDDrivePassive : public ECDDriveBase<SEGMENT_COUNT>
{
   public:
    ~ECDDrivePassive() = default;

    using ECDDriveBase<SEGMENT_COUNT>::ECDDriveBase;  // Inherit constructors
    using ECDDriveBase<SEGMENT_COUNT>::m_pins;
    using ECDDriveBase<SEGMENT_COUNT>::m_config;
    using ECDDriveBase<SEGMENT_COUNT>::m_hal;

    void drive(std::array<bool, SEGMENT_COUNT>&       currentStates,
               const std::array<bool, SEGMENT_COUNT>& nextStates) override
    {
        for (int i = 0; i < SEGMENT_COUNT; ++i)
        {
            m_hal->digitalWrite((*m_pins)[i], nextStates[i], 50);
            currentStates[i] = nextStates[i];
        }
    }
};
}  // namespace ecd
}  // namespace ynv