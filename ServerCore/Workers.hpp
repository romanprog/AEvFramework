#ifndef AEvConnection_HPP
#define AEvConnection_HPP

#include "../AsyncEvent/AEvBase/AEventAbstract.hpp"
#include "DnsBuffer.hpp"
#include <iostream>

using DnsReadBufferPtr = std::shared_ptr<DnsReadBuffer>;

class DnsTCPSession : public aev::AEventAbstract
{
public:

    explicit DnsTCPSession(aev::AEvChildConf && config,
                            asio::ip::tcp::socket && _soc,
                            ConfigData main_conf_);

    virtual ~DnsTCPSession() override;

private:

    asio::ip::tcp::socket _socket;
    ConfigData _main_config;
    DnsReadBufferPtr read_buffer {std::make_shared<DnsReadBuffer>()};

protected:
    virtual void _ev_begin() override;
    virtual void _ev_finish() override;
    virtual void _ev_stop() override;
    virtual void _ev_timeout() override;
    virtual void _ev_child_callback(aev::AEvPtrBase child_ptr, aev::AEvExitSignal & _ret) override;

    void _receive_request();
    void _db_search(DnsReadBufferPtr buff_);
    void _send_respond(std::string data);

};


#endif // AEvConnection_HPP
