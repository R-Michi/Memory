/**
* @file     app.h
* @brief    Main include file for the application.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "../memory/memory.h"
#include "config.h"

namespace memory
{
    namespace app
    {
        class Application : public CommandEngineBase
        {
        private:
            constexpr static char LIVE_MEMORY_FILE_PATH[]       = "live_memory/elements.dat";
            constexpr static char LIVE_MEMORY_PROCESS_PATH[]    = "live_memory/LiveMemory.exe";
            constexpr static char LIVE_MEMORY_PROCESS_NAME[]    = "LiveMemory.exe";
            constexpr static char DUMP_PROCESS_PATH[]           = "hexdump/HexDump.exe";
            constexpr static char DUMP_PROCESS_NAME[]           = "HexDump.exe";

            Config cfg;
            Table search_table, process_table;
            Buffer search_buffer, undo_buffer, redo_buffer;
            Process current_process;
            ProcessHandler process_handler;
            pid_t pid_live_memory, pid_dump, pid_this;

            // utility functions
            /**
            * @brief Calculates automatically the right SI-prefix for bytes. (kB, MB, GB, etc.)
            * @param[in] bytes: number of bytes
            * @param[out] prefix: SI-Prefix (kB, MB, GB, etc.)
            * @return number of kB, MB, GB, etc.
            */
            static double auto_SI(uint64_t bytes, std::string& prefix) noexcept;

            /**
            * @brief Lists all options that are unknown or invalid for a command.
            * @param[in] cmd: command
            * @param[in] in_options: all options of the command
            * @param[out] out_options: all unknown or invalid options
            */
            static void list_unknown_options(const Command& cmd, const std::vector<std::string>& in_options, std::vector<std::string>& out_options) noexcept;

            /**
            * @brief Checks if input is hexadecimal.
            * @param[in] inp: input string that represents the number
            * @return 'true' if number has leading 0x and 'false' if number has no leading 0x
            */
            static bool is_input_hex(const std::string& inp);

            /**
            * @brief Adds time prefix to message.
            * @param[in] msg: message to print
            * @return message to print with time prefix
            */
            static std::string make_msg(std::string msg);

            /**
            * @brief Makes an entry for search table.
            * @param[in] pid: process ID
            * @param[in] address: memory address
            * @param[in] size: size of the type or string
            * @param[in] value: bytes of the value
            * @param[out] entry: final entry, ready to be added to the table
            */
            void make_search_entry(pid_t pid, address_t address, size_t size, uint8_t* value, std::vector<std::string>& entry);

            /**
            * @brief Makes an entry for the process table.
            * @param[in] p: input process
            * @param[out] entry: final entry, ready to be added to the table
            */
            static void make_process_entry(const Process& p, std::vector<std::string>& entry);

            /** @brief Makes an backup for the search buffer. */
            void make_backup(void);

            /**
            * @brief Checks if the reference-value is between two values.
            * @param[in] a: lower limit
            * @param[in] b: upper limit
            * @param[in] ref: reference-value
            * @return 'true' if the check was successful
            */
            bool is_between(uint8_t* a, uint8_t* b, uint8_t* ref);

            /**
            * @brief Scans memory of a process.
            * @param[in] proc: process to scan
            * @param[in] a: lower limit of the value-range to search for
            * @param[in] b: upper limit of the value-range to search for
            * @param[in] size: size of the value or string
            * @param[out] limit: indicator if the buffer ran out of memory while scanning
            * @return number of found matches
            */
            uint64_t scan(Process& proc, uint8_t* a, uint8_t* b, size_t size, bool& limit);

            /**
            * @brief Updates all stored values.
            * @param[in] a: lower limit of the value-range to search for
            * @param[in] b: upper limit of the value-range to search for
            * @param[in] size: size of the value or string
            * @return number of updated values
            * NOTE: If a or b is 'nullptr', all values will only be re-read.
            */
            uint64_t update(uint8_t* a, uint8_t* b, size_t size);

            /**
            * @brief Writes to all stored addresses.
            * @param[in] x: value to write
            * @param[in] size: size of the value or string
            * @return number of written values
            */
            uint64_t write(uint8_t* x, size_t size);

            // commands in separate functions
            void cmd_help(const Command& cmd);
            void cmd_config(const Command& cmd);
            void cmd_list(const Command& cmd);
            void cmd_info(const Command& cmd);
            void cmd_open(const Command& cmd);
            void cmd_close(const Command& cmd);
            void cmd_clear(const Command& cmd);
            void cmd_read_single(const Command& cmd);
            void cmd_read_block(const Command& cmd);
            void cmd_search_exact(const Command& cmd);
            void cmd_search_range(const Command& cmd);
            void cmd_write_all(const Command& cmd);
            void cmd_write_single(const Command& cmd);
            void cmd_write_range(const Command& cmd);
            void cmd_update(const Command& cmd);
            void cmd_update_exact(const Command& cmd);
            void cmd_update_range(const Command& cmd);
            void cmd_undo(const Command& cmd);
            void cmd_redo(const Command& cmd);
            void cmd_show(const Command& cmd);
            void cmd_show_live(const Command& cmd);
            void cmd_dump(const Command& cmd);

        public:
            Application(void);
            virtual ~Application(void);
            virtual bool on_command(const Command& cmd);
        };
    }
}