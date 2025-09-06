
#pragma once

#include "anim.hpp"
#include "disp_decimal_number.hpp"

namespace ynv
{
namespace anim
{

class Anim15SegSignedPositiveCounterDown : public Anim<ynv::ecd::DispSignedNumber>
{
   public:
    Anim15SegSignedPositiveCounterDown(std::shared_ptr<ynv::ecd::DispSignedNumber> display)
        : Anim<ynv::ecd::DispSignedNumber>(display)
    {
    }

   protected:
    void transition() override
    {
        static int counter = 99;
        m_display->show(counter / 10, counter % 10, false);  // Update display with new values
        if (--counter == 0)
        {
            counter = 99;
        }
    }
};

}  // namespace anim
}  // namespace ynv
