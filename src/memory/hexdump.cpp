/**
* @file     hexdump.cpp
* @brief    Implementation of hex-dump-class.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#include "table.h"
#include <iostream>

using namespace memory;

HexDump::HexDump(void) noexcept : HexDump(0)
{
}

HexDump::HexDump(uint32_t lines, uint32_t width)
{
    this->add_column("Address");
    this->add_column("Data");

    this->iter = this->begin();
    this->_width = 0;
    this->resize(lines, width);
}

HexDump::~HexDump(void)
{

}

void HexDump::init_hex(void) noexcept
{
    this->init_hex_string.clear();
    for (uint32_t i = 0; i < this->_width; i++)
    {
        if ((i % 4) == 0 && i > 0)
            this->init_hex_string.push_back(' ');
        this->init_hex_string.insert(this->init_hex_string.size(), "00");
        if (i < (this->_width - 1))
            this->init_hex_string.push_back(' ');
    }
}

void HexDump::encode_hex(const uint8_t* bytes, std::string& str) noexcept
{
    constexpr static char table[] = "0123456789ABCDEF";

    str.clear();
    for (uint32_t i = 0; i < this->_width; i++)
    {
        if ((i % 4) == 0 && i > 0)
            str.push_back(' ');
        str.push_back(table[(bytes[i] >> 4) & 0x0F]);
        str.push_back(table[bytes[i] & 0x0F]);
        if (i < (this->_width - 1))
            str.push_back(' ');
    }
}

void HexDump::encode_addr(uint64_t address, std::string& str) noexcept
{
    constexpr static char table[] = "0123456789ABCDEF";

    str.clear();
    for (int32_t i = sizeof(uint64_t) * 8 - 4; i >= 0; i -= 4)
        str.push_back(table[(address >> i) & 0x0F]);
}

void HexDump::set(const uint8_t* bytes) noexcept
{
    if (this->iter == this->end()) return;

    std::string str;
    this->encode_hex(bytes, str);
    (*this->iter)[1] = str;
}

void HexDump::set(uint64_t address, const uint8_t* bytes) noexcept
{
    if (this->iter == this->end()) return;

    std::string str;
    this->encode_addr(address, str);
    (*this->iter)[0] = str;
    this->encode_hex(bytes, str);
    (*this->iter)[1] = str;
}

void HexDump::scroll_up(void) noexcept
{
    this->get_entries().pop_front();
    this->get_entries().push_back({ NULLPTR_STR, this->init_hex_string });
}

void HexDump::scroll_down(void) noexcept
{
    this->get_entries().pop_back();
    this->get_entries().push_front({ NULLPTR_STR, this->init_hex_string });
}

void HexDump::inc(void) noexcept
{
    if (this->iter != this->end())
        this->iter++;
}

void HexDump::dec(void) noexcept
{
    if (this->iter != this->begin())
        this->iter--;
}

void HexDump::front(void) noexcept
{
    this->iter = this->begin();
}

void HexDump::back(void) noexcept
{
    this->iter = this->end();
}

void HexDump::resize(uint32_t lines, uint32_t width)
{
    if (width != this->_width)
    {
        this->_width = width;
        this->init_hex();
        for (table_iterator iter = this->begin(); iter != this->end(); iter++)
        {
            (*iter)[0] = NULLPTR_STR;
            (*iter)[1] = this->init_hex_string;
        }
    }

    if (lines < this->entry_count())
    {
        while (this->entry_count() > lines)
            this->get_entries().pop_back();
    }
    else if (lines > this->entry_count())
    {
        while (this->entry_count() < lines)
            this->get_entries().push_back({ NULLPTR_STR, this->init_hex_string });
    }
}

uint32_t HexDump::line_count(void) const noexcept
{
    return this->entry_count();
}

uint32_t HexDump::width(void) const noexcept
{
    return this->_width;
}

void HexDump::print(void) const noexcept
{
    uint32_t max_width[this->col_count()];
    this->get_max_width(max_width);

    std::string sep_line, line;
    this->get_separator_line(max_width, sep_line);
    std::cout << sep_line << std::endl;

    for (const_table_iterator iter = this->begin(); iter != this->end(); iter++)
    {
        this->get_line(max_width, *iter, line);
        std::cout << line << std::endl;
    }
    std::cout << sep_line << std::endl;
}