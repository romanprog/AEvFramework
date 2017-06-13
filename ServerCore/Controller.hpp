#ifndef AEvAcceptor_HPP
#define AEvAcceptor_HPP
#include "../AsyncEvent/AEvBase/AEventAbstract.hpp"
#include "../Config/GlobalConf.hpp"
#include "../HUtils/HNet.hpp"
#include "DnsBuffer.hpp"
#include "Workers.hpp"


#include <iostream>
#include <thread>
#include <chrono>

class Controller : public aev::AEventAbstract
{
public:
    Controller(aev::AEvRootConf & config);
    Controller(aev::AEvChildConf &&config);
    virtual ~Controller() override;

private:

    // Init config by static object method.
    ConfigData &_main_config {Config::glob().get_conf()};

    virtual void _ev_begin() override;
    virtual void _evFinish() override;
    virtual void _ev_stop() override;
    virtual void _ev_timeout() override;
    virtual void _ev_child_callback(aev::AEvPtrBase child_ptr, aev::AEvExitSignal & _ret) override;

    void _start_acceeptors();
    void _reload_config();

};


#endif // AEvAcceptor_HPP
