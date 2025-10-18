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

enum SegmentPins_t
{
    // pin-0 is for common
    PIN_SEG_1  = 1,
    PIN_SEG_2  = 2,
    PIN_SEG_3  = 3,
    PIN_SEG_4  = 4,
    PIN_SEG_5  = 5,
    PIN_SEG_6  = 6,
    PIN_SEG_7  = 7,
    PIN_SEG_8  = 8,
    PIN_SEG_9  = 9,
    PIN_SEG_10 = 10,
    PIN_SEG_11 = 11,
    PIN_SEG_12 = 12,
    PIN_SEG_13 = 13,
    PIN_SEG_14 = 14,
    PIN_SEG_15 = 15
};

class ECDBase
{
   public:
    ~ECDBase() = default;

    virtual void init()                               = 0;  // Initialize the ECD
    virtual void reset()                              = 0;  // Reset the ECD to bleach state
    virtual void set()                                = 0;  // Set all segments to color state
    virtual void set(const std::vector<bool>& states) = 0;  // Set segments to specified states
    virtual void update()                             = 0;  // Apply m_nextStates to hardware
    virtual void toggle()                             = 0;  // Toggle the state of all segments
    virtual void printConfig() const                  = 0;  // Print the configuration of the ECD
};

template <int SEGMENT_COUNT>
class ECD : public ECDBase
{
   public:
    explicit ECD(const std::array<int, SEGMENT_COUNT>* pins, const ynv::app::AppConfig_t* appConfig)
        : m_pins(pins), m_states({}), m_nextStates({}), m_driver(nullptr), m_appConfig(appConfig)
    {
        assert(m_appConfig != nullptr);
    }

    ~ECD() = default;

    void init() override
    {
        m_config.maxAnalogValue = (1 << m_appConfig->analogResolution) - 1;  // Set max analog value based on resolution
        initConfig();  // Initialize the ECD with default configuration
        validateConfig();

        if (m_appConfig->activeDriving)
        {
            m_driver = std::make_unique<ECDDriveActive<SEGMENT_COUNT>>(  // Create the driving algorithm instance
                &m_config, m_pins, static_cast<ynv::driver::HALBase*>(m_appConfig->hal));
        }
        else
        {
            m_driver = std::make_unique<ECDDrivePassive<SEGMENT_COUNT>>(  // Create the driving algorithm instance
                &m_config, m_pins, static_cast<ynv::driver::HALBase*>(m_appConfig->hal));
        }
        assert(m_driver != nullptr);
    }

    void reset() override
    {
        m_nextStates.fill(false);  // All segments to bleach state
    }

    void set() override
    {
        m_nextStates.fill(true);  // All segments to color state
    }

    void toggle() override
    {
        for (int i = 0; i < SEGMENT_COUNT; ++i)
        {
            m_nextStates[i] = !m_states[i];  // Toggle each segment state
        }
    }

    void set(const std::vector<bool>& states) override
    {
        assert(states.size() == SEGMENT_COUNT);
        std::copy(states.begin(), states.end(), m_nextStates.begin());
    }

    // Animation will call this method to apply the next states to the hardware in a task loop.
    // NOTE: The total execution time may exceed one cycle of the animation.
    void update() override
    {
        assert(m_driver != nullptr);
        m_driver->drive(m_states, m_nextStates);  // Drive the segments based on current and next states
    }

    void printConfig() const override
    {
        m_config.print();  // Print the configuration of the ECD
    }

    int getSegmentCount() const
    {
        return SEGMENT_COUNT;  // Return the number of segments
    }

   protected:
    const std::array<int, SEGMENT_COUNT>* m_pins;        // Array to hold segment pin numbers
    std::array<bool, SEGMENT_COUNT>       m_states;      // Array to hold segment states (bleach=false or color=true)
    std::array<bool, SEGMENT_COUNT>       m_nextStates;  // Array to hold next states for segments
    ECDConfig_t                           m_config;      // Configuration for the ECD

    std::unique_ptr<ECDDriveBase<SEGMENT_COUNT>> m_driver;  // Driving algorithm for the ECD
    const ynv::app::AppConfig_t*                 m_appConfig;

    virtual void initConfig() = 0;  // Initialize the ECD with default configuration

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