/**
 * @file anim_09.hpp
 * @brief Increment animation for the 7-segment number display.
 * @date 2025-10-19
 * @copyright Copyright (c) 2025
 */

#pragma once

#include "anim.hpp"
#include "disp_dot_number.hpp"

namespace ynv
{
namespace anim
{

class Anim7SegNumCounterUp : public Anim<ynv::ecd::DispDotNumber>
{
   public:
    explicit Anim7SegNumCounterUp(std::shared_ptr<ynv::ecd::DispDotNumber> display)
        : Anim<ynv::ecd::DispDotNumber>(display)
    {
    }

   protected:
    void transition() override
    {
        static int number = 0;                  // Initialize number to 0
        number            = (number + 1) % 10;  // Increment number and wrap around at 10
        m_display->show(number);
    }
};

}  // namespace anim
}  // namespace ynv
