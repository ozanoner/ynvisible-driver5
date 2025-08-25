
#include "app_gui.hpp"

#include <array>

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
    const std::array<lv_dir_t, evalkit::DisplayInfo::DISP_CNT> dispDirs = {
        static_cast<lv_dir_t>(LV_DIR_BOTTOM | LV_DIR_RIGHT), static_cast<lv_dir_t>(LV_DIR_VER | LV_DIR_RIGHT),
        static_cast<lv_dir_t>(LV_DIR_VER | LV_DIR_RIGHT),    static_cast<lv_dir_t>(LV_DIR_VER | LV_DIR_RIGHT),
        static_cast<lv_dir_t>(LV_DIR_VER | LV_DIR_RIGHT),    static_cast<lv_dir_t>(LV_DIR_VER | LV_DIR_RIGHT),
        static_cast<lv_dir_t>(LV_DIR_VER | LV_DIR_RIGHT),    static_cast<lv_dir_t>(LV_DIR_VER | LV_DIR_RIGHT),
        static_cast<lv_dir_t>(LV_DIR_TOP | LV_DIR_RIGHT)};

    bsp_display_lock(0);

    lv_obj_t* tv = lv_tileview_create(lv_screen_active());

    for (size_t i = 0; i < dispDirs.size(); ++i)
    {
        // Create tile for each display
        lv_obj_t* tile = lv_tileview_add_tile(tv, 0, static_cast<evalkit::DisplayInfo::Ecd_e>(i), dispDirs[i]);
        lv_obj_t* img  = lv_image_create(tile);
        lv_image_set_src(img, DISP_FILE_MAP.at(static_cast<evalkit::DisplayInfo::Ecd_e>(i)));
        lv_obj_center(img);
        // add animation buttons
        addAnimationButtons(tv, static_cast<evalkit::DisplayInfo::Ecd_e>(i));
    }

    bsp_display_unlock();
    return ESP_OK;
}

esp_err_t Gui::addAnimationButtons(lv_obj_t* tv, evalkit::DisplayInfo::Ecd_e display)
{
    // add a new tile for animation buttons
    lv_obj_t* tile1 = lv_tileview_add_tile(tv, 1, display, LV_DIR_LEFT);
    // add anim buttons
    lv_obj_t* list1 = lv_list_create(tile1);
    lv_obj_set_size(list1, LV_PCT(100), LV_PCT(100));
    for (const auto& btnName : evalkit::DisplayInfo::DISP_ANIM_NAMES[display])
    {
        lv_obj_t* btn = lv_list_add_btn(list1, NULL, btnName);
    }

    lv_obj_t* label1 = lv_label_create(tile1);
    lv_label_set_recolor(label1, true);
    lv_label_set_text(label1, "#ff0000 Animation#");
    lv_obj_set_width(label1, 300);
    lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(label1, LV_ALIGN_BOTTOM_MID, 0, -40);

    return ESP_OK;
}

}  // namespace app