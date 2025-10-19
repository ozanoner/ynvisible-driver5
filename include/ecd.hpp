/**
 * @file ecd.hpp
 * @brief Core electrochromic display (ECD) classes and interfaces
 */

#pragma once

#include <array>
#include <cassert>
#include <cinttypes>
#include <memory>
#include <vector>

#include "app_config.hpp"
#include "ecd_drive_active.hpp"
#include "ecd_drive_base.hpp"
#include "ecd_drive_passive.hpp"

namespace ynv
{
namespace ecd
{

/**
 * @brief Segment pin enumeration for ECDs
 *
 * Defines pin numbers for up to 15 segments (pin 0 reserved for common electrode)
 */
enum SegmentPins_t
{
    // pin-0 is for common
    PIN_SEG_1  = 1,   ///< Segment 1 pin
    PIN_SEG_2  = 2,   ///< Segment 2 pin
    PIN_SEG_3  = 3,   ///< Segment 3 pin
    PIN_SEG_4  = 4,   ///< Segment 4 pin
    PIN_SEG_5  = 5,   ///< Segment 5 pin
    PIN_SEG_6  = 6,   ///< Segment 6 pin
    PIN_SEG_7  = 7,   ///< Segment 7 pin
    PIN_SEG_8  = 8,   ///< Segment 8 pin
    PIN_SEG_9  = 9,   ///< Segment 9 pin
    PIN_SEG_10 = 10,  ///< Segment 10 pin
    PIN_SEG_11 = 11,  ///< Segment 11 pin
    PIN_SEG_12 = 12,  ///< Segment 12 pin
    PIN_SEG_13 = 13,  ///< Segment 13 pin
    PIN_SEG_14 = 14,  ///< Segment 14 pin
    PIN_SEG_15 = 15   ///< Segment 15 pin
};

/**
 * @brief Abstract base class for all ECD implementations
 *
 * Defines common interface for electrochromic display operations
 */
class ECDBase
{
   public:
    ~ECDBase() = default;

    virtual void init()                               = 0;  ///< Initialize the ECD
    virtual void reset()                              = 0;  ///< Reset to bleach state
    virtual void set()                                = 0;  ///< Set all segments to color state
    virtual void set(const std::vector<bool>& states) = 0;  ///< Set specific segment states
    virtual void update()                             = 0;  ///< Apply pending state changes
    virtual void toggle()                             = 0;  ///< Toggle all segment states
    virtual void printConfig() const                  = 0;  ///< Print configuration
};

/**
 * @brief Template ECD class for multi-segment displays
 * @tparam SEGMENT_COUNT Number of segments in the display
 *
 * Provides state management, configuration, and driving capabilities
 * with support for both active and passive driving modes.
 */
template <int SEGMENT_COUNT>
class ECD : public ECDBase
{
   public:
    /**
     * @brief Constructor
     * @param pins Array of GPIO pin numbers for segments
     * @param appConfig Application configuration
     */
    explicit ECD(const std::array<int, SEGMENT_COUNT>* pins, const ynv::app::AppConfig_t* appConfig)
        : m_pins(pins), m_states({}), m_nextStates({}), m_driver(nullptr), m_appConfig(appConfig)
    {
        assert(m_appConfig != nullptr);
    }

    ~ECD() = default;

    /**
     * @brief Initialize ECD with configuration and driver
     */
    void init() override
    {
        m_config.maxAnalogValue = (1 << m_appConfig->analogResolution) - 1;
        initConfig();
        validateConfig();

        // Create appropriate driver based on configuration
        if (m_appConfig->activeDriving)
        {
            m_driver = std::make_unique<ECDDriveActive<SEGMENT_COUNT>>(
                &m_config, m_pins, static_cast<ynv::driver::HALBase*>(m_appConfig->hal));
        }
        else
        {
            m_driver = std::make_unique<ECDDrivePassive<SEGMENT_COUNT>>(
                &m_config, m_pins, static_cast<ynv::driver::HALBase*>(m_appConfig->hal));
        }
        assert(m_driver != nullptr);
    }

    /** @brief Reset all segments to bleached state */
    void reset() override { m_nextStates.fill(false); }

