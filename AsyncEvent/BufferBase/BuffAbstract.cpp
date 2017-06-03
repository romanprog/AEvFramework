#include "BuffAbstract.hpp"
#include "../../Logger/Logger.hpp"

BuffAbstract::BuffAbstract(size_t block_size_, size_t reserve_block_count_)
    : _basic_block_size(block_size_),
      _reserve_block_count(reserve_block_count_),
      _reserved(calculate_mem(_basic_block_size))
{
    _cdata = static_cast<char *>(malloc(_reserved));
}

BuffAbstract::~BuffAbstract()
{
    // Free main data block.
    free(_cdata);
}

const char *BuffAbstract::data() const
{
    return static_cast<const char * const> (_cdata);
}

void *BuffAbstract::data_top()
{
    // No free space.
    if (!size_avail())
        return nullptr;

    // Pointer to begin of free space.
    return _cdata + _top_offset;
}

void *BuffAbstract::vdata()
{
    return _cdata;
}

bool BuffAbstract::accept(size_t bytes_readed)
{
    if (bytes_readed > size_avail())
        return false;

    _top_offset += bytes_readed;

    when_new_data_acc(bytes_readed);
    return true;
}

void BuffAbstract::release(size_t size)
{
    if (_size > 80000000)
        throw std::out_of_range("Buffer overflow. Max - 80Mb");

    if (size < size_avail())
        return;

    size_t _reserved_free = _reserved - _top_offset;

    if (_reserved_free < size) {
        _reserved = calculate_mem(size);
        _cdata = static_cast<char *>(realloc(_cdata, _reserved));
    }
    _size = _top_offset + size;

}

size_t BuffAbstract::size_filled() const
{
    return _top_offset;
}


void BuffAbstract::reset(bool soft_reset)
{
    _top_offset = _size = 0;

    if (!soft_reset) {
        _reserved = calculate_mem(_basic_block_size);
        _cdata = static_cast<char *>(realloc(_cdata, _reserved));

    }
    when_reseted();
}

size_t BuffAbstract::size() const
{
    return _size;
}

size_t BuffAbstract::size_avail() const
{
    return _size - _top_offset;
}

size_t BuffAbstract::size_reserved() const
{
    return _reserved;
}

void BuffAbstract::operator <<(const char *str)
{
    size_t sz = std::strlen(str);
    release(sz);
    memcpy(data_top(), str, sz);
    accept(sz);
}

void BuffAbstract::operator <<(const std::string &str)
{
    release(str.size());
    memcpy(data_top(), str.data(), str.size());
    accept(str.size());
}

size_t BuffAbstract::calculate_mem(size_t block_size)
{
    // Base mem reserv calculate. 1 block for data needeng + 1 free block.
    return size_filled() + block_size * (_reserve_block_count + 1);
}

void BuffAbstract::when_new_data_acc(size_t bytes_readed)
{

}

void BuffAbstract::when_reseted()
{

}

size_t BuffAbstract::top_offset() const
{
    return _top_offset;
}

void BuffAbstract::change_data_top(size_t new_data_top)
{
    _top_offset = new_data_top;
}

void BuffAbstract::reset_size()
{
    _size = 0;
}
