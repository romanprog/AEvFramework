#ifndef TYPES
#define TYPES

#include <memory>
#include <asio.hpp>


using UdpEndpointPtr = std::unique_ptr<asio::ip::udp::endpoint>;


#endif // TYPES

