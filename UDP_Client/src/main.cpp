#include <iostream>
#include <thread>

#include "GateWay/Gateway.h"
#include "Generator/Generator.h"
#include "ConfirmList/ConfirmList.h"

int main()
{
    dev::Generator pcktGen(cfg::TARGET_SEND_PACKETS, cfg::THREAD_GENERATE_COUNTER);
    dev::ConfirmList cnfrmList(pcktGen.getPcktQueue());
    
    dev::Gateway& gateWay = dev::Gateway::Instance(cfg::TARGET_IP_V4, cfg::TARGET_PORT);

    //thread for sending built pckt throught once UDP sock
    std::thread([&gateWay, queue = pcktGen.getPcktQueue()](){
        dev::PacketQueue::wireData data;
        while (queue->pop(data))
        {
            gateWay.send(std::move(data));
        }
    }).detach();

    //Получение+удаление
    std::thread([&cnfrmList](){
            //GateWay is receiving UDP
            //Del
            //cnfrmList.eraseWaiter(seqNum network-ending);
    }).detach();

    //Проверка+отправка/удаление
    std::thread([&cnfrmList](){
            //Checking+re-send/del
            //cnfrmList.check();
    }).detach();

    //Вывод удаленного
    std::future<void> displayCnfrmed = std::async(std::launch::async, [&cnfrmList]{
        //Displaying till cfg::TARGET_SEND_PACKETS
        while(cnfrmList.getNextIdDisplay() != cfg::TARGET_SEND_PACKETS)
        {
            cnfrmList.displayConfirmedPckt();
        }
    });
    
    //there are threads for gerenerating pckts
    pcktGen.startGenerate(cnfrmList);
    
    displayCnfrmed.wait();
    return 0;
}