/**
* @file     process.cpp
* @brief    Implemenation of process-class.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#include "process.h"
#include <TlHelp32.h>

using namespace memory;

uint32_t Process::count_processes(bool accessable) noexcept
{
    HANDLE snap_proc = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (!snap_proc) return 0;

    uint32_t cnt = 0;
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    // get first process
    if (Process32First(snap_proc, &entry))
    {
        // get all other processes and count them
        do
        {
            // count only accessable processes, otherwise count all processes
            if (accessable)
            {
                HANDLE hp = OpenProcess(PROCESS_ALL_ACCESS, false, entry.th32ProcessID);
                if (hp != MEMORY_NULL_HANDLE && hp != MEMORY_INVALID_HANDLE)
                    ++cnt;
            }
            else
                ++cnt;
        } while (Process32Next(snap_proc, &entry));
    }
    return cnt;
}

bool Process::enum_processes(bool accessable, std::vector<Process>& processes)
{
    HANDLE snap_proc = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (!snap_proc) return false;

    Process p;
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    processes.clear();

    // get first process
    if (Process32First(snap_proc, &entry))
    {
        // get all other processes
        do
        {
            // list only accessable processes, otherwise list all processes
            if (accessable)
            {
                HANDLE hp = OpenProcess(PROCESS_ALL_ACCESS, false, entry.th32ProcessID);
                if (hp != MEMORY_NULL_HANDLE && hp != MEMORY_INVALID_HANDLE)
                {
                    p.init(entry.szExeFile, entry.th32ProcessID, entry.th32ParentProcessID, entry.cntThreads);
                    processes.push_back(std::move(p));
                }
            }
            else
            {
                p.init(entry.szExeFile, entry.th32ProcessID, entry.th32ParentProcessID, entry.cntThreads);
                processes.push_back(std::move(p));
            }
        } while (Process32Next(snap_proc, &entry));
    }

    return true;
}

bool Process::find_process(pid_t pid, Process& p) noexcept
{
    std::vector<Process> processes;
    enum_processes(false, processes);
    for (Process& cp : processes)
    {
        if (cp.pid() == pid)
        {
            p = std::move(cp);
            return true;
        }
    }
    return false;
}

bool Process::find_process(const std::string& win_name, Process& p) noexcept
{
    HWND window = FindWindow(nullptr, win_name.c_str());
    if (window == MEMORY_NULL_HANDLE || window == MEMORY_INVALID_HANDLE) return false;

    pid_t pid;
    GetWindowThreadProcessId(window, &pid);
    find_process(pid, p);
    return true;
}



Process::Process(void)
{
    this->_proc_handle = MEMORY_NULL_HANDLE;
    this->_pid = this->_ppid = this->_thread_count = 0;
}

Process::~Process(void)
{
    this->close();
}

void Process::init(const std::string& name, pid_t pid, pid_t ppid, uint32_t thc) noexcept
{
    this->_proc_name = name;
    this->_pid = pid;
    this->_ppid = ppid;
    this->_thread_count = thc;
    this->_proc_handle = MEMORY_NULL_HANDLE;
}

bool Process::open(void) noexcept
{
    this->_proc_handle = OpenProcess(PROCESS_ALL_ACCESS, false, this->_pid);
    if (!this->is_valid())
    {
        this->_proc_handle = MEMORY_NULL_HANDLE;
        return false;
    }
    return true;
}

void Process::close(void) noexcept
{
    if (this->is_valid())
    {
        CloseHandle(this->_proc_handle);
        this->_proc_handle = MEMORY_NULL_HANDLE;
    }
}

size_t Process::read(address_t dst, size_t size, void* buff)
{
    size_t rd_bytes;
    ReadProcessMemory(this->_proc_handle, reinterpret_cast<const void*>(dst), buff, size, &rd_bytes);
    return rd_bytes;
}

size_t Process::write(address_t dst, size_t size, const void* buff)
{
    size_t wr_bytes;
    WriteProcessMemory(this->_proc_handle, reinterpret_cast<void*>(dst), buff, size, &wr_bytes);
    return wr_bytes;
}

uint32_t Process::query(address_t begin, address_t end, std::vector<MemoryInfo>& mem_infos)
{
    if (!this->is_valid()) return 0;

    address_t cur = begin;
    bool finish = false;
    MEMORY_BASIC_INFORMATION info;

    mem_infos.clear();
    while (cur < end && !finish)
    {
        finish = (VirtualQueryEx(this->_proc_handle, reinterpret_cast<const void*>(cur), &info, sizeof(MEMORY_BASIC_INFORMATION)) == 0);
        // memory must be committed, which means it is in use and must have access rights (the no-access bit must not be set)
        if (!finish && (info.State & MEM_COMMIT) != 0 && (info.Protect & PAGE_NOACCESS) == 0)
        {
            MemoryInfo mem_info;
            mem_info.base = reinterpret_cast<address_t>(info.BaseAddress);
            mem_info.size = info.RegionSize;
            mem_infos.push_back(mem_info);
        }
        // set current address to the next memory page
        cur += info.RegionSize;
    }

    return mem_infos.size();
}

Process& Process::move(Process& proc)
{
    // close this process if it is open
    this->close();

    // copy data
    this->_proc_handle = proc._proc_handle;
    this->_proc_name = proc._proc_name;
    this->_pid = proc._pid;
    this->_ppid = proc._ppid;
    this->_thread_count = proc._thread_count;

    // ownership of the handle is transfered to this process
    proc._proc_handle = MEMORY_NULL_HANDLE;
    return *this;
}

Process& Process::copy(const Process& proc)
{
    // close this process if it is open
    this->close();

    // copy data
    this->_proc_name = proc._proc_name;
    this->_pid = proc._pid;
    this->_ppid = proc._ppid;
    this->_thread_count = proc._thread_count;

    // ownership of the open process is not transfered and still belongs to the other process
    return *this;
}
