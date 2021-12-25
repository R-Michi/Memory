/**
* @file     config.h
* @brief    Config-class definition.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "../memory/types.h"

namespace memory
{
    namespace app
    {
        class Config
        {
        private:
            constexpr static type_t DEFAULT_TYPE                = MEMORY_TYPE_INT8;
            constexpr static uint16_t DEFAULT_ALIGNMENT         = 1;
            constexpr static address_t DEFAULT_START_ADDRESS    = 0x0;
            constexpr static address_t DEFAULT_END_ADDRESS      = 0x8000000000000000;
            constexpr static address_t MAX_END_ADDRESS          = 0xFFFFFFFFFFFFFFFF;
            constexpr static size_t DEFAULT_SEARCH_SPLIT_SIZE   = 0x100000;     // 1MB
            constexpr static size_t DEFAULT_SEARCH_LIMIT_SIZE   = 0x8000000;    // 128MB
            constexpr static char CONFIG_PATH[]                 = "config.txt";

            type_t _type;
            size_t _type_size;
            uint16_t _alignment;
            address_t _start;
            address_t _end;
            size_t _search_split_size;
            size_t _search_limit_size;

        public:
            Config(void) noexcept;
            ~Config(void);

            inline void set_default_type(void) noexcept                 { this->set_type(DEFAULT_TYPE); }
            inline void set_default_aligment(void) noexcept             { this->set_aligment(DEFAULT_ALIGNMENT); }
            inline void set_default_start_address(void) noexcept        { this->set_start_address(DEFAULT_START_ADDRESS); }
            inline void set_default_end_address(void) noexcept          { this->set_end_address(DEFAULT_END_ADDRESS); }
            inline void set_default_search_split_size(void) noexcept    { this->set_search_split_size(DEFAULT_SEARCH_SPLIT_SIZE); }
            inline void set_default_search_limit_size(void) noexcept    { this->set_search_limit_size(DEFAULT_SEARCH_LIMIT_SIZE); }
            void set_defaults(void) noexcept;

            void set_type(type_t type) noexcept;
            void set_aligment(uint16_t alignment) noexcept       { this->_alignment = alignment; }
            void set_start_address(address_t address) noexcept   { this->_start = address; }
            void set_end_address(address_t address) noexcept     { this->_end = (address > MAX_END_ADDRESS) ? MAX_END_ADDRESS : address; }
            void set_search_split_size(size_t size) noexcept     { this->_search_split_size = size; }
            void set_search_limit_size(size_t size) noexcept     { this->_search_limit_size = size; }

            type_t type(void) const noexcept                     { return this->_type; }
            size_t type_size(void) const noexcept                { return this->_type_size; }
            uint16_t alignment(void) const noexcept              { return this->_alignment; }
            address_t start_address(void) const noexcept         { return this->_start; }
            address_t real_start_address(void) const noexcept    { return this->_start + ((this->_start % this->_alignment > 0) ? (this->_alignment - this->_start % this->_alignment) : 0);}
            address_t real_end_address(void) const noexcept      { return ((this->_end % this->_alignment > 0) ? (this->_end - this->_end % this->_alignment) : this->_end) - this->_type_size;}
            address_t end_address(void) const noexcept           { return this->_end; }
            size_t search_split_size(void) const noexcept        { return this->_search_split_size; }
            size_t search_limit_size(void) const noexcept        { return this->_search_limit_size; }

            bool save(void) const;
            bool load(void);
        };
    }
}