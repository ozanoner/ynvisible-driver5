
#pragma once

#include "anim.hpp"
#include "disp_decimal_number.hpp"

namespace ynv
{
namespace anim
{

class Anim15SegDecimalCounterDown : public Anim<ynv::ecd::DispDecimalNumber>
{
   public:
    Anim15SegDecimalCounterDown(std::shared_ptr<ynv::ecd::DispDecimalNumber> display)
        : Anim<ynv::ecd::DispDecimalNumber>(display)
    {
    }

   protected:
    void transition() override
    {
        static int counter = 99;

        m_display->show(counter / 10, counter % 10);  // Update display with new values
        if (--counter == 0)
        {
            counter = 99;
        }
    }
};

}  // namespace anim
}  // namespace ynv
