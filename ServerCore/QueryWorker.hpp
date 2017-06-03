#ifndef AEvConnection_HPP
#define AEvConnection_HPP

#include "../AsyncEvent/AEvBase/AEventAbstract.hpp"

#include <iostream>

class AEvSmtpSession : public AEventAbstract
{
public:

    explicit AEvSmtpSession(aev::AEvChildConf && config,
                            asio::ip::tcp::socket && _soc,
                            ConfigData main_conf_);

    virtual ~AEvSmtpSession() override;

private:

    asio::ip::tcp::socket _socket;
    const CommandsMap _handlers_map;
    ConfigData _main_config;

protected:
    virtual void _ev_begin() override;
    virtual void _ev_finish() override;
    virtual void _ev_stop() override;
    virtual void _ev_timeout() override;
    virtual void _ev_child_callback(AEvPtrBase child_ptr, AEvExitSignal & _ret) override;

    void _receive_request();
    void _send_respond(std::string data, ConfirmHendler confirm = nullptr);

};


#endif // AEvConnection_HPP
