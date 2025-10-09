#include "Packet/PacketBuilder/PacketBuilder.h"

dev::PacketBuilder::PacketBuilder()
{
    reset();
}

void dev::PacketBuilder::reset()
{
    m_product = std::make_shared<WirePacket>();
}

void dev::PacketBuilder::producePacket(uint16_t seqNum, uint64_t timeStampNs, size_t payLoadSize, 
    utls::PayLoadBuff&& payLoad) const
{
    auto& pckt = m_product->m_pcktData;
    
    pckt.m_seqNum = seqNum;
    pckt.m_timeStampNs = timeStampNs;
    pckt.m_payLoadSize = payLoadSize;
    pckt.m_payload = std::move(payLoad);
}

void dev::PacketBuilder::produceWireHeader() const
{
    utls::HeadBuff headerRawBuff = utls::getWireHeader(m_product->m_pcktData);
    utls::SHA256Buff checkSum = utls::calcSha256(headerRawBuff, m_product->m_pcktData.m_payload);
    
    auto& pckt = m_product->m_pcktData;
    auto& wireHeader = m_product->m_wireHeader;

    wireHeader.m_seqNum = htons(pckt.m_seqNum);
    wireHeader.m_timeStampNs = utls::htonll(pckt.m_timeStampNs);
    wireHeader.m_payLoadSize = utls::htonll(pckt.m_payLoadSize);
    wireHeader.m_checkSum = std::move(checkSum);
    wireHeader.m_wireHeader = std::move(headerRawBuff);
}

void dev::PacketBuilder::produceWirePacket() const
{
    auto& pckt = m_product->m_pcktData;
    m_product->m_wire.reserve(sizeof(utls::HeadBuff) + sizeof(pckt.m_payLoadSize));

    std::memcpy(m_product->m_wire.data(), m_product->m_wireHeader.m_wireHeader.data(), sizeof(utls::HeadBuff));
    if (pckt.m_payLoadSize)
    {
        std::memcpy(m_product->m_wire.data()+sizeof(utls::HeadBuff), pckt.m_payload.data(), pckt.m_payLoadSize);
    }
}

std::shared_ptr<dev::WirePacket> dev::PacketBuilder::getProduct()
{
    auto res = m_product;
    reset();
    return res;
}