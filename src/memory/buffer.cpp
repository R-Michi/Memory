/**
* @file     buffer.cpp
* @brief    Implementation of the Buffer-class.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#include "buffer.h"

using namespace memory;

Buffer::Buffer(size_t limit, size_t size)
{
    this->_begin = this->_end = this->_eob = nullptr;
    this->_cap = this->_size = 0;
    this->set_limit(limit);
    if(size > 0)
        this->resize(size);
}

Buffer::Buffer(const Buffer& buff) : Buffer()
{
    this->copy(buff);
}

Buffer::Buffer(Buffer&& buff) : Buffer()
{
    this->move(buff);
}

inline void* Buffer::addvp(void* vp, uint64_t x) noexcept
{
    return reinterpret_cast<void*>(reinterpret_cast<uint64_t>(vp) + x);
}

void Buffer::update_references(void) noexcept
{
    uint64_t begin = reinterpret_cast<uint64_t>(this->_begin);
    for (Element& e : this->_table)
    {
        e.data = reinterpret_cast<void*>(begin);
        begin += e.size;
    }
}

void Buffer::allocate(size_t size)
{
    if (this->_begin == nullptr)
    {
        this->_begin = malloc(size);
        if (this->_begin == nullptr)
            throw std::bad_array_new_length();
        this->_cap = size;
        this->_end = addvp(this->_begin, this->_size);
        this->_eob = addvp(this->_begin, this->_cap);
    }
}

void Buffer::reallocate(size_t new_size)
{
    if (this->_begin == nullptr)
        this->allocate(new_size);
    else
    {
        this->_begin = realloc(this->_begin, new_size);
        if (this->_begin == nullptr)
            throw std::bad_array_new_length();
        this->_cap = new_size;
        this->_end = addvp(this->_begin, this->_size);
        this->_eob = addvp(this->_begin, this->_cap);
        this->update_references();
    }
}

void Buffer::push(pid_t pid, uint64_t address, size_t size, type_t type, const void* data)
{
    size_t req_size = this->_size + size;

    // allocate more space if it is needed
    if (req_size > this->_cap)
        this->resize(req_size);

    // add address list element
    Element e = { pid, address, size, type, this->_end };
    this->_table.push_back(e);

    // write data to the buffer
    if (data != nullptr)
        memcpy(this->_end, data, size);
    else
        memset(this->_end, 0x00, size);

    this->_size = req_size;
    this->_end = addvp(this->_end, size);
}

bool Buffer::resize(size_t new_size)
{
    if (new_size <= this->_size) return false;

    // exception if buffer size exeeds the limit
    if (this->_limit > 0 && new_size > this->_limit)
        throw out_of_limit();

    // allocate new size
    this->reallocate(new_size);

    return true;
}

void Buffer::shrink_to_fit(void)
{
    if (this->_size == 0)
        this->clear();
    if (this->_cap > this->_size)
        this->reallocate(this->_size);
}

void Buffer::clear(void) noexcept
{
    if (this->_begin != nullptr)
    {
        free(this->_begin);
        this->_begin = this->_end = this->_eob = nullptr;
        this->_size = this->_cap = 0;
        this->_table.clear();
        this->_table.shrink_to_fit();   // may be slower but cunsumes less memory
    }
}

void Buffer::set_limit(size_t limit)
{ 
    if (limit < this->_cap)
        throw out_of_limit();
    this->_limit = limit; 
}

Buffer& Buffer::copy(const Buffer& buff)
{
    this->clear();
    this->set_limit(buff.limit());
    this->resize(buff.capacity());
    for (const Element& e : buff._table)
        this->push(e.pid, e.address, e.size, e.type, e.data);
    return *this;
}

Buffer& Buffer::move(Buffer& buff)
{
    this->copy(buff);
    buff.clear();
    return *this;
}

Buffer& Buffer::operator= (const Buffer& buff)
{
    return this->copy(buff);
}

Buffer& Buffer::operator= (Buffer&& buff)
{
    return this->move(buff);
}
