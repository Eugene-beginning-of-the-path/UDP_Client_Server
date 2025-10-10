#include <PacketQueue/PacketQueue.h>

void dev::PacketQueue::push(wireData&& pckt)
{
    {
        std::lock_guard<std::mutex> lock(m_queueMtx);
        m_queue.push(std::move(pckt));
    }
    m_cv.notify_one();
}

bool dev::PacketQueue::pop(wireData& out)
{
    std::unique_lock<std::mutex> lock(m_queueMtx);
    m_cv.wait(lock, [this](){ return m_exit || m_queue.size(); });

    if (m_queue.size())
    {
        out = std::move(m_queue.front());
        m_queue.pop();
        return true;
    }
    return false;
}

size_t dev::PacketQueue::close()
{
    size_t size;
    {
        std::lock_guard<std::mutex> lock(m_queueMtx);
        m_exit = true;
        size = m_queue.size();
    }
    m_cv.notify_all();
    return size;
}