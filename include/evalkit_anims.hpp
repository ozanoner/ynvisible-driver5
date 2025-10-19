/**
 * @file evalkit_anims.hpp
 * @brief Animation manager for YnVisible EvalKit displays
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

/** @brief Animation name constants */
constexpr const char* ANIM_NAME_TOGGLE     = "Toggle";
constexpr const char* ANIM_NAME_COUNT_UP   = "Count Up";
constexpr const char* ANIM_NAME_COUNT_DOWN = "Count Down";
constexpr const char* ANIM_NAME_TEST       = "Run test";

/**
 * @brief Singleton animation manager for EvalKit displays
 *
 * Manages animation instances and provides unified interface for
 * selecting and controlling animations on different display types.
 */
class EvalkitAnims
{
   public:
    /**
     * @brief Available animation types
     */
    enum Anim_t
    {
        ANIM_TOGGLE = 0,  ///< Toggle segments on/off
        ANIM_UP,          ///< Count up sequence
        ANIM_DOWN,        ///< Count down sequence
        ANIM_TEST,        ///< Test pattern animation
        ANIM_CNT          ///< Total count (invalid animation)
    };

    /**
     * @brief Get singleton instance
     * @return Reference to animation manager
     */
    static EvalkitAnims& getInstance()
    {
        static EvalkitAnims instance;
        return instance;
    }

    /**
     * @brief Select and start animation on specified display
     * @param disp Display type to animate
     * @param anim Animation type to run
     * @return Selected animation type
     */
    Anim_t select(ynv::ecd::EvalkitDisplays::ECDEvalkitDisplay_t disp, Anim_t anim);

    /**
     * @brief Get current active animation
     * @return Reference to current animation
     */
    AnimBase& getCurrentAnim() { return *(m_anims[m_currentAnim]); }

    /**
     * @brief Check if an animation is currently selected
     * @return true if animation is active
     */
    bool isSelected() const { return m_currentAnim != ANIM_CNT && m_anims[m_currentAnim] != nullptr; }

    /** @brief Animation state change callback function type */
    typedef void (*StateChangeCallback_f)(AnimBase::State_t);

    /**
     * @brief Register callback for animation state changes
     * @param cb Callback function
     */
    void registerStateChangeCallback(StateChangeCallback_f cb) { m_stateChangeCallback = cb; }

    /** @brief Mapping from animation enum to display name */
    inline static const std::map<Anim_t, std::string> m_animNames = {{ANIM_TOGGLE, ANIM_NAME_TOGGLE},
                                                                     {ANIM_UP, ANIM_NAME_COUNT_UP},
                                                                     {ANIM_DOWN, ANIM_NAME_COUNT_DOWN},
                                                                     {ANIM_TEST, ANIM_NAME_TEST},
                                                                     {ANIM_CNT, "None"}};

   private:
    /** @brief Private constructor for singleton */
    EvalkitAnims()
        : m_anims({}),
          m_currentAnim(ANIM_CNT),
          m_stateChangeCallback(nullptr),
          m_dispIndex(ynv::ecd::EvalkitDisplays::ECDEvalkitDisplay_t::EVALKIT_DISP_CNT)
    {
    }

    ~EvalkitAnims()                              = default;
    EvalkitAnims(const EvalkitAnims&)            = delete;
    EvalkitAnims& operator=(const EvalkitAnims&) = delete;

    std::array<std::unique_ptr<AnimBase>, ANIM_CNT> m_anims;                ///< Animation instances
    Anim_t                                          m_currentAnim;          ///< Currently selected animation
    StateChangeCallback_f                           m_stateChangeCallback;  ///< State change callback
    ynv::ecd::EvalkitDisplays::ECDEvalkitDisplay_t  m_dispIndex;            ///< Current display type

    /**
     * @brief Initialize animations for specified display type
     * @param disp Display type to set up
     */
    void setDisplay(ynv::ecd::EvalkitDisplays::ECDEvalkitDisplay_t disp);
};

}  // namespace anim
}  // namespace ynv