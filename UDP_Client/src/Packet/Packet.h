#pragma once
#include <cstdint>
#include <vector>

#include "Utils/Utils.h"

namespace dev
{
    struct Packet
    {
        static inline const uint16_t PCKT_HEADER_SIZE = (sizeof(uint16_t) + sizeof(uint64_t));

        //Header fields:
        uint16_t m_seqNum;                          //sequence number of pckt
        uint64_t m_timeStampNs;                     //time (nano sec) since the beggining of the epoch
        size_t   m_payLoadSize;                     //just pay load size
        utls::SHA256Buff m_checkSum;

        //PayLoad:
        std::vector<unsigned char> m_payload;       //random data with lenght [sequence number;2*sequence number] TODO:IP-FRAGMENTATION??
    };
}
