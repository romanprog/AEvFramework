#include "Acceptors.hpp"
#include "DnsBuffer.hpp"
#include "../HUtils/HNet.hpp"
#include "../AsyncEvent/SysSig/AEvSysSig.hpp"

AcceptorTCP::AcceptorTCP(aev::AEvChildConf &&config, const std::string &ip, const unsigned port)
    :aev::AEventAbstract(std::move(config)),
      _acceptor(_ev_loop->get_io_service()),
      _socket(_ev_loop->get_io_service()),
      _conn_ip(ip),
      _conn_port(port)
{
    log_debug_aev("AcceptorTCP CONSTRUCTOR! ");
}

AcceptorTCP::~AcceptorTCP()
{
    log_debug("AcceptorTCP DESTRUCTOR! ");
}

void AcceptorTCP::_ev_begin()
{
    if (!hnet::is_ip_v4(_conn_ip)) {
        log_main("Error! Wrong IP: %", _conn_ip);
        stop();
        return;
    }

    asio::ip::tcp::resolver resolver(_ev_loop->get_io_service());
    asio::ip::tcp::endpoint endpoint = *resolver.resolve({_conn_ip, std::to_string(_conn_port)});

    _acceptor.open(endpoint.protocol());
    _acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));
    _acceptor.bind(endpoint);
    _acceptor.listen();
    create_child<aev::AEvSysSig>(0);
    // Init SMTP protocol handlers map.
    _start_acceept();
}

void AcceptorTCP::_evFinish() {}

void AcceptorTCP::_ev_stop()
{
    asio::error_code ec;
    _acceptor.cancel(ec);
    log_debug_aev("AcceptorTCP _ev_stop: %", ec.value());
}

void AcceptorTCP::_ev_timeout()
{

}

void AcceptorTCP::_ev_child_callback(aev::AEvPtrBase child_ptr, aev::AEvExitSignal &_ret)
{

}

void AcceptorTCP::_start_acceept()
{
    using namespace std::placeholders;

    _acceptor.async_accept(_socket, wrap_asio_cb(
                               [this] (std::error_code ec)
    {
                               if (ec) {
                                   log_debug_aev("Error: %", ec.message());
                                   stop();
                                   return;
                               }

                               log_debug_aev("conn");

                               //                                        create_child<AEvSmtpSession>(0, std::move(_socket), _handlers_map,
                               //                                                              wrap_cb([this](SmtpStatePtr &&message_) {
                               //                                                                _new_message_handler(std::move(message_));
                               //                                                              }),_main_config);
                               auto sdel = std::move(_socket);
                               _start_acceept();
                           })
            );
}



AcceptorUDP::AcceptorUDP(aev::AEvChildConf &&config, const std::__cxx11::string &ip, const unsigned port)
    :aev::AEventAbstract(std::move(config)),
      _socket(_ev_loop->get_io_service()),
      _conn_ip(ip),
      _conn_port(port)
{

}

AcceptorUDP::~AcceptorUDP()
{

}

void AcceptorUDP::_ev_begin()
{
    if (!hnet::is_ip_v4(_conn_ip)) {
        log_main("Error! Wrong IP: %", _conn_ip);
        stop();
        return;
    }

    asio::ip::udp::resolver resolver(_ev_loop->get_io_service());
    asio::ip::udp::endpoint endpoint = *resolver.resolve({_conn_ip, std::to_string(_conn_port)});
    _socket.bind(endpoint);

    create_child<aev::AEvSysSig>(0);
    // Init SMTP protocol handlers map.
    _start_acceept();
}

void AcceptorUDP::_evFinish()
{

}

void AcceptorUDP::_ev_stop()
{

}

void AcceptorUDP::_ev_timeout()
{

}

void AcceptorUDP::_ev_child_callback(aev::AEvPtrBase child_ptr, aev::AEvExitSignal &_ret)
{

}

void AcceptorUDP::_start_acceept()
{

}
