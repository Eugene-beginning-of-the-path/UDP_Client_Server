#pragma once

#include <array>
#include <cstdint>

#include "Packet/Packet.h"

namespace dev::utls
{
    typedef std::array<unsigned char, Packet::PCKT_HEADER_SIZE> HeadBuff;
    typedef std::array<unsigned char, SHA256_DIGEST_LENGTH> SHA256Buff;
    typedef std::vector<unsigned char> PayLoadBuff;

    uint64_t timeStampNow();
    uint64_t htonll(uint64_t v);
    HeadBuff getRawNetworkHeader(const dev::Packet& pckt);
    SHA256Buff calcSha256(const HeadBuff& header, const PayLoadBuff& payLoad);
}