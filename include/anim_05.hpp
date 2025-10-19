/**
 * @file anim_05.hpp
 * @brief Decrement animation for the 15-segment signed number display.
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

class Anim15SegSignedNegativeCounterDown : public Anim<ynv::ecd::DispSignedNumber>
{
   public:
    explicit Anim15SegSignedNegativeCounterDown(std::shared_ptr<ynv::ecd::DispSignedNumber> display)
        : Anim<ynv::ecd::DispSignedNumber>(display)
    {
    }

   protected:
    void transition() override
    {
        static int counter = 1;
        m_display->show(counter / 10, counter % 10, true);  // Update display with new values
        if (++counter == 100)
        {
            counter = 1;
        }
    }
};

}  // namespace anim
}  // namespace ynv
