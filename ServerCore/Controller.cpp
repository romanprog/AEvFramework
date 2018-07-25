#include "Controller.hpp"
#include "DnsBuffer.hpp"
#include "Acceptors.hpp"

#include "../AsyncEvent/SysSig/AEvSysSig.hpp"
#include "../HUtils/HNet.hpp"


#include <iostream>
#include <thread>
#include <chrono>



Controller::Controller(aev::AEvRootConf &config)
    :aev::AEventAbstract(config)
{
    log_debug_aev("Controller CONSTRUCTOR! ");
}

Controller::Controller(aev::AEvChildConf &&config)
    :aev::AEventAbstract(std::move(config))
{
    log_debug_aev("Controller CONSTRUCTOR! ");
}

Controller::~Controller()
{
    log_debug("Controller DESTRUCTOR! ");
}

void Controller::_ev_begin()
{
    _start_acceeptors();
}

void Controller::_evFinish() {}

void Controller::_ev_stop()
{
    asio::error_code ec;
    log_debug_aev("Controller _ev_stop: %", ec.value());
}

void Controller::_ev_timeout()
{

}

void Controller::_ev_child_callback(aev::AEvPtrBase child_ptr, aev::AEvExitSignal &_ret)
{

}

void Controller::_start_acceeptors()
{
    log_debug("Starting TCP acceptor on %, port %", _main_config.bind_ip, _main_config.port);
    create_child<AcceptorTCP>(aev::NoTimeout, _main_config.bind_ip, _main_config.port);
    log_debug("Starting UDP acceptor on %, port %", _main_config.bind_ip, _main_config.port);
    create_child<AcceptorUDP>(aev::NoTimeout, _main_config.bind_ip, _main_config.port);
}

void Controller::_reload_config()
{
    if (!Config::glob().read_config("main.conf")) {
        log_main("Can't load config file. %", Config::glob().error_text());
        exit(0);
    }
    _main_config = Config::glob().get_conf();
}


