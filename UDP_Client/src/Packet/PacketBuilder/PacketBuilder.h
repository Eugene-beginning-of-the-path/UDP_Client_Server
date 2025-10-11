#pragma once

#include <memory>
#include <cstring>

#include "Packet/WireHeader.h"
#include "Packet/WirePacket.h"
#include "udptLib/Utils/Utils.h"
#include "udptLib/Packet/Packet.h"
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