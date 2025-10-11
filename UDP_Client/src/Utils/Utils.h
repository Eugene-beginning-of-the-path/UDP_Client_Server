#pragma once
#include <array>
#include <memory>
#include <cstdint>
#include <arpa/inet.h> //POSIX htons/htol
#include <openssl/sha.h>

#include "udptLib/Packet/Packet.h"
#include "udptLib/Utils/Utils.h"

namespace dev::utls
{
    HeadBuff getWireHeader(const dev::Packet& pckt);
}