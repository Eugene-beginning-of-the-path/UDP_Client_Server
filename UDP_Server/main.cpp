#include <iostream>
#include <vector>
#include <cstring>
#include <boost/asio.hpp>
#include <udptLib/Utils/Utils.h>

using boost::asio::ip::udp;

int main() {
    boost::asio::io_context io;
    udp::socket sock(io, udp::endpoint(boost::asio::ip::make_address("127.0.0.1"), 6666));
    udp::endpoint remote_ep;
    std::vector<unsigned char> buffer(65536);

    while (true) {
        boost::system::error_code ec;
        size_t len = sock.receive_from(boost::asio::buffer(buffer), remote_ep, 0, ec);
        if (ec || len < dev::Packet::PCKT_HEADER_SIZE) continue;

        // Парсинг заголовка
        uint16_t seqNum_be, seqNum;
        uint64_t timeStampNs_be, timeStampNs, payLoadSize_be, payLoadSize;
        dev::utls::SHA256Buff received_checksum;

        size_t offset = 0;
        std::memcpy(&seqNum_be, buffer.data() + offset, sizeof(seqNum_be));
        offset += sizeof(seqNum_be);
        std::memcpy(&timeStampNs_be, buffer.data() + offset, sizeof(timeStampNs_be));
        offset += sizeof(timeStampNs_be);
        std::memcpy(&payLoadSize_be, buffer.data() + offset, sizeof(payLoadSize_be));
        offset += sizeof(payLoadSize_be);
        std::memcpy(received_checksum.data(), buffer.data() + offset, SHA256_DIGEST_LENGTH);

        seqNum = ntohs(seqNum_be);
        timeStampNs = dev::utls::htonll(timeStampNs_be);
        payLoadSize = dev::utls::htonll(payLoadSize_be);

        if (len < dev::Packet::PCKT_HEADER_SIZE + payLoadSize) continue;

        // Проверка контрольной суммы
        dev::utls::HeadBuff headerRawBuff;
        headerRawBuff.fill(0);
        std::memcpy(headerRawBuff.data(), buffer.data(), sizeof(seqNum_be) + sizeof(timeStampNs_be) + sizeof(payLoadSize_be));
        dev::utls::PayLoadBuff payload(buffer.begin() + dev::Packet::PCKT_HEADER_SIZE, buffer.begin() + dev::Packet::PCKT_HEADER_SIZE + payLoadSize);
        dev::utls::SHA256Buff computed_checksum = dev::utls::calcSha256(headerRawBuff, payload);
        bool integrity_ok = (received_checksum == computed_checksum);

        // Вывод информации о пакете
        uint64_t recv_time_ns = dev::utls::timeStampNsNow();
        std::cout << "Packet #" << seqNum 
                  << " | Formation: " << dev::utls::formatedTimeStamp(timeStampNs)
                  << " | Received: " << dev::utls::formatedTimeStamp(recv_time_ns)
                  << " | Integrity: " << (integrity_ok ? "OK" : "FAIL") << std::endl;

        // Отправка подтверждения (ACK)
        // if (integrity_ok) {
        //     uint8_t ack_packet[2];
        //     uint16_t ack_seq = htons(seqNum);
        //     ack_packet[0] = static_cast<uint8_t>(ack_seq >> 8);
        //     ack_packet[1] = static_cast<uint8_t>(ack_seq & 0xFF);
        //     sock.send_to(boost::asio::buffer(ack_packet, sizeof(ack_packet)), remote_ep);
        // }
    }

    return 0;
}
