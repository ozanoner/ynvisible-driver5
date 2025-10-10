
#pragma once

#include <cinttypes>

#include "anim.hpp"
#include "disp_test.hpp"
#include "esp_log.h"

namespace ynv
{
namespace anim
{

class AnimTest : public Anim<ynv::ecd::DispTest>
{
   public:
    AnimTest(std::shared_ptr<ynv::ecd::DispTest> display) : Anim<ynv::ecd::DispTest>(display) { }

   protected:
    uint8_t m_pos = 0;  // Current segment position in the animation

    void transition() override
    {
        ESP_LOGI("AnimTest", "Coloring segment. index=%d", m_pos);
        m_display->show(m_pos++);
        m_pos %= m_display->getSegmentCount();
    }
};

}  // namespace anim
}  // namespace ynv
