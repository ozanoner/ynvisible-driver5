/**
 * @file anim_06.hpp
 * @brief Increment animation for the 15-segment decimal number display.
 * @date 2025-10-19
 * @copyright Copyright (c) 2025
 */

#pragma once

#include "anim.hpp"
#include "disp_decimal_number.hpp"

namespace ynv
{
namespace anim
{

class Anim15SegDecimalCounterUp : public Anim<ynv::ecd::DispDecimalNumber>
{
   public:
    explicit Anim15SegDecimalCounterUp(std::shared_ptr<ynv::ecd::DispDecimalNumber> display)
        : Anim<ynv::ecd::DispDecimalNumber>(display)
    {
    }

   protected:
    void transition() override
    {
        static int counter = 0;
        counter            = (counter + 1) % 100;     // Increment counter and wrap around at 100
        m_display->show(counter / 10, counter % 10);  // Update display with new values
    }
};

}  // namespace anim
}  // namespace ynv
