#include "Packet/PacketBuilder/PacketBuilder.h"

dev::PacketBuilder::PacketBuilder()
{
    reset();
}

void dev::PacketBuilder::reset()
{
    m_product = std::make_shared<WirePacket>();
}

void dev::PacketBuilder::producePacket(uint16_t seqNum, uint64_t timeStampNs, utls::PayLoadBuff&& payLoad) const
{
    auto& pckt = m_product->m_pcktData;
    
    pckt.m_seqNum = seqNum;
    pckt.m_timeStampNs = timeStampNs;
    pckt.m_payLoadSize = payLoad.size();
    pckt.m_payload = std::move(payLoad);
}

void dev::PacketBuilder::produceWireHeader() const
{
    utls::HeadBuff headerRawBuff = utls::getWireHeader(m_product->m_pcktData);
    utls::SHA256Buff checkSum = utls::calcSha256(headerRawBuff, m_product->m_pcktData.m_payload);
    std::memcpy(headerRawBuff.data()+sizeof(m_product->m_pcktData.m_timeStampNs)+sizeof(m_product->m_pcktData.m_seqNum)+
        sizeof(m_product->m_pcktData.m_payLoadSize), checkSum.data(), SHA256_DIGEST_LENGTH);
    
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
    m_product->m_wire.resize(Packet::PCKT_HEADER_SIZE + pckt.m_payLoadSize);

    std::memcpy(m_product->m_wire.data(), m_product->m_wireHeader.m_wireHeader.data(), Packet::PCKT_HEADER_SIZE);
    if (pckt.m_payLoadSize)
    {
        std::memcpy(m_product->m_wire.data()+sizeof(Packet::PCKT_HEADER_SIZE), pckt.m_payload.data(), pckt.m_payLoadSize);
    }
}

std::shared_ptr<dev::WirePacket> dev::PacketBuilder::getProduct()
{
    auto res = m_product;
    reset();
    return res;
}