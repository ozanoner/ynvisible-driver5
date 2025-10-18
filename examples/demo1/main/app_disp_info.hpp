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

// Map from disp::ECD_t to supported animation names
static inline const std::map<ECD_t, std::vector<const char*>> m_dispAnimNames = {
    {DISP431V2PV1, {ynv::anim::ANIM_NAME_TOGGLE, ynv::anim::ANIM_NAME_COUNT_UP, ynv::anim::ANIM_NAME_COUNT_DOWN}},
    {DISP433V1PV1, {ynv::anim::ANIM_NAME_TOGGLE, ynv::anim::ANIM_NAME_COUNT_UP}},
    {DISP434V1PV1, {ynv::anim::ANIM_NAME_TOGGLE, ynv::anim::ANIM_NAME_COUNT_UP}},
    {DISP437V2PV1, {ynv::anim::ANIM_NAME_TOGGLE}},
    {DISP438V2PV1, {ynv::anim::ANIM_NAME_TOGGLE, ynv::anim::ANIM_NAME_COUNT_UP, ynv::anim::ANIM_NAME_COUNT_DOWN}},
    {DISP440V2PV1, {ynv::anim::ANIM_NAME_TOGGLE}},
    {DISP442V2PV1, {ynv::anim::ANIM_NAME_TOGGLE}},
    {DISP443V2PV1, {ynv::anim::ANIM_NAME_TOGGLE}},
    {DISP444V1PV2, {ynv::anim::ANIM_NAME_TOGGLE, ynv::anim::ANIM_NAME_COUNT_UP, ynv::anim::ANIM_NAME_COUNT_DOWN}}};

// Map from disp::ECD_t to EvalkitDisplays::ECDEvalkitDisplay_t
static inline const std::map<ECD_t, ynv::ecd::EvalkitDisplays::ECDEvalkitDisplay_t> m_ecdTypeMap = {
    {DISP431V2PV1, ynv::ecd::EvalkitDisplays::EVALKIT_DISP_SEVEN_SEGMENT_BAR_DISPLAY},
    {DISP433V1PV1, ynv::ecd::EvalkitDisplays::EVALKIT_DISP_DECIMAL_NUMBER_DISPLAY},
    {DISP434V1PV1, ynv::ecd::EvalkitDisplays::EVALKIT_DISP_DOT_NUMBER_DISPLAY},
    {DISP437V2PV1, ynv::ecd::EvalkitDisplays::EVALKIT_DISP_SINGLE_SEGMENT_DISPLAY},
    {DISP438V2PV1, ynv::ecd::EvalkitDisplays::EVALKIT_DISP_THREE_SEGMENT_BAR_DISPLAY},
    {DISP440V2PV1, ynv::ecd::EvalkitDisplays::EVALKIT_DISP_SINGLE_SEGMENT_DISPLAY},
    {DISP442V2PV1, ynv::ecd::EvalkitDisplays::EVALKIT_DISP_SINGLE_SEGMENT_DISPLAY},
    {DISP443V2PV1, ynv::ecd::EvalkitDisplays::EVALKIT_DISP_SINGLE_SEGMENT_DISPLAY},
    {DISP444V1PV2, ynv::ecd::EvalkitDisplays::EVALKIT_DISP_SIGNED_NUMBER_DISPLAY}};
}  // namespace disp
}  // namespace app
