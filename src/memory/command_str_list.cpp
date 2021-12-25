/**
* @file     command_str_list.cpp
* @brief    Implemenation of argument and option list.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#include "command.h"

using namespace memory;

CmdStringList& CmdStringList::copy(const CmdStringList& other)
{
    this->strings = other.strings;
    return *this;
}

CmdStringList& CmdStringList::move(CmdStringList& other)
{
    this->strings = other.strings;
    other.strings.clear();
    return *this;
}



uint32_t CmdOpionList::find(const std::string& op, uint32_t start) const noexcept
{
    uint32_t p = NPOS;
    bool found = false;
    for (uint32_t i = start; i < this->size() && !found; i++)
    {
        if (this->at(i) == op)
        {
            p = i;
            found = true;
        }
    }
    return p;
}

uint32_t CmdOpionList::find_any(const std::vector<std::string>& ops, uint32_t start) const noexcept
{
    uint32_t p = NPOS;
    bool found = false;
    for (size_t i = 0; i < ops.size() && !found; i++)
        found = ((p = this->find(ops[i], start)) != NPOS);
    return p;
}

bool CmdOpionList::find_all(const std::vector<std::string>& ops, uint32_t start, std::vector<uint32_t>* indices) const noexcept
{
    if (indices != nullptr)
        indices->clear();

    uint32_t p;
    bool found = true;
    for (size_t i = 0; i < ops.size() && found; i++)
    {
        found = ((p = this->find(ops[i], start)) != NPOS);
        if (found && indices != nullptr)
            indices->push_back(p);
    }

    return found;
}

bool CmdOpionList::find_not(const std::vector<std::string>& ops, uint32_t start, std::vector<uint32_t>* indices) const noexcept
{
    if(indices != nullptr)
        indices->clear();

    bool any = false, found = false;
    for (uint32_t i = start; i < this->size(); i++)
    {
        for (size_t j = 0; j < ops.size(); j++)
        {
            if (ops[j] == this->at(i))
                found = true;
        }

        if (!found)
        {
            any = true;
            if(indices != nullptr)
                indices->push_back(i);
        }
        found = false;
    }

    return any;
}
