/**
* @file     live_memory/main.cpp
* @brief    Main function of live memory sub-program.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#define _CRT_SECURE_NO_WARNINGS
#define ELEMENT_FILE_PATH   "live_memory/elements.dat"
#define MAX_BUFFER_SIZE     2048

#include "../src/memory/memory.h"
#include <inttypes.h>
#include <thread>

// syntax: live_memory <begin entry> <end entry> <update speed [ms]>
int main(const int argc, const char* const * const argv)
{
    using namespace std::chrono;

    // check for corrent arguments
    if (argc != 4) return -1;

    // convert parameters
    uint32_t update_speed, begin_entry, end_entry, amount;

    sscanf(argv[1], "%" PRIu32, &begin_entry);
    sscanf(argv[2], "%" PRIu32, &amount);
    sscanf(argv[3], "%" PRIu32, &update_speed);
    end_entry = begin_entry + amount;

    // initialize table
    memory::Table table;
    table.add_column("PID");
    table.add_column("Address");
    table.add_column("Type");
    table.add_column("Size");
    table.add_column("Decimal");
    table.add_column("Hexadecimal");

    // read elements from file
    std::vector<memory::Buffer::Element> elements;
    if (!memory::utility::read_addresses_from_file(ELEMENT_FILE_PATH, elements))
        return -2;

    time_point<high_resolution_clock> tp;
    std::vector<std::string> entry(table.col_count());
    memory::Process cur_p;
    uint8_t buff[MAX_BUFFER_SIZE];

    // NOTE: This program is not returning, it gets started and terminated by the memory application.
    // However, it is cleaned up correctly as if it would return.
    while (true)
    {
        tp = high_resolution_clock::now() + milliseconds(update_speed);
        table.clear_entries();

        // iterate through every element
        for (uint32_t i = begin_entry; i < end_entry && i < elements.size(); i++)
        {
            const memory::Buffer::Element& e = elements[i];

            // find new process if old pid is different to the current pid
            if (e.pid != cur_p.pid() || !cur_p.is_valid())
            {
                cur_p.close();                  // close old process
                cur_p.init("", e.pid, 0, 0);    // init current process with stored PID, all other information are irelevent in this context, and is more efficient as Process::find_process
                if (!cur_p.open()) continue;    // open new process to read data from
            }

            size_t size = (e.size > MAX_BUFFER_SIZE) ? MAX_BUFFER_SIZE : e.size;
            uint64_t size_u64 = static_cast<uint64_t>(size); // size_t is not always 64 bit, its size is implementation defined

            // read the current value
            cur_p.read(e.address, size, buff);

            // build entry
            memory::utility::to_dec_str<memory::pid_t>(cur_p.pid(), entry[0]);
            memory::utility::to_hex_str<memory::address_t>(e.address, entry[1]);
            memory::utility::strtype(e.type, entry[2]);
            memory::utility::to_string(reinterpret_cast<const uint8_t*>(&size_u64), sizeof(uint64_t), memory::MEMORY_TYPE_UINT64, false, entry[3]);
            memory::utility::to_string(buff, size, e.type, false, entry[4]);
            memory::utility::to_string(buff, size, e.type, true, entry[5]);

            table.add(entry);
        }

        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 });
        table.print();
        std::this_thread::sleep_until(tp);
    }

    return 0;
}
