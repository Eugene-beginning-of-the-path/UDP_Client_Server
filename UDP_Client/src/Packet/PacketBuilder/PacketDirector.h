#pragma once

#include "Packet/PacketBuilder/PacketBuilder.h"

namespace dev
{
    class PacketDirector
    {
    protected:
        std::unique_ptr<PacketBuilder> m_builder;

    public:
        PacketDirector(std::unique_ptr<PacketBuilder> pcktBldr);
        void setBuilder(std::unique_ptr<PacketBuilder> pcktBldr);

        void buildProduct(uint16_t seqNum, uint64_t timeStampNs, utls::PayLoadBuff&& payLoad) const;

        std::shared_ptr<WirePacket> getProduct();
    };

}