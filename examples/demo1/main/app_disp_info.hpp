/**
 * @file app_disp_info.hpp
 * @brief ECD information and animation mapping
 * @date 2025-10-19
 * @copyright Copyright (c) 2025
 */

#pragma once

#include <map>
#include <vector>

#include "evalkit_anims.hpp"
#include "evalkit_displays.hpp"

namespace app
{

namespace disp
{
enum ECD_t
{
    DISP431V2PV1 = 0,
    DISP433V1PV1,
    DISP434V1PV1,
    DISP437V2PV1,
    DISP438V2PV1,
    DISP440V2PV1,
    DISP442V2PV1,
    DISP443V2PV1,
    DISP444V1PV2,
    DISP_CNT
};

struct DisplayAnimInfo_t
{
    ynv::ecd::EvalkitDisplays::ECDEvalkitDisplay_t displayType;
    const char*                                    animName;
    ynv::anim::EvalkitAnims::Anim_t                animType;

    DisplayAnimInfo_t(ynv::ecd::EvalkitDisplays::ECDEvalkitDisplay_t display, const char* name,
                      ynv::anim::EvalkitAnims::Anim_t anim)
        : displayType(display), animName(name), animType(anim)
    {
    }
};

// Map from disp::ECD_t to supported animations with full info
static inline const std::map<ECD_t, std::vector<DisplayAnimInfo_t>> ECD_ANIM_INFO = {
    {DISP431V2PV1,
     {{ynv::ecd::EvalkitDisplays::EVALKIT_DISP_SEVEN_SEGMENT_BAR_DISPLAY, ynv::anim::ANIM_NAME_TOGGLE,
       ynv::anim::EvalkitAnims::ANIM_TOGGLE},
      {ynv::ecd::EvalkitDisplays::EVALKIT_DISP_SEVEN_SEGMENT_BAR_DISPLAY, ynv::anim::ANIM_NAME_COUNT_UP,
       ynv::anim::EvalkitAnims::ANIM_UP},
      {ynv::ecd::EvalkitDisplays::EVALKIT_DISP_SEVEN_SEGMENT_BAR_DISPLAY, ynv::anim::ANIM_NAME_COUNT_DOWN,
       ynv::anim::EvalkitAnims::ANIM_DOWN}}},
    {DISP433V1PV1,
     {{ynv::ecd::EvalkitDisplays::EVALKIT_DISP_DECIMAL_NUMBER_DISPLAY, ynv::anim::ANIM_NAME_TOGGLE,
       ynv::anim::EvalkitAnims::ANIM_TOGGLE},
      {ynv::ecd::EvalkitDisplays::EVALKIT_DISP_DECIMAL_NUMBER_DISPLAY, ynv::anim::ANIM_NAME_COUNT_UP,
       ynv::anim::EvalkitAnims::ANIM_UP},
      {ynv::ecd::EvalkitDisplays::EVALKIT_DISP_DECIMAL_NUMBER_DISPLAY, ynv::anim::ANIM_NAME_COUNT_DOWN,
       ynv::anim::EvalkitAnims::ANIM_DOWN}}},
    {DISP434V1PV1,
     {{ynv::ecd::EvalkitDisplays::EVALKIT_DISP_DOT_NUMBER_DISPLAY, ynv::anim::ANIM_NAME_TOGGLE,
       ynv::anim::EvalkitAnims::ANIM_TOGGLE},
      {ynv::ecd::EvalkitDisplays::EVALKIT_DISP_DOT_NUMBER_DISPLAY, ynv::anim::ANIM_NAME_COUNT_UP,
       ynv::anim::EvalkitAnims::ANIM_UP},
      {ynv::ecd::EvalkitDisplays::EVALKIT_DISP_DOT_NUMBER_DISPLAY, ynv::anim::ANIM_NAME_COUNT_DOWN,
       ynv::anim::EvalkitAnims::ANIM_DOWN}}},
    {DISP437V2PV1,
     {{ynv::ecd::EvalkitDisplays::EVALKIT_DISP_SINGLE_SEGMENT_DISPLAY, ynv::anim::ANIM_NAME_TOGGLE,
       ynv::anim::EvalkitAnims::ANIM_TOGGLE}}},
    {DISP438V2PV1,
     {{ynv::ecd::EvalkitDisplays::EVALKIT_DISP_THREE_SEGMENT_BAR_DISPLAY, ynv::anim::ANIM_NAME_TOGGLE,
       ynv::anim::EvalkitAnims::ANIM_TOGGLE},
      {ynv::ecd::EvalkitDisplays::EVALKIT_DISP_THREE_SEGMENT_BAR_DISPLAY, ynv::anim::ANIM_NAME_COUNT_UP,
       ynv::anim::EvalkitAnims::ANIM_UP},
      {ynv::ecd::EvalkitDisplays::EVALKIT_DISP_THREE_SEGMENT_BAR_DISPLAY, ynv::anim::ANIM_NAME_COUNT_DOWN,
       ynv::anim::EvalkitAnims::ANIM_DOWN}}},
    {DISP440V2PV1,
     {{ynv::ecd::EvalkitDisplays::EVALKIT_DISP_SINGLE_SEGMENT_DISPLAY, ynv::anim::ANIM_NAME_TOGGLE,
       ynv::anim::EvalkitAnims::ANIM_TOGGLE}}},
    {DISP442V2PV1,
     {{ynv::ecd::EvalkitDisplays::EVALKIT_DISP_SINGLE_SEGMENT_DISPLAY, ynv::anim::ANIM_NAME_TOGGLE,
       ynv::anim::EvalkitAnims::ANIM_TOGGLE}}},
    {DISP443V2PV1,
     {{ynv::ecd::EvalkitDisplays::EVALKIT_DISP_SINGLE_SEGMENT_DISPLAY, ynv::anim::ANIM_NAME_TOGGLE,
       ynv::anim::EvalkitAnims::ANIM_TOGGLE}}},
    {DISP444V1PV2,
     {{ynv::ecd::EvalkitDisplays::EVALKIT_DISP_SIGNED_NUMBER_DISPLAY, ynv::anim::ANIM_NAME_TOGGLE,
       ynv::anim::EvalkitAnims::ANIM_TOGGLE},
      {ynv::ecd::EvalkitDisplays::EVALKIT_DISP_SIGNED_NUMBER_DISPLAY, ynv::anim::ANIM_NAME_COUNT_UP,
       ynv::anim::EvalkitAnims::ANIM_UP},
      {ynv::ecd::EvalkitDisplays::EVALKIT_DISP_SIGNED_NUMBER_DISPLAY, ynv::anim::ANIM_NAME_COUNT_DOWN,
       ynv::anim::EvalkitAnims::ANIM_DOWN}}}};
}  // namespace disp
}  // namespace app
