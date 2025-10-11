#pragma once
#include <cstdint>

//Instead of parsing cfg.json:
namespace cfg
{
    inline static constexpr const uint8_t   THREAD_GENERATE_COUNTER = 1;
    inline static constexpr const uint16_t  TARGET_SEND_PACKETS = 3;
    inline static constexpr const uint64_t  RTO_MS = 1'000;
    inline static constexpr const uint8_t   RETRIES_SEND = 1;

    inline static constexpr const char* TARGET_IP_V4 = "127.0.0.1";
    inline static constexpr uint16_t    TARGET_PORT = 6666;
}
