#include <iostream>
#include <algorithm>
#include <cstring>

#include <boost/asio.hpp>

using boost::asio::ip::udp;

int main() {
    std::cout << "Hello, World!" << std::endl;

    boost::asio::io_context io;
    udp::socket sock(io, udp::endpoint(boost::asio::ip::make_address("127.0.0.1"), 6666));

    std::vector<unsigned char> recv_buf(2048);
    udp::endpoint remote_ep;

    size_t n = sock.receive_from(boost::asio::buffer(recv_buf), remote_ep);
    uint16_t seqNum = 0;
    std::memcpy(&seqNum, recv_buf.data(), sizeof(seqNum)*8);
    seqNum = ntohs(seqNum);
    std::cout << seqNum << std::endl;
    while(true);
    
    return 0;
}