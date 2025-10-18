#include "ConfirmList/ConfirmList.h"
#include "udptLib/Utils/Utils.h"

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

bool dev::ConfirmList::check()
{
    uint64_t tsMsNow = utls::timeStampMsNow();

    std::lock_guard<std::mutex> lock(m_wtrsMtx);
    bool isNewCnfrmPckt = false;

    for (auto it = m_waiters.begin(); it != m_waiters.end(); )
    {
        if (cfg::RTO_MS <= tsMsNow - it->second.m_lastAttemptTsMs)
        {
            if (cfg::RETRIES_SEND <= it->second.m_retries)
            {
                std::unique_lock<std::shared_mutex> writerLock(m_confirmedMtx);
                m_confirmedList.try_emplace(it->first, std::make_pair(false, std::move(it->second)));
                it = m_waiters.erase(it);

                isNewCnfrmPckt = true;
                continue;
            }

            if (!it->second.m_pckt)
            {
                std::cerr << "InWait::Packet is lost" << std::endl;
                it = m_waiters.erase(it);
                continue;
            }
            m_pctkQueue->push(it->second.m_pckt->m_wire);
            it->second.m_retries++;
            it->second.m_lastAttemptTsMs = tsMsNow;
        }
        ++it;
    }

    return isNewCnfrmPckt;
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

const dev::ConfirmList* dev::ConfirmList::waitConfirmedPckt() const
{
    std::unique_lock<std::shared_mutex> lock(m_confirmedMtx);
    m_cv.wait(lock, [this](){ return !m_confirmedList.empty(); });
    return this;
}

void dev::ConfirmList::notifyAboutConfirmedPckt() const
{
    m_cv.notify_one();
}