#ifndef AEvAcceptor_HPP
#define AEvAcceptor_HPP
#include "../AsyncEvent/AEvBase/AEventAbstract.hpp"
#include "../Config/GlobalConf.hpp"
#include "../AsyncEvent/SysSig/AEvSysSig.hpp"
#include "DnsBuffer.hpp"
#include "../HUtils/HNet.hpp"


#include <iostream>
#include <thread>
#include <chrono>

class ListenerTCP : public aev::AEventAbstract
{
public:

    ListenerTCP(aev::AEvRootConf & config, const std::string &ip, const unsigned port);

    ListenerTCP(aev::AEvChildConf &&config, const std::string &ip, const unsigned port);
    virtual ~ListenerTCP() override;

private:

    asio::ip::tcp::acceptor _acceptor;
    asio::ip::tcp::socket _socket;
    std::string _conn_ip;
    unsigned _conn_port;
    // Init config by static object method.
    ConfigData _main_config {Config::glob().get_conf()};

    virtual void _ev_begin() override;
    virtual void _ev_finish() override;

    virtual void _ev_stop() override;

    virtual void _ev_timeout() override;
    virtual void _ev_child_callback(aev::AEvPtrBase child_ptr, aev::AEvExitSignal & _ret) override;

    void _start_acceept();

};

class ListenerUDP : public aev::AEventAbstract
{
public:

    ListenerUDP(aev::AEvRootConf & config, const std::string &ip, const unsigned port);

    ListenerUDP(aev::AEvChildConf &&config, const std::string &ip, const unsigned port);
    virtual ~ListenerUDP() override;

private:

    asio::ip::tcp::socket _socket;
    std::string _conn_ip;
    unsigned _conn_port;
    // Init config by static object method.
    ConfigData _main_config {Config::glob().get_conf()};

    virtual void _ev_begin() override;
    virtual void _ev_finish() override;

    virtual void _ev_stop() override;

    virtual void _ev_timeout() override;
    virtual void _ev_child_callback(aev::AEvPtrBase child_ptr, aev::AEvExitSignal & _ret) override;

    void _start_acceept();

};

#endif // AEvAcceptor_HPP
