#include "app_gui.hpp"
#include "app_hal.hpp"
#include "bsp/esp-bsp.h"
#include "esp_log.h"
#include "evalkit_anims.hpp"

namespace
{
app::Gui&                m_gui   = app::Gui::getInstance();
ynv::anim::EvalkitAnims& m_anims = ynv::anim::EvalkitAnims::getInstance();
}  // namespace

extern "C" void app_main(void)
{
    bsp_i2c_init();
    m_gui.init();
}
