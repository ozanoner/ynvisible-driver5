/**
 * @file anim.hpp
 * @brief Animation framework for electrochromic displays
 */
#pragma once

#include <cstdint>
#include <memory>

#include "esp_timer.h"

namespace ynv
{
namespace anim
{

/**
 * @brief Base class for all ECD animations
 *
 * Provides state management and common functionality for display animations
 */
class AnimBase
{
   public:
    /**
     * @brief Animation state enumeration
     */
    enum class State_t
    {
        IDLE,      ///< Animation not active
        READY,     ///< Animation ready to run
        RUNNING,   ///< Animation currently executing
        PAUSED,    ///< Animation temporarily stopped
        ABORTED,   ///< Animation stopped by user
        COMPLETED  ///< Animation finished normally
    };

    AnimBase() : m_state(State_t::IDLE), m_stateChangeCallback(nullptr) { }
    virtual ~AnimBase() = default;

    /**
     * @brief Update animation state (called in main loop)
     */
    virtual void update() = 0;

    /**
     * @brief Start the animation from idle state
     */
    void start()
    {
        if (m_state == State_t::IDLE)
        {
            setState(State_t::READY);
        }
    }

    /**
     * @brief Abort running or paused animation
     */
    void abort()
    {
        if (m_state == State_t::RUNNING || m_state == State_t::PAUSED)
        {
            setState(State_t::ABORTED);
        }
    }

    /**
     * @brief Pause running animation
     */
    void pause()
    {
        if (m_state == State_t::RUNNING)
        {
            setState(State_t::PAUSED);
        }
    }

    /**
     * @brief Resume paused animation
     */
    void resume()
    {
        if (m_state == State_t::PAUSED)
        {
            setState(State_t::RUNNING);
        }
    }

    /**
     * @brief Toggle animation state (idle→ready, running→paused, paused→running)
     * @return Current state after change
     */
    State_t changeState(void)
    {
        switch (m_state)
        {
            case State_t::IDLE:
                setState(State_t::READY);
                break;
            case State_t::RUNNING:
                setState(State_t::PAUSED);
                break;
            case State_t::PAUSED:
                setState(State_t::RUNNING);
                break;
            default:
                break;
        }
        return m_state;
    }

    /**
     * @brief Get current animation state
     * @return Current state
     */
    State_t getState() const { return m_state; }

    /** @brief State change callback function type */
    // TODO: C++-style using
    typedef void (*StateChangeCallback_f)(State_t);

    /**
     * @brief Register callback for state changes
     * @param cb Callback function
     */
    void registerStateChangeCallback(StateChangeCallback_f cb) { m_stateChangeCallback = cb; }

    /**
     * @brief Check if animation is currently running
     * @return true if running
     */
    bool isRunning() { return m_state == State_t::RUNNING; }

   protected:
    State_t               m_state;                ///< Current animation state
    StateChangeCallback_f m_stateChangeCallback;  ///< State change callback

    /** @brief Mark animation as completed */
    void complete() { setState(State_t::COMPLETED); }

    /**
     * @brief Set new state and trigger callback
     * @param state New state to set
     */
    void setState(State_t state)
    {
        m_state = state;
        if (m_stateChangeCallback)
        {
            m_stateChangeCallback(m_state);
        }
    }
};

/**
 * @brief Template animation class for specific display types
 * @tparam DisplayT Display class type
 */
template <typename DisplayT>
class Anim : public AnimBase
{
   public:
    /** @brief Animation transition rate in milliseconds */
    // TODO: Make configurable per animation
    static constexpr int TRANSITION_RATE_MS = 5000;

    explicit Anim(std::shared_ptr<DisplayT> display) : m_display(display) { }
    virtual ~Anim() = default;

    /**
     * @brief Update animation state in main loop
     */
    void update() override
    {
        uint32_t now = esp_timer_get_time() / 1000;  // Convert to milliseconds

        switch (m_state)
        {
            case State_t::READY:
                setState(State_t::RUNNING);
                m_display->set();
                m_display->update();
                m_lastUpdate = now;
                break;

            case State_t::RUNNING:
                if ((now - m_lastUpdate) >= TRANSITION_RATE_MS)
                {
                    transition();
                    m_lastUpdate = now;
                }
                m_display->update();
                break;

            case State_t::COMPLETED:
            case State_t::ABORTED:
                setState(State_t::IDLE);
                m_display->reset();
                m_display->update();
                break;

            case State_t::PAUSED:
                m_display->update();
                break;

            default:
                break;
        }
    }

   protected:
    std::shared_ptr<DisplayT> m_display;  ///< Display being animated

    /**
     * @brief Transition to next animation state (pure virtual)
     */
    virtual void transition() = 0;

   private:
    unsigned long m_lastUpdate = 0;  ///< Last update timestamp in milliseconds
};

}  // namespace anim
}  // namespace ynv