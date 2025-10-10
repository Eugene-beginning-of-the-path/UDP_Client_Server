#pragma once

#include "Utils/Utils.h"

namespace dev
{
    struct WireHeader
    {
        //Integer types are already big-ending sequence bytes:
        uint16_t m_seqNum;                          
        uint64_t m_timeStampNs;                     
        size_t   m_payLoadSize;

        utls::SHA256Buff m_checkSum;
                                     //          [---------------HEADER--------------]
        utls::HeadBuff m_wireHeader; //sequence: seqNum timeStamp payLoadSize checkSum
    };
}