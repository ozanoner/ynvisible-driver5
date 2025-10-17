#pragma once

#include <memory>

#include "esp_timer.h"

namespace ynv
{
namespace anim
{

class AnimBase
{
   public:
    enum class State_t
    {
        IDLE,
        READY,  // Animation is ready to run
        RUNNING,
        PAUSED,
        ABORTED,
        COMPLETED
    };

    AnimBase() : m_state(State_t::IDLE), m_stateChangeCallback(nullptr) { }
    virtual ~AnimBase()   = default;
    virtual void update() = 0;  // Update the animation state in the loop

    // button click to start the animation
    void start()
    {
        if (m_state == State_t::IDLE)
        {
            setState(State_t::READY);
        }
    }

    // button click to abort the animation
    void abort()
    {
        if (m_state == State_t::RUNNING || m_state == State_t::PAUSED)
        {
            setState(State_t::ABORTED);  // Stop the animation
        }
    }

    // button click to pause the animation
    void pause()
    {
        if (m_state == State_t::RUNNING)
        {
            setState(State_t::PAUSED);  // Pause the animation
        }
    }

    // button click to resume the animation
    void resume()
    {
        if (m_state == State_t::PAUSED)
        {
            setState(State_t::RUNNING);  // Resume the animation
        }
    }

    // Quick change the state for button clicks
    State_t changeState(void)
    {
        switch (m_state)
        {
            case State_t::IDLE:
                setState(State_t::READY);  // Transition to ready state
                break;

            case State_t::RUNNING:
                setState(State_t::PAUSED);  // Transition to paused state
                break;

            case State_t::PAUSED:
                setState(State_t::RUNNING);  // Resume the animation
                break;

            default:
                break;
        }

        return m_state;  // Return the current state after the change
    }

    // Get the current state of the animation
    State_t getState() const { return m_state; }

    typedef void (*StateChangeCallback_f)(State_t);
    void registerStateChangeCallback(StateChangeCallback_f cb)
    {
        m_stateChangeCallback = cb;  // Register a callback function for state changes
    }

    // Check if the animation is running
    bool isRunning()
    {
        return m_state == State_t::RUNNING;  // Check if the animation is running
    }

   protected:
    State_t               m_state;                // Current state of the animation
    StateChangeCallback_f m_stateChangeCallback;  // Callback function for state changes

    // internal function to mark the animation as completed
    void complete() { setState(State_t::COMPLETED); }  // Set the state to completed

    void setState(State_t state)
    {
        m_state = state;  // Set the current state of the animation
        if (m_stateChangeCallback)
        {
            m_stateChangeCallback(m_state);  // Call the registered callback if it exists
        }
    }
};

template <typename DisplayT>
class Anim : public AnimBase
{
   public:
    static constexpr int TRANSITION_RATE_MS = 5000;  // Transition rate to the next state in milliseconds

    Anim(std::shared_ptr<DisplayT> display) : m_display(display) { }
    virtual ~Anim() = default;

    // Update the animation state in a task loop
    void update() override
    {
        uint32_t now = esp_timer_get_time() / 1000;  // Convert microseconds to milliseconds

        switch (m_state)
        {
            case State_t::READY:
                setState(State_t::RUNNING);  // Transition to running state
                m_display->set();
                m_display->update();
                m_lastUpdate = now;
                break;

            case State_t::RUNNING:
                if ((now - m_lastUpdate) >= TRANSITION_RATE_MS)
                {
                    transition();        // Call the transition function to change the display state
                    m_lastUpdate = now;  // Update the last update time
                }
                m_display->update();
                break;

            case State_t::COMPLETED:      // Animation has completed
            case State_t::ABORTED:        // Animation has been stopped by the user
                setState(State_t::IDLE);  // Reset to idle state after completion
                m_display->reset();
                m_display->update();
                break;

            case State_t::PAUSED:  // Animation is paused
                m_display->update();
                break;

            default:
                // Do nothing for other states
                break;
        }
    }

   protected:
    std::shared_ptr<DisplayT> m_display;         // Reference to the display being animated
    virtual void              transition() = 0;  // Transition the display to the next state in the animation

   private:
    unsigned long m_lastUpdate = 0;  // Last update time in milliseconds
};

}  // namespace anim
}  // namespace ynv