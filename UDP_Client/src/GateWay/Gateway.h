#pragma once

#include <boost/asio.hpp>

#include <Patterns/Singleton/Singleton.h>

using boost::asio::ip::udp;

namespace dev
{
    class Gateway : public patrn::Singleton<Gateway>
    {
        friend patrn::Singleton<Gateway>;

    private:
        boost::asio::io_context m_io;
        udp::socket m_udpSock;
        udp::endpoint m_endPoint;

        inline static constexpr const char* DEFAULT_IP_V4 = "127.0.0.1";
        inline static constexpr uint16_t DEFAULT_PORT = 6666;

    protected:
        Gateway() : Gateway(DEFAULT_IP_V4, DEFAULT_PORT) { }

        Gateway(const std::string& ipv4, const uint16_t port) : 
        m_udpSock(m_io), m_endPoint(boost::asio::ip::make_address(ipv4), port)
        {
            m_udpSock.open(udp::v4());
            m_udpSock.connect(m_endPoint);
        }

    public:
        std::size_t send(std::vector<unsigned char>&& buff)
        {
            return m_udpSock.send(boost::asio::buffer(buff));
        }
        
    };
}