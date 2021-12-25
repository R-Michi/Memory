/**
* @file     command_decoder.cpp
* @brief    Implemenation of the command decoder itself.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#include "command.h"

using namespace memory;


Command& Command::copy(const Command& cmd) noexcept
{
    this->_name     = cmd._name;
    this->_options  = cmd._options;
    this->_args     = cmd._args;
    return *this;
}

Command& Command::move(Command& cmd) noexcept
{
    this->_name     = cmd._name;
    this->_options  = cmd._options;
    this->_args     = cmd._args;

    cmd._name.clear();
    cmd._options.clear();
    cmd._args.clear();
    return *this;
}

void Command::phrase(const std::string& str) noexcept
{
    std::string token;
    bool b = false, name_set = false;

    this->_args.clear();
    this->_options.clear();
    this->_name.clear();

    for (size_t i = 0; i <= str.size(); i++)
    {
        char c = (i < str.size()) ? str[i] : ' ';
        if (c == ' ' && token.size() > 0 && !b)
        {
            if (this->_name.empty())
                this->_name = token;
            // option must begin with '-' and must not be a nagative number
            else if (token[0] == '-' && !(token.size() >= 2 && (token[1] >= '0' && token[1] <= '9')))
                this->_options.push_back(token.substr(1));
            else
                this->_args.push_back(token);

            token.clear();
        }
        else if (c != ' ' || b)
        {
            if (c == '\"')
                b = !b;
            else
                token.push_back(c);
        }
    }
}
