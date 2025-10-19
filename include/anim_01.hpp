/**
 * @file anim_01.hpp
 * @brief Toggle animation (available for all display types).
 * @date 2025-10-19
 * @copyright Copyright (c) 2025
 */

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
    explicit AnimToggle(std::shared_ptr<ynv::ecd::ECDBase> display) : Anim<ynv::ecd::ECDBase>(display) { }

   protected:
    void transition() override
    {
        m_display->toggle();  // Toggle the display state
    }
};

}  // namespace anim
}  // namespace ynv
