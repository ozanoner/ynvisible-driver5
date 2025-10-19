
#pragma once

#include "anim.hpp"
#include "disp_dot_number.hpp"

namespace ynv
{
namespace anim
{

class Anim7SegNumCounterDown : public Anim<ynv::ecd::DispDotNumber>
{
   public:
    explicit Anim7SegNumCounterDown(std::shared_ptr<ynv::ecd::DispDotNumber> display)
        : Anim<ynv::ecd::DispDotNumber>(display)
    {
    }

   protected:
    void transition() override
    {
        static int number = 0;                  // Initialize number to 0
        number            = (number - 1) % 10;  // Decrement number and wrap around at 10
        m_display->show(number);
    }
};

}  // namespace anim
}  // namespace ynv