    /** @brief Set all segments to colored state */
    void set() override { m_nextStates.fill(true); }

    /** @brief Toggle current state of all segments */
    void toggle() override
    {
        for (int i = 0; i < SEGMENT_COUNT; ++i)
        {
            m_nextStates[i] = !m_states[i];
        }
    }

    /**
     * @brief Set specific segment states
     * @param states Vector of segment states (true=color, false=bleach)
     */
    void set(const std::vector<bool>& states) override
    {
        assert(states.size() == SEGMENT_COUNT);
        std::copy(states.begin(), states.end(), m_nextStates.begin());
    }

    /**
     * @brief Apply pending state changes to hardware
     *
     * Called by animation loop to drive segments to their target states
     */
    void update() override
    {
        assert(m_driver != nullptr);
        m_driver->drive(m_states, m_nextStates);
    }

    /** @brief Print ECD configuration parameters */
    void printConfig() const override { m_config.print(); }

    /**
     * @brief Get number of segments
     * @return Segment count
     */
    int getSegmentCount() const { return SEGMENT_COUNT; }

   protected:
    const std::array<int, SEGMENT_COUNT>* m_pins;        ///< Segment pin numbers
    std::array<bool, SEGMENT_COUNT>       m_states;      ///< Current segment states
    std::array<bool, SEGMENT_COUNT>       m_nextStates;  ///< Target segment states
    ECDConfig_t                           m_config;      ///< ECD configuration parameters

    std::unique_ptr<ECDDriveBase<SEGMENT_COUNT>> m_driver;     ///< Driving algorithm instance
    const ynv::app::AppConfig_t*                 m_appConfig;  ///< Application configuration

    /** @brief Initialize display-specific configuration (pure virtual) */
    virtual void initConfig() = 0;

    /**
     * @brief Validate configuration parameters
     *
     * Ensures all voltage and timing values are within acceptable ranges
     */
    void validateConfig()
    {
        assert(m_config.maxAnalogValue > 0);
        assert(m_config.coloringVoltage > 0);
        assert(m_config.bleachingVoltage > 0);
        assert(m_config.refreshColoringVoltage > 0);
        assert(m_config.refreshBleachingVoltage > 0);
        assert(m_config.refreshColorLimitHVoltage > 0);
        assert(m_config.refreshColorLimitLVoltage > 0);
        assert(m_config.refreshBleachLimitHVoltage > 0);
        assert(m_config.refreshBleachLimitLVoltage > 0);
        assert(m_config.coloringTime > 0);
        assert(m_config.bleachingTime > 0);
        assert(m_config.refreshColorPulseTime > 0);
        assert(m_config.refreshBleachPulseTime > 0);

        assert(m_config.coloringVoltage < m_config.maxAnalogValue);
        assert(m_config.bleachingVoltage < m_config.maxAnalogValue);
        assert(m_config.refreshColoringVoltage < m_config.maxAnalogValue);
        assert(m_config.refreshBleachingVoltage < m_config.maxAnalogValue);
        assert(m_config.refreshColorLimitHVoltage < m_config.maxAnalogValue &&
               m_config.refreshColorLimitHVoltage > (m_config.maxAnalogValue / 2));
        assert(m_config.refreshColorLimitLVoltage < m_config.maxAnalogValue &&
               m_config.refreshColorLimitLVoltage > (m_config.maxAnalogValue / 2));
        assert(m_config.refreshBleachLimitHVoltage < (m_config.maxAnalogValue / 2));
        assert(m_config.refreshBleachLimitLVoltage < (m_config.maxAnalogValue / 2));
    }

    /**
     * @brief 7-segment number display patterns
     * @return Array of bit patterns for digits 0-9
     */
    static constexpr std::array<uint8_t, 10> numberMask()
    {
        return {
            0b11111011,  // 0
            0b11100000,  // 1
            0b11011101,  // 2
            0b11110101,  // 3
            0b11100110,  // 4
            0b10110111,  // 5
            0b10111111,  // 6
            0b11100001,  // 7
            0b11111111,  // 8
            0b11110111   // 9
        };
    }
};

}  // namespace ecd
}  // namespace ynv