
#pragma once

#include "anim.hpp"
#include "ecd.hpp"

namespace ynv
{
namespace anim
{

class AnimToggle : public Anim<ynv::ecd::ECDBase>
{
   public:
    AnimToggle(std::shared_ptr<ynv::ecd::ECDBase> display) : Anim<ynv::ecd::ECDBase>(display) { }

   protected:
    void transition() override
    {
        m_display->toggle();  // Toggle the display state
    }
};

}  // namespace anim
}  // namespace ynv
