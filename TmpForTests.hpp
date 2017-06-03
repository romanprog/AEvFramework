#include <cstdlib>
#include <iostream>
#include "asio.hpp"
#include "Logger/Logger.hpp"
#include "AsyncEvent/BufferBase/BuffAbstract.hpp"

using asio::ip::udp;

class DnsReadBufferT : public BuffAbstract
{
public:
    DnsReadBufferT()
        :BuffAbstract(512,0)
    {}

    void clear() {}

private:


};




class server
{
public:
    server(asio::io_service& io_service, short port)
    {
        socket_ = std::make_shared<udp::socket>(io_service, udp::endpoint(udp::v4(), port));
        do_receive();
    }

    void do_receive()
    {
        socket_->async_receive_from(
                    asio::buffer(data_, max_length), sender_endpoint_,
                    [this](std::error_code ec, std::size_t bytes_recvd)
        {
            if (!ec && bytes_recvd > 0)
            {
                log_main("asdadssavsdv %", sender_endpoint_.address().to_string());
                do_send(bytes_recvd, std::move(*socket_));
            }
            else
            {
                do_receive();
            }
        });

    }

    void do_send(std::size_t length, udp::socket && soc)
    {
        soc.async_send_to(
                    asio::buffer(data_, length), sender_endpoint_,
                    [this](std::error_code /*ec*/, std::size_t /*bytes_sent*/)
        {
            do_receive();
        });
    }

private:
    std::shared_ptr<udp::socket> socket_ {};
    udp::endpoint sender_endpoint_;
    enum { max_length = 1024 };
    char data_[max_length];
};
