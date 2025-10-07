#include <iostream>
#include <thread>

#include "Packet/Packet.h"
#include "GateWay/Gateway.h"

int main()
{
    dev::Gateway& gateWay = dev::Gateway::Instance();

    auto foo = []()
    {

    };

    std::vector<std::jthread> threads;
    threads.reserve(10);
    for (size_t i = 0; i < threads.size(); i++)
    {
        threads.emplace_back(foo);
    }

    return 0;
}