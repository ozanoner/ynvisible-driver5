/**
 * @file anim_13.hpp
 * @brief Increment animation for the 3-segment bar display.
 * @date 2025-10-19
 * @copyright Copyright (c) 2025
 */

#pragma once

#include "anim.hpp"
#include "disp_3seg_bar.hpp"

namespace ynv
{
namespace anim
{

class Anim3SegBarCounterUp : public Anim<ynv::ecd::Disp3SegBar>
{
   public:
    explicit Anim3SegBarCounterUp(std::shared_ptr<ynv::ecd::Disp3SegBar> display) : Anim<ynv::ecd::Disp3SegBar>(display)
    {
    }

   protected:
    void transition() override
    {
        m_display->increment();  // Increment the bar display
    }
};

}  // namespace anim
}  // namespace ynv
