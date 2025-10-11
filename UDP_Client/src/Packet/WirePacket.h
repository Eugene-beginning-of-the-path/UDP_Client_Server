#pragma once
#include "Packet/WireHeader.h"
#include "udptLib/Packet/Packet.h"

namespace dev
{
    struct WirePacket
    {
        Packet m_pcktData;
        WireHeader m_wireHeader;
                                           //          [---------------HEADER--------------]
        std::vector<unsigned char> m_wire; //sequence: seqNum timeStamp payLoadSize checkSum payLoad
    };

    std::ostream & operator<<(std::ostream & strm, const WirePacket& pctk);
}
