/**
* @file     command_input.cpp
* @brief    User-input engine of commands.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#include "command.h"
#include <iostream>

using namespace memory;

void CommandEngineBase::get_command(Command& cmd)
{
    std::string str;
    std::cout << ">> ";
    std::getline(std::cin, str);    // read command string from console
    cmd.phrase(str);                // decode command into arguments
}