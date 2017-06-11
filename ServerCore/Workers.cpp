#include "Workers.hpp"
#include <functional>
#include <thread>

#include <iostream>


DnsTCPSession::DnsTCPSession(aev::AEvChildConf &&config,
                               asio::ip::tcp::socket &&_soc,
                               ConfigData main_conf_)

    :AEventAbstract::AEventAbstract(std::move(config)),
     _socket(std::move(_soc)),
     _main_config(std::move(main_conf_))
{
     log_debug_aev("AEvConnection CONSTRUCTOR! ");
}

DnsTCPSession::~DnsTCPSession()
{
    log_debug_aev("AEvConnection DESTRUCT" );
}

void DnsTCPSession::_ev_begin()
{
    log_debug_aev("AEvConnection START");
}

void DnsTCPSession::_ev_finish()
{
    log_debug_aev("AEvConnection FINISH");
}

void DnsTCPSession::_ev_stop()
{
    log_debug_aev("AEvConnection STOP");
    _ev_exit_signal = aev::AEvExitSignal::close_connection;
    _socket.cancel();
    _socket.close();
}

void DnsTCPSession::_ev_timeout()
{
    log_debug_aev("AEvConnection TIMEOUT");
}

void DnsTCPSession::_ev_child_callback(aev::AEvPtrBase child_ptr, aev::AEvExitSignal &_ret)
{

}

void DnsTCPSession::_receive_request()
{
    read_buffer->release(1024);
    log_debug_aev("AEvConnection ADD NEW HANDLER");
    _socket.async_read_some(asio::buffer(read_buffer->data_top(), read_buffer->size_avail()),
                            wrap_asio_cb([this](std::error_code ec, std::size_t bytes_transferred){

                                if (ec) {
                                    stop();
                                    return;
                                }
                                read_buffer->accept(bytes_transferred);

                            }));
}

void DnsTCPSession::_db_search(DnsReadBufferPtr buff_)
{

}



void DnsTCPSession::_send_respond(std::string data)
{

    log_debug_aev("AEvConnection ADD NEW HANDLER");

    _socket.async_send(asio::buffer(data), wrap_asio_cb([this](std::error_code ec, std::size_t bytes_transferred)->void
    {
        if (ec) {
            stop();
            return;
        }

        reset_and_start_timer();
    }));
}


