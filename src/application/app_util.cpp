/**
* @file     app_util.cpp
* @brief    Utility functions for the application.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#define _CRT_SECURE_NO_WARNINGS

#include "app.h"
#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip>

using namespace memory::app;

Application::Application(void)
{
    // init tables
    this->search_table.add_column("PID");
    this->search_table.add_column("Address");
    this->search_table.add_column("Type");
    this->search_table.add_column("Size");
    this->search_table.add_column("Decimal");
    this->search_table.add_column("Hexadecimal");

    this->process_table.add_column("Name");
    this->process_table.add_column("PID");
    this->process_table.add_column("Parent PID");
    this->process_table.add_column("Thread count");

    // init buffers
    this->search_buffer.set_limit(this->cfg.search_limit_size());
    this->undo_buffer.set_limit(this->cfg.search_limit_size());
    this->redo_buffer.set_limit(this->cfg.search_limit_size());

    // init others
    this->pid_live_memory = this->pid_dump = MEMORY_PID_INVALID;
    this->pid_this = GetCurrentProcessId();
}

Application::~Application(void)
{

}


double Application::auto_SI(uint64_t bytes, std::string& prefix) noexcept
{
    constexpr static char si_prefixes[12][4] = { "B", "kB", "MB", "GB", "TB", "PX", "EB", "ZB", "YB" };
    // 0 bytes deserves special case as the calculation breaks at 0
    if (bytes == 0)
    {
        prefix = si_prefixes[0];
        return 0;
    }

    double x, _floor, _fract;
    x = log2(static_cast<double>(bytes)) / 10.0;
    _floor = std::floor(x);
    _fract = x - _floor;
    prefix = si_prefixes[static_cast<size_t>(_floor)];
    return pow(2, 10 * _fract);
}

void Application::list_unknown_options(const Command& cmd, const std::vector<std::string>& in_options, std::vector<std::string>& out_options) noexcept
{
    out_options.clear();

    // find all options -> get a vector of idices of the options
    std::vector<uint32_t> indices;
    cmd.options().find_not(in_options, 0, &indices);

    // list the options of the indices, and insert leading '-' as for output purposes the leading '-' is requiered
    for (uint32_t i : indices)
        out_options.push_back(std::string("-") + cmd.options().at(i));
}

bool Application::is_input_hex(const std::string& inp)
{
    return (inp.substr(0, 2) == "0x");
}

std::string Application::make_msg(std::string msg)
{
    using namespace std::chrono;
    time_point<system_clock> tp = system_clock::now();
    time_t time = system_clock::to_time_t(tp);

    std::stringstream ss;
    ss << "[" << std::put_time(std::localtime(&time), "%H:%M:%S") << "]: " << msg;
    return ss.str();
}

void Application::make_search_entry(pid_t pid, address_t address, size_t size, uint8_t* value, std::vector<std::string>& entry)
{
    uint64_t s = static_cast<uint64_t>(size);  // size_t is not always 64 bit, its size is implementation defined
    utility::to_dec_str<pid_t>(pid, entry[0]);
    utility::to_hex_str<uint64_t>(address, entry[1]);
    utility::strtype(this->cfg.type(), entry[2]);
    utility::to_string(reinterpret_cast<const uint8_t*>(&s), sizeof(uint64_t), memory::MEMORY_TYPE_UINT64, false, entry[3]);
    if (this->cfg.type() == MEMORY_TYPE_STRING)
    {
        entry[4].clear();
        entry[5] = "---";
        for (size_t i = 0; i < size; i++)
        {
            // control characters are displayed as '.'
            if (value[i] < 32)
                entry[4].push_back('.');
            else
                entry[4].push_back(value[i]);
        }
    }
    else
    {
        utility::to_string(value, size, this->cfg.type(), false, entry[4]);
        utility::to_string(value, size, this->cfg.type(), true, entry[5]);
    }
}

void Application::make_process_entry(const Process& p, std::vector<std::string>& entry)
{
    entry[0] = p.name();
    utility::to_dec_str(p.pid(), entry[1]);
    utility::to_dec_str(p.parent_pid(), entry[2]);
    utility::to_dec_str(p.count_threads(), entry[3]);
}

void Application::make_backup(void)
{
    this->undo_buffer = std::move(this->search_buffer);
    this->redo_buffer.clear();
}

bool Application::is_between(uint8_t* _a, uint8_t* _b, uint8_t* _ref)
{
    switch (this->cfg.type())
    {
    case MEMORY_TYPE_INT8:
    {
        int8_t a, b, ref;
        memcpy(&a, _a, this->cfg.type_size());
        memcpy(&b, _b, this->cfg.type_size());
        memcpy(&ref, _ref, this->cfg.type_size());
        return (ref >= a) && (ref <= b);
    }
    case MEMORY_TYPE_UINT8:
    {
        uint8_t a, b, ref;
        memcpy(&a, _a, this->cfg.type_size());
        memcpy(&b, _b, this->cfg.type_size());
        memcpy(&ref, _ref, this->cfg.type_size());
        return (ref >= a) && (ref <= b);
    }
    case MEMORY_TYPE_INT16:
    {
        int16_t a, b, ref;
        memcpy(&a, _a, this->cfg.type_size());
        memcpy(&b, _b, this->cfg.type_size());
        memcpy(&ref, _ref, this->cfg.type_size());
        return (ref >= a) && (ref <= b);
    }
    case MEMORY_TYPE_UINT16:
    {
        uint16_t a, b, ref;
        memcpy(&a, _a, this->cfg.type_size());
        memcpy(&b, _b, this->cfg.type_size());
        memcpy(&ref, _ref, this->cfg.type_size());
        return (ref >= a) && (ref <= b);
    }
    case MEMORY_TYPE_INT32:
    {
        int32_t a, b, ref;
        memcpy(&a, _a, this->cfg.type_size());
        memcpy(&b, _b, this->cfg.type_size());
        memcpy(&ref, _ref, this->cfg.type_size());
        return (ref >= a) && (ref <= b);
    }
    case MEMORY_TYPE_UINT32:
    {
        uint32_t a, b, ref;
        memcpy(&a, _a, this->cfg.type_size());
        memcpy(&b, _b, this->cfg.type_size());
        memcpy(&ref, _ref, this->cfg.type_size());
        return (ref >= a) && (ref <= b);
    }
    case MEMORY_TYPE_INT64:
    {
        int64_t a, b, ref;
        memcpy(&a, _a, this->cfg.type_size());
        memcpy(&b, _b, this->cfg.type_size());
        memcpy(&ref, _ref, this->cfg.type_size());
        return (ref >= a) && (ref <= b);
    }
    case MEMORY_TYPE_UINT64:
    {
        uint64_t a, b, ref;
        memcpy(&a, _a, this->cfg.type_size());
        memcpy(&b, _b, this->cfg.type_size());
        memcpy(&ref, _ref, this->cfg.type_size());
        return (ref >= a) && (ref <= b);
    }
    case MEMORY_TYPE_FLOAT:
    {
        float a, b, ref;
        memcpy(&a, _a, this->cfg.type_size());
        memcpy(&b, _b, this->cfg.type_size());
        memcpy(&ref, _ref, this->cfg.type_size());
        return (ref >= a) && (ref <= b);
    }
    case MEMORY_TYPE_DOUBLE:
    {
        double a, b, ref;
        memcpy(&a, _a, this->cfg.type_size());
        memcpy(&b, _b, this->cfg.type_size());
        memcpy(&ref, _ref, this->cfg.type_size());
        return (ref >= a) && (ref <= b);
    }
    default: return false;
    }

    // The return is useless, but some compilers print a warning if the return is not there!
    return false;
}

uint64_t Application::scan(Process& proc, uint8_t* a, uint8_t* b, size_t size, bool& limit)
{
    if (!proc.is_valid()) return 0;
    if (a == nullptr || b == nullptr) return 0;
    bool equal = (memcmp(a, b, size) == 0);
    limit = false;

    // query memory pages
    std::vector<MemoryInfo> pages;
    proc.query(this->cfg.start_address(), this->cfg.end_address(), pages);

    // scan all memory pages
    address_t cur_addr;
    size_t rd_size;
    uint64_t count = 0;
    const size_t max_rd_size = this->cfg.search_split_size() + size - 1;
    uint8_t* buff = new uint8_t[max_rd_size];

    for (auto page = pages.begin(); page != pages.end() && !limit; page++)
    {
        // if memory page is too big, split it
        for (address_t i = 0; i < page->size && !limit; i += this->cfg.search_split_size())
        {
            cur_addr = page->base + i;
            rd_size = ((page->size - i) < max_rd_size) ? (page->size - i) : max_rd_size; // dont read out of bounds of the page

            // read memory
            proc.read(cur_addr, rd_size, buff);

            // scan memory for values
            for (address_t j = 0; j <= (rd_size - size) && !limit; j += this->cfg.alignment())
            {
                limit = (this->search_buffer.size() + size) > this->search_buffer.limit();
                if (!limit && (equal ? (memcmp(a, buff + j, size) == 0) : this->is_between(a, b, buff + j)))
                {
                    this->search_buffer.push(proc.pid(), cur_addr + j, size, this->cfg.type(), buff + j);
                    ++count;
                }
            }
        }
    }
    delete[] buff;
    return count;
}

uint64_t Application::update(uint8_t* a, uint8_t* b, size_t size)
{
    if (this->undo_buffer.table().size() == 0) return 0;

    uint8_t ref[size];
    bool equal = (memcmp(a, b, size) == 0);

    // update all saved addresses
    Process cur_p;
    for (Buffer::Element& e : this->undo_buffer.table())
    {
        // find new process if old pid is different to the current pid
        if (e.pid != cur_p.pid() || !cur_p.is_valid())
        {
            cur_p.close();                  // close old process
            cur_p.init("", e.pid, 0, 0);    // init current process with stored PID, all other information are irelevent in this context, and is more efficient as Process::find_process
            if (!cur_p.open()) continue;    // open new process to read data from
        }

        // read new value from address
        if (a != nullptr && b != nullptr)
        {
            // update address with new value
            cur_p.read(e.address, size, ref);
            if (equal ? memcmp(a, ref, size) == 0 : is_between(a, b, ref))
                this->search_buffer.push(e.pid, e.address, size, this->cfg.type(), ref);
        }
        else
        {
            // reread the current value
            cur_p.read(e.address, e.size, ref);
            this->search_buffer.push(e.pid, e.address, e.size, e.type, ref);
        }
    }

    cur_p.close();
    return this->search_buffer.table().size();
}

uint64_t Application::write(uint8_t* x, size_t size)
{
    if (this->search_buffer.table().size() == 0) return 0;

    // write to all saved addresses
    uint64_t count = 0;
    Process cur_p;
    for (Buffer::Element& e : this->search_buffer.table())
    {
        // find new process if old pid is different to the current pid
        if (e.pid != cur_p.pid() || !cur_p.is_valid())
        {
            cur_p.close();                  // close old process
            cur_p.init("", e.pid, 0, 0);    // init current process with stored PID, all other information are irelevent in this context, and is more efficient as Process::find_process
            if (!cur_p.open()) continue;    // open new process to read data from
        }

        // write to address
        uint8_t buff[e.size];
        memset(buff, 0, e.size);
        memcpy(buff, x, (size > e.size) ? e.size : size);
        cur_p.write(e.address, e.size, x);
        ++count;
    }
    cur_p.close();
    return count;
}
