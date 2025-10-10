#include <iostream>
#include <thread>

#include "GateWay/Gateway.h"
#include "Generator/Generator.h"

int main()
{
    dev::Generator pcktGen(7, 1);
    
    dev::Gateway& gateWay = dev::Gateway::Instance();
    std::future<void> ftr = std::async(std::launch::async, [&gateWay, queue = pcktGen.getPcktQueue()](){
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