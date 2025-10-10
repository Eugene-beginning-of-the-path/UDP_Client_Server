#pragma once

#include <openssl/sha.h>

#include <cstdint>
#include <vector>

namespace dev
{
    struct Packet
    {
        //НЕВЕРНО! пропущен sha256
        static constexpr inline const uint16_t PCKT_HEADER_SIZE = 
            sizeof(uint16_t) + sizeof(uint64_t) + sizeof(size_t) + sizeof(SHA256_DIGEST_LENGTH);

        //Header fields:
        uint16_t m_seqNum;                          //sequence number of pckt
        uint64_t m_timeStampNs;                     //time (nano sec) since the beggining of the epoch
        size_t   m_payLoadSize;                     //just pay load size

        //PayLoad:
        std::vector<unsigned char> m_payload;       //random data with lenght [sequence number;2*sequence number] TODO:IP-FRAGMENTATION??
    };
}
