#include <iostream>

#include <boost/asio.hpp>

using boost::asio::ip::udp;

// struct Packet
// {
//     uint32_t m_id;
//     uint64_t m_timestamp;
//     std::vector<unsigned char> m_payload;
// };

int main() {
    std::cout << "Hello, World!" << std::endl;

    // boost::asio::io_context io;
    // udp::socket sock(io, udp::endpoint(boost::asio::ip::make_address("127.0.0.1"), 6666));

    // std::vector<unsigned char> recv_buf(2048);
    // udp::endpoint remote_ep;

    // while (true)
    // {
    //     size_t n = sock.receive_from(boost::asio::buffer(recv_buf), remote_ep);
            
    //     auto* hdr = reinterpret_cast<const Packet*>(recv_buf.data());
    //     uint32_t id = htonl(hdr->id);
    //     uint64_t timestamp = htobe64(hdr->timestamp);

    //     std::cout << "Received Packet - ID: " << id << ", Timestamp: " << timestamp << std::endl;
    // }
    return 0;
}