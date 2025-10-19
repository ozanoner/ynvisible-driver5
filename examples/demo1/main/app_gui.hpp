/**
 * @file app_gui.hpp
 * @brief GUI management for YnVisible EvalKit demonstration
 */

#pragma once

#include <cinttypes>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <map>
#include <string>
#include <vector>

#include "app_disp_info.hpp"
#include "bsp/esp-bsp.h"
#include "disp_images.hpp"
#include "esp_err.h"

namespace app
{
/**
 * @brief Singleton GUI manager using LVGL for display selection and animation control
 */
class GUI
{
   public:
    /**
     * @brief Get singleton instance
     * @return Reference to GUI instance
     */
    static GUI& getInstance()
    {
        static GUI instance;
        return instance;
    }

    GUI(const GUI&)            = delete;
    GUI& operator=(const GUI&) = delete;
    ~GUI()                     = default;

    /**
     * @brief Initialize GUI system and create UI layout
     * @return ESP_OK on success
     */
    esp_err_t init();

    /**
     * @brief Button information structure
     */
    struct BtnInfo_t
    {
        const app::disp::DisplayAnimInfo_t* animInfo;     ///< Animation info
        std::function<void(lv_event_t*)>    animHandler;  ///< Event handler
        lv_obj_t*                           statusLabel;  ///< Status label
    };

    /**
     * @brief Register callback for animation button events
     * @param handler Callback function
     */
    void registerButtonHandler(std::function<void(const app::disp::DisplayAnimInfo_t*)> handler)
    {
        assert(handler);
        m_animHandler = handler;
    }

    static constexpr const char* TAG = "GUI";

   private:
    GUI() = default;

    /**
     * @brief RAII display lock
     */
    class GUILock
    {
       public:
        GUILock() { bsp_display_lock(0); }
        ~GUILock() { bsp_display_unlock(); }
    };

    /** @brief Mapping from ECD types to display images */
    static inline const std::map<app::disp::ECD_t, const lv_image_dsc_t*> m_dispFileMap = {
        {app::disp::DISP431V2PV1, &disp431v2pv1}, {app::disp::DISP433V1PV1, &disp433v1pv1},
        {app::disp::DISP434V1PV1, &disp434v1pv1}, {app::disp::DISP437V2PV1, &disp437v2pv1},
        {app::disp::DISP438V2PV1, &disp438v2pv1}, {app::disp::DISP440V2PV1, &disp440v2pv1},
        {app::disp::DISP442V2PV1, &disp442v2pv1}, {app::disp::DISP443V2PV1, &disp443v2pv1},
        {app::disp::DISP444V1PV2, &disp444v1pv2}};

    esp_err_t show();
    esp_err_t addAnimationButtons(lv_obj_t* tv, app::disp::ECD_t display);

    static constexpr size_t                                  MAX_BTN_INFOS = 64;
    std::vector<BtnInfo_t>                                   btnInfos;
    std::function<void(const app::disp::DisplayAnimInfo_t*)> m_animHandler = nullptr;
};
}  // namespace app