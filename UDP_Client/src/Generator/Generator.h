#pragma once

#include <atomic>
#include <random>
#include <functional>
#include <thread>

namespace dev
{
    class Generator
    {
    private:
        std::atomic_uint64_t m_globalId{0};
        uint16_t m_totalPcktSends{10'000};

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
        void initPRNGs();

        //Determine payload size in a random context [#seqNumPckt;#seqNumPckt*2].
        //Use UniformDistribution for hight-quality generator instead of just %(seqNumPckt*2).
        uint16_t calcPayLoadSize(uint16_t seqPckt, std::mt19937_64 & prng) const;
        void producerLogic(uint8_t threadId);

    public:
        Generator(uint64_t totalPcktSends, uint8_t countThreads, std::function<void(uint8_t threadId)> producerLogic);
        void startGenerate();
    };
}