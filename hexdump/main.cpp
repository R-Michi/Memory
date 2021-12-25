/**
* @file     hexdump/main.cpp
* @brief    Main function of hexdump sub-program.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#define _CRT_SECURE_NO_WARNINGS

#include "../src/memory/memory.h"
#include <thread>
#include <inttypes.h>

/* 
* Syntax: hexdump.exe <pid> <begin> <range> <byte width> <update speed [ms]>
* 
* INFO:
*   <pid>               : DECIMAL
*   <begin>             : HEXADECIMAL
*   <range>             : HEXADECIMAL
*   <byte width>        : HEXADECIMAL
*   <update speed [ms]> : DECIMAL
*/

void key_thread_func(const std::atomic_bool* running, std::atomic<memory::address_t>* begin, uint32_t width);

int main(const int argc, const char* const * const argv)
{
    using namespace std::chrono;

    // check correct argument length
    if (argc != 6) return -1;   // -1: invalid argument length

    // convert arguments
    memory::pid_t pid;
    std::atomic<memory::address_t> begin;
    memory::address_t tmp_begin, range;
    uint32_t width, tmp_update_speed;
    milliseconds update_speed;
    std::atomic_bool running = true;

    sscanf(argv[1], "%lu", &pid);
    sscanf(argv[2], "%" PRIx64, &tmp_begin);
    sscanf(argv[3], "%" PRIx64, &range);
    sscanf(argv[4], "%" PRIx32, &width);
    sscanf(argv[5], "%" PRIu32, &tmp_update_speed);
    begin = tmp_begin;
    update_speed = milliseconds(tmp_update_speed);

    // open process
    memory::Process proc;
    proc.init("", pid, 0, 0);     // other information are irrelevent
    if (!proc.open()) return -2;    // -2: failed to open process

    // create buffer
    uint8_t* buff = new uint8_t[range];

    // initialize hex dump
    memory::HexDump dump(range / width, width);

    // start key handler thread
    std::thread key_handler(key_thread_func, &running, &begin, width);

    // NOTE: This program is not returning, it gets started and terminated by the memory application.
    // However, it is cleaned up correctly as if it would return.
    time_point<high_resolution_clock> tp;
    while (true)
    {
        tp = high_resolution_clock::now() + update_speed;

        // read memory from process
        if (proc.read(begin, range, buff) == 0)
            memset(buff, 0xFF, range);

        // build hex dump table
        dump.front();
        for (uint32_t i = 0; i < dump.line_count(); i++)
        {
            dump.set(begin + i * width, buff + i * width);
            dump.inc();
        }

        // print table
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 });
        dump.print();

        // wait for tick to end
        std::this_thread::sleep_until(tp);
    }

    // stop key handler thread
    running = false;
    key_handler.join();

    // destroy buffer
    delete[] buff;
    return 0;
}

void key_thread_func(const std::atomic_bool* running, std::atomic<memory::address_t>* begin, uint32_t width)
{
    using namespace std::chrono;

    const milliseconds tick_time(20);
    time_point<high_resolution_clock> tp;
    HWND this_window = GetConsoleWindow();

    while (*running)
    {
        tp = high_resolution_clock::now() + tick_time;

        // for any key actions this console must be in foreground
        if (GetForegroundWindow() == this_window)
        {
            if (GetAsyncKeyState(VK_UP))
                *begin += width;
            else if (GetAsyncKeyState(VK_DOWN))
                *begin -= width;
        }

        std::this_thread::sleep_until(tp);
    }
}
