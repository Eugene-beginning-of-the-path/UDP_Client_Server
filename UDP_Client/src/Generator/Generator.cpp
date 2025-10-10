#include "Packet/Packet.h"
#include "Generator/Generator.h"
#include "Packet/PacketBuilder/PacketBuilder.h"
#include "Packet/PacketBuilder/PacketDirector.h"

dev::Generator::Generator(uint64_t totalPcktSends, uint8_t countThreads, 
        std::optional<std::function<void(uint8_t threadId)>> producerLogic) : 
    m_totalPcktSends(totalPcktSends),
    m_countThreads(countThreads),
    m_sendingQueue(std::make_shared<PacketQueue>())
{
    m_producerLogic = producerLogic.value_or(
        std::function<void(uint8_t)>([this](uint8_t id){
            this->producerLogic(id);
        }));
    
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

        std::vector<unsigned char> payload(calcPayLoadSize(seqPckt, prng));
        std::uniform_int_distribution<int> pl_dist(0, 255);
        for (auto & symbol : payload)
        {
            symbol = static_cast<unsigned char>(pl_dist(prng));
        }

        static thread_local std::unique_ptr<PacketDirector> builder = std::make_unique<PacketDirector>(std::make_unique<PacketBuilder>());
        builder->buildProduct(seqPckt, utls::timeStampNow(), std::move(payload));
        
        {
            std::lock_guard<std::mutex> lock(m_queueMtx);
            m_sendingQueue->push(std::move(builder->getProduct()->m_wire));
        }
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