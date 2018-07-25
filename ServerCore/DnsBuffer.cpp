#include "DnsBuffer.hpp"
#include "DnsTypes.hpp"
#include <cstring>
#include <iostream>

DnsBuffer::DnsBuffer()
    :_respond_ptr(std::make_unique<dns::DnsQuery>())
{ }

bool DnsBuffer::prepare_for_request(const std::string &name, dns::DnsQType t)
{
    // Resrt buffer before request creating.
    reset();
    err = dns::utils::create_request(name, t, _request);
    // Create DNS request pkg.
    if (err != dns::DnsError::noerror)
        return false;

    // Release buffer.
    release(dns::max_pkg_size);
    // Write DNS reqest to buffer.
    size_t req_size = dns::utils::request_buff_write(vdata(), _request);

    if (!req_size) {
        err = dns::DnsError::req_create_err;
        return false;
    }
    accept(req_size);
    return true;
}

bool DnsBuffer::prepare_for_respond()
{
    if (err != dns::DnsError::noerror)
        return false;

    reset();
    release(dns::max_pkg_size);
    return true;
}

bool DnsBuffer::read_respond(size_t bytes_readed)
{
    if (err != dns::DnsError::noerror)
        return false;

    accept(bytes_readed);
    err = dns::utils::buff_parse(vdata(), *_respond_ptr);
    if (err != dns::DnsError::noerror) {
        reset();
        return false;
    }
    return true;
}

dns::DnsRespondPtr DnsBuffer::withdraw_respond()
{
    return std::move(_respond_ptr);
}

void DnsBuffer::clear()
{
    reset();
    _request = dns::DnsQuery();
    _respond_ptr = std::make_unique<dns::DnsQuery>();
}

size_t DnsBuffer::calculate_mem(size_t block_size)
{
    // size_t block_size = dns::max_pkg_size ? dns::max_pkg_size : 1;
    size_t reserve_bl_count {1};

    return ((top_offset() + size_filled()) / block_size + reserve_bl_count) * block_size;
}

void DnsBuffer::when_new_data_acc(size_t bytes_readed)
{

}

DnsRequestBuffer::DnsRequestBuffer()
    :BuffAbstract(512,0)
{

}

void DnsRequestBuffer::clear()
{
    reset();
}

void DnsRequestBuffer::parse()
{
    err = dns::utils::buff_parse(vdata(), _respond);
    if (err != dns::DnsError::noerror) {
        log_debug("Parse ERROR");
        reset();
        // return false;
    }
    log_main(_respond.alist[1].req_name);
    log_debug("Parse OK");
    // return true;
}

