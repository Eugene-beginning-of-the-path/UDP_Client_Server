#pragma once

#include "Utils/Utils.h"

namespace dev
{
    struct WirePacket
    {
                                              //          [    HEADER    ]
        std::vector<unsigned char> m_rawData; //sequence: seqNum timeStamp payLoad 
        utls::SHA256Buff m_checkSum;
    };
}