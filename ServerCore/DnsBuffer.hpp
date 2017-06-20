#ifndef DNSBUFFER_HPP
#define DNSBUFFER_HPP

#include "../AsyncEvent/BufferBase/PBufferAbstract.hpp"
#include "DnsTypes.hpp"
#include "DnsUtils.hpp"
#include "../Logger/Logger.hpp"

#include <string>
#include <vector>

class DnsRequestBuffer;
using DnsReadBufferPtr = std::unique_ptr<DnsRequestBuffer>;

class DnsBuffer : public BuffAbstract
{
public:
    DnsBuffer();

    bool prepare_for_request(const std::string & name, dns::DnsQType t);
    bool prepare_for_respond();
    bool read_respond(size_t bytes_readed);
    dns::DnsError get_error() const
    {
        return err;
    }
    dns::DnsRespondPtr withdraw_respond();
    void clear();

private:

    virtual size_t calculate_mem(size_t block_size) override;
    virtual void when_new_data_acc(size_t bytes_readed) override;
    dns::DnsError err;

    bool redy_for_respond {false};
    dns::DnsQuery _request;
    dns::DnsRespondPtr _respond_ptr;
};


class DnsRequestBuffer : public BuffAbstract
{
public:
    DnsRequestBuffer();

    inline dns::DnsError get_error() const
    {
        return err;
    }


    void clear();
    void parse();

private:

    dns::DnsError err;
    dns::DnsQuery _respond;
    dns::DnsQuery _request;
};


#endif // DNSBUFFER_HPP
