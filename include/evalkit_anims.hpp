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
#include <functional>
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
        ANIM_UP,
        ANIM_DOWN,
        ANIM_TEST,
        ANIM_CNT
    };

    static EvalkitAnims& getInstance()
    {
        static EvalkitAnims instance;
        return instance;
    }

    void init(const ynv::app::AppConfig_t* appConfig);

    Anim_t    select(ynv::ecd::EvalkitDisplays::ECDEvalkitDisplay_t disp, Anim_t anim);
    AnimBase& getCurrentAnim() { return *(m_anims[m_currentAnim]); }

    typedef void (*StateChangeCallback_f)(AnimBase::State_t);
    void registerStateChangeCallback(StateChangeCallback_f cb) { m_stateChangeCallback = cb; }

   private:
    EvalkitAnims() : m_anims({}), m_currentAnim(ANIM_CNT), m_stateChangeCallback(nullptr) { }
    ~EvalkitAnims()                              = default;
    EvalkitAnims(const EvalkitAnims&)            = delete;
    EvalkitAnims& operator=(const EvalkitAnims&) = delete;

    std::array<std::unique_ptr<AnimBase>, ANIM_CNT> m_anims;

    Anim_t                m_currentAnim;
    StateChangeCallback_f m_stateChangeCallback;

    inline static const std::map<Anim_t, std::string> m_animNames = {{ANIM_TOGGLE, "Toggle"},
                                                                     {ANIM_UP, "Count Up"},
                                                                     {ANIM_DOWN, "Count Down"},
                                                                     {ANIM_TEST, "Run test"},
                                                                     {ANIM_CNT, "None"}};

    const ynv::app::AppConfig_t*                   m_appConfig = nullptr;
    ynv::ecd::EvalkitDisplays::ECDEvalkitDisplay_t m_disp =
        ynv::ecd::EvalkitDisplays::ECDEvalkitDisplay_t::EVALKIT_DISP_CNT;

    void setDisplay();
};

}  // namespace anim
}  // namespace ynv