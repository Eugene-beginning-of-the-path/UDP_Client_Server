#include "ConfirmList/ConfirmList.h"
#include "Utils/Utils.h"

#include <iostream>

dev::ConfirmList::ConfirmList(std::shared_ptr<PacketQueue> pcktQueue)
{
    if (!pcktQueue)
    {
        throw std::logic_error("ConfirmList: pcktQueue is nullptr");
    }

    m_pctkQueue = pcktQueue;
    m_waiters.reserve(cfg::TARGET_SEND_PACKETS);
}

uint16_t dev::ConfirmList::getNextIdDisplay() const
{
    return m_idNext.load();
}

void dev::ConfirmList::push(uint16_t key, InWait&& val)
{
    std::lock_guard<std::mutex> lock(m_wtrsMtx);
    m_waiters.try_emplace(key, val);
}

void dev::ConfirmList::check()
{
    uint64_t ts_now = utls::timeStampNow();

    std::lock_guard<std::mutex> lock(m_wtrsMtx);
    for (auto & el : m_waiters)
    {
        if (cfg::RTO_NANO_SEC <= ts_now - el.second.m_lastAttemptTs)
        {
            if (cfg::RETRIES_SEND <= el.second.m_retries)
            {
                std::unique_lock<std::shared_mutex> writerLock(m_confirmedMtx);
                m_confirmedList.try_emplace(el.first, std::make_pair(false, el.second));
            }

            if (!el.second.m_pckt)
            {
                std::cerr << "InWait::Packet is lost" << std::endl;
                continue;
            }
            m_pctkQueue->push(el.second.m_pckt->m_wire);
            el.second.m_retries++;
            el.second.m_lastAttemptTs = ts_now;
        }
    }
}

bool dev::ConfirmList::eraseWaiter(uint16_t seqNum, isConfirmed isCnfrm)
{
    std::scoped_lock scLock(m_wtrsMtx, m_confirmedMtx);
    auto it = m_waiters.find(seqNum);
    if (it != m_waiters.end())
    {
        m_confirmedList.emplace(it->first, std::make_pair(isCnfrm, std::move(it->second)));
        m_waiters.erase(it);
        return true;
    }
    return false;
}

void dev::ConfirmList::displayConfirmedPckt() const
{
    std::shared_lock<std::shared_mutex> readerLock(m_confirmedMtx);
    auto elToDisplay = m_confirmedList.find(m_idNext.load());
    if (elToDisplay != m_confirmedList.end())
    {
        std::cout << "Packet #" << m_idNext.load() << (elToDisplay->second.first ? "(Success)" : "(Fail)")
            << ":\n" << *elToDisplay->second.second.m_pckt.get() << std::endl << std::endl;

        m_idNext++;
    }
}

size_t dev::ConfirmList::clearConfirmedList()
{
    std::unique_lock<std::shared_mutex> writerLock(m_confirmedMtx);
    size_t size = m_confirmedList.size();
    m_confirmedList.clear();
    return size;
}