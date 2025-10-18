#include <cinttypes>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <map>
#include <string>

#include "app_disp_info.hpp"
#include "disp_images.hpp"
#include "esp_err.h"

namespace app
{
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

    struct BtnInfo
    {
        app::DisplayInfo::ECD_t           display;
        const char*                       animBtnName;
        std::function<void(lv_event_t*)>* animHandler;
        bool                              checked;
        lv_obj_t*                         statusLabel;
    };

    static constexpr const char* TAG = "GUI";

   private:
    // Private constructor
    GUI() = default;

    static inline const std::map<app::DisplayInfo::ECD_t, const lv_image_dsc_t*> m_dispFileMap = {
        {app::DisplayInfo::DISP431V2PV1, &disp431v2pv1}, {app::DisplayInfo::DISP433V1PV1, &disp433v1pv1},
        {app::DisplayInfo::DISP434V1PV1, &disp434v1pv1}, {app::DisplayInfo::DISP437V2PV1, &disp437v2pv1},
        {app::DisplayInfo::DISP438V2PV1, &disp438v2pv1}, {app::DisplayInfo::DISP440V2PV1, &disp440v2pv1},
        {app::DisplayInfo::DISP442V2PV1, &disp442v2pv1}, {app::DisplayInfo::DISP443V2PV1, &disp443v2pv1},
        {app::DisplayInfo::DISP444V1PV2, &disp444v1pv2}};

    esp_err_t show();
    esp_err_t addAnimationButtons(lv_obj_t* tv, app::DisplayInfo::ECD_t display);

    static constexpr size_t MAX_BTN_INFOS = 64;
    std::vector<BtnInfo>    btnInfos;
};
}  // namespace app