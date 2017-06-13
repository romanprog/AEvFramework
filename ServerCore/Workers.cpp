#include "Workers.hpp"
#include <functional>
#include <thread>

#include <iostream>


DnsTCPWorker::DnsTCPWorker(aev::AEvChildConf &&config,
                               asio::ip::tcp::socket &&_soc,
                               ConfigData main_conf_)

    :AEventAbstract::AEventAbstract(std::move(config)),
     _socket(std::move(_soc)),
     _main_config(std::move(main_conf_))
{
     log_debug_aev("AEvConnection CONSTRUCTOR! ");
}

DnsTCPWorker::~DnsTCPWorker()
{
    log_debug_aev("AEvConnection DESTRUCT" );
}

void DnsTCPWorker::_ev_begin()
{
    log_debug_aev("AEvConnection START");
}

void DnsTCPWorker::_evFinish()
{
    log_debug_aev("AEvConnection FINISH");
}

void DnsTCPWorker::_ev_stop()
{
    log_debug_aev("AEvConnection STOP");
    _ev_exit_signal = aev::AEvExitSignal::close_connection;
    _socket.cancel();
    _socket.close();
}

void DnsTCPWorker::_ev_timeout()
{
    log_debug_aev("AEvConnection TIMEOUT");
}

void DnsTCPWorker::_ev_child_callback(aev::AEvPtrBase child_ptr, aev::AEvExitSignal &_ret)
{

}

void DnsTCPWorker::_receive_request()
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

void DnsTCPWorker::_db_search(DnsReadBufferPtr buff_)
{

}



void DnsTCPWorker::_send_respond(std::string data)
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



DnsUDPWorker::DnsUDPWorker(aev::AEvChildConf &&config, ConfigData main_conf_, asio::ip::udp::endpoint &&r_epoint_, DnsReadBufferPtr buff_)
    : AEventAbstract::AEventAbstract(std::move(config)),
      _remote_endpoint(std::move(r_epoint_)),
      _main_config(std::move(main_conf_)),
      _read_buffer(buff_)
{

}

DnsUDPWorker::~DnsUDPWorker()
{

}

void DnsUDPWorker::_ev_begin()
{

}

void DnsUDPWorker::_evFinish()
{

}

void DnsUDPWorker::_ev_stop()
{

}

void DnsUDPWorker::_ev_timeout()
{

}

void DnsUDPWorker::_ev_child_callback(aev::AEvPtrBase child_ptr, aev::AEvExitSignal &_ret)
{

}
