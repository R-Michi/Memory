/**
* @file     utility.cpp
* @brief    Implemenation of utility functions.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#define _CRT_SECURE_NO_WARNINGS
#include "utility.h"
#include <fstream>
#include <sstream>
#include <inttypes.h>

using namespace memory;

void to_lover_case(std::string& str) noexcept;

void to_lover_case(std::string& str) noexcept
{
    for (char& c : str)
    {
        if (c >= 'A' && c <= 'Z')
            c += 0x20;
    }
}


bool utility::write_addresses_to_file(const std::string& path, const Buffer& buff)
{
    std::fstream file(path, std::ios::out | std::ios::trunc | std::ios::binary);
    if (!file) return false;

    // write the number of elemets to file
    uint32_t count = htons<uint32_t>(static_cast<uint32_t>(buff.table().size()));
    file.write(reinterpret_cast<const char*>(&count), sizeof(uint32_t));

    // write elements to file
    for (const memory::Buffer::Element& e : buff.table())
    {
        pid_t pid = htons<pid_t>(e.pid);
        uint64_t addr = htons<uint64_t>(e.address);
        uint64_t size = htons<uint64_t>(e.size);
        uint8_t type = static_cast<uint8_t>(e.type);

        file.write(reinterpret_cast<const char*>(&pid), sizeof(pid_t));
        file.write(reinterpret_cast<const char*>(&addr), sizeof(uint64_t));
        file.write(reinterpret_cast<const char*>(&size), sizeof(uint64_t));
        file.write(reinterpret_cast<const char*>(&type), sizeof(uint8_t));
    }

    file.close();
    return true;
}

bool utility::read_addresses_from_file(const std::string& path, std::vector<memory::Buffer::Element>& elements)
{
    std::fstream file(path, std::ios::in | std::ios::binary);
    if (!file) return false;

    // read the number of elements
    uint32_t count;
    file.read(reinterpret_cast<char*>(&count), sizeof(uint32_t));
    count = htons<uint32_t>(count);

    // read elements from file
    for (uint32_t i = 0; i < count; i++)
    {
        pid_t pid;
        uint64_t addr, size;
        uint8_t type;   // must not have a byte order transition since it is only one byte in size

        file.read(reinterpret_cast<char*>(&pid), sizeof(pid_t));
        file.read(reinterpret_cast<char*>(&addr), sizeof(uint64_t));
        file.read(reinterpret_cast<char*>(&size), sizeof(uint64_t));
        file.read(reinterpret_cast<char*>(&type), sizeof(uint8_t));

        pid = htons<pid_t>(pid);
        addr = htons<uint64_t>(addr);
        size = htons<uint64_t>(size);

        Buffer::Element e;
        e.pid = pid;
        e.address = addr;
        e.size = size;
        e.type = static_cast<type_t>(type);
        e.data = nullptr;   // data is read manually

        elements.push_back(e);
    }

    file.close();
    return true;
}

void utility::to_string(const uint8_t* buff, size_t size, type_t type, bool hex, std::string& str)
{
    std::stringstream ss;

    str.clear();
    switch (type)
    {
    case MEMORY_TYPE_INT8:
    {
        int8_t x;
        memcpy(&x, buff, sizeof(int8_t));

        if (hex)
            to_hex_str_no_zero<int8_t>(x, str);
        else
        {
            ss << static_cast<int16_t>(x);  // int8_t is interpreted as char-type
            str = ss.str();
        }
        break;
    }
    case MEMORY_TYPE_UINT8:
    {
        uint8_t x;
        memcpy(&x, buff, sizeof(uint8_t));

        if (hex)
            to_hex_str_no_zero<uint8_t>(x, str);
        else
        {
            ss << static_cast<uint16_t>(x);  // uint8_t is interpreted as char-type
            str = ss.str();
        }
        break;
    }
    case MEMORY_TYPE_INT16:
    {
        int16_t x;
        memcpy(&x, buff, sizeof(int16_t));

        if (hex)
            to_hex_str_no_zero<int16_t>(x, str);
        else
        {
            ss << x;
            str = ss.str();
        }
        break;
    }
    case MEMORY_TYPE_UINT16:
    {
        uint16_t x;
        memcpy(&x, buff, sizeof(uint16_t));

        if (hex)
            to_hex_str_no_zero<uint16_t>(x, str);
        else
        {
            ss << x;
            str = ss.str();
        }
        break;
    }
    case MEMORY_TYPE_INT32:
    {
        int32_t x;
        memcpy(&x, buff, sizeof(int32_t));

        if (hex)
            to_hex_str_no_zero<int32_t>(x, str);
        else
        {
            ss << x;
            str = ss.str();
        }
        break;
    }
    case MEMORY_TYPE_UINT32:
    {
        uint32_t x;
        memcpy(&x, buff, sizeof(uint32_t));

        if (hex)
            to_hex_str_no_zero<uint32_t>(x, str);
        else
        {
            ss << x;
            str = ss.str();
        }
        break;
    }
    case MEMORY_TYPE_INT64:
    {
        int64_t x;
        memcpy(&x, buff, sizeof(int64_t));

        if (hex)
            to_hex_str_no_zero<int64_t>(x, str);
        else
        {
            ss << x;
            str = ss.str();
        }
        break;
    }
    case MEMORY_TYPE_UINT64:
    {
        uint64_t x;
        memcpy(&x, buff, sizeof(uint64_t));

        if (hex)
            to_hex_str_no_zero<uint64_t>(x, str);
        else
        {
            ss << x;
            str = ss.str();
        }
        break;
    }
    case MEMORY_TYPE_FLOAT:
    {
        float x;
        uint32_t x2;
        memcpy(&x, buff, sizeof(float));
        memcpy(&x2, buff, sizeof(uint32_t));

        if (hex)
            to_hex_str_no_zero<uint32_t>(x2, str);
        else
        {
            ss << x;
            str = ss.str();
        }
        break;
    }
    case MEMORY_TYPE_DOUBLE:
    {
        double x;
        uint64_t x2;
        memcpy(&x, buff, sizeof(double));
        memcpy(&x2, buff, sizeof(uint64_t));

        if (hex)
            to_hex_str_no_zero<uint64_t>(x2, str);
        else
        {
            ss << x;
            str = ss.str();
        }
        break;
    }
    case MEMORY_TYPE_STRING:
    {
        str.resize(size);
        memcpy(str.data(), buff, size);
        break;
    }
    default:
        break;
    }
}

void utility::to_bytes(const std::string& str, size_t size, memory::type_t type, bool hex, uint8_t* buff)
{
    if (type == MEMORY_TYPE_INT8 || type == MEMORY_TYPE_UINT8)
    {
        uint8_t x;
        sscanf(str.c_str(), ((hex) ? "%" PRIx8 : ("%" PRIu8)), &x);
        memcpy(buff, &x, sizeof(uint8_t));
    }
    else if (type == MEMORY_TYPE_INT16 || type == MEMORY_TYPE_UINT16)
    {
        uint16_t x;
        sscanf(str.c_str(), ((hex) ? "%" PRIx16 : ("%" PRIu16)), &x);
        memcpy(buff, &x, sizeof(uint16_t));
    }
    else if (type == MEMORY_TYPE_INT32 || type == MEMORY_TYPE_UINT32)
    {
        uint32_t x;
        sscanf(str.c_str(), ((hex) ? "%" PRIx32 : ("%" PRIu32)), &x);
        memcpy(buff, &x, sizeof(uint32_t));
    }
    else if (type == MEMORY_TYPE_INT64 || type == MEMORY_TYPE_UINT64)
    {
        uint64_t x;
        sscanf(str.c_str(), ((hex) ? "%" PRIx64 : ("%" PRIu64)), &x);
        memcpy(buff, &x, sizeof(uint64_t));
    }
    else if (type == MEMORY_TYPE_FLOAT)
    {
        float x;
        sscanf(str.c_str(), "%f", &x);
        memcpy(buff, &x, sizeof(float));
    }
    else if (type == MEMORY_TYPE_DOUBLE)
    {
        double x;
        sscanf(str.c_str(), "%lf", &x);
        memcpy(buff, &x, sizeof(double));
    }
    else if (type == MEMORY_TYPE_STRING)
    {
        memcpy(buff, str.c_str(), size);
    }
}

bool utility::is_hex(std::string str) noexcept
{
    if (str.substr(0, 2) == "0x")
        str.erase(str.begin(), str.begin() + 2);
    return (!str.empty() && str.find_first_not_of("0123456789ABCDEFabcdef") == std::string::npos);
}

bool utility::is_floating_point(std::string str) noexcept
{
    to_lover_case(str);
    if (str == "nan" || str == "inf") return true;
    return (!str.empty() && str.find_first_not_of("-0123456789.") == std::string::npos);
}

void utility::strtype(type_t type, std::string& str)
{
    switch (type)
    {
    case MEMORY_TYPE_INT8:      str = "int8";           break;
    case MEMORY_TYPE_UINT8:     str = "uint8";          break;
    case MEMORY_TYPE_INT16:     str = "int16";          break;
    case MEMORY_TYPE_UINT16:    str = "uint16";         break;
    case MEMORY_TYPE_INT32:     str = "int32";          break;
    case MEMORY_TYPE_UINT32:    str = "uint32";         break;
    case MEMORY_TYPE_INT64:     str = "int64";          break;
    case MEMORY_TYPE_UINT64:    str = "uint64";         break;
    case MEMORY_TYPE_FLOAT:     str = "float";          break;
    case MEMORY_TYPE_DOUBLE:    str = "double";         break;
    case MEMORY_TYPE_STRING:    str = "string";         break;
    case MEMORY_TYPE_VOID:      str = "void";           break;
    default:                    str = "unknown type";   break;
    }
}

void utility::strtype_ext(memory::type_t type, std::string& str)
{
    switch (type)
    {
    case MEMORY_TYPE_INT8:      str = "1-Byte signed integer";          break;
    case MEMORY_TYPE_UINT8:     str = "1-Byte unsigned integer";        break;
    case MEMORY_TYPE_INT16:     str = "2-Byte signed integer";          break;
    case MEMORY_TYPE_UINT16:    str = "2-Byte unsigned integer";        break;
    case MEMORY_TYPE_INT32:     str = "4-Byte signed integer";          break;
    case MEMORY_TYPE_UINT32:    str = "4-Byte unsigned integer";        break;
    case MEMORY_TYPE_INT64:     str = "8-Byte signed integer";          break;
    case MEMORY_TYPE_UINT64:    str = "8-Byte unsigned integer";        break;
    case MEMORY_TYPE_FLOAT:     str = "4-Byte floating point IEEE 754"; break;
    case MEMORY_TYPE_DOUBLE:    str = "8-Byte floating point IEEE 754"; break;
    case MEMORY_TYPE_STRING:    str = "string";                         break;
    case MEMORY_TYPE_VOID:      str = "void";                           break;
    default:                    str = "unknown type";                   break;
    }
}
