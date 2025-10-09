#include <cinttypes>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <map>
#include <string>

#include "disp_images.hpp"
#include "esp_err.h"
#include "evalkit_displays.hpp"

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
        evalkit::DisplayInfo::Ecd_e       display;
        const char*                       animBtnName;
        std::function<void(lv_event_t*)>* animHandler;
        bool                              checked;
        lv_obj_t*                         statusLabel;
    };

    static constexpr const char* TAG = "GUI";

   private:
    // Private constructor
    GUI() = default;

    static inline const std::map<evalkit::DisplayInfo::Ecd_e, const lv_image_dsc_t*> DISP_FILE_MAP = {
        {evalkit::DisplayInfo::DISP431V2PV1, &disp431v2pv1}, {evalkit::DisplayInfo::DISP433V1PV1, &disp433v1pv1},
        {evalkit::DisplayInfo::DISP434V1PV1, &disp434v1pv1}, {evalkit::DisplayInfo::DISP437V2PV1, &disp437v2pv1},
        {evalkit::DisplayInfo::DISP438V2PV1, &disp438v2pv1}, {evalkit::DisplayInfo::DISP440V2PV1, &disp440v2pv1},
        {evalkit::DisplayInfo::DISP442V2PV1, &disp442v2pv1}, {evalkit::DisplayInfo::DISP443V2PV1, &disp443v2pv1},
        {evalkit::DisplayInfo::DISP444V1PV2, &disp444v1pv2}};

    esp_err_t show();
    esp_err_t addAnimationButtons(lv_obj_t* tv, evalkit::DisplayInfo::Ecd_e display);

    static constexpr size_t MAX_BTN_INFOS = 64;
    std::vector<BtnInfo>    btnInfos;
};
}  // namespace app