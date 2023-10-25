/**
* @file     utility.h
* @brief    Definition of utility functions.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "buffer.h"
#include <vector>

namespace memory
{
    namespace utility
    {
        /**
        * @brief Converts a variable from host to network byteorder.
        * @param[in] x: host byteorder variable
        * @return network byteorder variable
        */
        template<typename T>
        static T htons(T x) noexcept
        {
            T v;
            uint8_t b[sizeof(T)];

            uint32_t j = 0;
            for (int32_t i = sizeof(T) - 1; i >= 0; i--)
                b[j++] = (x >> i * 8) & 0xFF;

            // save tranalation from an byte array to a uint32_t type
            // Copilers should be able to optimize it in such a way
            // that it is only interpreted as another type.
            memcpy(&v, b, sizeof(T));
            return v;
        }

        /**
        * @brief Converts a variable from network to host byteorder.
        * @param[in] x: network byteorder variable
        * @return host byteorder variable
        */
        template<typename T>
        static T ntohs(T x) noexcept
        {
            T v;
            uint8_t b[sizeof(T)];

            uint32_t j = 0;
            for (uint32_t i = 0; i < sizeof(T); i++)
                b[j++] = (x >> i * 8) & 0xFF;

            memcpy(&v, b, sizeof(T));
            return v;
        }

        /**
        * @brief Converts a value to hexadecimal string with leading zeros.
        * @param[in] x: variable to convert
        * @param[out] str: hex string
        */
        template<typename T>
        static void to_hex_str(T x, std::string& str)
        {
            str.clear();
            constexpr static char table[] = "0123456789ABCDEF";
            for (int32_t i = sizeof(T) * 8 - 4; i >= 0; i -= 4)
                str.push_back(table[(x >> i) & 0x0F]);
        }

        /**
        * @brief Converts a value to hexadecimal string without leading zeros.
        * @param[in] x: variable to convert
        * @param[out] str: hex string
        */
        template<typename T>
        static void to_hex_str_no_zero(T x, std::string& str)
        {
            str.clear();
            constexpr static char table[] = "0123456789ABCDEF";
            for (int32_t i = sizeof(T) * 8 - 4; i >= 0; i -= 4)
                str.push_back(table[(x >> i) & 0x0F]);

            bool ready = false;
            auto iter = str.begin();
            for (; iter != str.end() && !ready; iter++)
            {
                if (*iter != '0')
                    ready = true;
            }
            str.erase(str.begin(), --iter);
        }

        /**
        * @brief Converts a value to decimal string.
        * @param[in] x: variable to convert
        * @param[out] str: hex string
        */
        template<typename T>
        static void to_dec_str(T x, std::string& str)
        {
            std::stringstream ss;
            ss << x;
            str = ss.str();
        }

        /**
        * @brief Writes all addresses of a buffer to a file.
        * @param[in] path: path to the file
        * @param[in] buff: buffer of the addresses that should be printed
        * @return 'true' if the operation was successful and
        *   'false' if the file could not be opened.
        */
        bool write_addresses_to_file(const std::string& path, const Buffer& buff);

        /**
        * @brief Reads address from a file and stores them into a vector.
        * @param[in] path: path to the file
        * @param[out] addresses: address storage vector
        * @return 'true' if the operation was successful and
        *   'false' if the file could not be opened.
        */
        bool read_addresses_from_file(const std::string& path, std::vector<memory::Buffer::Element>& elements);

        /**
        * @brief Converts a value which is stored in a byte array into a string.
        * @param[in] buff: buffer where the data is stored
        * @param[in] size: size of the value
        * @param[in] type: type of the value
        * @param[in] hex: should the value be converted to hexadecimal
        * @param[out] str: output string
        */
        void to_string(const uint8_t* buff, size_t size, memory::type_t type, bool hex, std::string& str);

        /**
        * @brief Converts a string into a value which is stored in a byte array.
        * @param[in] str: input string
        * @param[in] size: size of the value
        * @param[in] type: type of the value
        * @param[in] hex: should the value be converted to hexadecimal
        * @param[out] buff: bytes of the value
        */
        void to_bytes(const std::string& str, size_t size, memory::type_t type, bool hex, uint8_t* buff);

        /**
        * @brief Check if a string matches a decimal non floating point value.
        * @param[in] str: string to check
        * @return 'true' if the string matches a decimal non floating point value and 'false' if not.
        */
        inline bool is_dec(const std::string& str) noexcept 
        { return (!str.empty() && str.find_first_not_of("-0123456789") == std::string::npos); }

        /**
        * @brief Check if a string matches a hexadecimal value.
        * @param[in] str: string to check
        * @return 'true' if the string matches a hexadecimal value and 'false' if not.
        */
        bool is_hex(std::string str) noexcept;

        /**
        * @brief Check if a string matches a floating point value.
        * @param[in] str: string to check
        * @return 'true' if the string matches a floating point value and 'false' if not.
        */
        bool is_floating_point(std::string str) noexcept;

        /**
        * @brief Checks if the type is a floating point type.
        * @param[in] type: type to check
        * @return 'true' if the type is a floating point type and 'false' if not.
        */
        inline bool is_floating_point(type_t type) noexcept 
        { return (type == MEMORY_TYPE_FLOAT || type == MEMORY_TYPE_DOUBLE); }

        /**
        * @brief Checks if the type is a integral type.
        * @param[in] type: type to check
        * @return 'true' if the type is s string and 'false' if not.
        */
        inline bool is_string(type_t type) noexcept 
        { return (type == MEMORY_TYPE_STRING); }

        /**
        * @brief Checks if the type is an integral type.
        * @param[in] type: type to check
        * @return 'true' if the type is an integral type and 'false' if not.
        */
        inline bool is_integral(type_t type) noexcept 
        { return !(is_floating_point(type) || is_string(type)); }

        /**
        * @brief Converts the type to a string.
        * @param[in] type: type to convert
        * @param[out] str: output string
        */
        void strtype(memory::type_t type, std::string& str);

        /**
        * @brief Converts the type to extended information.
        * @param[in] type: type to convert
        * @param[out] str: output string
        */
        void strtype_ext(memory::type_t type, std::string& str);
    }
}

