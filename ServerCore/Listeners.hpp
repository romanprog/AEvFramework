#ifndef AEvAcceptor_HPP
#define AEvAcceptor_HPP
#include "../AsyncEvent/AEvBase/AEventAbstract.hpp"
// #include "QueryWorker.hpp"
#include "../Config/GlobalConf.hpp"
#include "../AsyncEvent/SysSig/AEvSysSig.hpp"
#include "DnsBuffer.hpp"
#include "../HUtils/HNet.hpp"


#include <iostream>
#include <thread>
#include <chrono>

template <typename SocT>
class ListenerBase : public aev::AEventAbstract
{
public:

    ListenerBase(aev::AEvRootConf & config, const std::string &ip, const unsigned port)
        :aev::AEventAbstract(config),
          _acceptor(_ev_loop->get_io_service()),
          _socket(_ev_loop->get_io_service()),
          _conn_ip(ip),
          _conn_port(port)
    {
        log_debug_aev("ListenerBase CONSTRUCTOR! ");
    }

    ListenerBase(aev::AEvChildConf &&config, const std::string &ip, const unsigned port)
        :aev::AEventAbstract(std::move(config)),
          _acceptor(_ev_loop->get_io_service()),
          _socket(_ev_loop->get_io_service()),
          _conn_ip(ip),
          _conn_port(port)
    {
        log_debug_aev("ListenerBase CONSTRUCTOR! ");
    }
    virtual ~ListenerBase() override
    {
       log_debug("AEvAcceptor DESTRUCTOR! ");
    }

private:

    typename SocT::acceptor _acceptor;
    typename SocT::socket _socket;
    std::string _conn_ip;
    unsigned _conn_port;
    // Init config by static object method.
    ConfigData _main_config {Config::glob().get_conf()};

    virtual void _ev_begin() override
    {
        if (!hnet::is_ip_v4(_conn_ip)) {
            stop();
            return;
        }

        typename SocT::resolver resolver(_ev_loop->get_io_service());
        typename SocT::endpoint endpoint = *resolver.resolve({_conn_ip, std::to_string(_conn_port)});

        _acceptor.open(endpoint.protocol());
        _acceptor.set_option(typename SocT::acceptor::reuse_address(true));
        _acceptor.bind(endpoint);
        _acceptor.listen();
        create_child<aev::AEvSysSig>(0);
        // Init SMTP protocol handlers map.
        _start_acceept();
    }
    virtual void _ev_finish() override
    {}

    virtual void _ev_stop() override
    {
        asio::error_code ec;
        _acceptor.cancel(ec);
        log_debug_aev("AEvConnection _ev_stop: " );
    }

    virtual void _ev_timeout() override
    {

    }
    virtual void _ev_child_callback(aev::AEvPtrBase child_ptr, aev::AEvExitSignal & _ret) override
    {

    }

    void _start_acceept()
    {
        using namespace std::placeholders;
        _acceptor.async_accept(_socket, wrap_asio_cb(
                                   [this] (std::error_code ec)
                                   {
                                        if (ec) {
                                            stop();
                                            return;
                                        }

                                        log_debug_aev("conn");

//                                        create_child<AEvSmtpSession>(0, std::move(_socket), _handlers_map,
  //                                                              wrap_cb([this](SmtpStatePtr &&message_) {
    //                                                                _new_message_handler(std::move(message_));
      //                                                              }),_main_config);
                                        _start_acceept();
                                  })
                                );
    }

};

#endif // AEvAcceptor_HPP
