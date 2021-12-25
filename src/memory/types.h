/**
* @file     types.h
* @brief    Type definitions.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include <Windows.h>
#include <cstdint>

#define MEMORY_NULL_HANDLE      nullptr
#define MEMORY_INVALID_HANDLE   INVALID_HANDLE_VALUE
#define MEMORY_PID_INVALID      0


namespace memory
{
    using pid_t = DWORD;
    using process_t = HANDLE;
    using job_t = HANDLE;
    using window_t = HWND;
    using address_t = uint64_t;

    enum type_t : uint8_t
    {
        MEMORY_TYPE_VOID = 0x0,
        MEMORY_TYPE_INT8 = 0x1,
        MEMORY_TYPE_UINT8 = 0x2,
        MEMORY_TYPE_INT16 = 0x3,
        MEMORY_TYPE_UINT16 = 0x4,
        MEMORY_TYPE_INT32 = 0x5,
        MEMORY_TYPE_UINT32 = 0x6,
        MEMORY_TYPE_INT64 = 0x7,
        MEMORY_TYPE_UINT64 = 0x8,
        MEMORY_TYPE_FLOAT = 0x9,
        MEMORY_TYPE_DOUBLE = 0xA,
        MEMORY_TYPE_STRING = 0xB
    };

    struct MemoryInfo
    {
        address_t base;
        address_t size;
    };
}
