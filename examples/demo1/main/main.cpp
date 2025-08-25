#include "app_gui.hpp"
#include "bsp/esp-bsp.h"
#include "esp_log.h"

namespace
{
app::Gui& m_gui = app::Gui::getInstance();
}

extern "C" void app_main(void)
{
    bsp_i2c_init();
    m_gui.init();
}
