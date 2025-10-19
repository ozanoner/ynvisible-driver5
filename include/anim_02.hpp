
#pragma once

#include "anim.hpp"
#include "disp_decimal_number.hpp"

namespace ynv
{
namespace anim
{

class Anim15SegSignedPositiveCounterUp : public Anim<ynv::ecd::DispSignedNumber>
{
   public:
    explicit Anim15SegSignedPositiveCounterUp(std::shared_ptr<ynv::ecd::DispSignedNumber> display)
        : Anim<ynv::ecd::DispSignedNumber>(display)
    {
    }

   protected:
    void transition() override
    {
        static int counter = 0;
        counter            = (counter + 1) % 100;            // Increment counter and wrap around at 100
        m_display->show(counter / 10, counter % 10, false);  // Update display with new values
    }
};

}  // namespace anim
}  // namespace ynv
