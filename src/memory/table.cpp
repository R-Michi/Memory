/**
* @file     table.cpp
* @brief    Implemenation of Table-class.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#include "table.h"
#include "utility.h"
#include <iostream>
#include <fstream>

using namespace memory;

void Table::add_column(const std::string& title) noexcept
{
    this->cols.push_back(title);
}

void Table::add(const std::vector<std::string>& entry) noexcept
{
    std::vector<std::string> vec = entry;
    vec.resize(this->cols.size());
    this->entries.push_back(vec);
}

void Table::remove(table_iterator iter) noexcept
{
    this->entries.erase(iter);
}

void Table::remove(const_table_iterator iter) noexcept
{
    this->entries.erase(iter);
}

void Table::get_max_width(uint32_t* width) const noexcept
{
    for (size_t i = 0; i < this->cols.size(); i++)
        width[i] = this->cols[i].size();

    for (auto iter = this->entries.begin(); iter != this->entries.end(); iter++)
    {
        for (size_t i = 0; i < iter->size(); i++)
        {
            if ((*iter)[i].size() > width[i])
                width[i] = (*iter)[i].size();
        }
    }
}

void Table::get_line(const uint32_t* width, const std::vector<std::string>& entry, std::string& line) const noexcept
{
    line.clear();
    for (size_t col = 0; col < this->cols.size(); col++)
    {
        line.push_back(VERTICAL_CHAR);
        line.push_back(' ');
        line += entry[col]; // seems to be faster than string::insert
        line.insert(line.size(), width[col] - entry[col].size(), ' ');
        line.push_back(' ');
    }
    line.push_back(VERTICAL_CHAR);
}

void Table::get_separator_line(const uint32_t* width, std::string& line) const noexcept
{
    line.clear();
    for (size_t col = 0; col < this->cols.size(); col++)
    {
        line.push_back(CORNER_CHAR);
        line.insert(line.size(), width[col] + 2, HORIZONTAL_CHAR);
    }
    line.push_back(CORNER_CHAR);
}

void Table::clear(void) noexcept
{
    this->cols.clear();
    this->entries.clear();
}

void Table::clear_entries(void) noexcept
{
    this->entries.clear();
}

void Table::print(void) const noexcept
{
    uint32_t width[this->cols.size()];
    this->get_max_width(width);

    std::string sep_line, line;
    this->get_separator_line(width, sep_line);
    this->get_line(width, this->cols, line);

    std::cout << sep_line << std::endl;
    std::cout << line << std::endl;
    std::cout << sep_line << std::endl;

    for (const std::vector<std::string>& entry : this->entries)
    {
        this->get_line(width, entry, line);
        std::cout << line << std::endl;
    }

    std::cout << sep_line << std::endl;
}

bool Table::print_to_file(const std::string& path) const noexcept
{
    std::fstream file(path, std::ios::out | std::ios::trunc | std::ios::binary);
    if (!file)
        return false;

    // syntax: <column count 4B> { <sizeof col 0 8B> <string of col 0> <sizeof col 1 8B> <string of col 1> <sizeof col 2 8B> <string of col 2> ...}
    // all values get stored in network byteorder

    /* print columns */
    // print column count
    uint32_t col_count = utility::htons<uint32_t>(static_cast<uint32_t>(this->col_count()));
    file.write(reinterpret_cast<const char*>(&col_count), sizeof(uint32_t));

    // print columns
    for (const std::string& col : this->cols)
    {
        // pint column string size
        uint64_t size = utility::htons<uint64_t>(static_cast<uint64_t>(col.size()));
        file.write(reinterpret_cast<const char*>(&size), sizeof(uint64_t));

        // print column string
        file.write(col.data(), col.size());
    }

    // syntax: <entry count 4B> {entry1:{<sizeof col 0 8B> <string of col 0>...} entry2:{<sizeof col 0 8B> <string of col 0>...}...}
    // all values get stored in network byteorder

    /* print entries */
    // print entry count
    uint32_t entry_count = utility::htons<uint32_t>(static_cast<uint32_t>(this->entry_count()));
    file.write(reinterpret_cast<const char*>(&entry_count), sizeof(uint32_t));

    // print entries
    for (const std::vector<std::string>& entry: this->entries)
    {
        // print columns of entry
        for (const std::string& str : entry)
        {
            // pint column string size
            uint64_t size = utility::htons<uint64_t>(static_cast<uint64_t>(str.size()));
            file.write(reinterpret_cast<const char*>(&size), sizeof(uint64_t));

            // print string
            file.write(str.data(), str.size());
        }
    }

    file.close();
    return true;
}

bool Table::read_from_file(const std::string& path) noexcept
{
    this->clear();

    std::fstream file(path, std::ios::in | std::ios::binary);
    if (!file)
        return false;

    // syntax: <column count 4B> { <sizeof col 0 8B> <string of col 0> <sizeof col 1 8B> <string of col 1> <sizeof col 2 8B> <string of col 2> ...}
    // all values get stored in network byteorder

    /* read columns */
    // read column count
    uint32_t col_count;
    file.read(reinterpret_cast<char*>(&col_count), sizeof(uint32_t));
    col_count = utility::htons<uint32_t>(col_count);

    // read columns
    for (uint32_t i = 0; i < col_count; i++)
    {
        // read string size
        uint64_t size;
        file.read(reinterpret_cast<char*>(&size), sizeof(uint64_t));
        size = utility::htons<uint64_t>(size);

        // read string
        std::string str;
        str.resize(size);
        file.read(str.data(), str.size());

        this->add_column(str);
    }

    // syntax: <entry count 4B> {entry1:{<sizeof col 0 8B> <string of col 0>...} entry2:{<sizeof col 0 8B> <string of col 0>...}...}
    // all values get stored in network byteorder

    /* read entries */
    // read entry count
    uint32_t entry_count;
    file.read(reinterpret_cast<char*>(&entry_count), sizeof(uint32_t));
    entry_count = utility::htons<uint32_t>(entry_count);

    // read entries
    for (uint32_t i = 0; i < entry_count; i++)
    {
        std::vector<std::string> entry;

        // read columns of entry
        for (uint32_t j = 0; j < col_count; j++)
        {
            // read size of string
            // read string size
            uint64_t size;
            file.read(reinterpret_cast<char*>(&size), sizeof(uint64_t));
            size = utility::htons<uint64_t>(size);

            // read string
            std::string str;
            str.resize(size);
            file.read(str.data(), str.size());

            entry.push_back(str);
        }
        this->add(entry);
    }

    return true;
}

uint32_t Table::entry_count(void) const noexcept
{
    return this->entries.size();
}

uint32_t Table::col_count(void) const noexcept
{
    return this->cols.size();
}

Table::table_iterator Table::begin(void) noexcept
{
    return this->entries.begin();
}

Table::table_iterator Table::end(void) noexcept
{
    return this->entries.end();
}

Table::const_table_iterator Table::begin(void) const noexcept
{
    return this->entries.begin();
}


Table::const_table_iterator Table::end(void) const noexcept
{
    return this->entries.end();
}

std::list<std::vector<std::string>>& Table::get_entries(void) noexcept
{
    return this->entries;
}
