/**
 * @file evalkit_anims.hpp
 * @brief  Evaluation Kit animations interface.
 * This header defines the EvalkitAnims class, which manages different animations
 * available on the Ynvisible Evaluation Kit v5. It provides a unified interface for
 * initializing and accessing the animations.
 * @version 0.1
 * @date 2025-08-08
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include <array>
#include <map>
#include <memory>

#include "anim.hpp"
#include "app_config.hpp"
#include "evalkit_displays.hpp"

namespace ynv
{
namespace anim
{

using ECDEvalkitDisplay_t = ynv::ecd::EvalkitDisplays::ECDEvalkitDisplay_t;

class EvalkitAnims
{
   public:
    enum Anim_t
    {
        ANIM_TOGGLE = 0,
        ANIM_15SEGSIGNED_POSITIVE_COUNTER_UP,
        ANIM_15SEGSIGNED_POSITIVE_COUNTER_DOWN,
        ANIM_15SEGSIGNED_NEGATIVE_COUNTER_UP,
        ANIM_15SEGSIGNED_NEGATIVE_COUNTER_DOWN,
        ANIM_15SEGDEC_COUNTER_UP,
        ANIM_15SEGDEC_COUNTER_DOWN,
        ANIM_1SEG_ON,
        ANIM_7SEGNUM_COUNTER_UP,
        ANIM_7SEGNUM_COUNTER_DOWN,
        ANIM_7SEGBAR_COUNTER_UP,
        ANIM_7SEGBAR_COUNTER_DOWN,
        ANIM_3SEGBAR_COUNTER_UP,
        ANIM_3SEGBAR_COUNTER_DOWN,
        ANIM_3SEGBAR_POS,
        ANIM_TEST,
        ANIM_CNT
    };

    static EvalkitAnims& getInstance()
    {
        static EvalkitAnims instance;
        return instance;
    }

    void init(const ynv::app::AppConfig_t* appConfig);

    Anim_t select(Anim_t anim, bool forward = true);

    Anim_t next();
    Anim_t previous();

    AnimBase& getCurrentAnim()
    {
        return *(m_anims[m_currentAnim]);  // Return a reference to the animation object
    }

    typedef void (*StateChangeCallback_f)(AnimBase::State_t);
    void registerStateChangeCallback(StateChangeCallback_f cb)
    {
        m_stateChangeCallback = cb;  // Register a callback function for state changes
    }

    const std::string& getAnimName(Anim_t anim) const
    {
        auto it = m_animNames.find(anim);
        if (it != m_animNames.end())
        {
            return it->second;  // Return the name of the animation
        }
        return m_animNames.at(ANIM_CNT);  // Default to ANIM_NONE if not found
    }

   private:
    EvalkitAnims()
        : m_anims({}),
          m_currentAnim(ANIM_TOGGLE),
          m_displayType(ECDEvalkitDisplay_t::EVALKIT_DISP_SINGLE_SEGMENT_DISPLAY),
          m_stateChangeCallback(nullptr)
    {
    }
    ~EvalkitAnims()                              = default;
    EvalkitAnims(const EvalkitAnims&)            = delete;
    EvalkitAnims& operator=(const EvalkitAnims&) = delete;

    std::array<std::unique_ptr<AnimBase>, ANIM_CNT> m_anims;

    Anim_t              m_currentAnim;
    ECDEvalkitDisplay_t m_displayType;

    StateChangeCallback_f m_stateChangeCallback;

    inline static const std::map<Anim_t, std::string> m_animNames = {
        {ANIM_TOGGLE, "Anim_t::ANIM_TOGGLE"},
        {ANIM_15SEGSIGNED_POSITIVE_COUNTER_UP, "Anim_t::ANIM_15SEGSIGNED_POSITIVE_COUNTER_UP"},
        {ANIM_15SEGSIGNED_POSITIVE_COUNTER_DOWN, "Anim_t::ANIM_15SEGSIGNED_POSITIVE_COUNTER_DOWN"},
        {ANIM_15SEGSIGNED_NEGATIVE_COUNTER_UP, "Anim_t::ANIM_15SEGSIGNED_NEGATIVE_COUNTER_UP"},
        {ANIM_15SEGSIGNED_NEGATIVE_COUNTER_DOWN, "Anim_t::ANIM_15SEGSIGNED_NEGATIVE_COUNTER_DOWN"},
        {ANIM_15SEGDEC_COUNTER_UP, "Anim_t::ANIM_15SEGDEC_COUNTER_UP"},
        {ANIM_15SEGDEC_COUNTER_DOWN, "Anim_t::ANIM_15SEGDEC_COUNTER_DOWN"},
        {ANIM_1SEG_ON, "Anim_t::ANIM_1SEG_ON"},
        {ANIM_7SEGNUM_COUNTER_UP, "Anim_t::ANIM_7SEGNUM_COUNTER_UP"},
        {ANIM_7SEGNUM_COUNTER_DOWN, "Anim_t::ANIM_7SEGNUM_COUNTER_DOWN"},
        {ANIM_7SEGBAR_COUNTER_UP, "Anim_t::ANIM_7SEGBAR_COUNTER_UP"},
        {ANIM_7SEGBAR_COUNTER_DOWN, "Anim_t::ANIM_7SEGBAR_COUNTER_DOWN"},
        {ANIM_3SEGBAR_COUNTER_UP, "Anim_t::ANIM_3SEGBAR_COUNTER_UP"},
        {ANIM_3SEGBAR_COUNTER_DOWN, "Anim_t::ANIM_3SEGBAR_COUNTER_DOWN"},
        {ANIM_3SEGBAR_POS, "Anim_t::ANIM_3SEGBAR_POS"},
        {ANIM_TEST, "Anim_t::ANIM_TEST"},
        {ANIM_CNT, "Anim_t::ANIM_NONE"}};

    const ynv::app::AppConfig_t* m_appConfig = nullptr;
};

}  // namespace anim
}  // namespace ynv