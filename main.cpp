/**
* @file     main.cpp
* @brief    Main function of memory application.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#include "src/application/app.h"
#include <iostream>

int main()
{
    try
    {
        memory::app::Application app;
        memory::Command cmd;
        bool running = true;
        while (running)
        {
            app.get_command(cmd);
            running = app.on_command(cmd);
        }
    }
    catch (std::exception& e)
    {
        std::cout << "Error occured: " << e.what() << std::endl;
    }
    return 0;
}
