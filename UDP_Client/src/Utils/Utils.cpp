#include "Utils/Utils.h"

#include <bit>         //std::endian
#include <chrono>
#include <cstring>
#include <fmt/format.h>
#include <fmt/chrono.h>
#include <openssl/sha.h>

uint64_t dev::utls::timeStampNsNow()
{
    auto ns = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now()).time_since_epoch();
    return static_cast<uint64_t>(ns.count());
}

uint64_t dev::utls::timeStampMsNow()
{
    using clock = std::chrono::steady_clock;
    const auto ms = std::chrono::time_point_cast<std::chrono::milliseconds>(clock::now())
                        .time_since_epoch();
    return static_cast<uint64_t>(ms.count());
}

std::string dev::utls::fomatedTimeStamp(uint64_t tsNanoSec) 
{
    std::chrono::sys_time<std::chrono::nanoseconds> tp{std::chrono::nanoseconds{tsNanoSec}};

    auto secs = std::chrono::floor<std::chrono::seconds>(tp);
    auto nsec = static_cast<long long>((std::chrono::nanoseconds{tsNanoSec} - secs.time_since_epoch()).count());
    
    return fmt::format("{:%Y-%m-%d %H:%M:%S}.{:09}", secs, nsec);
}

uint64_t dev::utls::htonll(uint64_t v)
{
    if constexpr (std::endian::native == std::endian::little)
        return __builtin_bswap64(v);
    else
        return v;
}

dev::utls::HeadBuff dev::utls::getWireHeader(const dev::Packet& pckt)
{
    std::array<unsigned char, dev::Packet::PCKT_HEADER_SIZE> headerRawBuff;
    headerRawBuff.fill(0);

    const uint16_t seq = htons(pckt.m_seqNum);
    const uint64_t tmNs = htonll(pckt.m_timeStampNs);
    const size_t payLoadSize = htonll(pckt.m_payLoadSize);

    std::memcpy(headerRawBuff.data(),               &seq,   sizeof(seq));
    std::memcpy(headerRawBuff.data()+sizeof(seq),   &tmNs,  sizeof(tmNs));
    std::memcpy(headerRawBuff.data()+sizeof(tmNs),  &payLoadSize,  sizeof(payLoadSize));
    return headerRawBuff;
}

dev::utls::SHA256Buff dev::utls::calcSha256(const HeadBuff& header, const PayLoadBuff& payLoad)
{
    SHA256Buff res;

    SHA256_CTX ctx;
    SHA256_Init(&ctx);

    SHA256_Update(&ctx, header.data(), header.size());
    SHA256_Update(&ctx, payLoad.data(), payLoad.size());

    SHA256_Final(res.data(), &ctx);
    return res;
}