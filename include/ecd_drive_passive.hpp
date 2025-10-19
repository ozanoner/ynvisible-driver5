/**
 * @file ecd_drive_passive.hpp
 * @brief Passive driving implementation for ECDs
 */
#pragma once

#include <array>

#include "ecd_drive_base.hpp"
#include "ynv_hal.hpp"

namespace ynv
{
namespace ecd
{
/**
 * @brief Passive ECD driver with simple state-based control
 * @tparam SEGMENT_COUNT Number of display segments
 *
 * Provides basic ECD driving without voltage feedback or refresh operations.
 * Uses simple timing-based control for coloring and bleaching operations.
 */
template <int SEGMENT_COUNT>
class ECDDrivePassive : public ECDDriveBase<SEGMENT_COUNT>
{
   public:
    ~ECDDrivePassive() = default;

    using ECDDriveBase<SEGMENT_COUNT>::ECDDriveBase;  // Inherit constructors
    using ECDDriveBase<SEGMENT_COUNT>::m_pins;
    using ECDDriveBase<SEGMENT_COUNT>::m_config;
    using ECDDriveBase<SEGMENT_COUNT>::m_hal;

    /**
     * @brief Drive ECD segments with passive control
     * @param currentStates Current segment states (updated to match nextStates)
     * @param nextStates Target segment states
     *
     * Applies coloring or bleaching voltage to each segment based on target state.
     * No feedback monitoring - relies on fixed timing parameters.
     */
    void drive(std::array<bool, SEGMENT_COUNT>&       currentStates,
               const std::array<bool, SEGMENT_COUNT>& nextStates) override
    {
        for (int i = 0; i < SEGMENT_COUNT; ++i)
        {
            // Apply voltage based on desired state (true=color, false=bleach)
            m_hal->digitalWrite(
                (*m_pins)[i], nextStates[i], nextStates[i] ? m_config->coloringTime : m_config->bleachingTime,
                nextStates[i] ? (m_config->maxAnalogValue - m_config->coloringVoltage) : m_config->bleachingVoltage);

            currentStates[i] = nextStates[i];  // Update current state
        }
    }
};
}  // namespace ecd
}  // namespace ynv