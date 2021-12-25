/**
* @file     app_commands.cpp
* @brief    Implementation of all commands.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#define _CRT_SECURE_NO_WARNINGS

#include "app.h"
#include "app_msg.h"

#include <iostream>
#include <inttypes.h>
#include <chrono>
#include <cmath>
#include <algorithm>

using namespace memory::app;


void Application::cmd_help(const Command& cmd)
{
    // syntax check
    if (cmd.args().size() > 1)
    {
        std::cout << make_msg(msg_help_syntax()) << std::endl;
        return;
    }

    // check for invalid options (all options)
    std::vector<std::string> unknown_options;
    list_unknown_options(cmd, {}, unknown_options);
    if (unknown_options.size() > 0)
    {
        std::cout << make_msg(msg_unknown_options(cmd.name(), unknown_options)) << std::endl;
        return;
    }

    if (cmd.args().size() == 0)
    {
        std::cout << msg_help_general() << std::endl;
    }
    else if (cmd.args().size() == 1)
    {
        if (cmd.args().at(0) == "exit")                                             { std::cout << msg_help_exit()          << std::endl; }
        else if (cmd.args().at(0) == "help")                                        { std::cout << msg_help_help()          << std::endl; }
        else if (cmd.args().at(0) == "config")                                      { std::cout << msg_help_config()        << std::endl; }
        else if (cmd.args().at(0) == "list")                                        { std::cout << msg_help_list()          << std::endl; }
        else if (cmd.args().at(0) == "info")                                        { std::cout << msg_help_info()          << std::endl; }
        else if (cmd.args().at(0) == "open")                                        { std::cout << msg_help_open()          << std::endl; }
        else if (cmd.args().at(0) == "close")                                       { std::cout << msg_help_close()         << std::endl; }
        else if (cmd.args().at(0) == "clear"        || cmd.args().at(0) == "cls")   { std::cout << msg_help_clear()         << std::endl; }
        else if (cmd.args().at(0) == "read_single"  || cmd.args().at(0) == "rs")    { std::cout << msg_help_rs()            << std::endl; }
        else if (cmd.args().at(0) == "read_block"   || cmd.args().at(0) == "rb")    { std::cout << msg_help_rb()            << std::endl; }
        else if (cmd.args().at(0) == "search_exact" || cmd.args().at(0) == "se")    { std::cout << msg_help_se()            << std::endl; }
        else if (cmd.args().at(0) == "search_range" || cmd.args().at(0) == "sr")    { std::cout << msg_help_sr()            << std::endl; }
        else if (cmd.args().at(0) == "write_all"    || cmd.args().at(0) == "wa")    { std::cout << msg_help_wa()            << std::endl; }
        else if (cmd.args().at(0) == "write_single" || cmd.args().at(0) == "ws")    { std::cout << msg_help_ws()            << std::endl; }
        else if (cmd.args().at(0) == "write_range"  || cmd.args().at(0) == "wr")    { std::cout << msg_help_wr()            << std::endl; }
        else if (cmd.args().at(0) == "update")                                      { std::cout << msg_help_update()        << std::endl; }
        else if (cmd.args().at(0) == "update_exact" || cmd.args().at(0) == "ue")    { std::cout << msg_help_update_exact()  << std::endl; }
        else if (cmd.args().at(0) == "update_range" || cmd.args().at(0) == "ur")    { std::cout << msg_help_update_range()  << std::endl; }
        else if (cmd.args().at(0) == "undo")                                        { std::cout << msg_help_undo()          << std::endl; }
        else if (cmd.args().at(0) == "redo")                                        { std::cout << msg_help_redo()          << std::endl; }
        else if (cmd.args().at(0) == "show")                                        { std::cout << msg_help_show()          << std::endl; }
        else if (cmd.args().at(0) == "show_live"    || cmd.args().at(0) == "sl")    { std::cout << msg_help_sl()            << std::endl; }
        else if (cmd.args().at(0) == "dump")                                        { std::cout << msg_help_dump()          << std::endl; }
        else                                                                        { std::cout << make_msg(msg_help_invalid(cmd.args().at(0))) << std::endl; }
    }
}

void Application::cmd_config(const Command& cmd)
{
    // syntax check
    if (cmd.args().size() > 2)
    {
        std::cout << make_msg(msg_config_syntax()) << std::endl;
        return;
    }

    // check for invalid options
    const std::vector<std::string> all_options = { "-reset" };
    std::vector<std::string> unknown_options;
    list_unknown_options(cmd, all_options, unknown_options);
    if (unknown_options.size() > 0)
    {
        std::cout << make_msg(msg_unknown_options(cmd.name(), unknown_options)) << std::endl;
        return;
    }

    bool arg_size_2 = (cmd.args().size() == 2);
    bool reset      = (cmd.options().find("-reset", 0) != CmdOpionList::NPOS);

    if (cmd.args().size() == 0)
    {
        // reset settings
        if (reset)
        {
            this->cfg.set_defaults();
            std::cout << make_msg(msg_config_reset()) << std::endl;
        }
        // print settings
        else
            std::cout << msg_config_print_all(this->cfg) << std::endl;
    }
    else
    {
        if (cmd.args().at(0) == "type")
        {
            if (reset)
            {
                this->cfg.set_default_type();
                std::cout << make_msg(msg_config_reset_type(this->cfg.type())) << std::endl;
            }
            else if (arg_size_2)
            {
                type_t old_type = this->cfg.type(), new_type;
                // convert argument to type and size
                if      (cmd.args().at(1) == "int8")    new_type = MEMORY_TYPE_INT8;
                else if (cmd.args().at(1) == "uint8")   new_type = MEMORY_TYPE_UINT8;
                else if (cmd.args().at(1) == "int16")   new_type = MEMORY_TYPE_INT16;
                else if (cmd.args().at(1) == "uint16")  new_type = MEMORY_TYPE_UINT16;
                else if (cmd.args().at(1) == "int32")   new_type = MEMORY_TYPE_INT32;
                else if (cmd.args().at(1) == "uint32")  new_type = MEMORY_TYPE_UINT32;
                else if (cmd.args().at(1) == "int64")   new_type = MEMORY_TYPE_INT64;
                else if (cmd.args().at(1) == "uint64")  new_type = MEMORY_TYPE_UINT64;
                else if (cmd.args().at(1) == "float")   new_type = MEMORY_TYPE_FLOAT;
                else if (cmd.args().at(1) == "double")  new_type = MEMORY_TYPE_DOUBLE;
                else if (cmd.args().at(1) == "string")  new_type = MEMORY_TYPE_STRING;
                else
                {
                    std::cout << make_msg(msg_unknown_type(cmd.args().at(1))) << std::endl;
                    return;
                }
                this->cfg.set_type(new_type);
                std::cout << make_msg(msg_config_change_type(old_type, new_type)) << std::endl;
            }
            else
                std::cout << make_msg(msg_config_print_type(this->cfg.type())) << std::endl;
        }
        else if (cmd.args().at(0) == "alignment")
        {
            if (reset)
            {
                this->cfg.set_default_aligment();
                std::cout << make_msg(msg_config_reset_alignment(this->cfg.alignment())) << std::endl;
            }
            else if (arg_size_2)
            {
                uint16_t old_align = this->cfg.alignment(), new_align;
                if (!utility::is_dec(cmd.args().at(1)))
                {
                    std::cout << make_msg(msg_not_dec(cmd.args().at(1), 2, cmd.name())) << std::endl;
                    return;
                }
                sscanf(cmd.args().at(1).c_str(), "%" PRIu16, &new_align);

                float x = log2f(static_cast<float>(new_align));
                if ((x - std::floor(x)) > 0.0f)
                {
                    std::cout << make_msg(msg_config_alignment_power2()) << std::endl;
                    return;
                }

                this->cfg.set_aligment(new_align);
                std::cout << make_msg(msg_config_change_alignment(old_align, new_align)) << std::endl;
            }
            else
                std::cout << make_msg(msg_config_print_alignment(this->cfg.alignment())) << std::endl;
        }
        else if (cmd.args().at(0) == "start_address")
        {
            if (reset)
            {
                this->cfg.set_default_start_address();
                std::cout << make_msg(msg_config_reset_sa(this->cfg.start_address())) << std::endl;
            }
            else if (arg_size_2)
            {
                address_t old_sa = this->cfg.start_address(), new_sa;
                if (!utility::is_hex(cmd.args().at(1)))
                {
                    std::cout << make_msg(msg_not_hex(cmd.args().at(1), 2, cmd.name())) << std::endl;
                    return;
                }
                sscanf(cmd.args().at(1).c_str(), "%" PRIX64, &new_sa);

                this->cfg.set_start_address(new_sa);
                std::cout << make_msg(msg_config_change_sa(old_sa, new_sa)) << std::endl;
            }
            else
                std::cout << make_msg(msg_config_print_sa(this->cfg.start_address())) << std::endl;
        }
        else if (cmd.args().at(0) == "end_address")
        {
            if (reset)
            {
                this->cfg.set_default_end_address();
                std::cout << make_msg(msg_config_reset_ea(this->cfg.end_address())) << std::endl;
            }
            else if (arg_size_2)
            {
                address_t old_ea = this->cfg.end_address(), new_ea;
                if (!utility::is_hex(cmd.args().at(1)))
                {
                    std::cout << make_msg(msg_not_hex(cmd.args().at(1), 2, cmd.name())) << std::endl;
                    return;
                }
                sscanf(cmd.args().at(1).c_str(), "%" PRIX64, &new_ea);

                this->cfg.set_end_address(new_ea);
                std::cout << make_msg(msg_config_change_ea(old_ea, new_ea)) << std::endl;
            }
            else
                std::cout << make_msg(msg_config_print_ea(this->cfg.end_address())) << std::endl;
        }
        else if (cmd.args().at(0) == "search_split_size")
        {
            if (reset)
            {
                this->cfg.set_default_search_split_size();
                std::cout << make_msg(msg_config_reset_sss(this->cfg.search_split_size())) << std::endl;
            }
            else if (arg_size_2)
            {
                size_t old_sss = this->cfg.search_split_size(), new_sss;
                if (!utility::is_dec(cmd.args().at(1)))
                {
                    std::cout << make_msg(msg_not_dec(cmd.args().at(1), 2, cmd.name())) << std::endl;
                    return;
                }
                sscanf(cmd.args().at(1).c_str(), "%zu", &new_sss);

                this->cfg.set_search_split_size(new_sss);
                std::cout << make_msg(msg_config_change_sss(old_sss, new_sss)) << std::endl;
            }
            else
                std::cout << make_msg(msg_config_print_sss(this->cfg.search_split_size())) << std::endl;
        }
        else if (cmd.args().at(0) == "search_limit_size")
        {
            if (reset)
            {
                this->cfg.set_default_search_limit_size();

                // update buffers according to the new limit, NOTE: buffers get cleared
                this->search_buffer.clear();
                this->undo_buffer.clear();
                this->redo_buffer.clear();
                this->search_buffer.set_limit(this->cfg.search_limit_size());
                this->undo_buffer.set_limit(this->cfg.search_limit_size());
                this->redo_buffer.set_limit(this->cfg.search_limit_size());

                std::cout << make_msg(msg_config_reset_sls(this->cfg.search_limit_size())) << std::endl;
            }
            else if (arg_size_2)
            {
                size_t old_sls = this->cfg.search_limit_size(), new_sls;
                if (!utility::is_dec(cmd.args().at(1)))
                {
                    std::cout << make_msg(msg_not_dec(cmd.args().at(1), 2, cmd.name())) << std::endl;
                    return;
                }
                sscanf(cmd.args().at(1).c_str(), "%zu", &new_sls);
                this->cfg.set_search_limit_size(new_sls);

                // update buffers according to the new limit, NOTE: buffers get cleared
                this->search_buffer.clear();
                this->undo_buffer.clear();
                this->redo_buffer.clear();
                this->search_buffer.set_limit(this->cfg.search_limit_size());
                this->undo_buffer.set_limit(this->cfg.search_limit_size());
                this->redo_buffer.set_limit(this->cfg.search_limit_size());

                std::cout << make_msg(msg_config_change_sls(old_sls, new_sls)) << std::endl;
            }
            else
                std::cout << make_msg(msg_config_print_sls(this->cfg.search_limit_size())) << std::endl;
        }
        else
        {
            std::cout << make_msg(msg_unknown_argument(cmd.name(), cmd.args().at(0), 1)) << std::endl;
            return;
        }
    }
    // save config if anything has changed
    if (reset || arg_size_2)
        std::cout << make_msg(this->cfg.save() ? msg_config_save_success() : msg_config_save_failure()) << std::endl;
}

void Application::cmd_list(const Command& cmd)
{
    // syntax check
    if (cmd.args().size() != 0)
    {
        std::cout << make_msg(msg_list_syntax()) << std::endl;
        return;
    }

    // check for invalid options
    const std::vector<std::string> all_options = { "a", "-access" };
    std::vector<std::string> unknown_options;
    list_unknown_options(cmd, all_options, unknown_options);
    if (unknown_options.size() > 0)
    {
        std::cout << make_msg(msg_unknown_options(cmd.name(), unknown_options)) << std::endl;
        return;
    }

    // get all processess
    std::vector<std::string> entry(this->process_table.col_count());
    std::vector<Process> processes;
    Process::enum_processes(cmd.options().find_any(all_options, 0) != CmdOpionList::NPOS, processes);
    std::sort(processes.begin(), processes.end());

    // list processes in table
    for (const Process& p : processes)
    {
        make_process_entry(p, entry);
        this->process_table.add(entry);
    }

    // print table
    std::cout << make_msg(msg_list_processes(processes.size())) << std::endl;
    this->process_table.print();
    this->process_table.clear_entries();
}

void Application::cmd_info(const Command& cmd)
{
    // syntax check
    if (cmd.args().size() != 0)
    {
        std::cout << make_msg(msg_info_syntax()) << std::endl;
        return;
    }

    // check for invalid options (all options)
    std::vector<std::string> unknown_options;
    list_unknown_options(cmd, {}, unknown_options);
    if (unknown_options.size() > 0)
    {
        std::cout << make_msg(msg_unknown_options(cmd.name(), unknown_options)) << std::endl;
        return;
    }

    // print process information
    std::cout << "Current open process:" << std::endl;
    if (this->current_process.is_valid())
    {
        std::cout << "\tName: " << this->current_process.name() << std::endl;
        std::cout << "\tPID: " << this->current_process.pid() << std::endl;
        std::cout << "\tParent PID: " << this->current_process.parent_pid() << std::endl;
        std::cout << "\tThread count: " << this->current_process.count_threads() << std::endl;
    }
    else
    {
        std::cout << "\tName: ----" << std::endl;
        std::cout << "\tPID: ----" << std::endl;
        std::cout << "\tParent PID: ----" << std::endl;
        std::cout << "\tThread count: ----" << std::endl;
    }
    std::cout << std::endl;

    // print live memory information
    std::cout << "Live memory:" << std::endl;
    if (this->pid_live_memory == MEMORY_PID_INVALID)
    {
        std::cout << "\tState: STOPPED" << std::endl;
        std::cout << "\tPID: ----" << std::endl;
    }
    else
    {
        std::cout << "\tState: RUNNING" << std::endl;
        std::cout << "\tPID: " << this->pid_live_memory << std::endl;
    }
    std::cout << std::endl;

    // print memory dump information
    std::cout << "Memory dump:" << std::endl;
    if (this->pid_dump == MEMORY_PID_INVALID)
    {
        std::cout << "\tState: STOPPED" << std::endl;
        std::cout << "\tPID: ----" << std::endl;
    }
    else
    {
        std::cout << "\tState: RUNNING" << std::endl;
        std::cout << "\tPID: " << this->pid_dump << std::endl;
    }
    std::cout << std::endl;

    // print application information
    std::string pre1, pre2;
    double bytes1, bytes2;
    std::cout << "General application information:" << std::endl;

    bytes1 = auto_SI(this->search_buffer.size(), pre1);
    bytes2 = auto_SI(this->search_buffer.limit(), pre2);
    std::cout << "\tSearch buffer size: " << bytes1 << pre1 << " / " << bytes2 << pre2 << std::endl;
    std::cout << "\tSearch buffer element count: " << this->search_buffer.table().size() << std::endl;

    bytes1 = auto_SI(this->undo_buffer.size(), pre1);
    bytes2 = auto_SI(this->undo_buffer.limit(), pre2);
    std::cout << "\tUndo buffer size: " << bytes1 << pre1 << " / " << bytes2 << pre2 << std::endl;
    std::cout << "\tUndo buffer element count: " << this->undo_buffer.table().size() << std::endl;

    bytes1 = auto_SI(this->redo_buffer.size(), pre1);
    bytes2 = auto_SI(this->redo_buffer.limit(), pre2);
    std::cout << "\tRedo buffer size: " << bytes1 << pre1 << " / " << bytes2 << pre2 << std::endl;
    std::cout << "\tRedo buffer element count: " << this->redo_buffer.table().size() << std::endl;

    bytes1 = auto_SI(this->search_buffer.size() + this->undo_buffer.size() + this->redo_buffer.size(), pre1);
    std::cout << "\tTotal used up memory by buffers: " << bytes1 << pre1 << std::endl;
    std::cout << std::endl;
}

void Application::cmd_open(const Command& cmd)
{
    // syntax check
    if (cmd.args().size() != 1)
    {
        std::cout << make_msg(msg_open_syntax()) << std::endl;
        return;
    }

    // check for invalid options
    const std::vector<std::string> all_options = { "w", "-window" };
    std::vector<std::string> unknown_options;
    list_unknown_options(cmd, all_options, unknown_options);
    if (unknown_options.size() > 0)
    {
        std::cout << make_msg(msg_unknown_options(cmd.name(), unknown_options)) << std::endl;
        return;
    }

    Process old_process = std::move(this->current_process);
    // -w and --window are not set, use process ID
    if (cmd.options().find_any(all_options, 0) == memory::CmdOpionList::NPOS)
    {
        // pid must be a decimal number
        if (!memory::utility::is_dec(cmd.args().at(0)))
        {
            std::cout << make_msg(msg_not_dec(cmd.args().at(0), 1, cmd.name())) << std::endl;
            return;
        }
        pid_t pid;
        sscanf(cmd.args().at(0).c_str(), "%lu", &pid);

        // find process ID
        if (!Process::find_process(pid, this->current_process))
        {
            std::cout << make_msg(msg_open_find_pid_failure(pid)) << std::endl;
            return;
        }
        std::cout << make_msg(msg_open_find_pid_success(pid)) << std::endl;
    }
    // -w or --window is set, search window name
    else
    {
        // find window and get process ID from window
        if (!Process::find_process(cmd.args().at(0), this->current_process))
        {
            std::cout << make_msg(msg_open_find_window_failure(cmd.args().at(0))) << std::endl;
            return;
        }
        std::cout << make_msg(msg_open_find_window_success(cmd.args().at(0))) << std::endl;
    }

    // close process if there is currently one open
    if (old_process.is_valid())
    {
        old_process.close();
        std::cout << make_msg(msg_close_process(old_process.pid())) << std::endl;
    }

    // open new process
    if (!this->current_process.open())
    {
        std::cout << make_msg(msg_open_failure(this->current_process.pid())) << std::endl;
        return;
    }
    std::cout << make_msg(msg_open_success(this->current_process.pid())) << std::endl;
}

void Application::cmd_close(const Command& cmd)
{
    // syntax check
    if (cmd.args().size() != 0)
    {
        std::cout << make_msg(msg_close_syntax()) << std::endl;
        return;
    }

    // check for invalid options
    const std::vector<std::string> all_options = { "p", "-process", "l", "-live", "d", "-dump" };
    std::vector<std::string> unknown_options;
    list_unknown_options(cmd, all_options, unknown_options);
    if (unknown_options.size() > 0)
    {
        std::cout << make_msg(msg_unknown_options(cmd.name(), unknown_options)) << std::endl;
        return;
    }

    // close process
    if (cmd.options().size() == 0 || cmd.options().find_any({ "p", "-process" }, 0) != memory::CmdOpionList::NPOS)
    {
        if (this->current_process.is_valid())
        {
            this->current_process.close();
            std::cout << make_msg(msg_close_process(this->current_process.pid())) << std::endl;
        }
        else if (cmd.options().size() > 0)
            std::cout << make_msg(msg_close_process_failure()) << std::endl;
    }

    // close live memory
    if (cmd.options().size() == 0 || cmd.options().find_any({ "l", "-live" }, 0) != memory::CmdOpionList::NPOS)
    {
        if (this->pid_live_memory != MEMORY_PID_INVALID)
        {
            this->process_handler.stop_process(this->pid_live_memory);
            std::cout << make_msg(msg_close_live_memory(this->pid_live_memory)) << std::endl;
            this->pid_live_memory = MEMORY_PID_INVALID;
        }
        else if (cmd.options().size() > 0)
            std::cout << make_msg(msg_close_live_memory_failure()) << std::endl;
    }

    // close memory dump
    if (cmd.options().size() == 0 || cmd.options().find_any({ "d", "-dump" }, 0) != memory::CmdOpionList::NPOS)
    {
        if (this->pid_dump != MEMORY_PID_INVALID)
        {
            this->process_handler.stop_process(this->pid_dump);
            std::cout << make_msg(msg_close_hex_dump(this->pid_dump)) << std::endl;
            this->pid_dump = MEMORY_PID_INVALID;
        }
        else if (cmd.options().size() > 0)
            std::cout << make_msg(msg_close_hex_dump_failure()) << std::endl;
    }
}

void Application::cmd_clear(const Command& cmd)
{
    // syntax check
    if (cmd.args().size() != 0)
    {
        std::cout << make_msg(msg_clear_syntax()) << std::endl;
        return;
    }

    // check for invalid options (all options)
    std::vector<std::string> unknown_options;
    list_unknown_options(cmd, {}, unknown_options);
    if (unknown_options.size() > 0)
    {
        std::cout << make_msg(msg_unknown_options(cmd.name(), unknown_options)) << std::endl;
        return;
    }

    std::system("cls");
}

void Application::cmd_read_single(const Command& cmd)
{
    // syntax check
    if ((!utility::is_string(this->cfg.type()) && cmd.args().size() != 1) || (utility::is_string(this->cfg.type()) && cmd.args().size() != 2))
    {
        std::cout << make_msg(msg_rs_syntax()) << std::endl;
        return;
    }

    // check for invalid options (all options)
    std::vector<std::string> unknown_options;
    list_unknown_options(cmd, {}, unknown_options);
    if (unknown_options.size() > 0)
    {
        std::cout << make_msg(msg_unknown_options(cmd.name(), unknown_options)) << std::endl;
        return;
    }

    // check for correct arguments
    if (!utility::is_hex(cmd.args().at(0)))
    {
        std::cout << make_msg(msg_not_hex(cmd.args().at(0), 1, cmd.name())) << std::endl;
        return;
    }
    if (utility::is_string(this->cfg.type()) && !memory::utility::is_dec(cmd.args().at(1)))
    {
        std::cout << make_msg(msg_not_dec(cmd.args().at(1), 2, cmd.name())) << std::endl;
        return;
    }

    // check for open process
    if (!this->current_process.is_valid())
    {
        std::cout << make_msg(msg_close_process_failure()) << std::endl;    // reuse message
        return;
    }

    // convert arguments
    memory::address_t addr;
    size_t size = this->cfg.type_size();
    sscanf(cmd.args().at(0).c_str(), "%" PRIx64, &addr);
    if (utility::is_string(this->cfg.type()))
    {
        uint32_t str_size;
        sscanf(cmd.args().at(1).c_str(), "%" PRIu32, &str_size);
        size = str_size;
    }

    // read process memory
    uint8_t value[size];
    if (this->current_process.read(addr, size, value) == 0)
    {
        std::cout << make_msg(msg_rs_failed(addr)) << std::endl;
        return;
    }

    // write value to buffer
    this->make_backup();
    this->search_buffer.push(this->current_process.pid(), addr, size, this->cfg.type(), value);
    std::cout << make_msg(msg_rs_success(addr)) << std::endl;

    // build table (only for read_single)
    std::vector<std::string> entry(this->search_table.col_count());
    this->search_table.clear_entries();
    this->make_search_entry(this->current_process.pid(), addr, size, value, entry);
    this->search_table.add(entry);

    // print table instantly (only for read_single)
    this->search_table.print();
    this->search_table.clear_entries();
}

void Application::cmd_read_block(const Command& cmd)
{
    using namespace std::chrono;

    // syntax check
    if ((!utility::is_string(this->cfg.type()) && cmd.args().size() != 2) || (utility::is_string(this->cfg.type()) && cmd.args().size() != 3))
    {
        std::cout << make_msg(msg_rb_syntax()) << std::endl;
        return;
    }

    // check for invalid options (all options)
    std::vector<std::string> unknown_options;
    list_unknown_options(cmd, {}, unknown_options);
    if (unknown_options.size() > 0)
    {
        std::cout << make_msg(msg_unknown_options(cmd.name(), unknown_options)) << std::endl;
        return;
    }

    // check for correct arguments
    if (!utility::is_hex(cmd.args().at(0)))
    {
        std::cout << make_msg(msg_not_hex(cmd.args().at(0), 1, cmd.name())) << std::endl;
        return;
    }
    if (!utility::is_hex(cmd.args().at(1)))
    {
        std::cout << make_msg(msg_not_hex(cmd.args().at(1), 2, cmd.name())) << std::endl;
        return;
    }
    if (utility::is_string(this->cfg.type()) && !utility::is_dec(cmd.args().at(2)))
    {
        std::cout << make_msg(msg_not_dec(cmd.args().at(2), 3, cmd.name())) << std::endl;
        return;
    }

    // check for open process
    if (!this->current_process.is_valid())
    {
        std::cout << make_msg(msg_close_process_failure()) << std::endl;    // reuse message
        return;
    }

    // convert arguments
    address_t start, range, end;
    uint32_t size = this->cfg.type_size();

    sscanf(cmd.args().at(0).c_str(), "%" PRIx64, &start);
    sscanf(cmd.args().at(1).c_str(), "%" PRIx64, &range);
    if (utility::is_string(this->cfg.type()))
        sscanf(cmd.args().at(2).c_str(), "%" PRIu32, &size);

    // range must not be smaller than type-size
    if (range < size)
    {
        std::cout << make_msg(msg_read_minimum(size)) << std::endl;
        return;
    }

    // calculation of start and end
    start   = start + ((start % this->cfg.alignment() > 0) ? (this->cfg.alignment() - start % this->cfg.alignment()) : 0);
    end     = start + ((range % this->cfg.alignment() > 0) ? (range - range % this->cfg.alignment()) : range) - size;

    // read memory
    this->make_backup();
    this->search_buffer.resize(this->cfg.search_limit_size());
    std::cout << make_msg(msg_read_start(start, end + size)) << std::endl;

    uint8_t value[size];
    uint64_t count = 0;
    time_point<high_resolution_clock> t0 = high_resolution_clock::now();
    for (address_t a = start; a <= end; a += this->cfg.alignment())
    {
        if (this->current_process.read(a, size, value) > 0)
        {
            this->search_buffer.push(this->current_process.pid(), a, size, this->cfg.type(), value);
            ++count;
        }
    }
    this->search_buffer.shrink_to_fit();
    time_point<high_resolution_clock> t1 = high_resolution_clock::now();
    std::cout << make_msg(msg_read_finish(count, duration_cast<microseconds>(t1 - t0).count() / 1000.0)) << std::endl;
}

void Application::cmd_search_exact(const Command& cmd)
{
    using namespace std::chrono;

    // syntax check
    if (cmd.args().size() != 1)
    {
        std::cout << make_msg(msg_se_syntax()) << std::endl;
        return;
    }

    // check for invalid options
    const std::vector<std::string> all_options = { "a", "-all" };
    std::vector<std::string> unknown_options;
    list_unknown_options(cmd, all_options, unknown_options);
    if (unknown_options.size() > 0)
    {
        std::cout << make_msg(msg_unknown_options(cmd.name(), unknown_options)) << std::endl;
        return;
    }

    // check for correct arguments
    bool is_hex = is_input_hex(cmd.args().at(0));
    if (!utility::is_string(this->cfg.type()))
    {
        if (utility::is_floating_point(this->cfg.type()) && !utility::is_floating_point(cmd.args().at(0)))
        {
            std::cout << make_msg(msg_not_dec(cmd.args().at(0), 1, cmd.name())) << std::endl;
            return;
        }
        else if (utility::is_integral(this->cfg.type()))
        {
            if (!is_hex && !utility::is_dec(cmd.args().at(0)))
            {
                std::cout << make_msg(msg_not_dec(cmd.args().at(0), 1, cmd.name())) << std::endl;
                return;
            }
            if (is_hex && !utility::is_hex(cmd.args().at(0)))
            {
                std::cout << make_msg(msg_not_hex(cmd.args().at(0), 1, cmd.name())) << std::endl;
                return;
            }
        }
    }

    // check for open process
    bool all = cmd.options().find_any(all_options, 0) != CmdOpionList::NPOS;
    if (!all && !this->current_process.is_valid())
    {
        std::cout << make_msg(msg_close_process_failure()) << std::endl;    // reuse message
        return;
    }

    // convert arguments
    size_t size = (utility::is_string(this->cfg.type())) ? cmd.args().at(0).size() : this->cfg.type_size();
    uint8_t in_value[size];
    utility::to_bytes(cmd.args().at(0), size, this->cfg.type(), is_hex, in_value);

    // get processes
    std::vector<Process> processes;
    if (all)    Process::enum_processes(true, processes);
    else        processes.push_back(this->current_process);

    // make backup for search buffer
    this->make_backup();
    this->search_buffer.resize(this->cfg.search_limit_size());

    // scan processes
    uint64_t sum_count = 0, sub_count;
    bool limit = false;
    time_point<high_resolution_clock> t0 = high_resolution_clock::now();

    // iterate through all processes
    uint32_t i = 1, n = processes.size();
    std::cout << make_msg(msg_search_start(processes.size())) << std::endl;
    for (auto proc = processes.begin(); proc != processes.end() && !limit; proc++)
    {
        std::cout << make_msg(msg_search_sub_start(proc->name(), proc->pid(), i++, n)) << std::endl;
        // must not scan the own process
        if (proc->open() && proc->pid() != this->pid_this)
        {
            sub_count = this->scan(*proc, in_value, in_value, size, limit);
            sum_count += sub_count;
            proc->close();
            std::cout << make_msg(msg_search_sub_finish(proc->name(), proc->pid(), sub_count)) << std::endl;
        }
        else
            std::cout << make_msg(msg_search_sub_failure(proc->name(), proc->pid())) << std::endl;
    }
    time_point<high_resolution_clock> t1 = high_resolution_clock::now();
    this->search_buffer.shrink_to_fit();
    std::cout << make_msg(limit ? msg_search_interrupt() : msg_search_finish(sum_count, duration_cast<milliseconds>(t1 - t0).count() / 1000.0)) << std::endl;
}

void Application::cmd_search_range(const Command& cmd)
{
    using namespace std::chrono;

    // syntax check
    if (cmd.args().size() != 2)
    {
        std::cout << make_msg(msg_se_syntax()) << std::endl;
        return;
    }

    // check for invalid options
    const std::vector<std::string> all_options = { "a", "-all" };
    std::vector<std::string> unknown_options;
    list_unknown_options(cmd, all_options, unknown_options);
    if (unknown_options.size() > 0)
    {
        std::cout << make_msg(msg_unknown_options(cmd.name(), unknown_options)) << std::endl;
        return;
    }

    // this command does not work with strings
    if (utility::is_string(this->cfg.type()))
    {
        std::cout << make_msg(msg_sr_string()) << std::endl;
        return;
    }

    // check for correct arguments
    bool is_hex[2];
    for (uint32_t i = 0; i < 2; i++)
    {
        is_hex[i] = is_input_hex(cmd.args().at(i));
        if (utility::is_floating_point(this->cfg.type()) && !utility::is_floating_point(cmd.args().at(i)))
        {
            std::cout << make_msg(msg_not_dec(cmd.args().at(i), i + 1, cmd.name())) << std::endl;
            return;
        }
        else if (utility::is_integral(this->cfg.type()))
        {
            if (!is_hex[i] && !utility::is_dec(cmd.args().at(i)))
            {
                std::cout << make_msg(msg_not_dec(cmd.args().at(i), i + 1, cmd.name())) << std::endl;
                return;
            }
            if (is_hex[i] && !utility::is_hex(cmd.args().at(i)))
            {
                std::cout << make_msg(msg_not_hex(cmd.args().at(i), i + 1, cmd.name())) << std::endl;
                return;
            }
        }
    }

    // check for open process
    bool all = cmd.options().find_any(all_options, 0) != CmdOpionList::NPOS;
    if (!all && !this->current_process.is_valid())
    {
        std::cout << make_msg(msg_close_process_failure()) << std::endl;    // reuse message
        return;
    }

    // convert arguments
    uint8_t in_value1[this->cfg.type_size()], in_value2[this->cfg.type_size()];
    utility::to_bytes(cmd.args().at(0), this->cfg.type_size(), this->cfg.type(), is_hex[0], in_value1);
    utility::to_bytes(cmd.args().at(1), this->cfg.type_size(), this->cfg.type(), is_hex[1], in_value2);

    // get processes
    std::vector<Process> processes;
    if (all)    Process::enum_processes(true, processes);
    else        processes.push_back(this->current_process);

    // make backup for search buffer
    this->make_backup();
    this->search_buffer.resize(this->cfg.search_limit_size());

    // scan processes
    uint64_t sum_count = 0, sub_count;
    bool limit = false;
    time_point<high_resolution_clock> t0 = high_resolution_clock::now();

    // iterate through all processes
    uint32_t i = 1, n = processes.size();
    std::cout << make_msg(msg_search_start(processes.size())) << std::endl;
    for (auto proc = processes.begin(); proc != processes.end() && !limit; proc++)
    {
        std::cout << make_msg(msg_search_sub_start(proc->name(), proc->pid(), i++, n)) << std::endl;
        // must not scan the own process
        if (proc->open() && proc->pid() != this->pid_this)
        {
            sub_count = this->scan(*proc, in_value1, in_value2, this->cfg.type_size(), limit);
            sum_count += sub_count;
            proc->close();
            std::cout << make_msg(msg_search_sub_finish(proc->name(), proc->pid(), sub_count)) << std::endl;
        }
        else
            std::cout << make_msg(msg_search_sub_failure(proc->name(), proc->pid())) << std::endl;
    }
    time_point<high_resolution_clock> t1 = high_resolution_clock::now();
    this->search_buffer.shrink_to_fit();
    std::cout << make_msg(limit ? msg_search_interrupt() : msg_search_finish(sum_count, duration_cast<milliseconds>(t1 - t0).count() / 1000.0)) << std::endl;
}

void Application::cmd_write_all(const Command& cmd)
{
    using namespace std::chrono;

    // syntax check
    if (cmd.args().size() != 1)
    {
        std::cout << make_msg(msg_wa_syntax()) << std::endl;
        return;
    }

    // check for invalid options (all options)
    std::vector<std::string> unknown_options;
    list_unknown_options(cmd, {}, unknown_options);
    if (unknown_options.size() > 0)
    {
        std::cout << make_msg(msg_unknown_options(cmd.name(), unknown_options)) << std::endl;
        return;
    }

    // check for correct arguments
    bool is_hex = is_input_hex(cmd.args().at(0));
    if (!utility::is_string(this->cfg.type()))
    {
        if (utility::is_floating_point(this->cfg.type()) && !utility::is_floating_point(cmd.args().at(0)))
        {
            std::cout << make_msg(msg_not_dec(cmd.args().at(0), 1, cmd.name())) << std::endl;
            return;
        }
        else if (utility::is_integral(this->cfg.type()))
        {
            if (!is_hex && !utility::is_dec(cmd.args().at(0)))
            {
                std::cout << make_msg(msg_not_dec(cmd.args().at(0), 1, cmd.name())) << std::endl;
                return;
            }
            if (is_hex && !utility::is_hex(cmd.args().at(0)))
            {
                std::cout << make_msg(msg_not_hex(cmd.args().at(0), 1, cmd.name())) << std::endl;
                return;
            }
        }
    }

    // check for open process
    if (!this->current_process.is_valid())
    {
        std::cout << make_msg(msg_close_process_failure()) << std::endl;    // reuse message
        return;
    }

    // convert arguments
    size_t size = (utility::is_string(this->cfg.type())) ? cmd.args().at(0).size() : this->cfg.type_size();
    uint8_t in_value[size];
    utility::to_bytes(cmd.args().at(0), size, this->cfg.type(), is_hex, in_value);

    // write memory
    std::cout << make_msg(msg_wa_start()) << std::endl;
    time_point<high_resolution_clock> t0 = high_resolution_clock::now();
    uint64_t count = this->write(in_value, size);
    time_point<high_resolution_clock> t1 = high_resolution_clock::now();
    std::cout << make_msg(msg_write_finish(count, duration_cast<microseconds>(t1 - t0).count() / 1000.0)) << std::endl;
}
 
void Application::cmd_write_single(const Command& cmd)
{
    // syntax check
    if (cmd.args().size() != 2)
    {
        std::cout << make_msg(msg_ws_syntax()) << std::endl;
        return;
    }

    // check for invalid options (all options)
    std::vector<std::string> unknown_options;
    list_unknown_options(cmd, {}, unknown_options);
    if (unknown_options.size() > 0)
    {
        std::cout << make_msg(msg_unknown_options(cmd.name(), unknown_options)) << std::endl;
        return;
    }

    // check for correct arguments
    bool is_hex = is_input_hex(cmd.args().at(1));
    if (!utility::is_hex(cmd.args().at(0)))
    {
        std::cout << make_msg(msg_not_hex(cmd.args().at(0), 1, cmd.name()));
        return;
    }
    if (!utility::is_string(this->cfg.type()))
    {
        if (utility::is_floating_point(this->cfg.type()) && !utility::is_floating_point(cmd.args().at(1)))
        {
            std::cout << make_msg(msg_not_dec(cmd.args().at(1), 2, cmd.name())) << std::endl;
            return;
        }
        else if (utility::is_integral(this->cfg.type()))
        {
            if (!is_hex && !utility::is_dec(cmd.args().at(1)))
            {
                std::cout << make_msg(msg_not_dec(cmd.args().at(1), 2, cmd.name())) << std::endl;
                return;
            }
            if (is_hex && !utility::is_hex(cmd.args().at(1)))
            {
                std::cout << make_msg(msg_not_hex(cmd.args().at(1), 2, cmd.name())) << std::endl;
                return;
            }
        }
    }

    // check for open process
    if (!this->current_process.is_valid())
    {
        std::cout << make_msg(msg_close_process_failure()) << std::endl;    // reuse message
        return;
    }

    // convert arguments
    address_t addr;
    sscanf(cmd.args().at(0).c_str(), "%" PRIx64, &addr);
    size_t size = (utility::is_string(this->cfg.type())) ? cmd.args().at(1).size() : this->cfg.type_size();
    uint8_t in_value[size];
    utility::to_bytes(cmd.args().at(1), size, this->cfg.type(), is_hex, in_value);

    // write memory
    if (this->current_process.write(addr, size, in_value) == 0)
        std::cout << make_msg(msg_ws_failure(addr)) << std::endl;
    else
        std::cout << make_msg(msg_ws_success(addr)) << std::endl;
}

void Application::cmd_write_range(const Command& cmd)
{
    using namespace std::chrono;

    // syntax check
    if (cmd.args().size() != 3)
    {
        std::cout << make_msg(msg_wr_syntax()) << std::endl;
        return;
    }

    // check for invalid options (all options)
    std::vector<std::string> unknown_options;
    list_unknown_options(cmd, {}, unknown_options);
    if (unknown_options.size() > 0)
    {
        std::cout << make_msg(msg_unknown_options(cmd.name(), unknown_options)) << std::endl;
        return;
    }

    // check for correct arguments
    bool is_hex = is_input_hex(cmd.args().at(2));
    if (!utility::is_hex(cmd.args().at(0)))
    {
        std::cout << make_msg(msg_not_hex(cmd.args().at(0), 1, cmd.name()));
        return;
    }
    if (!utility::is_hex(cmd.args().at(1)))
    {
        std::cout << make_msg(msg_not_hex(cmd.args().at(1), 2, cmd.name()));
        return;
    }
    if (!utility::is_string(this->cfg.type()))
    {
        if (utility::is_floating_point(this->cfg.type()) && !utility::is_floating_point(cmd.args().at(2)))
        {
            std::cout << make_msg(msg_not_dec(cmd.args().at(2), 3, cmd.name())) << std::endl;
            return;
        }
        else if (utility::is_integral(this->cfg.type()))
        {
            if (!is_hex && !utility::is_dec(cmd.args().at(2)))
            {
                std::cout << make_msg(msg_not_dec(cmd.args().at(2), 3, cmd.name())) << std::endl;
                return;
            }
            if (is_hex && !utility::is_hex(cmd.args().at(2)))
            {
                std::cout << make_msg(msg_not_hex(cmd.args().at(2), 3, cmd.name())) << std::endl;
                return;
            }
        }
    }

    // check for open process
    if (!this->current_process.is_valid())
    {
        std::cout << make_msg(msg_close_process_failure()) << std::endl;    // reuse message
        return;
    }

    // convert arguments
    address_t start, range, end;
    sscanf(cmd.args().at(0).c_str(), "%" PRIx64, &start);
    sscanf(cmd.args().at(1).c_str(), "%" PRIx64, &range);
    size_t size = (utility::is_string(this->cfg.type())) ? cmd.args().at(2).size() : this->cfg.type_size();
    uint8_t in_value[size];
    utility::to_bytes(cmd.args().at(2), size, this->cfg.type(), is_hex, in_value);

    // range must not be smaller than type-size
    if (range < size)
    {
        std::cout << make_msg(msg_read_minimum(size)) << std::endl;
        return;
    }

    // calculation of start and end
    start = start + ((start % this->cfg.alignment() > 0) ? (this->cfg.alignment() - start % this->cfg.alignment()) : 0);
    end = start + ((range % this->cfg.alignment() > 0) ? (range - range % this->cfg.alignment()) : range) - size;

    // write memory
    uint64_t count = 0;
    std::cout << make_msg(msg_wr_start(start, end + size)) << std::endl;
    time_point<high_resolution_clock> t0 = high_resolution_clock::now();
    for (address_t a = start; a <= end; a += this->cfg.alignment())
    {
        if (this->current_process.write(a, size, in_value))
            ++count;
    }
    time_point<high_resolution_clock> t1 = high_resolution_clock::now();
    std::cout << make_msg(msg_write_finish(count, duration_cast<microseconds>(t1 - t0).count() / 1000.0)) << std::endl;
}

void Application::cmd_update(const Command& cmd)
{
    using namespace std::chrono;

    // syntax check
    if (cmd.args().size() != 0)
    {
        std::cout << make_msg(msg_update_syntax()) << std::endl;
        return;
    }

    // check for invalid options (all options)
    std::vector<std::string> unknown_options;
    list_unknown_options(cmd, {}, unknown_options);
    if (unknown_options.size() > 0)
    {
        std::cout << make_msg(msg_unknown_options(cmd.name(), unknown_options)) << std::endl;
        return;
    }

    // backup buffer
    this->make_backup();
    this->search_buffer.resize(this->undo_buffer.size());

    // update values
    time_point<high_resolution_clock> t0 = high_resolution_clock::now();
    uint64_t count = this->update(nullptr, nullptr, 0);
    time_point<high_resolution_clock> t1 = high_resolution_clock::now();

    this->search_buffer.shrink_to_fit();
    std::cout << make_msg(msg_update_success(count, duration_cast<microseconds>(t1 - t0).count() / 1000.0)) << std::endl;
}

void Application::cmd_update_exact(const Command& cmd)
{
    using namespace std::chrono;

    // syntax check
    if (cmd.args().size() != 1)
    {
        std::cout << make_msg(msg_ue_syntax()) << std::endl;
        return;
    }

    // check for invalid options (all options)
    std::vector<std::string> unknown_options;
    list_unknown_options(cmd, {}, unknown_options);
    if (unknown_options.size() > 0)
    {
        std::cout << make_msg(msg_unknown_options(cmd.name(), unknown_options)) << std::endl;
        return;
    }

    // check for correct arguments
    bool is_hex = is_input_hex(cmd.args().at(0));
    if (!utility::is_string(this->cfg.type()))
    {
        if (utility::is_floating_point(this->cfg.type()) && !utility::is_floating_point(cmd.args().at(0)))
        {
            std::cout << make_msg(msg_not_dec(cmd.args().at(0), 1, cmd.name())) << std::endl;
            return;
        }
        else if (utility::is_integral(this->cfg.type()))
        {
            if (!is_hex && !utility::is_dec(cmd.args().at(0)))
            {
                std::cout << make_msg(msg_not_dec(cmd.args().at(0), 1, cmd.name())) << std::endl;
                return;
            }
            if (is_hex && !utility::is_hex(cmd.args().at(0)))
            {
                std::cout << make_msg(msg_not_hex(cmd.args().at(0), 1, cmd.name())) << std::endl;
                return;
            }
        }
    }

    // convert arguments
    size_t size = (utility::is_string(this->cfg.type())) ? cmd.args().at(0).size() : this->cfg.type_size();
    uint8_t in_value[size];
    utility::to_bytes(cmd.args().at(0), size, this->cfg.type(), is_hex, in_value);

    // backup buffer
    this->make_backup();
    this->search_buffer.resize(this->undo_buffer.size());

    // search for new values in buffer
    time_point<high_resolution_clock> t0 = high_resolution_clock::now();
    uint64_t count = this->update(in_value, in_value, size);
    time_point<high_resolution_clock> t1 = high_resolution_clock::now();

    this->search_buffer.shrink_to_fit();
    std::cout << make_msg(msg_update_success(count, duration_cast<microseconds>(t1 - t0).count() / 1000.0)) << std::endl;
}

void Application::cmd_update_range(const Command& cmd)
{
    using namespace std::chrono;

    // syntax check
    if (cmd.args().size() != 2)
    {
        std::cout << make_msg(msg_sr_syntax()) << std::endl;
        return;
    }

    // check for invalid options (all options)
    std::vector<std::string> unknown_options;
    list_unknown_options(cmd, {}, unknown_options);
    if (unknown_options.size() > 0)
    {
        std::cout << make_msg(msg_unknown_options(cmd.name(), unknown_options)) << std::endl;
        return;
    }

    // this command does not work with strings
    if (utility::is_string(this->cfg.type()))
    {
        std::cout << make_msg(msg_ur_string()) << std::endl;
        return;
    }

    // check for correct arguments
    bool is_hex[2];
    for (uint32_t i = 0; i < 2; i++)
    {
        is_hex[i] = is_input_hex(cmd.args().at(i));
        if (utility::is_floating_point(this->cfg.type()) && !utility::is_floating_point(cmd.args().at(i)))
        {
            std::cout << make_msg(msg_not_dec(cmd.args().at(i), i + 1, cmd.name())) << std::endl;
            return;
        }
        else if (utility::is_integral(this->cfg.type()))
        {
            if (!is_hex[i] && !utility::is_dec(cmd.args().at(i)))
            {
                std::cout << make_msg(msg_not_dec(cmd.args().at(i), i + 1, cmd.name())) << std::endl;
                return;
            }
            if (is_hex[i] && !utility::is_hex(cmd.args().at(i)))
            {
                std::cout << make_msg(msg_not_hex(cmd.args().at(i), i + 1, cmd.name())) << std::endl;
                return;
            }
        }
    }

    // convert arguments
    uint8_t in_value1[this->cfg.type_size()], in_value2[this->cfg.type_size()];
    utility::to_bytes(cmd.args().at(0), this->cfg.type_size(), this->cfg.type(), is_hex[0], in_value1);
    utility::to_bytes(cmd.args().at(1), this->cfg.type_size(), this->cfg.type(), is_hex[1], in_value2);

    // backup buffer
    this->make_backup();
    this->search_buffer.resize(this->undo_buffer.size());

    // search for new values in buffer
    time_point<high_resolution_clock> t0 = high_resolution_clock::now();
    uint64_t count = this->update(in_value1, in_value2, this->cfg.type_size());
    time_point<high_resolution_clock> t1 = high_resolution_clock::now();

    this->search_buffer.shrink_to_fit();
    std::cout << make_msg(msg_update_success(count, duration_cast<microseconds>(t1 - t0).count() / 1000.0)) << std::endl;
}

void Application::cmd_undo(const Command& cmd)
{
    using namespace std::chrono;

    // syntax check
    if (cmd.args().size() != 0)
    {
        std::cout << make_msg(msg_undo_syntax()) << std::endl;
        return;
    }

    // check for invalid options (all options)
    std::vector<std::string> unknown_options;
    list_unknown_options(cmd, {}, unknown_options);
    if (unknown_options.size() > 0)
    {
        std::cout << make_msg(msg_unknown_options(cmd.name(), unknown_options)) << std::endl;
        return;
    }

    // check if undo is aviable
    if (this->undo_buffer.size() > 0)
    {
        time_point<high_resolution_clock> t0 = high_resolution_clock::now();
        this->redo_buffer = this->search_buffer;
        this->search_buffer = std::move(this->undo_buffer);
        time_point<high_resolution_clock> t1 = high_resolution_clock::now();
        std::cout << make_msg(msg_undo_done(duration_cast<microseconds>(t1 - t0).count() / 1000.0)) << std::endl;
    }
    else
        std::cout << make_msg(msg_undo_none()) << std::endl;
}

void Application::cmd_redo(const Command& cmd)
{
    using namespace std::chrono;

    // syntax check
    if (cmd.args().size() != 0)
    {
        std::cout << make_msg(msg_redo_syntax()) << std::endl;
        return;
    }

    // check for invalid options (all options)
    std::vector<std::string> unknown_options;
    list_unknown_options(cmd, {}, unknown_options);
    if (unknown_options.size() > 0)
    {
        std::cout << make_msg(msg_unknown_options(cmd.name(), unknown_options)) << std::endl;
        return;
    }

    // check if redo is aviable
    if (this->redo_buffer.size() > 0)
    {
        time_point<high_resolution_clock> t0 = high_resolution_clock::now();
        this->undo_buffer = this->search_buffer;
        this->search_buffer = std::move(this->redo_buffer);
        time_point<high_resolution_clock> t1 = high_resolution_clock::now();
        std::cout << make_msg(msg_redo_done(duration_cast<microseconds>(t1 - t0).count() / 1000.0)) << std::endl;
    }
    else
        std::cout << make_msg(msg_redo_none()) << std::endl;
}

void Application::cmd_show(const Command& cmd)
{
    // syntax check
    if (cmd.args().size() != 2)
    {
        std::cout << make_msg(msg_show_syntax()) << std::endl;
        return;
    }

    // check for invalid options (all options)
    std::vector<std::string> unknown_options;
    list_unknown_options(cmd, {}, unknown_options);
    if (unknown_options.size() > 0)
    {
        std::cout << make_msg(msg_unknown_options(cmd.name(), unknown_options)) << std::endl;
        return;
    }

    // check for correct arguments
    if (!utility::is_dec(cmd.args().at(0)))
    {
        std::cout << make_msg(msg_not_dec(cmd.args().at(0), 1, cmd.name())) << std::endl;
        return;
    }
    if (!utility::is_dec(cmd.args().at(1)))
    {
        std::cout << make_msg(msg_not_dec(cmd.args().at(1), 2, cmd.name())) << std::endl;
        return;
    }

    // convert arguments
    uint32_t start, end;
    sscanf(cmd.args().at(0).c_str(), "%" PRIu32, &start);
    sscanf(cmd.args().at(1).c_str(), "%" PRIu32, &end); // end = amount
    end += start;

    // build table
    std::vector<std::string> entry(this->search_table.col_count());
    for (uint32_t i = start; i < end && i < this->search_buffer.table().size(); i++)
    {
        const Buffer::Element& e = this->search_buffer.table()[i];
        make_search_entry(e.pid, e.address, e.size, reinterpret_cast<uint8_t*>(e.data), entry);
        this->search_table.add(entry);
    }
    this->search_table.print();
    this->search_table.clear_entries();
}

void Application::cmd_show_live(const Command& cmd)
{
    // syntax check
    if (cmd.args().size() != 3)
    {
        std::cout << make_msg(msg_sl_syntax()) << std::endl;
        return;
    }

    // check for invalid options (all options)
    std::vector<std::string> unknown_options;
    list_unknown_options(cmd, {}, unknown_options);
    if (unknown_options.size() > 0)
    {
        std::cout << make_msg(msg_unknown_options(cmd.name(), unknown_options)) << std::endl;
        return;
    }

    // check for correct arguments
    if (!utility::is_dec(cmd.args().at(0)))
    {
        std::cout << make_msg(msg_not_dec(cmd.args().at(0), 1, cmd.name())) << std::endl;
        return;
    }
    if (!utility::is_dec(cmd.args().at(1)))
    {
        std::cout << make_msg(msg_not_dec(cmd.args().at(1), 2, cmd.name())) << std::endl;
        return;
    }
    if (!utility::is_dec(cmd.args().at(2)))
    {
        std::cout << make_msg(msg_not_dec(cmd.args().at(2), 3, cmd.name())) << std::endl;
        return;
    }

    // stop live memory view if it is currently running
    // setting pid to invalid is not necessary because this check is not needed further down in this command
    if (this->pid_live_memory != MEMORY_PID_INVALID)
    {
        this->process_handler.stop_process(this->pid_live_memory);
        std::cout << make_msg(msg_close_live_memory(this->pid_live_memory)) << std::endl;
    }

    // build command for live memory process
    std::stringstream live_memory_cmd;
    live_memory_cmd << LIVE_MEMORY_PROCESS_NAME << " " << cmd.args().at(0) << " " << cmd.args().at(1) << " " << cmd.args().at(2);

    // print addresses to file
    std::cout << make_msg(msg_sl_print_start(LIVE_MEMORY_FILE_PATH)) << std::endl;
    if (!utility::write_addresses_to_file(LIVE_MEMORY_FILE_PATH, this->search_buffer))
    {
        std::cout << make_msg(msg_sl_print_failure(LIVE_MEMORY_FILE_PATH)) << std::endl;
        return;
    }
    std::cout << make_msg(msg_sl_print_finish(LIVE_MEMORY_FILE_PATH)) << std::endl;

    // start live memory view process
    this->pid_live_memory = this->process_handler.start_process(LIVE_MEMORY_PROCESS_PATH, live_memory_cmd.str());
    if (this->pid_live_memory == MEMORY_PID_INVALID)
    {
        std::cout << make_msg(msg_sl_process_failure()) << std::endl;
        return;
    }
    std::cout << make_msg(msg_sl_process_success(this->pid_live_memory)) << std::endl;
}

void Application::cmd_dump(const Command& cmd)
{
    // syntax check
    if (cmd.args().size() != 4)
    {
        std::cout << make_msg(msg_dump_syntax()) << std::endl;
        return;
    }

    // check for invalid options (all options)
    std::vector<std::string> unknown_options;
    list_unknown_options(cmd, {}, unknown_options);
    if (unknown_options.size() > 0)
    {
        std::cout << make_msg(msg_unknown_options(cmd.name(), unknown_options)) << std::endl;
        return;
    }

    // check for correct arguments
    if (!utility::is_hex(cmd.args().at(0)))
    {
        std::cout << make_msg(msg_not_hex(cmd.args().at(0), 1, cmd.name())) << std::endl;
        return;
    }
    if (!utility::is_hex(cmd.args().at(1)))
    {
        std::cout << make_msg(msg_not_hex(cmd.args().at(1), 2, cmd.name())) << std::endl;
        return;
    }
    if (!utility::is_hex(cmd.args().at(2)))
    {
        std::cout << make_msg(msg_not_hex(cmd.args().at(1), 3, cmd.name())) << std::endl;
        return;
    }
    if (!utility::is_dec(cmd.args().at(3)))
    {
        std::cout << make_msg(msg_not_dec(cmd.args().at(3), 4, cmd.name())) << std::endl;
        return;
    }

    // check for open process
    if (!this->current_process.is_valid())
    {
        std::cout << make_msg(msg_close_process_failure()) << std::endl;    // reuse message
        return;
    }

    // stop memory dump if it is currently running
    // setting pid to invalid is not necessary because this check is not needed further down in this command
    if (this->pid_dump != MEMORY_PID_INVALID)
    {
        this->process_handler.stop_process(this->pid_dump);
        std::cout << make_msg(msg_close_hex_dump(this->pid_dump)) << std::endl;
    }

    // build memory dump command
    std::stringstream dump_cmd;
    dump_cmd << DUMP_PROCESS_NAME << " " << this->current_process.pid() << " " << cmd.args().at(0) << " " << cmd.args().at(1) << " " << cmd.args().at(2) << " " << cmd.args().at(3);

    // start memory dump
    this->pid_dump = this->process_handler.start_process(DUMP_PROCESS_PATH, dump_cmd.str());
    if (this->pid_dump == MEMORY_PID_INVALID)
        std::cout << make_msg(msg_dump_failure()) << std::endl;
    else
        std::cout << make_msg(msg_dump_success(this->pid_dump)) << std::endl;
}


bool Application::on_command(const Command& cmd)
{
    if (cmd.name() == "")                                           return true;
    if (cmd.name() == "exit")                                       return false;
    if (cmd.name() == "help")                                       this->cmd_help(cmd);
    else if (cmd.name() == "config")                                this->cmd_config(cmd);
    else if (cmd.name() == "list")                                  this->cmd_list(cmd);
    else if (cmd.name() == "info")                                  this->cmd_info(cmd);
    else if (cmd.name() == "open")                                  this->cmd_open(cmd);
    else if (cmd.name() == "close")                                 this->cmd_close(cmd);
    else if (cmd.name() == "clear"          || cmd.name() == "cls") this->cmd_clear(cmd);
    else if (cmd.name() == "read_single"    || cmd.name() == "rs")  this->cmd_read_single(cmd);
    else if (cmd.name() == "read_block"     || cmd.name() == "rb")  this->cmd_read_block(cmd);
    else if (cmd.name() == "search_exact"   || cmd.name() == "se")  this->cmd_search_exact(cmd);
    else if (cmd.name() == "search_range"   || cmd.name() == "sr")  this->cmd_search_range(cmd);
    else if (cmd.name() == "write_all"      || cmd.name() == "wa")  this->cmd_write_all(cmd);
    else if (cmd.name() == "write_single"   || cmd.name() == "ws")  this->cmd_write_single(cmd);
    else if (cmd.name() == "write_range"    || cmd.name() == "wr")  this->cmd_write_range(cmd);
    else if (cmd.name() == "update")                                this->cmd_update(cmd);
    else if (cmd.name() == "update_exact"   || cmd.name() == "ue")  this->cmd_update_exact(cmd);
    else if (cmd.name() == "update_range"   || cmd.name() == "ur")  this->cmd_update_range(cmd);
    else if (cmd.name() == "undo")                                  this->cmd_undo(cmd);
    else if (cmd.name() == "redo")                                  this->cmd_redo(cmd);
    else if (cmd.name() == "show")                                  this->cmd_show(cmd);
    else if (cmd.name() == "show_live"      || cmd.name() == "sl")  this->cmd_show_live(cmd);
    else if (cmd.name() == "dump")                                  this->cmd_dump(cmd);
    else                                                            std::cout << make_msg(msg_unknown_command(cmd.name())) << std::endl;
    return true;
}
