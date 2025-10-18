#include <cinttypes>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <map>
#include <string>

#include "app_disp_info.hpp"
#include "bsp/esp-bsp.h"
#include "disp_images.hpp"
#include "esp_err.h"

namespace app
{
/**
 * @brief GUI class for managing the graphical user interface
 */
class GUI
{
   public:
    // Get the singleton instance
    static GUI& getInstance()
    {
        static GUI instance;
        return instance;
    }

    // Delete copy constructor and assignment operator
    GUI(const GUI&)            = delete;
    GUI& operator=(const GUI&) = delete;

    ~GUI() = default;

    esp_err_t init();

    struct BtnInfo_t
    {
        const app::disp::DisplayAnimInfo_t* animInfo;
        std::function<void(lv_event_t*)>    animHandler;
        lv_obj_t*                           statusLabel;
        // bool                                checked;
    };

    void registerButtonHandler(std::function<void(const app::disp::DisplayAnimInfo_t*)> handler)
    {
        assert(handler);
        m_animHandler = handler;
    }

    static constexpr const char* TAG = "GUI";

   private:
    // Private constructor
    GUI() = default;

    /**
     * @brief RAII class for GUI locking
     */
    class GUILock
    {
       public:
        GUILock() { bsp_display_lock(0); }
        ~GUILock() { bsp_display_unlock(); }
    };

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