
#include "app_gui.hpp"

#include "bsp/esp-bsp.h"
#include "esp_log.h"
#include "lvgl.h"

#define APP_DISP_DEFAULT_BRIGHTNESS (50)

namespace app
{
esp_err_t Gui::init()
{
    /* Initialize display and LVGL */
    bsp_display_start();

    /* Set default display brightness */
    bsp_display_brightness_set(APP_DISP_DEFAULT_BRIGHTNESS);

    /* Add and show LVGL objects on display */
    show();

    return ESP_OK;
}

esp_err_t Gui::show()
{
    bsp_display_lock(0);

    lv_obj_t* tv    = lv_tileview_create(lv_screen_active());
    lv_obj_t* tile1 = lv_tileview_add_tile(tv, 0, 0, LV_DIR_BOTTOM);
    lv_obj_t* img1  = lv_image_create(tile1);
    lv_image_set_src(img1, DISP_FILE_MAP.at(evalkit::DISP431V2PV1));
    lv_obj_center(img1);

    bsp_display_unlock();
    return ESP_OK;
}

}  // namespace app