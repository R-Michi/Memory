/**
* @file     process_handler.cpp
* @brief    Implementation of process-handler-class.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#include "process_handler.h"
#include <stdexcept>
#include <vector>

using namespace memory;

ProcessHandler::ProcessHandler(void)
{
    // initialize job object to make processes depend on the current application
    this->job = CreateJobObject(nullptr, nullptr);

    JOBOBJECT_BASIC_LIMIT_INFORMATION limit_info = {};
    limit_info.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
    
    this->extended_limit_info = {};
    this->extended_limit_info.BasicLimitInformation = limit_info;

    if (!SetInformationJobObject(job, JobObjectExtendedLimitInformation, &this->extended_limit_info, sizeof(this->extended_limit_info)))
        throw std::runtime_error("In memory::ProcessHandler::CONSTRUCT: Failed to set up process handler!");
}

ProcessHandler::~ProcessHandler(void)
{
    // terminate all processes and close their handles
    for (const std::pair<pid_t, PROCESS_INFORMATION>& ppi : this->processes)
    {
        TerminateProcess(ppi.second.hProcess, 0);
        WaitForSingleObject(ppi.second.hProcess, INFINITE);
        CloseHandle(ppi.second.hProcess);
    }
    // destroy the job object handle
    CloseHandle(this->job);
}

pid_t ProcessHandler::start_process(const std::string& path, const std::string& args) noexcept
{
    // make a cleanup that all terminated processes are removed from the map
    this->cleanup();

    // start the process and assign it to the job
    std::string _args = args;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    memset(&si, 0, sizeof(si));
    memset(&pi, 0, sizeof(pi));
    si.cb = sizeof(si);

    if (!CreateProcess(path.c_str(), _args.data(), nullptr, nullptr, false, CREATE_NEW_CONSOLE, nullptr, nullptr, &si, &pi))
        return MEMORY_PID_INVALID;

    AssignProcessToJobObject(this->job, pi.hProcess);
    this->processes[pi.dwProcessId] = pi;
    return pi.dwProcessId;
}

bool ProcessHandler::stop_process(pid_t pid) noexcept
{
    // make a cleanup that all terminated processes are removed from the map
    this->cleanup();
    if (this->processes.count(pid) == 0) return false;

    // terminate process and close its handle
    const PROCESS_INFORMATION& pi = this->processes[pid];
    TerminateProcess(pi.hProcess, 0);
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    this->processes.erase(pid);
    return true;
}

bool ProcessHandler::is_process_running(pid_t pid) noexcept
{
    // check if process is still running
    process_t process = OpenProcess(SYNCHRONIZE, false, pid);   // open process
    DWORD ret = WaitForSingleObject(process, 0);                // get return value of process
    CloseHandle(process);                                       // close process
    return (ret == WAIT_TIMEOUT);                               // true if process is still running
}

void ProcessHandler::cleanup(void) noexcept
{
    // remove all processes from the map that have been terminated
    // without a call to ProcessHandler::stop_process

    // get all pids that have been terminated
    std::vector<pid_t> terminated_pids;
    for (const std::pair<pid_t, PROCESS_INFORMATION>& ppi : this->processes)
    {
        if (!is_process_running(ppi.first))
        {
            CloseHandle(ppi.second.hProcess);   // to make sure that the handle is 100% closed even if the process has been terminated
            terminated_pids.push_back(ppi.first);
        }
    }

    // remove them from the map
    for (pid_t pid : terminated_pids)
        this->processes.erase(pid);
}
