/**
* @file     app_msg.h
* @brief    All messages for the application.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "../memory/types.h"
#include <sstream>

namespace memory
{
    namespace app
    {
        // messages for command help
        inline std::string msg_help_syntax(void)
        {
            return "Syntax: help [<command>]";
        }
        inline std::string msg_help_general(void)
        {
            return  "\n-------------------------------------------------- Aviable Commands --------------------------------------------------\n"
                    "exit                   Exits the program.\n"
                    "help                   Shows help messages for commands, type \"help <command>\" for more detailed command information.\n"
                    "config                 Manages application settings.\n"
                    "list                   Lits all running processes.\n"
                    "info                   Prints information about the application.\n"
                    "open                   Opens a process ID or finds a window by name.\n"
                    "close                  Closes current running secondary threads and processes, or closes the current open process.\n"
                    "cls or clear           Cleares the screen.\n"
                    "read_single or rs      Reads data from a single memory address.\n"
                    "read_block or rb       Reads data from a range of memory addresses.\n"
                    "search_exact or se     Searches for an exact value in memory.\n"
                    "search_range or sr     Searches for a range of values in memory.\n"
                    "write_all or wa        Writes to all addresses that are currently stored.\n"
                    "write_single or ws     Writes to a single memory address.\n"
                    "write_range or wr      Writes to a range of memory addresses.\n"
                    "upate                  Updates the currently stored values.\n"
                    "upate_exact or ue      Searches for an exact new value in the currently stored values.\n"
                    "upate_range or ur      Searches for an range of values in the currently stored values.\n"
                    "undo                   Undoes the current read or search operation.\n"
                    "redo                   Redoes the current read or search operation.\n"
                    "show                   Showes the currently read addresses and values.\n"
                    "show_live or sl        Showes the currently read addresses and values with live update.\n"
                    "dump                   Makes a memory dump.\n"
                    "save                   Saves the last read/updated addresses and values to a file.\n\n";
        }
        inline std::string msg_help_exit(void)
        {
            return  "\n--------------------------------------------------- Command: exit ---------------------------------------------------\n"
                    "Command: exit\n"
                    "Syntax: exit\n"
                    "Description: exits the program\n\n";
        }
        inline std::string msg_help_help(void)
        {
            return  "\n--------------------------------------------------- Command: help ---------------------------------------------------\n"
                    "Command: help\n"
                    "Syntax: help [<command>]\n"
                    "Description: shows help information for commands\n"
                    "Arguments:\n"
                    "   - NO ARGUMENT                               shows general information about all commands\n"
                    "   - <command>                                 shows more detailed information about one specific command\n\n";
        }
        inline std::string msg_help_config(void)
        {
            return  "\n-------------------------------------------------- Command: config --------------------------------------------------\n"
                    "Command: config\n"
                    "Syntax: config [<setting>] [<new value>]\n"
                    "Description: manages application settings\n"
                    "Arguments:\n"
                    "   - NO ARGUMENT                               shows all settings\n"
                    "   - <setting>                                 shows the curret setting\n"
                    "   - <new value>                               new value for setting\n"
                    "Options:\n"
                    "   - --reset                                   resets all settings to default values\n"
                    "                                               or only one setting to its default value\n"
                    "Aviable settings:\n"
                    "   - type              STRING                  type setting\n"
                    "   - aligment          2B unsigned DECIMAL     memory aligment\n"
                    "   - start_address     8B HEXADECIMAL          start address of searching\n"
                    "   - end_address       8B HEXADECIMAL          end address of searching\n"
                    "   - search_split_size 8B unsigned DECIMAL     how large a memory block can get before it is split\n"
                    "   - search_limit_size 8B unsigned DECIMAL     how much memory (in bytes) is allowed to be stored when searching\n"
                    "Aviable types:\n"
                    "   - int8                                      reads signed 1 byte values from memory\n"
                    "   - uint8                                     reads unisgned 1 byte values from memory\n"
                    "   - int16                                     reads signed 2 byte values from memory\n"
                    "   - uint16                                    reads unsigned 2 byte values from memory\n"
                    "   - int32                                     reads signed 4 byte values from memory\n"
                    "   - uint32                                    reads unsigned 4 byte values from memory\n"
                    "   - int64                                     reads signed 8 byte values from memory\n"
                    "   - uint64                                    reads unsigned 8 byte values from memory\n"
                    "   - float                                     reads 4 byte floating point values(IEEE 754 standard)\n"
                    "   - double                                    reads 8 byte floating point values(IEEE 754 standard)\n"
                    "   - string                                    reads strings from memory\n";
        }
        inline std::string msg_help_list(void)
        {
            return  "\n---------------------------------------------------- Command: list --------------------------------------------------\n"
                    "Command: list\n"
                    "Syntax: list\n"
                    "Description: lits all running processes\n"
                    "Options:\n"
                    "   - NO OPTION                                 lists all running processes\n"
                    "   - -a or --access                            lists only accessable processes\n\n";

        }
        inline std::string msg_help_info(void)
        {
            return  "\n---------------------------------------------------- Command: info --------------------------------------------------\n"
                    "Command: info\n"
                    "Syntax: info\n"
                    "Description: prints information about the application\n\n";
        }
        inline std::string msg_help_open(void)
        {
            return  "\n--------------------------------------------------- Command: open ---------------------------------------------------\n"
                    "Command: open\n"
                    "Syntax: open <pid>|<window name>\n"
                    "Description: opens a process ID or finds a window by name\n"
                    "Arguments:\n"
                    "   - <pid>             4B unsigned DECIMAL     ID of the process that should be opened\n"
                    "   - <window name>     STRING                  name of the window, which process should be opened (STRING)\n"
                    "Options:\n"
                    "   - NO OPTION                                 if no option is set, a process is opened by its process ID\n"
                    "   - -w or --window                            findes a window by its name and opens its process\n\n";
        }
        inline std::string msg_help_close(void)
        {
            return  "\n--------------------------------------------------- Command: close ---------------------------------------------------\n"
                    "Command: close\n"
                    "Syntax: close\n"
                    "Descriptions: closes current running secondary threads and processes, or closes the current open process\n"
                    "Options:\n"
                    "   - NO OPTION:            closes everything that has been opened\n"
                    "   - -p or --process       only closes the current open process\n"
                    "   - -l or --live          only closes live memory showcase\n"
                    "   - -d or --dump          only closes hex dump\n\n";
        }
        inline std::string msg_help_clear(void)
        {
            return  "\n----------------------------------------------- Command: clear or cls -----------------------------------------------\n"
                    "Command: cls or clear\n"
                    "Syntax: cls | clear\n"
                    "Description: cleares the screen\n\n";
        }
        inline std::string msg_help_rs(void)
        {
            return  "\n--------------------------------------------- Command: read_single or rs ---------------------------------------------\n"
                    "Command: read_single or rs\n"
                    "Syntax: read_single | rs <address> [<str_size>]\n"
                    "Description: reads data from a single memory address\n"
                    "Arguments:\n"
                    "   - <address>         8B HEXADECIMAL          address where data should be read\n"
                    "   - <str_size>        4B unsigned DECIMAL     optional parameter, if the current data-type is a string,\n"
                    "                                               it defines the size of the string to read\n\n";
        }
        inline std::string msg_help_rb(void)
        {
            return  "\n--------------------------------------------- Command: read_block or rb ---------------------------------------------\n"
                    "Command: read_block or rb\n"
                    "Syntax: read_block | rb <start> <range> [<str_size>]\n"
                    "Description: reads data from a range of memory addresses\n"
                    "Arguments:\n"
                    "   - <start>           8B HEXADECIMAL          start address where data should be read\n"
                    "   - <range>           8B HEXADECIMAL          number of bytes that should be read\n"
                    "   - <str_size>        4B unsigned DECIMAL     optional parameter, if the current data-type is a string,\n"
                    "                                               it defines the size of the string to read\n\n";
        }
        inline std::string msg_help_se(void)
        {
            return  "\n-------------------------------------------- Command: search_exact or se --------------------------------------------\n"
                    "Command: search_exact or se\n"
                    "Syntax: search_exact | se <value>\n"
                    "Description: searches for an exact value in memory\n"
                    "Arguments:\n"
                    "    - <value>           set data - type        exact value to search for\n"
                    "Options:\n"
                    "    - -a or --all                              searches in all accessable processes\n\n";
        }
        inline std::string msg_help_sr(void)
        {
            return  "\n-------------------------------------------- Command: search_range or sr --------------------------------------------\n"
                    "Command: search_range or sr\n"
                    "Syntax: search_range | sr <begin value> <end value>\n"
                    "Description: searches for a range of values in memory\n"
                    "Arguments:\n"
                    "   - <begin value>     set data - type         lower end of the value range to search for\n"
                    "   - <end value>       set data - type         upper end of the value range to search for\n"
                    "Options:\n"
                    "   - -a or --all                                searches in all accessable processes\n\n";
        }
        inline std::string msg_help_wa(void)
        {
            return  "\n---------------------------------------------- Command: write_all or wa ----------------------------------------------\n"
                    "Command: write_all or wa\n"
                    "Syntax: write_all | wa <value>\n"
                    "Description: writes to all addresses that are currently stored\n"
                    "Arguments:\n"
                    "   - <value>           set data-type           value that should be written\n\n";

        }
        inline std::string msg_help_ws(void)
        {
            return  "\n-------------------------------------------- Command: write_single or ws --------------------------------------------\n"
                    "Command: write_single or ws\n"
                    "Syntax: write_single | ws <address> <value>\n"
                    "Description: writes to a single memory address\n"
                    "Arguments:\n"
                    "   - <address>         8B HEXADECIMAL          address where data should be written to\n"
                    "   - <value>           set data-type           value that should be written\n\n";
        }
        inline std::string msg_help_wr(void)
        {
            return  "\n--------------------------------------------- Command: write_range or wr ---------------------------------------------\n"
                    "Command: write_range or wr\n"
                    "Syntax: write_range | wr <begin address> <range> <value>\n"
                    "Description: writes to a range of memory addresses\n"
                    "Arguments:\n"
                    "   - <start>           8B HEXADECIMAL          start address where data should be written to\n"
                    "   - <range>           8B HEXADECIMAL          number of bytes that should be written\n"
                    "   - <value>           set data-type           value that should be written\n\n";
        }
        inline std::string msg_help_update(void)
        {
            return  "\n-------------------------------------------------- Command: update --------------------------------------------------\n"
                    "Command: update\n"
                    "Syntax: update\n"
                    "Description: updates the currently stored values\n\n";
        }
        inline std::string msg_help_update_exact(void)
        {
            return  "\n-------------------------------------------- Command: update_exact or ue --------------------------------------------\n"
                    "Command: update_exact or ue\n"
                    "Syntax: update_exact | ue <value>\n"
                    "Description: searches for an exact new value in the currently stored values\n"
                    "Arguments:\n"
                    "   - <value>           set data - type           new exact value to search for\n\n";
        }
        inline std::string msg_help_update_range(void)
        {
            return  "\n-------------------------------------------- Command: update_range or ur --------------------------------------------\n"
                    "Command: update_range or ur\n"
                    "Syntax: update_range | ur <begin value> <end value>\n"
                    "Description: updates the currently stored values or searches for a new value\n"
                    "Arguments:\n"
                    "   - <begin value>     set data - type           lower end of the value range to search for\n"
                    "   - <end value>       set data - type           upper end of the value range to search for\n\n";
        }
        inline std::string msg_help_undo(void)
        {
            return  "\n--------------------------------------------------- Command: undo ---------------------------------------------------\n"
                    "Command: undo\n"
                    "Syntax: undo\n"
                    "Description: undoes the current read or search operation\n\n";
        }
        inline std::string msg_help_redo(void)
        {
            return  "\n--------------------------------------------------- Command: redo ---------------------------------------------------\n"
                    "Command: redo\n"
                    "Syntax: redo\n"
                    "Description: redoes the current read or search operation\n\n";

        }
        inline std::string msg_help_show(void)
        {
            return  "\n--------------------------------------------------- Command: show ---------------------------------------------------\n"
                    "Command: show\n"
                    "Syntax: show <start_entry> <amount>\n"
                    "Description: showes the currently read addresses and values\n"
                    "Arguments:\n"
                    "   - <start_entry>     4B unsigned DECIMAL     number of the start entry\n"
                    "   - <amount>          4B unsigned DECIMAL     amount of entries that should be shown\n\n";
        }
        inline std::string msg_help_sl(void)
        {
            return  "\n------------------------------------------------ Command: show or sl ------------------------------------------------\n"
                    "Command: show_live or sl\n"
                    "Syntax: show_live | sl <start entry> <amount> <update speed>\n"
                    "Description: showes the currently read addresses and values with live update\n"
                    "Arguments:\n"
                    "   - <start_entry>     4B unsigned DECIMAL     ID of the start entry\n"
                    "   - <amount>          4B unsigned DECIMAL     amount of entries that should be shown\n"
                    "   - <update speed>    4B unsigned DECIMAL     update speed in milliseconds\n\n";
        }
        inline std::string msg_help_dump(void)
        {
            return  "\n--------------------------------------------------- Command: dump ---------------------------------------------------\n"
                    "Command: dump\n"
                    "Syntax: dump <begin address> <range> <width> <update speed>\n"
                    "Decription: makes a memory dump\n"
                    "Arguments:\n"
                    "   - <begin address>       8B HEXADECIMAL          start address of the memory dump\n"
                    "   - <range>               8B HEXADECIMAL          number of bytes that should be dumped\n"
                    "   - <width>               4B HEXADECIMAL          number of bytes that are contained by one line of the memory dump\n"
                    "   - <update speed>        4B unsigned DECIMAL     update speed in milliseconds\n\n";
        }
        inline std::string msg_help_save(void)
        {
            return  "\n--------------------------------------------------- Command: save ---------------------------------------------------\n"
                    "Command: save\n"
                    "Syntax: save <start entry> <amount> <file name>\n"
                    "Description: saves the last read/updated addresses and values to a file\n"
                    "   - <start_entry>     4B unsigned DECIMAL     ID of the start entry\n"
                    "   - <amount>          4B unsigned DECIMAL     number of entries that shound be saved\n"
                    "   - <file name>       STRING                  name of the file where the entries are saved\n"
                    "Options:\n"
                    "   - NO OPTION:                                saves the entries in plane text, where each column is separated by comma ','\n"
                    "   - -b or --binary                            saves the entries in binary\n"
                    "   - -csv                                      saves the entries in csv format (this option uses an automatic file-type \".csv\")\n\n";
        }
        inline std::string msg_help_invalid(const std::string& cmd)
        {
            std::stringstream ss;
            ss << "Unknown command: \"" << cmd << "\"";
            return ss.str();
        }

        // messages for command config
        inline std::string msg_config_syntax(void)
        {
            return "Syntax: config [<setting>] [<new value>]";
        }
        inline std::string msg_config_save_success(void)
        {
            return "Config saved.";
        }
        inline std::string msg_config_save_failure(void)
        {
            return "Failed to save config.";
        }
        inline std::string msg_config_reset(void)
        {
            return "Resetted all options to it's default values.";
        }
        inline std::string msg_config_reset_type(type_t type)
        {
            std::string str1, str2;
            utility::strtype(type, str1);
            utility::strtype_ext(type, str2);

            std::stringstream ss;
            ss << "Resetted type to: " << str1 << " (" << str2 << ")";
            return ss.str();
        }
        inline std::string msg_config_reset_alignment(uint16_t alignment)
        {
            std::stringstream ss;
            ss << "Resetted alignment to: " << alignment << " bytes";
            return ss.str();
        }
        inline std::string msg_config_reset_sa(address_t addr)
        {
            std::stringstream ss;
            ss << std::hex << "Resetted start address to: " << addr;
            return ss.str();
        }
        inline std::string msg_config_reset_ea(address_t addr)
        {
            std::stringstream ss;
            ss << std::hex << "Resetted end address to: " << addr;
            return ss.str();
        }
        inline std::string msg_config_reset_sss(size_t size)
        {
            std::stringstream ss;
            ss << "Resetted search split size to: " << size << " bytes";
            return ss.str();
        }
        inline std::string msg_config_reset_sls(size_t size)
        {
            std::stringstream ss;
            ss << "Resetted search limit size to: " << size << " bytes";
            return ss.str();
        }
        inline std::string msg_config_print_all(const Config& cfg)
        {
            std::string str1, str2;
            utility::strtype(cfg.type(), str1);
            utility::strtype_ext(cfg.type(), str2);

            std::stringstream ss;
            ss << "\nSETTINGS:\n";
            ss << "Type: "                  << str1 << " (" << str2 << ")\n";
            ss << "Sizeof type: "           << std::dec << cfg.type_size() << " bytes\n";
            ss << "Alignment: "             << std::dec << cfg.alignment() << " bytes\n";
            ss << "Search Start address: "  << std::hex << cfg.start_address() << "\n";
            ss << "Search End address: "    << std::hex << cfg.end_address() << "\n";
            ss << "Search split size: "     << std::dec << cfg.search_split_size() << " bytes\n";
            ss << "Search limit size: "     << std::dec << cfg.search_limit_size() << " bytes\n";
            return ss.str();
        }
        inline std::string msg_config_print_type(type_t type)
        {
            std::string str1, str2;
            utility::strtype(type, str1);
            utility::strtype_ext(type, str2);

            std::stringstream ss;
            ss << "Current type is: " << str1 << " (" << str2 << ")";
            return ss.str();
        }
        inline std::string msg_config_print_alignment(uint16_t alignment)
        {
            std::stringstream ss;
            ss << "Current alignment is: " << alignment << " bytes";
            return ss.str();
        }
        inline std::string msg_config_print_sa(address_t addr)
        {
            std::stringstream ss;
            ss << std::hex << "Current start address is: " << addr;
            return ss.str();
        }
        inline std::string msg_config_print_ea(address_t addr)
        {
            std::stringstream ss;
            ss << std::hex << "Current end address is: " << addr;
            return ss.str();
        }
        inline std::string msg_config_print_sss(size_t size)
        {
            std::stringstream ss;
            ss << "Current search split size is: " << size << " bytes";
            return ss.str();
        }
        inline std::string msg_config_print_sls(size_t size)
        {
            std::stringstream ss;
            ss << "Current search limit size is: " << size << " bytes";
            return ss.str();
        }
        inline std::string msg_config_change_type(type_t old_type, type_t new_type)
        {
            std::string str1, str2, str3, str4;
            utility::strtype(old_type, str1);
            utility::strtype_ext(old_type, str2);
            utility::strtype(new_type, str3);
            utility::strtype_ext(new_type, str4);

            std::stringstream ss;
            ss << "Changed type from " << str1 << " (" << str2 << ") to " << str3 << " (" << str4 << ")";
            return ss.str();
        }
        inline std::string msg_config_change_alignment(uint16_t old_alignment, uint16_t new_alignment)
        {
            std::stringstream ss;
            ss << "Changed alignment from " << old_alignment << " bytes to " << new_alignment << " bytes";
            return ss.str();
        }
        inline std::string msg_config_change_sa(address_t old_addr, address_t new_addr)
        {
            std::stringstream ss;
            ss << std::hex << "Changed start address from " << old_addr << " to " << new_addr;
            return ss.str();
        }
        inline std::string msg_config_change_ea(address_t old_addr, address_t new_addr)
        {
            std::stringstream ss;
            ss << std::hex << "Changed end address from " << old_addr << " to " << new_addr;
            return ss.str();
        }
        inline std::string msg_config_change_sss(size_t old_size, size_t new_size)
        {
            std::stringstream ss;
            ss << "Changed search split size from " << old_size << " bytes to " << new_size << " bytes";
            return ss.str();
        }
        inline std::string msg_config_change_sls(size_t old_size, size_t new_size)
        {
            std::stringstream ss;
            ss << "Changed search limit size from " << old_size << " bytes to " << new_size << " bytes";
            return ss.str();
        }
        inline std::string msg_config_alignment_power2(void)
        {
            return "Alignment must be a power of 2!";
        }

        // messages for command list
        inline std::string msg_list_syntax(void)
        {
            return "Syntax: list";
        }
        inline std::string msg_list_processes(uint32_t n)
        {
            std::stringstream ss;
            ss << "Listing " << n << " processes:";
            return ss.str();
        }

        // messages for command info
        inline std::string msg_info_syntax(void)
        {
            return "Syntax: info";
        }

        // messages for command open
        inline std::string msg_open_syntax(void) noexcept
        {
            return  "Syntax: open <pid>|<window name>";
        }
        inline std::string msg_open_success(pid_t pid) noexcept
        {
            std::stringstream ss;
            ss << "Successfully opened process with ID: " << pid;
            return ss.str();
        }
        inline std::string msg_open_failure(pid_t pid) noexcept
        {
            std::stringstream ss;
            ss << "Failed to open process with ID: " << pid;
            return ss.str();
        }
        inline std::string msg_open_find_pid_success(pid_t pid)
        {
            std::stringstream ss;
            ss << "Successfully found process with ID: " << pid;
            return ss.str();
        }
        inline std::string msg_open_find_pid_failure(pid_t pid)
        {
            std::stringstream ss;
            ss << "Faild to find process with ID: " << pid;
            return ss.str();
        }
        inline std::string msg_open_find_window_success(const std::string& name)
        {
            std::stringstream ss;
            ss << "Successfully found window with name: \"" << name << "\"";
            return ss.str();
        }
        inline std::string msg_open_find_window_failure(const std::string& name)
        {
            std::stringstream ss;
            ss << "Faild to find window with name: \"" << name << "\"";
            return ss.str();
        }

        // messages for command close
        inline std::string msg_close_syntax(void)
        {
            return "Syntax: close";
        }
        inline std::string msg_close_process(pid_t pid)
        {
            std::stringstream ss;
            ss << "Closed process with ID: " << pid;
            return ss.str();
        }
        inline std::string msg_close_live_memory(pid_t pid)
        {
            std::stringstream ss;
            ss << "Stopped live memory process (process ID: " << pid << ")";
            return ss.str();
        }
        inline std::string msg_close_hex_dump(pid_t pid)
        {
            std::stringstream ss;
            ss << "Stopped memory dump process (process ID: " << pid << ")";
            return ss.str();
        }
        inline std::string msg_close_process_failure(void)
        {
            return "No process has been opened.";
        }
        inline std::string msg_close_live_memory_failure(void)
        {
            return "Live memory showcase has not been started.";
        }
        inline std::string msg_close_hex_dump_failure(void)
        {
            return "Memory dump has not been started.";
        }

        // messaged for command clear
        inline std::string msg_clear_syntax(void) noexcept
        {
            return  "Syntax: cls | clear";
        }

        // messages for command category read
        inline std::string msg_read_start(address_t start, address_t end)
        {
            std::stringstream ss;
            ss << "Reading memory from address " << std::hex << start << " to address " << std::hex << end;
            return ss.str();
        }
        inline std::string msg_read_finish(uint64_t count, double time_ms)
        {
            std::stringstream ss;
            ss << "Read " << count << " values in " << time_ms << "ms.";
            return ss.str();
        }
        inline std::string msg_read_minimum(size_t min)
        {
            std::stringstream ss;
            ss << "At least " << min << " bytes must be read.";
            return ss.str();
        }

        // messages for command read_single or rs
        inline std::string msg_rs_syntax(void)
        {
            return "Syntax: read_single | rs <address> [<str_size>]";
        }
        inline std::string msg_rs_failed(memory::address_t addr)
        {
            std::stringstream ss;
            ss << "Failed to read memory from address: " << std::hex << addr;
            return ss.str();
        }
        inline std::string msg_rs_success(memory::address_t addr)
        {
            std::stringstream ss;
            ss << "Successfully read memory from address: " << std::hex << addr;
            return ss.str();
        }

        // messages for command read_block or rb
        inline std::string msg_rb_syntax(void)
        {
            return "Syntax: read_block | rb <start> <range> [<str_size>]";
        }

        // messages for command category search
        inline std::string msg_search_start(uint32_t n)
        {
            std::stringstream ss;
            ss << "Scanning " << n << " processes...";
            return ss.str();
        }
        inline std::string msg_search_finish(uint64_t count, double time_s)
        {
            std::stringstream ss;
            ss << "Finished scanning, found " << count << " matches in " << time_s << "s.";
            return ss.str();
        }
        inline std::string msg_search_interrupt(void)
        {
            return "Interrupted scanning! Buffer out of memory.";
        }
        inline std::string msg_search_sub_start(const std::string& name, pid_t pid, uint32_t i, uint32_t n)
        {
            std::stringstream ss;
            ss << "Scanning process " << name << " (PID: " << pid << ", " << i << "/" << n << ")...";
            return ss.str();
        }
        inline std::string msg_search_sub_failure(const std::string& name, pid_t pid)
        {
            std::stringstream ss;
            ss << "Failed to scan process " << name << " (PID: " << pid << ")!";
            return ss.str();
        }
        inline std::string msg_search_sub_finish(const std::string& name, pid_t pid, uint64_t count)
        {
            std::stringstream ss;
            ss << "Successfully scanned process " << name << " (PID: " << pid << "), found " << count << " matches.";
            return ss.str();
        }

        // messages for command search_exact or se
        inline std::string msg_se_syntax(void)
        {
            return "Syntax: search_exact | se <value>";
        }

        // messages for command search_range or sr
        inline std::string msg_sr_syntax(void)
        {
            return "Syntax: search_range | sr <begin value> <end value>";
        }
        inline std::string msg_sr_string(void)
        {
            return "Command search_range or sr does not work with strings.";
        }

        // messages for command category write
        inline std::string msg_write_finish(uint64_t count, double time_ms)
        {
            std::stringstream ss;
            ss << "Written " << count << " values in " << time_ms << " ms.";
            return ss.str();
        }

        // messages for command write_all or wa
        inline std::string msg_wa_syntax(void)
        {
            return "Syntax: write_all | wa <value>";
        }
        inline std::string msg_wa_start(void)
        {
            return "Writing memory...";
        }

        // messages for command write_single or ws
        inline std::string msg_ws_syntax(void)
        {
            return "Syntax: write_single | ws <address> <value>";
        }
        inline std::string msg_ws_failure(address_t addr)
        {
            std::stringstream ss;
            ss << "Failed to write to address " << std::hex << addr << std::endl;
            return ss.str();
        }
        inline std::string msg_ws_success(address_t addr)
        {
            std::stringstream ss;
            ss << "Successfully written to address " << std::hex << addr << std::endl;
            return ss.str();
        }

        // messages for command write_range or wr
        inline std::string msg_wr_syntax(void)
        {
            return "Syntax: write_range | wr <start> <range> <value>";
        }
        inline std::string msg_wr_start(address_t start, address_t end)
        {
            std::stringstream ss;
            ss << "Writing memory from address " << std::hex << start << " to address " << std::hex << end;
            return ss.str();
        }

        // messages for command category update
        inline std::string msg_update_success(uint64_t count, double time_ms)
        {
            std::stringstream ss;
            ss << count << " values have been updated in " << time_ms << "ms";
            return ss.str();
        }

        // messages for command update
        inline std::string msg_update_syntax(void)
        {
            return "Syntax: update";
        }

        // messages for command update_exact or ue
        inline std::string msg_ue_syntax(void)
        {
            return "Syntax: update_exact | ue <value>";
        }

        // messages for command update_range or ur
        inline std::string msg_ur_syntax(void)
        {
            return "Syntax: update_range | ur <begin value> <end value>";
        }
        inline std::string msg_ur_string(void)
        {
            return "Command update_range or ur does not work with strings.";
        }

        // messages for command show
        inline std::string msg_show_syntax(void)
        {
            return "Syntax: show <start_entry> <amount>";
        }

        // messages for command show_live or sl
        inline std::string msg_sl_syntax(void)
        {
            return "Syntax: show_live | sl <start_entry> <amount> <update speed>";
        }
        inline std::string msg_sl_print_start(const std::string& path)
        {
            return "Printing addresses to file \"" + path + "\"...";
        }
        inline std::string msg_sl_print_finish(const std::string& path)
        {
            return "Successfully printed addresses to file \"" + path + "\".";
        }
        inline std::string msg_sl_print_failure(const std::string& path)
        {
            return "Failed to print addresses to file \"" + path + "\".";
        }
        inline std::string msg_sl_process_success(pid_t pid)
        {
            std::stringstream ss;
            ss << "Succesfully started live-memory-view (PID: " << pid << ").";
            return ss.str();
        }
        inline std::string msg_sl_process_failure(void)
        {
            return "Failed to start live-memory-view.";
        }

        // messages for command undo
        inline std::string msg_undo_syntax(void)
        {
            return "Syntax: undo";
        }
        inline std::string msg_undo_done(double time_ms)
        {
            std::stringstream ss;
            ss << "Undo done in " << time_ms << " ms.";
            return ss.str();
        }
        inline std::string msg_undo_none(void)
        {
            return "Nothing to undo.";
        }

        // messages for command redo
        inline std::string msg_redo_syntax(void)
        {
            return "Syntax: redo";
        }
        inline std::string msg_redo_done(double time_ms)
        {
            std::stringstream ss;
            ss << "Redo done in " << time_ms << " ms.";
            return ss.str();
        }
        inline std::string msg_redo_none(void)
        {
            return "Nothing to redo.";
        }

        // messages for command dump
        inline std::string msg_dump_syntax(void)
        {
            return "Syntax: dump <begin address> <range> <width> <update speed>";
        }
        inline std::string msg_dump_success(pid_t pid)
        {
            std::stringstream ss;
            ss << "Succesfully started memory dump (PID: " << pid << ").";
            return ss.str();
        }
        inline std::string msg_dump_failure(void)
        {
            return "Failed to start memory dump.";
        }

        // messages for command save
        inline std::string msg_save_syntax(void)
        {
            return "Syntax: save <start entry> <amount> <file name>";
        }
        inline std::string msg_save_option_conflict(void)
        {
            return "Conflicting options \"-csv\" and \"-b\" or \"-csv\" and \"--binary\" respectively.";
        }
        inline std::string msg_save_file_failure(const std::string& name)
        {
            return std::string("Failed to open file \"") + name + std::string("\"");
        }

        // messages for number format checks
        inline std::string msg_not_dec(const std::string& arg, uint32_t arg_nr, const std::string& cmd_name)
        {
            std::stringstream ss;
            ss << "Argument " << arg_nr << " of command \"" << cmd_name << "\" is not a decimal number: \"" << arg << "\"";
            return ss.str();
        }
        inline std::string msg_not_hex(const std::string& arg, uint32_t arg_nr, const std::string& cmd_name)
        {
            std::stringstream ss;
            ss << "Argument " << arg_nr << " of command \"" << cmd_name << "\" is not a hexadecimal number: \"" << arg << "\"";
            return ss.str();
        }

        // messages for something unknown
        inline std::string msg_unknown_command(const std::string& cmd)
        {
            std::stringstream ss;
            ss << "Unknown command \"" << cmd << "\"; Type \"help\" for more information.";
            return ss.str();
        }
        inline std::string msg_unknown_argument(const std::string& cmd, const std::string& arg, uint32_t arg_nr)
        {
            std::stringstream ss;
            ss << "Argument " << arg_nr << " \"" << arg << "\" is invalid for command \"" << cmd << "\". Type \"help " << cmd << "\" for more information.";
            return ss.str();
        }
        inline std::string msg_unknown_options(const std::string& cmd, const std::vector<std::string>& options)
        {
            std::stringstream ss;
            ss << "Unknown options of command \"" << cmd << "\":";
            for (const std::string& str : options)
                ss << " " << str;
            ss << "; Type \"help " << cmd << "\" for more information.";
            return ss.str();
        }
        inline std::string msg_unknown_type(const std::string& type)
        {
            std::stringstream ss;
            ss << "Unknown type: " << type << std::endl;
            return ss.str();
        }
    }
}
