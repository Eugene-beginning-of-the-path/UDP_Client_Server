#pragma once
#include "PacketQueue/PacketQueue.h"
#include "ConfirmList/InWait.h"
#include "Config.h"

#include <map>
#include <unordered_map>
#include <shared_mutex>

//Waiting list for confirmation of sent packets
namespace dev
{
    class ConfirmList
    {
        using isConfirmed = bool;

    private:
        std::unordered_map<uint16_t, InWait> m_waiters;
        std::mutex m_wtrsMtx;

        std::shared_ptr<PacketQueue> m_pctkQueue;

        std::map<uint16_t, std::pair<isConfirmed, InWait>> m_confirmedList;
        mutable std::shared_mutex m_confirmedMtx;
        mutable std::condition_variable_any m_cv;

        mutable std::atomic_uint16_t m_idNext{0};
    
    public:
        ConfirmList(std::shared_ptr<PacketQueue> pcktQueue);

        uint16_t getNextIdDisplay() const;
        void push(uint16_t key, InWait&& val);
        bool check();
        bool eraseWaiter(uint16_t seqNum, isConfirmed isCnfrm = true);
        void displayConfirmedPckt() const;
        size_t clearConfirmedList();

        const dev::ConfirmList* waitConfirmedPckt() const;
        void notifyAboutConfirmedPckt() const;
    };
}
