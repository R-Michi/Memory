/**
* @file     process_handler.h
* @brief    Definitions of process-handler-class.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "types.h"
#include <map>
#include <string>

namespace memory
{
    class ProcessHandler
    {
    private:
        job_t job;
        JOBOBJECT_EXTENDED_LIMIT_INFORMATION extended_limit_info;
        std::map<pid_t, PROCESS_INFORMATION> processes;

        /**
        * @brief Checks if a process is running.
        * @param[in] pid: process id of the process to check
        * @return 'true' if process is running and 'false' if not
        */
        bool is_process_running(pid_t pid) noexcept;

        /**
        * @brief Removes all processes that have not been closed by a call to ProcessHandler::stop_process,
        *   for example by clicking on the close button.
        */
        void cleanup(void) noexcept;

    public:
        ProcessHandler(void);
        virtual ~ProcessHandler(void);

        /**
        * @brief Starts a process.
        * @param[in] path: path to the programm to open
        * @param[in] args: argv arguments (including the program name)
        * @return Process ID of the opened process or 0 if the process was unable to open.
        */
        pid_t start_process(const std::string& path, const std::string& args) noexcept;

        /**
        * @brief Stops a process.
        * @param[in] pid: pid of the process to stop
        * @return 'true' if the process has successfully been terminated and
        *   'false' if the process had already been terminated before.
        */
        bool stop_process(pid_t pid) noexcept;
    };
}
