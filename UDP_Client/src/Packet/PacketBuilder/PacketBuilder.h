#pragma once

#include <memory>
#include <cstring>

#include "Utils/Utils.h"
#include "Packet/Packet.h"
#include "Packet/WireHeader.h"
#include "Packet/WirePacket.h"
#include "Patterns/Builder/Builder.h"

namespace dev
{
    class PacketBuilder : public Builder
    {
    private:
        std::shared_ptr<WirePacket> m_product;

    public:
        PacketBuilder();
        void reset() override;

        void producePacket(uint16_t seqNum, uint64_t timeStampNs, utls::PayLoadBuff&& payLoad) const;
        void produceWireHeader() const;
        void produceWirePacket() const;

        std::shared_ptr<WirePacket> getProduct();
    };
}