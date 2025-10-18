#include "app_gui.hpp"
#include "app_hal.hpp"
#include "bsp/esp-bsp.h"
#include "esp_log.h"
#include "evalkit_anims.hpp"
#include "evalkit_displays.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace
{
app::GUI& m_gui = app::GUI::getInstance();

ynv::app::AppConfig_t appConfig;

auto& anims    = ynv::anim::EvalkitAnims::getInstance();
auto& displays = ynv::ecd::EvalkitDisplays::getInstance();
auto& hal      = app::hal::HAL::getInstance();
}  // namespace

// Animation update task
static void animTask(void* pvParameters);

extern "C" void app_main(void)
{
    // set application config
    appConfig.hal = &hal;
    // appConfig.displayIndex = ynv::ecd::EvalkitDisplays::ECDEvalkitDisplay_t::EVALKIT_DISP_TEST;

    appConfig.activeDriving     = true;
    appConfig.analogResolution  = 12;
    appConfig.maxSegmentVoltage = ynv::app::AppConfig_t::MAX_SEGMENT_VOLTAGE;
    appConfig.highPinVoltage    = ynv::app::AppConfig_t::HIGH_PIN_VOLTAGE;

    bsp_i2c_init();
    m_gui.init();

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

    // anims.select(ynv::ecd::EvalkitDisplays::ECDEvalkitDisplay_t::EVALKIT_DISP_TEST,
    //              ynv::anim::EvalkitAnims::Anim_t::ANIM_TOGGLE);

    m_gui.registerButtonHandler(
        [](const app::disp::DisplayAnimInfo_t* info)
        {
            if (info != nullptr)
            {
                anims.select(info->displayType, info->animType);
            }
        });

    // Create animation task
    (void)xTaskCreate(animTask, "anim-update", 4096, nullptr, 5, nullptr);
}

static void animTask(void* pvParameters)
{
    while (true)
    {
        if (anims.isSelected())
        {
            anims.getCurrentAnim().update();
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
