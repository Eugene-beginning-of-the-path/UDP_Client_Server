#pragma once
#include <cstdint>
#include <vector>

namespace dev
{
    struct Packet
    {
        uint16_t m_id;
        uint64_t m_timestamp;
        std::vector<unsigned char> m_payload;
        //SHA256
    };
}
