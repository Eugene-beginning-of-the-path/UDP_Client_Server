#pragma once
#include "Packet/WirePacket.h"

#include <memory>

namespace dev
{
    struct InWait
    {
        std::shared_ptr<WirePacket> m_pckt;

        uint64_t m_lastAttemptTsMs{0};   //time stamp of last sent
        uint64_t m_rto{0};             //delay between attempts to send pckt
        uint8_t m_retries{0};          //current attempts to success send and get confirmation by Server

        bool m_isOk{false};
    };
}