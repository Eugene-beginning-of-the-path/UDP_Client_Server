#include <iostream>
#include <thread>

#include "udptLib/GateWay/Gateway.h"
#include "Generator/Generator.h"
#include "ConfirmList/ConfirmList.h"

int main()
{
    dev::Generator pcktGen(cfg::TARGET_SEND_PACKETS, cfg::THREAD_GENERATE_COUNTER);
    dev::ConfirmList cnfrmList(pcktGen.getPcktQueue());
    
    dev::Gateway& gateWay = dev::Gateway::Instance(cfg::TARGET_IP_V4, cfg::TARGET_PORT);

    //Thread for sending built pckt throught once UDP sock
    std::thread([&gateWay, queue = pcktGen.getPcktQueue()](){
        dev::PacketQueue::wireData data;
        while (queue->pop(data))
        {
            gateWay.send(std::move(data));
        }
    }).detach();

    //There are threads for generating pckts
    pcktGen.startGenerate(cnfrmList);
    sleep(2);
    
    //Receiving + deleting
    std::thread([&cnfrmList](){
            //GateWay is receiving UDP
            //Del
            std::vector<int> gotSeqNum {1,0,2};
            while(!gotSeqNum.empty())
            {
                //cnfrmList.eraseWaiter(seqNum network-ending):
                if (cnfrmList.eraseWaiter(*gotSeqNum.begin()))
                {
                    cnfrmList.notifyAboutConfirmedPckt();
                    gotSeqNum.erase(gotSeqNum.begin());
                }
            }
    }).detach();

    // Checking + re-send/del
    std::thread([&cnfrmList](){
            while(true) 
            {
                if (cnfrmList.check())
                {
                    cnfrmList.notifyAboutConfirmedPckt();
                }
            }
    }).detach();

    //Displaying deleted msgs
    std::future<void> displayCnfrmed = std::async(std::launch::async, [&cnfrmList]{
        //Displaying till cfg::TARGET_SEND_PACKETS
        while(cnfrmList.getNextIdDisplay() != cfg::TARGET_SEND_PACKETS)
        {
            cnfrmList.waitConfirmedPckt()->displayConfirmedPckt();
        }
    });
    
    displayCnfrmed.wait();
    return 0;
}