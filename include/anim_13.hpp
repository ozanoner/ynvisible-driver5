
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
    Anim3SegBarCounterUp(std::shared_ptr<ynv::ecd::Disp3SegBar> display) : Anim<ynv::ecd::Disp3SegBar>(display) { }

   protected:
    void transition() override
    {
        m_display->increment();  // Increment the bar display
    }
};

}  // namespace anim
}  // namespace ynv
