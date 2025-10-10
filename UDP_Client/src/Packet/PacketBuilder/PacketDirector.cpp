#include "Packet/PacketBuilder/PacketDirector.h"

#include <stdexcept>

dev::PacketDirector::PacketDirector(std::unique_ptr<PacketBuilder> pcktBldr) : m_builder(std::move(pcktBldr)) { }

void dev::PacketDirector::setBuilder(std::unique_ptr<PacketBuilder> pcktBldr)
{
    m_builder = std::move(pcktBldr);
}

void dev::PacketDirector::buildProduct(uint16_t seqNum, uint64_t timeStampNs, utls::PayLoadBuff&& payLoad) const
{
    if (!m_builder)
    {
        throw std::logic_error("PacketDirector: builder is nullptr");
    }

    m_builder->producePacket(seqNum, timeStampNs, std::move(payLoad));
    m_builder->produceWireHeader();
    m_builder->produceWirePacket();
}

std::shared_ptr<dev::WirePacket> dev::PacketDirector::getProduct()
{
    return m_builder->getProduct();
}