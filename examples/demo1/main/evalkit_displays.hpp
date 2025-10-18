#pragma once

#include <array>
#include <vector>

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
};
}  // namespace app
