
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

    lv_obj_t* tv = lv_tileview_create(lv_screen_active());

    // Create tile for DISP431V2PV1
    lv_obj_t* tile1 = lv_tileview_add_tile(tv, 0, evalkit::DisplayInfo::DISP431V2PV1,
                                           static_cast<lv_dir_t>(LV_DIR_BOTTOM | LV_DIR_RIGHT));
    lv_obj_t* img1  = lv_image_create(tile1);
    lv_image_set_src(img1, DISP_FILE_MAP.at(evalkit::DisplayInfo::DISP431V2PV1));
    lv_obj_center(img1);
    // add animation buttons
    addAnimationButtons(tv, evalkit::DisplayInfo::DISP431V2PV1);

    // Create tile for DISP433V1PV1
    tile1 = lv_tileview_add_tile(tv, 0, evalkit::DisplayInfo::DISP433V1PV1,
                                 static_cast<lv_dir_t>(LV_DIR_VER | LV_DIR_RIGHT));
    img1  = lv_image_create(tile1);
    lv_image_set_src(img1, DISP_FILE_MAP.at(evalkit::DisplayInfo::DISP433V1PV1));
    lv_obj_center(img1);
    // add animation buttons
    addAnimationButtons(tv, evalkit::DisplayInfo::DISP433V1PV1);

    // create tile for DISP434V1PV1
    tile1 = lv_tileview_add_tile(tv, 0, evalkit::DisplayInfo::DISP434V1PV1,
                                 static_cast<lv_dir_t>(LV_DIR_VER | LV_DIR_RIGHT));
    img1  = lv_image_create(tile1);
    lv_image_set_src(img1, DISP_FILE_MAP.at(evalkit::DisplayInfo::DISP434V1PV1));
    lv_obj_center(img1);
    // add animation buttons
    addAnimationButtons(tv, evalkit::DisplayInfo::DISP434V1PV1);

    // create tile for DISP437V2PV1
    tile1 = lv_tileview_add_tile(tv, 0, evalkit::DisplayInfo::DISP437V2PV1,
                                 static_cast<lv_dir_t>(LV_DIR_VER | LV_DIR_RIGHT));
    img1  = lv_image_create(tile1);
    lv_image_set_src(img1, DISP_FILE_MAP.at(evalkit::DisplayInfo::DISP437V2PV1));
    lv_obj_center(img1);
    // add animation buttons
    addAnimationButtons(tv, evalkit::DisplayInfo::DISP437V2PV1);

    // create tile for DISP438V2PV1
    tile1 = lv_tileview_add_tile(tv, 0, evalkit::DisplayInfo::DISP438V2PV1,
                                 static_cast<lv_dir_t>(LV_DIR_VER | LV_DIR_RIGHT));
    img1  = lv_image_create(tile1);
    lv_image_set_src(img1, DISP_FILE_MAP.at(evalkit::DisplayInfo::DISP438V2PV1));
    lv_obj_center(img1);
    // add animation buttons
    addAnimationButtons(tv, evalkit::DisplayInfo::DISP438V2PV1);

    // create tile for DISP440V2PV1
    tile1 = lv_tileview_add_tile(tv, 0, evalkit::DisplayInfo::DISP440V2PV1,
                                 static_cast<lv_dir_t>(LV_DIR_VER | LV_DIR_RIGHT));
    img1  = lv_image_create(tile1);
    lv_image_set_src(img1, DISP_FILE_MAP.at(evalkit::DisplayInfo::DISP440V2PV1));
    lv_obj_center(img1);
    // add animation buttons
    addAnimationButtons(tv, evalkit::DisplayInfo::DISP440V2PV1);

    // create tile for DISP442V2PV1
    tile1 = lv_tileview_add_tile(tv, 0, evalkit::DisplayInfo::DISP442V2PV1,
                                 static_cast<lv_dir_t>(LV_DIR_VER | LV_DIR_RIGHT));
    img1  = lv_image_create(tile1);
    lv_image_set_src(img1, DISP_FILE_MAP.at(evalkit::DisplayInfo::DISP442V2PV1));
    lv_obj_center(img1);
    // add animation buttons
    addAnimationButtons(tv, evalkit::DisplayInfo::DISP442V2PV1);

    // create tile for DISP443V2PV1
    tile1 = lv_tileview_add_tile(tv, 0, evalkit::DisplayInfo::DISP443V2PV1,
                                 static_cast<lv_dir_t>(LV_DIR_VER | LV_DIR_RIGHT));
    img1  = lv_image_create(tile1);
    lv_image_set_src(img1, DISP_FILE_MAP.at(evalkit::DisplayInfo::DISP443V2PV1));
    lv_obj_center(img1);
    // add animation buttons
    addAnimationButtons(tv, evalkit::DisplayInfo::DISP443V2PV1);

    // create tile for DISP444V1PV2
    tile1 = lv_tileview_add_tile(tv, 0, evalkit::DisplayInfo::DISP444V1PV2,
                                 static_cast<lv_dir_t>(LV_DIR_TOP | LV_DIR_RIGHT));
    img1  = lv_image_create(tile1);
    lv_image_set_src(img1, DISP_FILE_MAP.at(evalkit::DisplayInfo::DISP444V1PV2));
    lv_obj_center(img1);
    // add animation buttons
    addAnimationButtons(tv, evalkit::DisplayInfo::DISP444V1PV2);

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

    return ESP_OK;
}

}  // namespace app