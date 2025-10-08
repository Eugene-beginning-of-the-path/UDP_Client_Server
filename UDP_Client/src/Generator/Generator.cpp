#include "Generator/Generator.h"
#include "Packet/Packet.h"
#include "Utils/Utils.h"

#include <algorithm>

dev::Generator::Generator(uint64_t totalPcktSends, uint8_t countThreads, std::function<void(uint8_t threadId)> producerLogic) : 
    m_totalPcktSends(totalPcktSends),
    m_countThreads(countThreads),
    m_producerLogic(producerLogic)
{
    m_threads.reserve(m_countThreads);
    initPRNGs();
}

void dev::Generator::initPRNGs()
{
    m_prngVec.reserve(m_countThreads);

    std::vector<uint32_t> ent = { m_rd(), m_rd(), m_rd(), m_rd() };
    for (size_t i = 0; i < m_countThreads; i++)
    {
        std::seed_seq ss{ 
            ent[0], 
            ent[1], 
            ent[2], 
            ent[3],
            static_cast<uint32_t>(i)
        };
        m_prngVec.emplace_back(ss);
    }
}

uint16_t dev::Generator::calcPayLoadSize(uint16_t seqPckt, std::mt19937_64 & prng) const
{
    std::uniform_int_distribution<uint64_t> dist(seqPckt, 2 * seqPckt);
    return dist(prng);
}

void dev::Generator::producerLogic(uint8_t threadId)
{
    std::mt19937_64 & prng = m_prngVec[threadId];
    while(true)
    {
        uint16_t seqPckt = m_globalId.fetch_add(1);
        if (m_totalPcktSends <= seqPckt)
        {
            break;
        }

        uint16_t payLoadSize = calcPayLoadSize(seqPckt, prng);

        std::vector<unsigned char> payload(payLoadSize);
        std::uniform_int_distribution<int> pl_dist(0, 255);
        for (auto & symbol : payload)
        {
            symbol = static_cast<unsigned char>(pl_dist(prng));
        }

        //Пора создавать Packet и наполнять его
        Packet pckt{ .m_seqNum = seqPckt, .m_timeStampNs = dev::utls::timeStampNow(), .m_payload = std::move(payload) };
        auto headerRawBuff = utls::getRawNetworkHeader(pckt);
        utls::SHA256Buff checkSum = utls::calcSha256(headerRawBuff, payload);
    }
}

void dev::Generator::startGenerate()
{
    for (size_t i = 0; i < m_countThreads; i++)
    {
        m_threads.emplace_back([this, i](){
            m_producerLogic(i);
        });
    }
}