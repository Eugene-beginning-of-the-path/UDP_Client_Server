#include "Utils/Utils.h"

#include <bit>         //std::endian
#include <chrono>
#include <cstring>
#include <fmt/format.h>
#include <fmt/chrono.h>
#include <openssl/sha.h>

dev::utls::HeadBuff dev::utls::getWireHeader(const dev::Packet& pckt)
{
    std::array<unsigned char, dev::Packet::PCKT_HEADER_SIZE> headerRawBuff;
    headerRawBuff.fill(0);

    const uint16_t seq = htons(pckt.m_seqNum);
    const uint64_t tmNs = htonll(pckt.m_timeStampNs);
    const size_t payLoadSize = htonll(pckt.m_payLoadSize);

    std::memcpy(headerRawBuff.data(),               &seq,   sizeof(seq));
    std::memcpy(headerRawBuff.data()+sizeof(seq),   &tmNs,  sizeof(tmNs));
    std::memcpy(headerRawBuff.data()+sizeof(seq)+sizeof(tmNs),  &payLoadSize,  sizeof(payLoadSize));
    return headerRawBuff;
}