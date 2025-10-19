/**
 * @file anim_15.hpp
 * @brief Set position animation for the 3-segment bar display.
 * @date 2025-10-19
 * @copyright Copyright (c) 2025
 */

#pragma once

#include "anim.hpp"
#include "disp_3seg_bar.hpp"

namespace ynv
{
namespace anim
{

class Anim3SegBarPos : public Anim<ynv::ecd::Disp3SegBar>
{
   public:
    explicit Anim3SegBarPos(std::shared_ptr<ynv::ecd::Disp3SegBar> display) : Anim<ynv::ecd::Disp3SegBar>(display) { }

   protected:
    void transition() override
    {
        int        pos[3] = {1, 0, 2};  // Positions for the 3 segments
        static int posIdx = 0;
        m_display->position(pos[posIdx]);  // Set the initial position
        posIdx = (posIdx + 1) % 3;         // Cycle through positions
    }
};

}  // namespace anim
}  // namespace ynv
