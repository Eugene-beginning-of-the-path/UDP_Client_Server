#include "GateWay/Gateway.h"

dev::Gateway::Gateway(const std::string& ipv4, const uint16_t port) : 
    m_udpSock(m_io), m_endPoint(boost::asio::ip::make_address(ipv4), port)
{
    m_udpSock.open(udp::v4());
    m_udpSock.connect(m_endPoint);
}

std::size_t dev::Gateway::send(std::vector<unsigned char>&& buff)
{
    return m_udpSock.send(boost::asio::buffer(buff));
}