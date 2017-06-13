#ifndef ACCEPTORS_H
#define ACCEPTORS_H
#include "../AsyncEvent/AEvBase/AEventAbstract.hpp"
#include "../Config/GlobalConf.hpp"
#include "../HUtils/HNet.hpp"
#include "DnsBuffer.hpp"
#include "Workers.hpp"

#include <iostream>

class AcceptorTCP : public aev::AEventAbstract
{
public:

    AcceptorTCP(aev::AEvChildConf &&config, const std::string &ip, const unsigned port);
    virtual ~AcceptorTCP() override;

private:

    asio::ip::tcp::acceptor _acceptor;
    asio::ip::tcp::socket _socket;
    std::string _conn_ip;
    unsigned _conn_port;
    // Init config by static object method.
    ConfigData _main_config {Config::glob().get_conf()};

    virtual void _ev_begin() override;
    virtual void _evFinish() override;

    virtual void _ev_stop() override;

    virtual void _ev_timeout() override;
    virtual void _ev_child_callback(aev::AEvPtrBase child_ptr, aev::AEvExitSignal & _ret) override;

    void _start_acceept();

};

class AcceptorUDP : public aev::AEventAbstract
{
public:

    AcceptorUDP(aev::AEvChildConf &&config, const std::string &ip, const unsigned port);
    virtual ~AcceptorUDP() override;

private:
    asio::ip::udp::socket _socket;
    asio::ip::udp::endpoint _sender_endpoint;
    std::string _conn_ip;
    unsigned _conn_port;
    // Init config by static object method.
    ConfigData _main_config {Config::glob().get_conf()};

    virtual void _ev_begin() override;
    virtual void _evFinish() override;

    virtual void _ev_stop() override;

    virtual void _ev_timeout() override;
    virtual void _ev_child_callback(aev::AEvPtrBase child_ptr, aev::AEvExitSignal & _ret) override;

    void _start_acceept();

};


#endif // ACCEPTORS_H
