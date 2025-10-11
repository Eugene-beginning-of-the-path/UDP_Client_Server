#include "Packet/WirePacket.h"
#include "udptLib/Utils/Utils.h"

#include <iostream>

std::ostream & dev::operator<<(std::ostream & strm, const dev::WirePacket& pctk)
{
    strm << "pckt:\t" << std::endl;
    strm << "\tseq_num:\t" << pctk.m_pcktData.m_seqNum << std::endl;
    strm << "\tts:\t\t" << utls::formatedTimeStamp(pctk.m_pcktData.m_timeStampNs) << std::endl;
    strm << "\tpayload_size:\t" << pctk.m_pcktData.m_payLoadSize << std::endl;
    strm << "\tcheck_sum:\t";
    for (const auto el : pctk.m_wireHeader.m_checkSum)
    {
        strm << (int)el;
    }
    
    strm << std::endl;

    strm << "\tpayload:\t";
    for (const auto el : pctk.m_pcktData.m_payload)
    {
        strm << el;
    }
    return strm;
}