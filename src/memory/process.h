/**
* @file     process.h
* @brief    Definitions of process-class.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "types.h"
#include <vector>
#include <string>

namespace memory
{
    class Process
    {
    public:
        /**
        * @brief Counts the number of processes running on the system.
        * @param[in] accessable: indicator if only accessable processes should be counted
        * @return Number of processes running on the system.
        */
        static uint32_t count_processes(bool accessable) noexcept;

        /**
        * @brief Enumerates all running processes of the system.
        * @param[in] accessable: indicator if only accessable processes should be enumerated
        * @param[out] infos: returned processes sorted by process ID, NOTE: the processes have not been opened
        * @return 'true' if at least one process has been found and 'false' if no processes have benn found.
        */
        static bool enum_processes(bool accessable, std::vector<Process>& processes);

        /**
        * @brief Finds the process ID of a window.
        * @param[in] win_name: ID of the process
        * @return 'true' if a window was found, 'false' if no window was found.
        */
        static bool find_process(pid_t pid, Process& p) noexcept;

        /**
        * @brief Finds the process ID of a window.
        * @param[in] win_name: name of the window
        * @return 'true' if a process was found, 'false' if no process was found.
        */
        static bool find_process(const std::string& win_name, Process& p) noexcept;

    private:
        HANDLE _proc_handle;
        std::string _proc_name;
        pid_t _pid, _ppid;
        uint32_t _thread_count;

    public:
        Process(void);

        /**
        * @brief Moves the process.
        *        Note, if the source process is valid, it will become invalid after move.
        * @param[in] proc: process to move
        */
        inline Process(Process&& proc) : Process() { this->move(proc); }

        /**
        * @brief Copies the process.
        *        Note, if the source process is valid, the destination will be invalid.
        * @param[in] proc: process to copy
        */
        inline Process(const Process& proc) : Process() { this->copy(proc); }

        virtual ~Process(void);

        /**
        * @brief Initialized the process instance.
        * @param[in] name: name of the process
        * @param[in] pid: ID of this process
        * @param[in] ppid: ID of the parent process
        * @param[in] thc: thread count of this process
        */
        void init(const std::string& name, pid_t pid, pid_t ppid, uint32_t thc) noexcept;

        /**
        * @brief Opens this process.
        * @return 'true' if process could be opened, 'false' if process could not be opened.
        */
        bool open(void) noexcept;

        /** @brief Closes the process. */
        void close(void) noexcept;

        /**
        * @brief Read data from the current open process.
        * @param[in] dst: destination address to read data from
        * @param[in] size: number of bytes to read
        * @param[out] buff: buffer where the read data gets stored
        * @return number of actually read bytes
        */
        size_t read(address_t dst, size_t size, void* buff);

        /**
        * @brief Write data to the current open process.
        * @param[in] dst: destination address to write data to
        * @param[in] size: number of bytes to write
        * @param[in] buff: buffer that contains the data to write
        * @return number of actually written bytes
        */
        size_t write(address_t dst, size_t size, const void* buff);

        /**
        * @brief Queries all memory pages of process.
        * @param[in] begin: begin address to query
        * @param[in] end: end address to query
        * @param[in] flags: query option flags
        * @param[out] modules: info to memory pages
        * @return Number of queried memory pages.
        */
        uint32_t query(address_t begin, address_t end, std::vector<MemoryInfo>& mem_infos);

        /** @return Name of the process. */
        const std::string& name(void) const noexcept { return this->_proc_name; }

        /**  @return ID of the process.*/
        pid_t pid(void) const noexcept { return this->_pid; }

        /** @return ID of the parent process. */
        pid_t parent_pid(void) const noexcept { return this->_ppid; }

        /** @return Number of threads running on this process. */
        uint32_t count_threads(void) const noexcept { return this->_thread_count; }

        /** @return 'true' if the process is valid, 'false' if the process is invalid. */
        bool is_valid(void) const noexcept { return (this->_proc_handle != MEMORY_NULL_HANDLE && this->_proc_handle != MEMORY_INVALID_HANDLE); }

        /**
        * @brief Copies the process.
        *        Note, if the source process is valid, the destination will be invalid.
        * @param[in] proc: process to copy
        * @return moved process
        */
        Process& copy(const Process& proc);

        /**
        * @brief Moves the process.
        *        Note, if the source process is valid, it will become invalid after move.
        * @param[in] proc: process to move
        * @return moved process
        */
        Process& move(Process& proc);

        /**
        * @brief Moves the process.
        *        Note, if the source process is valid, it will become invalid after move.
        * @param[in] proc: process to move
        * @return moved process
        */
        inline Process& operator= (Process&& proc) { return this->move(proc); }

        /**
        * @brief Copies the process.
        *        Note, if the source process is valid, the destination will be invalid.
        * @param[in] proc: process to copy
        * @return moved process
        */
        inline Process& operator= (const Process& proc) { return this->copy(proc); }

        /** @brief Compares if the ID o this process is smaller than the ID of the other process. */
        bool operator< (const Process& proc) const noexcept { return (this->_pid < proc._pid); }
    };
}