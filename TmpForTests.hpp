#include <cstdlib>
#include <iostream>
#include <asio/steady_timer.hpp>
#include "asio.hpp"
#include "Logger/Logger.hpp"
#include "AsyncEvent/BufferBase/BuffAbstract.hpp"

void process_mem_usage(double& vm_usage, double& resident_set)
{
   using std::ios_base;
   using std::ifstream;
   using std::string;

   vm_usage     = 0.0;
   resident_set = 0.0;

   // 'file' stat seems to give the most reliable results
   //
   ifstream stat_stream("/proc/self/stat",ios_base::in);

   // dummy vars for leading entries in stat that we don't care about
   //
   string pid, comm, state, ppid, pgrp, session, tty_nr;
   string tpgid, flags, minflt, cminflt, majflt, cmajflt;
   string utime, stime, cutime, cstime, priority, nice;
   string O, itrealvalue, starttime;

   // the two fields we want
   //
   unsigned long vsize;
   long rss;

   stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
               >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
               >> utime >> stime >> cutime >> cstime >> priority >> nice
               >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

   stat_stream.close();

   long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
   vm_usage     = vsize / 1024.0;
   resident_set = rss * page_size_kb;
}


using asio::ip::udp;

class DnsReadBufferT : public BuffAbstract
{
public:
    DnsReadBufferT()
        :BuffAbstract(512,0)
    {}

    void clear() {}

private:


};




class server
{
public:
    server(asio::io_service& io_service, short port)
        : socket_(io_service, udp::endpoint(udp::v4(), port))    {
        do_receive();
    }

    void do_receive()
    {
        std::shared_ptr<udp::endpoint> sender_endpoint {std::make_shared<udp::endpoint>()};
        socket_.async_receive_from(
                    asio::buffer(data_, max_length), *sender_endpoint,
                    [this, sender_endpoint](std::error_code ec, std::size_t bytes_recvd)
        {
            if (!ec && bytes_recvd > 0)
            {
                log_main("asdadssavsdv %", sender_endpoint_.address().to_string());
                wait(sender_endpoint, bytes_recvd);
            }
            do_receive();

        });

    }

    void wait(std::shared_ptr<udp::endpoint> endpoint_, size_t sz_)
    {
        std::shared_ptr<asio::steady_timer> timer = std::make_shared<asio::steady_timer>(socket_.get_io_service());
        timer->expires_from_now(std::chrono::milliseconds(wt));
        wt = 1000;
        timer->async_wait([this, endpoint_, sz_, timer](const asio::error_code & ec)
        {
           if (ec)
               return;

           do_send(endpoint_, sz_);
        });
    }

    void do_send(std::shared_ptr<udp::endpoint> endpoint_, size_t sz_)
    {
        socket_.async_send_to(
                    asio::buffer(data_, sz_), *endpoint_,
                    [this](std::error_code /*ec*/, std::size_t /*bytes_sent*/)
        {
            do_receive();
        });
    }

private:
    udp::socket socket_ ;
    udp::endpoint sender_endpoint_;
    unsigned wt {5000};
    enum { max_length = 1024 };
    char data_[max_length];
};
