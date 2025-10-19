
#pragma once

#include "anim.hpp"
#include "disp_7seg_bar.hpp"

namespace ynv
{
namespace anim
{

class Anim7SegBarCounterUp : public Anim<ynv::ecd::Disp7SegBar>
{
   public:
    explicit Anim7SegBarCounterUp(std::shared_ptr<ynv::ecd::Disp7SegBar> display) : Anim<ynv::ecd::Disp7SegBar>(display)
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
