#include "QueryWorker.hpp"
#include <functional>
#include <thread>

#include <iostream>


AEvSmtpSession::AEvSmtpSession(aev::AEvChildConf &&config,
                               asio::ip::tcp::socket &&_soc,
                               ConfigData main_conf_)

    :AEventAbstract::AEventAbstract(std::move(config)),
     _socket(std::move(_soc)),
     _main_config(std::move(main_conf_))
{
     log_debug_aev("AEvConnection CONSTRUCTOR! ");
}

AEvSmtpSession::~AEvSmtpSession()
{
    log_debug_aev("AEvConnection DESTRUCT" );
}

void AEvSmtpSession::_ev_begin()
{
    log_debug_aev("AEvConnection START");
    _send_respond("220 " + _main_config.smtp_welcome + "\r\n");
}

void AEvSmtpSession::_ev_finish()
{
    log_debug_aev("AEvConnection FINISH");
}

void AEvSmtpSession::_ev_stop()
{
    log_debug_aev("AEvConnection STOP");
    _ev_exit_signal = AEvExitSignal::close_connection;
    _socket.cancel();
    _socket.close();
}

void AEvSmtpSession::_ev_timeout()
{
    log_debug_aev("AEvConnection TIMEOUT");
}

void AEvSmtpSession::_ev_child_callback(AEvPtrBase child_ptr, AEvExitSignal &_ret)
{

}

void AEvSmtpSession::_receive_request()
{
    _read_cmd_buffer.release(1024);
    log_debug_aev("AEvConnection ADD NEW HANDLER");
    _socket.async_read_some(asio::buffer(_read_cmd_buffer.data_top(), _read_cmd_buffer.size_avail()),
                            wrap_asio_cb([this](std::error_code ec, std::size_t bytes_transferred){

                                if (ec) {
                                    stop();
                                    return;
                                }
                                _read_cmd_buffer.accept(bytes_transferred);
                                _begin_transaction();
                            }));
}



void AEvSmtpSession::_send_respond(std::string data, ConfirmHendler confirm)
{

    log_debug_aev("AEvConnection ADD NEW HANDLER");

    _socket.async_send(asio::buffer(data), wrap_asio_cb([this, confirm](std::error_code ec, std::size_t bytes_transferred)->void
    {
        if (ec) {
            stop();
            return;
        }

        if (confirm) {
            confirm(false);
        }
        reset_and_start_timer();
    }));
}


