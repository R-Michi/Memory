/**
* @file     buffer.h
* @brief    Definition of the Buffer-class. This buffer stores data that is read from memory.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "buffer.h"
#include "types.h"
#include <vector>
#include <stdexcept>

namespace memory
{
    class out_of_limit : public std::out_of_range
    {
    public:
        out_of_limit(void) : out_of_range("Size of memory::Buffer exeeds the storage limit.") {};
    };

    class Buffer
    {
    public:
        struct Element
        {
            pid_t pid;
            uint64_t address;
            size_t size;
            type_t type;
            void* data;
        };

    private:
        void* _begin, * _end, * _eob;
        size_t _size, _cap, _limit;
        std::vector<Element> _table;

        /**
        * @brief Adds a value to a void pointer.
        * @param[in] vp: void pointer 
        * @param[in] x: value to add
        * @return result void pointer
        */
        static inline void* addvp(void* vp, uint64_t x) noexcept;

        /** @brief Updates the list to data references after reallocate. */
        void update_references(void) noexcept;

        /**
        * @brief Allocates memory.
        * @param[in] size: size in bytes to allocate
        * @throw bad_array_new_length exception if memory allocation failed
        */
        void allocate(size_t size);

        /**
        * @brief Reallocates memory.
        * @param[in] new_size: new size to allocate
        * @throw bad_array_new_length exception if memory allocation failed
        */
        void reallocate(size_t new_size);

    public:
        /**
        * @param[in] limit: maximum buffer storage limit
        * @param[in] size: preallocated memory size in bytes
        * @throw bad_array_new_length exception if memory allocation failed
        * @throw out_of_limit if the buffer's storage size exeeds the storage limit
        */
        explicit Buffer(size_t limit = 0, size_t size = 0);

        /** 
        * @brief Copy constructor 
        * @param[in] buff: buffer to copy
        * @throw bad_array_new_length exception if memory allocation failed
        * @throw out_of_limit if the buffer's storage size exeeds the storage limit
        */
        Buffer(const Buffer& buff);

        /** 
        * @brief Move constructor
        * @param[in] buff: buffer to move
        * @throw bad_array_new_length exception if memory allocation failed
        * @throw out_of_limit if the buffer's storage size exeeds the storage limit
        */
        Buffer(Buffer&& buff);

        /** @brief Destructor */
        virtual ~Buffer(void) { this->clear(); }

        /**
        * @brief Pushes data to the buffer.
        * @param[in] address: address of read memory
        * @param[in] size: size of the read value
        * @param[in] type: type of the value
        * @param[in] data: read value
        * @throw bad_array_new_length exception if memory allocation failed
        * @throw out_of_limit if the buffer's storage size exeeds the storage limit
        */
        void push(pid_t pid, uint64_t address, size_t size, type_t type, const void* data);

        /**
        * @brief Resize buffer to a new size.
        * @param[in] new_size: new buffer size
        * @throw bad_array_new_length exception if memory allocation failed
        * @throw out_of_limit if the buffer's storage size exeeds the storage limit
        */
        bool resize(size_t new_size);

        /** 
        * @brief Shrinks the buffer to the minimum required size. 
        * @throw bad_array_new_length exception if memory allocation failed
        */
        void shrink_to_fit(void);

        /** @brief Cleares the buffer. */
        void clear(void) noexcept;

        /**
        * @brief Sets the limit of the buffer storage.
        * @param[in] limit: buffer limit
        * @throw out_of_limit exception if limit is smaller than the buffer's capacity
        */
        void set_limit(size_t limit);

        /** @return the buffer's used storage size in bytes */
        size_t size(void)       const noexcept { return this->_size; }

        /** @return the buffer's allocated capacity (size) in bytes */
        size_t capacity(void)   const noexcept { return this->_cap; }

        /** @return the buffer's storage size limit in bytes */
        size_t limit(void)      const noexcept { return this->_limit; }

        /** @return a list of all elements */
        std::vector<Element>& table(void)         noexcept          { return this->_table; }
        const std::vector<Element>& table(void)   const noexcept    { return this->_table; }

        /** 
        * @brief copies a buffer 
        * @param[in] buff: buffer to copy
        * @return reference to this buffer
        * @throw bad_array_new_length exception if memory allocation failed
        * @throw out_of_limit if the buffer's storage size exeeds the storage limit
        */
        Buffer& copy(const Buffer& buff);

        /**
        * @brief moves a buffer
        * @param[in] buff: buffer to move
        * @return reference to this buffer
        * @throw bad_array_new_length exception if memory allocation failed
        * @throw out_of_limit if the buffer's storage size exeeds the storage limit
        */
        Buffer& move(Buffer& buff);

        /**
        * @brief copies a buffer
        * @param[in] buff: buffer to copy
        * @return reference to this buffer
        * @throw bad_array_new_length exception if memory allocation failed
        * @throw out_of_limit if the buffer's storage size exeeds the storage limit
        */
        Buffer& operator= (const Buffer& buff);

        /**
        * @brief moves a buffer
        * @param[in] buff: buffer to move
        * @return reference to this buffer
        * @throw bad_array_new_length exception if memory allocation failed
        * @throw out_of_limit if the buffer's storage size exeeds the storage limit
        */
        Buffer& operator= (Buffer&& buff);
    };
}