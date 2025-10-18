/**
 * @file evalkit_anims.cpp
 * @brief  Evaluation Kit animations implementation.
 * @version 0.1
 * @date 2025-08-08
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "evalkit_anims.hpp"

#include <memory>

#include "anim_01.hpp"
#include "anim_02.hpp"
#include "anim_03.hpp"
#include "anim_04.hpp"
#include "anim_05.hpp"
#include "anim_06.hpp"
#include "anim_07.hpp"
#include "anim_08.hpp"
#include "anim_09.hpp"
#include "anim_10.hpp"
#include "anim_11.hpp"
#include "anim_12.hpp"
#include "anim_13.hpp"
#include "anim_14.hpp"
#include "anim_15.hpp"
#include "anim_test.hpp"

namespace ynv
{
namespace anim
{

EvalkitAnims::Anim_t EvalkitAnims::select(ynv::ecd::EvalkitDisplays::ECDEvalkitDisplay_t disp,
                                          EvalkitAnims::Anim_t                           anim)
{
    assert(anim < ANIM_CNT);
    assert(disp < ynv::ecd::EvalkitDisplays::ECDEvalkitDisplay_t::EVALKIT_DISP_CNT);

    if (anim == m_currentAnim && disp == m_dispIndex)
    {
        // If the selected animation is already current, do nothing.
        return m_currentAnim;
    }

    if (disp != m_dispIndex)
    {
        setDisplay(disp);
    }

    // Set the current animation to the selected one
    m_currentAnim = anim;
    if (m_anims[m_currentAnim] != nullptr)
    {
        m_anims[m_currentAnim]->registerStateChangeCallback(m_stateChangeCallback);
        m_anims[m_currentAnim]->start();  // Start the newly selected animation
    }
    return m_currentAnim;
}

void EvalkitAnims::setDisplay(ynv::ecd::EvalkitDisplays::ECDEvalkitDisplay_t disp)
{
    m_anims = {};  // reset the list of animations

    auto& displays = ynv::ecd::EvalkitDisplays::getInstance();
    auto  display  = displays.selectDisplay(disp);
    m_dispIndex    = disp;

    switch (displays.getDisplayIndex())
    {
        case ECDEvalkitDisplay_t::EVALKIT_DISP_SINGLE_SEGMENT_DISPLAY:
            m_anims[ANIM_TOGGLE] = std::make_unique<AnimToggle>(display);
            break;
        case ECDEvalkitDisplay_t::EVALKIT_DISP_THREE_SEGMENT_BAR_DISPLAY:
            m_anims[ANIM_TOGGLE] = std::make_unique<AnimToggle>(display);
            m_anims[ANIM_UP] =
                std::make_unique<Anim3SegBarCounterUp>(std::static_pointer_cast<ynv::ecd::Disp3SegBar>(display));
            m_anims[ANIM_DOWN] =
                std::make_unique<Anim3SegBarCounterDown>(std::static_pointer_cast<ynv::ecd::Disp3SegBar>(display));
            break;
        case ECDEvalkitDisplay_t::EVALKIT_DISP_SEVEN_SEGMENT_BAR_DISPLAY:
            m_anims[ANIM_TOGGLE] = std::make_unique<AnimToggle>(display);
            m_anims[ANIM_UP] =
                std::make_unique<Anim7SegBarCounterUp>(std::static_pointer_cast<ynv::ecd::Disp7SegBar>(display));
            m_anims[ANIM_DOWN] =
                std::make_unique<Anim7SegBarCounterDown>(std::static_pointer_cast<ynv::ecd::Disp7SegBar>(display));
            break;
        case ECDEvalkitDisplay_t::EVALKIT_DISP_DOT_NUMBER_DISPLAY:
            m_anims[ANIM_TOGGLE] = std::make_unique<AnimToggle>(display);
            m_anims[ANIM_UP] =
                std::make_unique<Anim7SegNumCounterUp>(std::static_pointer_cast<ynv::ecd::DispDotNumber>(display));
            m_anims[ANIM_DOWN] =
                std::make_unique<Anim7SegNumCounterDown>(std::static_pointer_cast<ynv::ecd::DispDotNumber>(display));
            break;
        case ECDEvalkitDisplay_t::EVALKIT_DISP_DECIMAL_NUMBER_DISPLAY:
            m_anims[ANIM_TOGGLE] = std::make_unique<AnimToggle>(display);
            m_anims[ANIM_UP]     = std::make_unique<Anim15SegDecimalCounterUp>(
                std::static_pointer_cast<ynv::ecd::DispDecimalNumber>(display));
            m_anims[ANIM_DOWN] = std::make_unique<Anim15SegDecimalCounterDown>(
                std::static_pointer_cast<ynv::ecd::DispDecimalNumber>(display));
            break;
        case ECDEvalkitDisplay_t::EVALKIT_DISP_SIGNED_NUMBER_DISPLAY:
            m_anims[ANIM_TOGGLE] = std::make_unique<AnimToggle>(display);
            m_anims[ANIM_UP]     = std::make_unique<Anim15SegSignedPositiveCounterUp>(
                std::static_pointer_cast<ynv::ecd::DispSignedNumber>(display));
            m_anims[ANIM_DOWN] = std::make_unique<Anim15SegSignedPositiveCounterDown>(
                std::static_pointer_cast<ynv::ecd::DispSignedNumber>(display));
            break;

        case ECDEvalkitDisplay_t::EVALKIT_DISP_TEST:
            m_anims[ANIM_TOGGLE] = std::make_unique<AnimToggle>(display);
            m_anims[ANIM_TEST]   = std::make_unique<AnimTest>(std::static_pointer_cast<ynv::ecd::DispTest>(display));
            break;

        default:
            break;
    }

    display->printConfig();
}

}  // namespace anim
}  // namespace ynv