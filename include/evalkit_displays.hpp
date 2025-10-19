/**
 * @file evalkit_displays.hpp
 * @brief Display manager for YnVisible EvalKit v5 displays
 */

#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <functional>
#include <memory>

#include "app_config.hpp"
#include "disp_3seg_bar.hpp"
#include "disp_7seg_bar.hpp"
#include "disp_decimal_number.hpp"
#include "disp_dot_number.hpp"
#include "disp_signed_number.hpp"
#include "disp_single_segment.hpp"
#include "disp_test.hpp"
#include "ecd.hpp"

namespace ynv
{
namespace ecd
{

/**
 * @brief Singleton manager for EvalKit display types
 *
 * Provides unified interface for initializing and accessing different
 * electrochromic display types available on the YnVisible EvalKit v5.
 */
class EvalkitDisplays
{
   public:
    /**
     * @brief Supported EvalKit display types
     */
    enum ECDEvalkitDisplay_t
    {
        EVALKIT_DISP_SINGLE_SEGMENT_DISPLAY = 0,  ///< Single segment (DISP442V2PV1, etc.)
        EVALKIT_DISP_THREE_SEGMENT_BAR_DISPLAY,   ///< 3-segment bar (DISP438V2PV1)
        EVALKIT_DISP_SEVEN_SEGMENT_BAR_DISPLAY,   ///< 7-segment bar (DISP431V2PV1)
        EVALKIT_DISP_DOT_NUMBER_DISPLAY,          ///< Dot number (DISP434V1PV1)
        EVALKIT_DISP_DECIMAL_NUMBER_DISPLAY,      ///< Decimal number (DISP433V1PV1)
        EVALKIT_DISP_SIGNED_NUMBER_DISPLAY,       ///< Signed number (DISP444V1PV2)
        EVALKIT_DISP_TEST,                        ///< Test display (generic)
        EVALKIT_DISP_CNT                          ///< Total count (invalid)
    };

    /**
     * @brief Get singleton instance
     * @return Reference to display manager
     */
    static EvalkitDisplays& getInstance()
    {
        static EvalkitDisplays instance;
        return instance;
    }

    /**
     * @brief Initialize all display instances
     * @param appConfig Application configuration
     */
    void init(const ynv::app::AppConfig_t* appConfig)
    {
        assert(appConfig != nullptr);
        m_appConfig = appConfig;

        // Create all display instances
        m_displays[EVALKIT_DISP_SINGLE_SEGMENT_DISPLAY] =
            std::make_shared<DispSingleSegment>(&DispSingleSegment::PINS, m_appConfig);
        m_displays[EVALKIT_DISP_THREE_SEGMENT_BAR_DISPLAY] =
            std::make_shared<Disp3SegBar>(&Disp3SegBar::PINS, m_appConfig);
        m_displays[EVALKIT_DISP_SEVEN_SEGMENT_BAR_DISPLAY] =
            std::make_shared<Disp7SegBar>(&Disp7SegBar::PINS, m_appConfig);
        m_displays[EVALKIT_DISP_DOT_NUMBER_DISPLAY] =
            std::make_shared<DispDotNumber>(&DispDotNumber::PINS, m_appConfig);
        m_displays[EVALKIT_DISP_DECIMAL_NUMBER_DISPLAY] =
            std::make_shared<DispDecimalNumber>(&DispDecimalNumber::PINS, m_appConfig);
        m_displays[EVALKIT_DISP_SIGNED_NUMBER_DISPLAY] =
            std::make_shared<DispSignedNumber>(&DispSignedNumber::PINS, m_appConfig);
        m_displays[EVALKIT_DISP_TEST] = std::make_shared<DispTest>(&DispTest::PINS, m_appConfig);

        // Initialize all displays
        std::for_each(m_displays.begin(), m_displays.end(), [](auto& d) { d->init(); });

        // Set default display
        m_dispIndex  = EVALKIT_DISP_TEST;
        m_displayPtr = m_displays[m_dispIndex];
    }

    /**
     * @brief Get current display instance
     * @return Shared pointer to current display
     */
    std::shared_ptr<ECDBase> getDisplay() const
    {
        assert(m_displayPtr != nullptr);
        return m_displayPtr;
    }

    /**
     * @brief Get current display type
     * @return Current display index
     */
    ECDEvalkitDisplay_t getDisplayIndex() const { return m_dispIndex; }

    /**
     * @brief Select display by type
     * @param displayIndex Display type to select
     * @return Shared pointer to selected display
     */
    std::shared_ptr<ECDBase> selectDisplay(ECDEvalkitDisplay_t displayIndex)
    {
        assert(displayIndex >= 0 && displayIndex < EVALKIT_DISP_CNT);
        m_dispIndex  = displayIndex;
        m_displayPtr = m_displays[m_dispIndex];
        assert(m_displayPtr != nullptr);
        return m_displayPtr;
    }

   private:
    /** @brief Private constructor for singleton */
    EvalkitDisplays() : m_displayPtr(nullptr), m_appConfig(nullptr) { }

    EvalkitDisplays(const EvalkitDisplays&)            = delete;
    EvalkitDisplays& operator=(const EvalkitDisplays&) = delete;

    std::array<std::shared_ptr<ECDBase>, EVALKIT_DISP_CNT> m_displays;    ///< All display instances
    std::shared_ptr<ECDBase>                               m_displayPtr;  ///< Current display pointer
    const ynv::app::AppConfig_t*                           m_appConfig;   ///< Application configuration
    ECDEvalkitDisplay_t                                    m_dispIndex;   ///< Current display index
};

}  // namespace ecd
}  // namespace ynv