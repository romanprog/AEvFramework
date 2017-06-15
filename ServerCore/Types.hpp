#ifndef TYPES
#define TYPES

#include <memory>

using DnsReadBufferPtr = std::unique_ptr<DnsReadBuffer>;
using UdpEndpointPtr = std::unique_ptr<asio::ip::udp::endpoint>;


#endif // TYPES

