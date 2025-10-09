#pragma once

#include "Packet/Packet.h"
#include "Packet/WireHeader.h"

namespace dev
{
    struct WirePacket
    {
        Packet m_pcktData;
        WireHeader m_wireHeader;
                                           //          [---------------HEADER--------------]
        std::vector<unsigned char> m_wire; //sequence: seqNum timeStamp payLoadSize checkSum payLoad
    };
}
