/**
* @file     table.h
* @brief    Definitions of Table-class.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include <vector>
#include <list>
#include <string>

namespace memory
{
    class Table
    {
    private:
        std::vector<std::string> cols;
        std::list<std::vector<std::string>> entries;

    protected:
        using table_iterator = std::list<std::vector<std::string>>::iterator;
        using const_table_iterator = std::list<std::vector<std::string>>::const_iterator;

        constexpr static char CORNER_CHAR = '+';
        constexpr static char HORIZONTAL_CHAR = '-';
        constexpr static char VERTICAL_CHAR = '|';

        /**
        * @brief Returns the maximum column width.
        * @param[out] width: max column width
        */
        void get_max_width(uint32_t* width) const noexcept;

        /**
        * @brief Generates a line to print from an entry.
        * @param[in] width: width of the column
        * @param[in] entry: the entry to print
        * @param[out] line: line-string
        */
        void get_line(const uint32_t* width, const std::vector<std::string>& entry, std::string& line) const noexcept;

        /**
        * @brief Generates a line to separate sections of the table.
        * @param[in] width: width of the column
        * @param[out] line: line-string
        */
        void get_separator_line(const uint32_t* width, std::string& line) const noexcept;

        /** @return All entries. */
        std::list<std::vector<std::string>>& get_entries(void) noexcept;

    public:
        Table(void) noexcept = default;
        virtual ~Table(void) = default;

        /**
        * @brief Adds a column to the table.
        * @param[in] title: title of the column
        */
        void add_column(const std::string& title) noexcept;

        /**
        * @brief Adds a new entry at the end of the table.
        * @param[in] entry: entry for every column
        */
        void add(const std::vector<std::string>& entry) noexcept;

        /**
        * @brief Removes an entry.
        * @param[in] iter: iterator to the element to remove
        */
        void remove(table_iterator iter) noexcept;
        void remove(const_table_iterator iter) noexcept;

        /** @return Number of columns. */
        uint32_t col_count(void) const noexcept;

        /** @return Number of entries. */
        uint32_t entry_count(void) const noexcept;

        /** @return An iterator to the first entry. */
        table_iterator begin(void) noexcept;
        const_table_iterator begin(void) const noexcept;

        /** @return An iterator AFTER the last entry. */
        table_iterator end(void) noexcept;
        const_table_iterator end(void) const noexcept;

        /** @brief Cleares the whole table. */
        void clear(void) noexcept;

        /** @brief Cleares all entries. */
        void clear_entries(void) noexcept;

        /** @brief Prints the table. */
        virtual void print(void) const noexcept;

        /** 
        * @brief Prints the table to a file, in binary.
        * @param[in] path: path to the file
        * @return 'true' if everything was successful and
        *   'false' if the file could not be opened.
        */
        virtual bool print_to_file(const std::string& path) const noexcept;

        /**
        * @brief Reads the data from the file.
        * @param[in] path: path to the file
        * @return 'true' if everything was successful and
        *   'false' if the file could not be opened.
        */
        virtual bool read_from_file(const std::string& path) noexcept;
    };

    class HexDump : protected Table
    {
    private:
        uint32_t _width;
        table_iterator iter;
        std::string init_hex_string;

        /** @brief Initializes 'init_hex_string' width 0-bytes. */
        void init_hex(void) noexcept;

    protected:
        constexpr static char NULLPTR_STR[] = "0000000000000000";   // address: 0x0 in string format

        /**
        * @brief Encodes bytes into a string.
        * @param[in] bytes: bytes to encode
        * @param[out] str: encoded string
        */
        void encode_hex(const uint8_t* bytes, std::string& str) noexcept;

        /**
        * @brief Encodes an address into a string.
        * @param[in] address: address to encode
        * @param[out] str: encoded string
        */
        void encode_addr(uint64_t address, std::string& str) noexcept;

    public:
        HexDump(void) noexcept;
        explicit HexDump(uint32_t lines, uint32_t width = 0x10);
        virtual ~HexDump(void);

        /**
        * @brief Sets the bytes at the internal iterator.
        * @param[in] bytes: bytes to set
        */
        void set(const uint8_t* bytes) noexcept;

        /**
        * @brief Sets the bytes and the address at the internal iterator.
        * @param[in] address: address to set
        * @param[in] bytes: bytes to set
        */
        void set(uint64_t address, const uint8_t* bytes) noexcept;

        /** @brief Scrolls the memory-table up. */
        void scroll_up(void) noexcept;

        /** @brief Scrolls the memory-table down. */
        void scroll_down(void) noexcept;

        /** @brief Increments the internal iterator. */
        void inc(void) noexcept;

        /** @brief Decrements the internal iterator. */
        void dec(void) noexcept;

        /** @brief Sets the internal iterator to the begin. */
        void front(void) noexcept;

        /** @brief Sets the internal iterator to the end. */
        void back(void) noexcept;

        /**
        * @brief Resizes the memory-table.
        * @param[in] lines: new line count
        * @param[in] width: new width
        */
        void resize(uint32_t lines, uint32_t width);

        /** @return Number of lines. */
        uint32_t line_count(void) const noexcept;

        /** @return Current width. */
        uint32_t width(void) const noexcept;

        /** @brief Prints the memory table. */
        virtual void print(void) const noexcept override;
    };
}
