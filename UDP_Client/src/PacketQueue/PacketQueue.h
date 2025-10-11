#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

#include <udptLib/Packet/Packet.h>

namespace dev
{
    class PacketQueue
    {
    public:
        using wireData = std::vector<unsigned char>;

    public:
        std::queue<wireData> m_queue;
        std::mutex m_queueMtx;
        
        bool m_exit = false;
        std::condition_variable m_cv;
        
    public:
        void push(wireData& pckt);
        bool pop(wireData& out);
        size_t close();
    };
}