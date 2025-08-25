#include <cinttypes>
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>

#include "disp_images.hpp"
#include "esp_err.h"
#include "evalkit_displays.hpp"

namespace app
{
class Gui
{
   public:
    // Get the singleton instance
    static Gui& getInstance()
    {
        static Gui instance;
        return instance;
    }

    // Delete copy constructor and assignment operator
    Gui(const Gui&)            = delete;
    Gui& operator=(const Gui&) = delete;

    ~Gui() = default;

    esp_err_t init();

   private:
    // Private constructor
    Gui() = default;

    static constexpr const char* TAG             = "Gui";
    static constexpr const char* DISP_IMAGES_DIR = "disp";
    static constexpr size_t      MAX_FILE_LEN    = 9216;

    static inline const std::map<evalkit::Ecd_e, const lv_image_dsc_t*> DISP_FILE_MAP = {
        {evalkit::DISP431V2PV1, &disp431v2pv1}, {evalkit::DISP433V1PV1, &disp433v1pv1},
        {evalkit::DISP434V1PV1, &disp434v1pv1}, {evalkit::DISP437V2PV1, &disp437v2pv1},
        {evalkit::DISP438V2PV1, &disp438v2pv1}, {evalkit::DISP440V2PV1, &disp440v2pv1},
        {evalkit::DISP442V2PV1, &disp442v2pv1}, {evalkit::DISP443V2PV1, &disp443v2pv1},
        {evalkit::DISP444V1PV2, &disp444v1pv2}};

    esp_err_t show();
};
}  // namespace app