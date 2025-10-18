#pragma once

#include <array>
#include <map>
#include <vector>

#include "evalkit_displays.hpp"

namespace app
{

struct DisplayInfo
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

    static inline const std::array<std::vector<const char*>, DISP_CNT> m_dispAnimNames = {
        {{"Toggle", "Count up", "Count down"},
         {"Toggle", "Count up"},
         {"Toggle", "Count up"},
         {"Toggle"},
         {"Toggle", "Count up", "Count down"},
         {"Toggle"},
         {"Toggle"},
         {"Toggle"},
         {"Toggle", "Count up", "Count down"}}};

    // Map from DisplayInfo::ECD_t to EvalkitDisplays::ECDEvalkitDisplay_t
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
};
}  // namespace app
