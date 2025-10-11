#pragma once
#include <array>
#include <vector>
#include <memory>
#include <cstdint>
#include <arpa/inet.h> //POSIX htons/htol
#include <openssl/sha.h>

#include "udptLib/Packet/Packet.h"

namespace dev::utls
{
    typedef std::array<unsigned char, Packet::PCKT_HEADER_SIZE> HeadBuff;
    typedef std::array<unsigned char, SHA256_DIGEST_LENGTH> SHA256Buff;
    typedef std::vector<unsigned char> PayLoadBuff;

    uint64_t timeStampNsNow();
    uint64_t timeStampMsNow();
    std::string formatedTimeStamp(uint64_t tsNanoSec);
    uint64_t htonll(uint64_t v);
    SHA256Buff calcSha256(const HeadBuff& header, const PayLoadBuff& payLoad);
}