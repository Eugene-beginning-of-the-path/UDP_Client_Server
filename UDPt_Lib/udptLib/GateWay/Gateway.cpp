#include "udptLib/GateWay/Gateway.h"

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

std::size_t dev::Gateway::receive(std::vector<unsigned char>& out)
{
    out.resize(65536); // максимальный размер UDP (с запасом под datagram)
    boost::system::error_code ec;
    std::size_t n = m_udpSock.receive(boost::asio::buffer(out), 0, ec);
    if (ec) throw boost::system::system_error(ec);
    out.resize(n);
    return n;
}