/**
 * @file anim_08.hpp
 * @brief On animation for the single segment display.
 * @date 2025-10-19
 * @copyright Copyright (c) 2025
 */

#pragma once

#include "anim.hpp"
#include "disp_single_segment.hpp"

namespace ynv
{
namespace anim
{

class Anim1SegOn : public Anim<ynv::ecd::DispSingleSegment>
{
   public:
    explicit Anim1SegOn(std::shared_ptr<ynv::ecd::DispSingleSegment> display)
        : Anim<ynv::ecd::DispSingleSegment>(display)
    {
    }

   protected:
    void transition() override
    {
        m_display->toggle();  // Toggle the display state
    }
};

}  // namespace anim
}  // namespace ynv
