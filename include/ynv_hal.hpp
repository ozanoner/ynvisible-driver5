#pragma once

#include "app_config.hpp"
#include "esp_err.h"

namespace ynv
{
namespace driver
{
class HALBase
{
   public:
    virtual esp_err_t digitalWrite(int pin, bool high, int delay = 10, int common = 0) = 0;
    virtual int       analogRead(int pin)                                              = 0;

    static constexpr const char* TAG = "HAL";

   protected:
    ynv::app::AppConfig_t* m_appConfig;  // Pointer to application configuration
};

}  // namespace driver
}  // namespace ynv
