#include "app_gui.hpp"

#include <array>

#include "bsp/esp-bsp.h"
#include "esp_log.h"
#include "lvgl.h"

#define APP_DISP_DEFAULT_BRIGHTNESS (50)

namespace app
{
esp_err_t GUI::init()
{
    btnInfos.reserve(MAX_BTN_INFOS);
    /* Initialize display and LVGL */
    bsp_display_start();

    /* Set default display brightness */
    bsp_display_brightness_set(APP_DISP_DEFAULT_BRIGHTNESS);

    /* Add and show LVGL objects on display */
    show();

    return ESP_OK;
}

esp_err_t GUI::show()
{
    GUILock lock;

    constexpr std::array<lv_dir_t, app::disp::DISP_CNT> dispDirs = {
        static_cast<lv_dir_t>(LV_DIR_BOTTOM | LV_DIR_RIGHT), static_cast<lv_dir_t>(LV_DIR_VER | LV_DIR_RIGHT),
        static_cast<lv_dir_t>(LV_DIR_VER | LV_DIR_RIGHT),    static_cast<lv_dir_t>(LV_DIR_VER | LV_DIR_RIGHT),
        static_cast<lv_dir_t>(LV_DIR_VER | LV_DIR_RIGHT),    static_cast<lv_dir_t>(LV_DIR_VER | LV_DIR_RIGHT),
        static_cast<lv_dir_t>(LV_DIR_VER | LV_DIR_RIGHT),    static_cast<lv_dir_t>(LV_DIR_VER | LV_DIR_RIGHT),
        static_cast<lv_dir_t>(LV_DIR_TOP | LV_DIR_RIGHT)};

    lv_obj_t* tv = lv_tileview_create(lv_screen_active());

    for (size_t i = 0; i < dispDirs.size(); ++i)
    {
        // Create tile for each display
        lv_obj_t* tile = lv_tileview_add_tile(tv, 0, static_cast<app::disp::ECD_t>(i), dispDirs[i]);
        lv_obj_t* img  = lv_image_create(tile);
        lv_image_set_src(img, m_dispFileMap.at(static_cast<app::disp::ECD_t>(i)));
        lv_obj_center(img);
        // add animation buttons
        addAnimationButtons(tv, static_cast<app::disp::ECD_t>(i));
    }

    return ESP_OK;
}

static void btnEventHandler(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        GUI::BtnInfo_t& btnInfo = *static_cast<GUI::BtnInfo_t*>(lv_event_get_user_data(e));
        ESP_LOGI(GUI::TAG, "Button event (%s)", btnInfo.animInfo->animName);

        // btnInfo.checked = !btnInfo.checked;
        // lv_label_set_text(btnInfo.statusLabel, btnInfo.checked ? "#00ff00 Playing#" : "#ff0000 Select#");

        if (btnInfo.animHandler)
        {
            btnInfo.animHandler(e);
        }
    }
}

esp_err_t GUI::addAnimationButtons(lv_obj_t* tv, app::disp::ECD_t display)
{
    // add a new tile for animation buttons
    lv_obj_t* tile1 = lv_tileview_add_tile(tv, 1, display, LV_DIR_LEFT);

    // status label
    lv_obj_t* label1 = lv_label_create(tile1);
    lv_label_set_recolor(label1, true);
    lv_label_set_text(label1, "#ff0000 Select#");
    lv_obj_set_width(label1, 300);
    lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(label1, LV_ALIGN_TOP_MID, 0, 20);

    // add anim buttons
    lv_obj_t* list1 = lv_list_create(tile1);
    lv_obj_set_size(list1, LV_PCT(100), LV_PCT(100));
    for (const auto& animInfo : app::disp::ECD_ANIM_INFO.at(display))
    {
        btnInfos.emplace_back(BtnInfo_t {
            &animInfo,
            [this](lv_event_t* e)
            {
                GUI::BtnInfo_t& btnInfo = *static_cast<GUI::BtnInfo_t*>(lv_event_get_user_data(e));
                if (m_animHandler != nullptr)
                {
                    ESP_LOGI(GUI::TAG, "Calling animation handler for animation: %s", btnInfo.animInfo->animName);
                    m_animHandler(btnInfo.animInfo);
                }
            },
            label1});

        lv_obj_t* btn = lv_list_add_btn(list1, nullptr, animInfo.animName);
        lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
        lv_obj_add_event_cb(btn, btnEventHandler, LV_EVENT_VALUE_CHANGED, &btnInfos.back());
    }
    lv_obj_align_to(list1, label1, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);

    return ESP_OK;
}

}  // namespace app