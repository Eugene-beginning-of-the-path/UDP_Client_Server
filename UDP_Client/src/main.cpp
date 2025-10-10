#include <iostream>
#include <thread>

#include "GateWay/Gateway.h"
#include "Generator/Generator.h"

int main()
{
    dev::Generator pcktGen(cfg::TARGET_SEND_PACKETS, cfg::THREAD_GENERATE_COUNTER);
    
    dev::Gateway& gateWay = dev::Gateway::Instance(cfg::TARGET_IP_V4, cfg::TARGET_PORT);

    std::future<void> sender = std::async(std::launch::async, [&gateWay, queue = pcktGen.getPcktQueue()](){
        dev::PacketQueue::wireData data;
        while (queue->pop(data))
        {
            gateWay.send(std::move(data));
        }
    });
    
    pcktGen.startGenerate();
    
    while(true)
    {
        // std::this_thread::sleep_for(std::chrono::seconds(3));
        // std::cout << pcktGen.getPcktQueue()->m_queue.size() << std::endl;
    }

    return 0;
}