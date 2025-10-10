#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

#include <Packet/Packet.h>

namespace dev
{
    class PacketQueue
    {
    private:
        std::queue<Packet> m_queue;
        std::mutex m_queueMtx;

        bool m_exit = false;
        std::condition_variable m_cv;

    public:
        void push(Packet&& pckt);
        bool pop(Packet& out);
        size_t close();
    };
}