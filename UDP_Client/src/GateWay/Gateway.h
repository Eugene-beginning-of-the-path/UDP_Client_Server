#pragma once
#include <boost/asio.hpp>

#include "Patterns/Singleton/Singleton.h"

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
        Gateway(const std::string& ipv4 = DEFAULT_IP_V4, const uint16_t port = DEFAULT_PORT);

    public:
        std::size_t send(std::vector<unsigned char>&& buff);
    };
}