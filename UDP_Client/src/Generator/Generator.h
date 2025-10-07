#pragma once

#include <algorithm>
#include <atomic>
#include <random>
#include <functional>
#include <thread>

class Generator
{
private:
    std::atomic_uint64_t m_globalId{0};
    uint64_t m_totalPcktSends{0};

    inline static constexpr const uint16_t MIN_SIZE_PAYLOAD = 32;
    inline static constexpr const uint16_t MAX_SIZE_PAYLOAD = 900;

    std::random_device m_rd;
    std::vector<std::mt19937_64> m_prngVec;

    const uint8_t m_countThreads{0};
    std::vector<std::jthread> m_threads;
    std::function<void(uint8_t threadId)> m_producerLogic;

private:

    //Getting great quility seed for PRNG initialization algo
    //  instead of just once calling std::random_device().
    void initPRNGs()
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

    //Determine payload size in a random context [#seqNumPckt;#seqNumPckt*2].
    //Use UniformDistribution for hight-quality generator instead of just %(seqNumPckt*2).
    uint16_t payLoadSize(uint64_t seqPckt, std::mt19937_64 & prng) const
    {
        std::uniform_int_distribution<uint64_t> dist(seqPckt, 2 * seqPckt);
        uint64_t rndSize = dist(prng);
        return static_cast<uint16_t>(std::min(std::max(rndSize, MIN_SIZE_PAYLOAD), MAX_SIZE_PAYLOAD)); //!!нужно делить пакет на две отправки, а не сужать его до [32;900]
    }

    void producerLogic(uint8_t threadId)
    {
        std::mt19937_64 & prng = m_prngVec[threadId];
        while(true)
        {
            uint64_t seqPckt = m_globalId.fetch_add(1);
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
        }
    }

public:
    Generator(uint64_t totalPcktSends, uint8_t countThreads, std::function<void()> producerLogic) : 
        m_totalPcktSends(totalPcktSends),
        m_countThreads(countThreads),
        m_producerLogic(producerLogic)
    {
        m_threads.reserve(m_countThreads);
        initPRNGs();
    }

    void startGenerate()
    {
        for (size_t i = 0; i < m_countThreads; i++)
        {
            m_threads.emplace_back([this, i](){
                m_producerLogic(i);
            });
        }
    }
};