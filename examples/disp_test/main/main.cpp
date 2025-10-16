
#include "app_hal.hpp"
#include "esp_log.h"
#include "evalkit_anims.hpp"
#include "evalkit_displays.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace
{
ynv::app::AppConfig_t appConfig;

auto& anims    = ynv::anim::EvalkitAnims::getInstance();
auto& displays = ynv::ecd::EvalkitDisplays::getInstance();
auto& hal      = app::hal::HAL::getInstance();
}  // namespace

extern "C" void app_main(void)
{
    // set application config
    appConfig.hal          = &hal;
    appConfig.displayIndex = ynv::ecd::EvalkitDisplays::ECDEvalkitDisplay_t::EVALKIT_DISP_TEST;

#ifdef CONFIG_ECD_DRIVING_ACTIVE
    appConfig.activeDriving = true;
#else
    appConfig.activeDriving = false;
#endif

    appConfig.analogResolution  = 12;
    appConfig.maxSegmentVoltage = ynv::app::AppConfig_t::MAX_SEGMENT_VOLTAGE;
    appConfig.highPinVoltage    = ynv::app::AppConfig_t::HIGH_PIN_VOLTAGE;

    // initialise HAL
    hal.init(&appConfig,
             // mux
             {.s0     = GPIO_NUM_11,
              .s1     = GPIO_NUM_9,
              .s2     = GPIO_NUM_14,
              .s3     = GPIO_NUM_13,
              .signal = GPIO_NUM_10,
              .enable = GPIO_NUM_12},
             // dac
             {.busHandle  = nullptr,
              .i2cPort    = 0,
              .i2cAddr    = 0x60,
              .i2cSdaGpio = GPIO_NUM_38,
              .i2cSclGpio = GPIO_NUM_41,
              .i2cFreqHz  = 100000});

    // initialise displays
    displays.init(&appConfig);

    // initialise animations
    anims.init(&appConfig);
    anims.select(ynv::anim::EvalkitAnims::Anim_t::ANIM_TOGGLE);

    auto& currentAnim = anims.getCurrentAnim();
    currentAnim.start();

    while (true)
    {
        currentAnim.update();
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}
